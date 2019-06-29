import os
import sys
import inspect
import math
import re
import shutil
import subprocess
import glob

# https://stackoverflow.com/questions/279237/import-a-module-from-a-relative-path
script_folder = os.path.abspath(os.path.split(inspect.getfile( inspect.currentframe() ))[0])
native_folder = os.path.abspath(os.path.join(os.path.split(inspect.getfile( inspect.currentframe() ))[0], "Native"))
unity_native_folder = os.path.abspath(os.path.join(os.path.split(inspect.getfile( inspect.currentframe() ))[0], "Assets/Native"))
unity_native_bindings_folder = os.path.abspath(os.path.join(os.path.split(inspect.getfile( inspect.currentframe() ))[0], "Assets/Native/Bindings"))
if native_folder not in sys.path:
    sys.path.insert(0, native_folder)

import native

def version_check(name, actual, needed):
    if (actual < needed):
        print(f"{name} is version {actual}. FAILED: Needs {needed}.")
        exit(1)
    print(f"{name} is version {actual}.")

def cmake_version_check():
    try:
        result = subprocess.run(["cmake", "--version"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    except FileNotFoundError as f:
        print(f"cmake not found.")
        exit(1)
    if (result.returncode != 0):
        print("cmake not found or not configured right")
        print(result.stdout.decode('utf8'))
    match = re.search(r"""(\d+)\.(\d+)\.(\d+)""", result.stdout.decode('utf8'))
    version_check("cmake", tuple(map(int, match.groups())), (3, 14))

def ninja_version_check():
    try:
        result = subprocess.run(["ninja", "--version"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    except FileNotFoundError as f:
        print(f"ninja not found.")
        exit(1)
    if (result.returncode != 0):
        print("ninja not found or not configured right")
        print(result.stdout.decode('utf8'))
    match = re.search(r"""(\d+)\.(\d+)\.(\d+)""", result.stdout.decode('utf8'))
    version_check("ninja", tuple(map(int, match.groups())), (1,))

def msbuild_version_check():
    try:
        result = subprocess.run(["msbuild", "-version", "-noLogo"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    except FileNotFoundError as f:
        print(f"msbuild not found.")
        exit(1)
    if (result.returncode != 0):
        print("msbuild not found or not configured right, Please make sure you have run vcvarsall64.bat.")
        print(result.stdout.decode('utf8'))
    match = re.search(r"""(\d+)\.(\d+)\.(\d+)\.(\d+)""", result.stdout.decode('utf8'))
    version_check("msbuild", tuple(map(int, match.groups())), (0,))

def cl_version_check():
    try:
        result = subprocess.run(["cl"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    except FileNotFoundError as f:
        print(f"cl not found.")
        exit(1)
    if (result.returncode != 0):
        print("cl not found or not configured right, Please make sure you have run vcvarsall64.bat.")
        print(result.stdout.decode('utf8'))
    match = re.search(r"""(\d+)\.(\d+)\.(\d+)\.(\d+)""", result.stdout.decode('utf8'))
    version_check("cl", tuple(map(int, match.groups())), (19, 20))

def swig_version_check():
    try:
        result = subprocess.run(["swig", "-version"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    except FileNotFoundError as f:
        print(f"swig2 not found.")
        exit(1)
    if (result.returncode != 0):
        print("swig not found or not configured right")
        print(result.stdout.decode('utf8'))
    match = re.search(r"""(\d+)\.(\d+)\.(\d+)""", result.stdout.decode('utf8'))
    version_check("swig", tuple(map(int, match.groups())), (4, 0))


def version_checks(ninja):
    cmake_version_check()
    if ninja:
        ninja_version_check()
        cl_version_check()
        msbuild_version_check()
    swig_version_check()

def invoke_cmake(debug, ninja, verbose, extra_args):
    if (native.platform == native.Platform.Windows) and not ninja:
        tool = ["-GVisual Studio 16 2019", "-A", "x64"]
    else:
        tool = ["-GNinja"]

    debug_str = 'Debug' if debug else 'Release'
    debug_lower = 'debug' if debug else 'release'
    build_folder = os.path.join(native_folder, f"build-auto-{debug_lower}")
    os.makedirs(build_folder, exist_ok=True)

    csharp_bindings_output_dir = os.path.join(build_folder, f"bindings/csharp/")
    if os.path.exists(csharp_bindings_output_dir):
        print(f"Removing folder {csharp_bindings_output_dir}")
        shutil.rmtree(csharp_bindings_output_dir)
        
    swig_location = shutil.which("swig")
    cmake_args = ["cmake", "-S", native_folder, "-B", build_folder, *tool, f"-DSWIG_EXECUTABLE={swig_location}", f"-DCMAKE_BUILD_TYPE={debug_str}", f"-DCMAKE_TOOLCHAIN_FILE={os.path.join(native_folder, 'build-vcpkg/scripts/buildsystems/vcpkg.cmake')}"] + extra_args
    result = subprocess.run(cmake_args)

    if result.returncode != 0:
        arg_string = '"' + '" "'.join(cmake_args) + '"'
        print(f"cmake returned {result.returncode}. Arguments: {arg_string}")
        exit(result.returncode)
    
    cpus = os.cpu_count()
    cores_in_build = int(math.ceil(cpus * 1.25))

    build_args = ["cmake", "--build", build_folder, f"-j{cores_in_build}", "--config", debug_str, "--target", "bve-cs"]
    if verbose:
        build_args.append("--verbose")
    result2 = subprocess.run(build_args)
    if result2.returncode != 0:
        arg_string = '"' + '" "'.join(build_args) + '"'
        print(f'cmake build returned {result2.returncode}. Arguments: {arg_string}')
        exit(result2.returncode)

    unity_native_dll_folder = os.path.join(os.path.join(unity_native_folder, "Binaries"), native.platformName)

    # Remove old libraries
    if (native.platform == native.Platform.Windows):
        old_libraries = glob.glob(os.path.join(unity_native_dll_folder, f"*.dll")) + glob.glob(os.path.join(unity_native_dll_folder, f"*.pdb"))
    else:
        old_libraries = glob.glob(os.path.join(unity_native_dll_folder, "*.so"))

    try:
        for lib in old_libraries:
            print(f"Removing {lib}")
            os.remove(lib)
        if os.path.exists(unity_native_bindings_folder):
            print(f"Removing folder {unity_native_bindings_folder}")
            shutil.rmtree(unity_native_bindings_folder)
    except PermissionError as e:
        ext = os.path.splitext(e.filename)[1]
        if ext in [".dll", ".so", ".dynlib", ".pdb"]:
            print(f"Unity needs to be closed so {e.filename} can be overwritten.")
            exit(1)
        else:
            raise

    # Copy current libraries
    if (native.platform == native.Platform.Windows):
        if ninja:
            libraries = glob.glob(os.path.join(build_folder, f"bin/*.dll")) + glob.glob(os.path.join(build_folder, f"bin/*.pdb"))
        else:
            libraries = glob.glob(os.path.join(build_folder, f"bin/{debug_str}/*.dll")) + glob.glob(os.path.join(build_folder, f"bin/{debug_str}/*.pdb"))
    else:
        libraries = glob.glob(os.path.join(build_folder, "bin/*.so"))

    os.makedirs(unity_native_dll_folder, exist_ok=True)
    os.makedirs(unity_native_bindings_folder, exist_ok=True)
    print(f"Copying shared libraries to {unity_native_dll_folder}")
    for lib in libraries:
        print(f"Copying {os.path.basename(lib)}")
        shutil.copy(lib, unity_native_dll_folder)

    bindings = glob.glob(os.path.join(build_folder, f"bindings/csharp/**/*.cs"), recursive=True)
    print(f"Copying c# binding files to {unity_native_bindings_folder}")
    for cs in bindings:
        local_relative = os.path.relpath(cs, os.path.join(build_folder, "bindings/csharp"))
        dst = os.path.join(unity_native_bindings_folder, local_relative)
        print(f"Copying {local_relative}")
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        shutil.copy(cs, dst)

SWIG_REGEX_STR = r"""\[global::System\.Runtime\.InteropServices\.DllImport\("([^"]*)", EntryPoint="([^"]*)"\)\]\s*public static extern (\S*)\s([^(]*)\(([^)]*)\);"""
SWIG_REGEX = re.compile(SWIG_REGEX_STR, re.UNICODE | re.MULTILINE)

SWIG_CTOR_REGEX_STR = r"""static [^(]*?PINVOKE\(\)\s*{\s*}"""
SWIG_CTOR_REGEX = re.compile(SWIG_CTOR_REGEX_STR, re.UNICODE | re.MULTILINE)

class Function:
    def __init__(self, match):
        self.native_name = match.group(2)
        self.return_type = match.group(3)
        self.csharp_name = match.group(4)
        self.arguments = match.group(5)

def process_pinvoke_file(filename):
    dll_map = {}
    with open(filename, "r+") as f:
        filestr = f.read()
        for match in SWIG_REGEX.finditer(filestr):
            dll = match.group(1)
            func = Function(match)
            if dll not in dll_map:
                dll_map[dll] = []
            dll_map[dll].append(func)

        replace = SWIG_REGEX.sub(
            "[global::System.Runtime.InteropServices.UnmanagedFunctionPointer(global::System.Runtime.InteropServices.CallingConvention.CDecl)]\n"
            "public delegate \3 delegate_\4_t(\5);\n"
            "public static delegate_\4_t \4;"
        )

        print(replace)

        # f.truncate(0)

        # f.write(replace)

def main(args):
    if "--debug" in args:
        debug = True
        args.remove("--debug")
    else:
        debug = False
    if ("--ninja" in args) or (not native.Platform.Windows):
        ninja = True
        args.remove("--ninja")
    else:
        ninja = False
    if "--verbose" in args:
        verbose = True
        args.remove("--verbose")
    else:
        verbose = False
    if ("--help" in args):
        print("build.py [--debug] <EXTRA_CMAKE_ARGS>")
        exit(1)

    version_checks(ninja)

    native.main(["--vcpkg-dir", os.path.join(native_folder, "build-vcpkg"), native_folder])

    invoke_cmake(debug, ninja, verbose, args)

if __name__ == "__main__":
    main(sys.argv[1:])
