#!/usr/bin/env python3

import argparse
import glob
import itertools
import json
import multiprocessing
import multiprocessing.pool
import os
import pprint
import re
import shutil
import subprocess
import sys

clang_tidy_possible_names = ["clang-tidy-7.0",
                             "clang-tidy-6.0",
                             "clang-tidy-5.0",
                             "clang-tidy-4.0",
                             "clang-tidy-3.9",
                             "clang-tidy-3.8",
                             "clang-tidy"]

clang_tidy_exec = ""
clang_tidy_checks = ["boost*",
                     "bugprone*",
                     "cert*",
                     "clang-analizer-*",
                     "clang-diagnostic-*",
                     "cppcoreguidelines*",
                     "-cppcoreguidelines-pro-bounds-constant-array-index",
                     "google*",
                     "llvm*",
                     "-llvm-header-guard",
                     "misc*",
                     "modernize*",
                     "performance*",
                     "-performance-move-const-arg",
                     "readability*"]

folders_to_run = ["."]
extentions_to_run = [".cpp", ".cxx", ".cc"]

warning_regex = "([^\\s]+):(\\d+):(\\d+): (warning|note|error|fatal error): ((?:operator\\[\\]|[^\\n\\[])+)(?:\\[([^\\]]+)\\])?"
warning_regex_program = re.compile(warning_regex, re.ASCII | re.MULTILINE)

clang_tidy_build_dir = None

clang_tidy_raw_options = []

verbose = False
raw = False


def find_clang_tidy():
    global clang_tidy_exec

    for name in clang_tidy_possible_names:
        path = shutil.which(name)
        if (path is not None):
            clang_tidy_exec = path
            break

    if (len(clang_tidy_exec) == 0):
        print("Clang Tidy not found. Please install to path.", file=sys.stderr)
        exit(1)

    if (verbose):
        print("Clang Tidy found at: {}\n".format(clang_tidy_exec), file=sys.stderr)


def read_settings(filename):
    global clang_tidy_checks
    global folders_to_run

    try:
        with open(filename, 'r') as f:
            if (verbose):
                print("Settings file: {} found.".format(filename), file=sys.stderr)

            settings = json.load(f)

            if ("checks" in settings):
                clang_tidy_checks = settings["checks"]
            if ("folders" in settings):
                folders_to_run = settings["folders"]
    except FileNotFoundError:
        if (verbose):
            print("Settings file: {} not found.".format(filename), file=sys.stderr)


def find_all_files(input_files):
    if (input_files is not None):
        return [os.path.normpath(infile) for infile in input_files]

    file_list = []

    if (verbose):
        print("File glob patterns: ", file=sys.stderr)

    for folder in folders_to_run:
        for ext in extentions_to_run:
            glob_str = folder + "/**/*" + ext
            if (verbose):
                print(glob_str, file=sys.stderr)
            file_list.extend(glob.glob(glob_str, recursive=True))
    return file_list


class ClangTidyError(Exception):
    pass


def run_file(filename):
    full_name = os.path.join(os.getcwd(), filename)

    folders = "({}).*".format("|".join([os.path.abspath(folder)
                                        for folder in folders_to_run]))

    checks = "-checks={}".format(",".join(clang_tidy_checks))

    if (clang_tidy_build_dir is not None):
        build_arg = "-p={}".format(clang_tidy_build_dir)
    else:
        build_arg = ""

    args = [clang_tidy_exec, checks, "-header-filter={}".format(folders), full_name, build_arg] + clang_tidy_raw_options

    if (verbose):
        print(args, file=sys.stderr)
    else:
        print("{} {}".format(clang_tidy_exec, filename), file=sys.stderr)

    clang_tidy_result = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    if (raw or clang_tidy_result.returncode != 0):
        sys.stdout.write(str(clang_tidy_result.stdout, 'utf-8'))
        sys.stderr.write(str(clang_tidy_result.stderr, 'utf-8'))

    if (clang_tidy_result.returncode != 0):
        raise ClangTidyError("{} returned non-zero.".format(clang_tidy_exec))

    return str(clang_tidy_result.stdout, 'utf-8')


class ParsedOutput:
    diagnostic_text = ""
    file = ""
    line = 0
    char = 0
    diagnostic_type = ""
    error = ""
    check = ""
    begin_match = 0
    end_match = 0
    full_text = ""


def parse_output(raw_output):
    match_iter = warning_regex_program.finditer(raw_output)

    matches = []

    for match in match_iter:
        po = ParsedOutput()

        po.diagnostic_text = match.group(0)
        po.file = match.group(1)
        po.line = int(match.group(2))
        po.char = int(match.group(3))
        po.diagnostic_type = match.group(4)
        po.error = match.group(5)
        po.check = match.group(6)
        po.begin_match = match.start()
        po.end_match = match.end()

        matches.append(po)

    if len(matches) > 0:
        for elem1, elem2 in zip(matches, matches[1:]):
            #  Plus one to remove the newline
            elem1.full_text = raw_output[elem1.end_match + 1:elem2.begin_match - 1]

        matches[-1].full_text = raw_output[matches[-1].end_match + 1:-1]

    combined_matches = []

    for elem in matches:
        if elem.diagnostic_type == "note":
            try:
                combined_matches[-1].append(elem)
            except IndexError as e:
                print(combined_matches)
                print(elem.full_text)
                raise e
        else:
            combined_matches.append([elem])

    return combined_matches


def recolor_output(parsed_output):
    if (parsed_output.diagnostic_type == "note"):
        warning_color = "\033[30;1m"
    elif (parsed_output.diagnostic_type == "warning"):
        warning_color = "\033[35;1m"
    else:
        warning_color = "\033[31;1m"

    if (parsed_output.check is not None):
        check_string = "[{}]".format(parsed_output.check)
    else:
        check_string = ""

    header = "\033[0;1m{}:{}:{}: {}{}: \033[0;1m{} {}\033[0m".format(parsed_output.file,
                                                                     parsed_output.line,
                                                                     parsed_output.char,
                                                                     warning_color,
                                                                     parsed_output.diagnostic_type,
                                                                     parsed_output.error,
                                                                     check_string)

    if (len(parsed_output.full_text) > 0):
        # first line is normal text
        # second line bold green
        # third line is normal green
        # no newline at beginning of fulltext
        body = re.sub("^(.*)\n(.*)(\n)?",
                      "\\1\n\033[0;32;1m\\2\\3\033[0;32m",
                      parsed_output.full_text,
                      1) \
               + "\033[0m"

        return "{}\n{}".format(header, body)
    else:
        return header


def nocolor_output(parsed_output):
    if (parsed_output.check is not None):
        check_string = "[{}]".format(parsed_output.check)
    else:
        check_string = ""

    header = "{}:{}:{}: {}: {} {}".format(parsed_output.file,
                                          parsed_output.line,
                                          parsed_output.char,
                                          parsed_output.diagnostic_type,
                                          parsed_output.error,
                                          check_string)

    if (len(parsed_output.full_text) > 0):
        return "{}\n{}".format(header, parsed_output.full_text)
    else:
        return header


def main():
    global clang_tidy_exec
    global clang_tidy_raw_options
    global clang_tidy_build_dir
    global folders_to_run
    global raw
    global verbose

    parser = argparse.ArgumentParser(description="Allows easier use of clang-tidy through multithreading and"
                                                 "elimination of duplicates from output.",
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument("-j",
                        metavar="jobs",
                        type=int,
                        nargs='?',
                        default=(multiprocessing.cpu_count() + 1),
                        help="Amount of jobs to run")
    parser.add_argument("--folders",
                        metavar="folder",
                        type=str,
                        nargs='+',
                        help="Folders to run through clang-tidy, recursively")
    parser.add_argument("--files",
                        metavar="file.cpp",
                        type=str,
                        nargs='*',
                        help="Files to lint")
    parser.add_argument("-f", "--fix",
                        action="store_true",
                        default=False,
                        help="Automatically fix problems.")
    parser.add_argument("-s", "--format-style",
                        metavar="style",
                        type=str,
                        nargs='?',
                        default="none",
                        help="clang-format style to format code in. Possible options: none/file/{ <json> },llvm,etc.")
    parser.add_argument("-p", "-bd", "--build_dir",
                        metavar="/path/to/build/tree",
                        type=str,
                        nargs='?',
                        help="Folder to find compile-commands.json in. Defaults to $PWD")
    parser.add_argument("-r", "--raw",
                        action="store_true",
                        help="Dump raw output for clang-tidy to stdout and stderr")
    parser.add_argument("-v", "--verbose",
                        action="store_true",
                        help="Print extra information")
    parser.add_argument("-nc", "--no-color",
                        action="store_true",
                        help="Don't recolor output")
    parser.add_argument("--path-to-clang-tidy",
                        metavar="path",
                        type=str,
                        help="Path to clang-tidy executable")
    parser.add_argument("--config",
                        metavar=".clang-tidy-manager",
                        default=".clang-tidy-manager",
                        type=str,
                        help="Path to configuration file")
    parser.add_argument("--raw-options",
                        nargs=argparse.REMAINDER,
                        help="Pass remaining arguments right to clang-tidy")

    args = parser.parse_args()

    if (args.raw_options):
        clang_tidy_raw_options = args.raw_options

    if (args.build_dir is not None):
        clang_tidy_build_dir = args.build_dir

    raw = args.raw
    verbose = args.verbose

    if (args.path_to_clang_tidy is not None):
        clang_tidy_exec = args.path_to_clang_tidy
    else:
        find_clang_tidy()

    read_settings(args.config)

    if (args.folders is not None):
        folders_to_run = args.folders

    jobs = args.j

    if (args.fix):
        clang_tidy_raw_options.append("--fix")
        if (jobs > 1):
            print("Multiple jobs not recommended when fixing problems. Dropping to single job.", file=sys.stderr)
            jobs = 1

    clang_tidy_raw_options.append("-format-style={}".format(args.format_style))

    tp = multiprocessing.pool.ThreadPool(jobs)

    raw_outputs = tp.map(run_file, find_all_files(args.files))
    parsed_outputs = map(parse_output, raw_outputs)

    #  flatten into one error list
    flattened_parsed = sorted(itertools.chain.from_iterable(parsed_outputs),
                              key=lambda warning: (warning[0].file, warning[0].line, warning[0].char, warning[0].error))

    unique_dictionary = {warning[0].diagnostic_text: warning for warning in flattened_parsed}
    unique_list = list(unique_dictionary.values())

    flattened_unique = list(itertools.chain.from_iterable(unique_list))

    if (not args.no_color):
        output = list(map(recolor_output, flattened_unique))
    else:
        output = list(map(nocolor_output, flattened_unique))

    for warning in output:
        print(warning)

    print("\n{} warnings/errors.".format(len(unique_list)))
    print("{} notes.".format(len(flattened_unique) - len(unique_list)))


if __name__ == "__main__":
    main()
