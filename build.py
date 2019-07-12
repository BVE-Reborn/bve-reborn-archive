import os
import sys
import inspect
import math
import re
import shutil
import subprocess
import glob

# https://stackoverflow.com/questions/279237/import-a-module-from-a-relative-path
script_folder = os.path.abspath(os.path.split(inspect.getfile(inspect.currentframe()))[0])
cpp_folder = os.path.abspath(os.path.join(os.path.split(inspect.getfile(inspect.currentframe()))[0], "Native/cpp"))
rust_folder = os.path.abspath(os.path.join(os.path.split(inspect.getfile(inspect.currentframe()))[0], "Native/rust"))
unity_native_folder = os.path.abspath(
    os.path.join(os.path.split(inspect.getfile(inspect.currentframe()))[0], "Assets/Native"))
unity_native_bindings_folder = os.path.abspath(
    os.path.join(os.path.split(inspect.getfile(inspect.currentframe()))[0], "Assets/Native/Bindings"))
if cpp_folder not in sys.path:
    sys.path.insert(0, cpp_folder)

import native


def version_check(name, actual, needed):
    if actual < needed:
        print(f"{name} is version {actual}. FAILED: Needs {needed}.")
        exit(1)
    print(f"{name} is version {actual}.")


def cmake_version_check():
    try:
        result = subprocess.run(["cmake", "--version"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    except FileNotFoundError as f:
        print(f"cmake not found.")
        exit(1)
    if result.returncode != 0:
        print("cmake not found or not configured right")
        print(result.stdout.decode('utf8'))
        exit(1)
    match = re.search(r"""(\d+)\.(\d+)\.(\d+)""", result.stdout.decode('utf8'))
    version_check("cmake", tuple(map(int, match.groups())), (3, 14))


def ninja_version_check():
    try:
        result = subprocess.run(["ninja", "--version"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    except FileNotFoundError as f:
        print(f"ninja not found.")
        exit(1)
    if result.returncode != 0:
        print("ninja not found or not configured right")
        print(result.stdout.decode('utf8'))
        exit(1)
    match = re.search(r"""(\d+)\.(\d+)\.(\d+)""", result.stdout.decode('utf8'))
    version_check("ninja", tuple(map(int, match.groups())), (1,))


def msbuild_version_check():
    try:
        result = subprocess.run(["msbuild", "-version", "-noLogo"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    except FileNotFoundError as f:
        print(f"msbuild not found, please make sure you have run vcvarsall64.bat.")
        exit(1)
    if result.returncode != 0:
        print("msbuild not found or not configured right, please make sure you have run vcvarsall64.bat.")
        print(result.stdout.decode('utf8'))
        exit(1)
    match = re.search(r"""(\d+)\.(\d+)\.(\d+)\.(\d+)""", result.stdout.decode('utf8'))
    version_check("msbuild", tuple(map(int, match.groups())), (0,))


def cl_version_check():
    try:
        result = subprocess.run(["cl"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    except FileNotFoundError as f:
        print(f"cl not found, please make sure you have run vcvarsall64.bat.")
        exit(1)
    if result.returncode != 0:
        print("cl not found or not configured right, please make sure you have run vcvarsall64.bat.")
        print(result.stdout.decode('utf8'))
        exit(1)
    match = re.search(r"""(\d+)\.(\d+)\.(\d+)\.(\d+)""", result.stdout.decode('utf8'))
    version_check("cl", tuple(map(int, match.groups())), (19, 20))


def swig_version_check():
    try:
        result = subprocess.run(["swig", "-version"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    except FileNotFoundError as f:
        print(f"swig not found.")
        exit(1)
    if result.returncode != 0:
        print("swig not found or not configured right")
        print(result.stdout.decode('utf8'))
        exit(1)
    match = re.search(r"""(\d+)\.(\d+)\.(\d+)""", result.stdout.decode('utf8'))
    version_check("swig", tuple(map(int, match.groups())), (4, 0))


def cbindgen_version_check():
    try:
        result = subprocess.run(["cbindgen", "--version"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    except FileNotFoundError as f:
        print(f"cbindgen not found.")
        exit(1)
    if result.returncode != 0:
        print("cbindgen not found or not configured right")
        print(result.stdout.decode('utf8'))
        exit(1)
    match = re.search(r"""(\d+)\.(\d+)\.(\d+)""", result.stdout.decode('utf8'))
    version_check("cbindgen", tuple(map(int, match.groups())), (0, 9, 0))


if native.platform == native.Platform.Windows:
    rust_toolchain = "+nightly-msvc"
    rust_toolchain_error = "cargo not found or not configured right. Must have nightly-msvc toolchain installed.\n" +\
            "rustup toolchain install nightly-msvc"
else:
    rust_toolchain = "+nightly"
    rust_toolchain_error = "cargo not found or not configured right. Must have nightly toolchain installed.\n" +\
            "rustup toolchain install nightly"


def cargo_version_check():
    try:
        result = subprocess.run(["cargo", rust_toolchain, "--version"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    except FileNotFoundError as f:
        print(f"cargo not found.")
        exit(1)
    if result.returncode != 0:
        print(rust_toolchain_error)
        print(result.stdout.decode('utf8'))
        exit(1)
    match = re.search(r"""(\d+)\.(\d+)\.(\d+)""", result.stdout.decode('utf8'))
    version_check("cargo", tuple(map(int, match.groups())), (1, 38, 0))


def version_checks(ninja):
    cmake_version_check()
    if ninja:
        ninja_version_check()
        if native.platform == native.Platform.Windows:
            cl_version_check()
            msbuild_version_check()
    swig_version_check()
    cbindgen_version_check()
    cargo_version_check()


def invoke_cargo(debug):
    print("\n\nBuilding Rust Code:")
    if debug:
        cargo_args = ["cargo", rust_toolchain, "build"]
    else:
        cargo_args = ["cargo", rust_toolchain, "build", "--release"]
    result = subprocess.run(cargo_args, cwd=rust_folder)

    if result.returncode != 0:
        arg_string = '"' + '" "'.join(cargo_args) + '"'
        print(f"cargo returned {result.returncode}. Arguments: {arg_string}")
        exit(result.returncode)


def invoke_cbindgen(debug):
    print("Calling cbindgen")
    debug_lower = 'debug' if debug else 'release'

    os.makedirs(os.path.join(rust_folder, f'target/{debug_lower}/include/bve-reborn-rust'), exist_ok=True)

    cbindgen_args = ["cbindgen",
                     os.path.join(rust_folder, "cbindgen-meta"),
                     "-c", os.path.join(rust_folder, "cbindgen.toml"),
                     "-o", os.path.join(rust_folder, f'target/{debug_lower}/include/bve-reborn-rust.h')]
    result = subprocess.run(cbindgen_args, cwd=rust_folder)

    if result.returncode != 0:
        arg_string = '"' + '" "'.join(cbindgen_args) + '"'
        print(f"cbindgen returned {result.returncode}. Arguments: {arg_string}")
        exit(result.returncode)


def invoke_cmake(debug, ninja, verbose, extra_args):
    print("\n\nBuilding C++ Code:")
    if (native.platform == native.Platform.Windows) and not ninja:
        tool = ["-GVisual Studio 16 2019", "-A", "x64"]
    else:
        tool = ["-GNinja"]

    debug_str = 'Debug' if debug else 'Release'
    debug_lower = 'debug' if debug else 'release'
    build_folder = os.path.join(cpp_folder, f"build-auto-{debug_lower}")
    os.makedirs(build_folder, exist_ok=True)

    csharp_bindings_output_dir = os.path.join(build_folder, f"bindings/csharp/")
    if os.path.exists(csharp_bindings_output_dir):
        print(f"Removing folder {csharp_bindings_output_dir}")
        shutil.rmtree(csharp_bindings_output_dir)

    if native.platform == native.Platform.Windows:
        rust_dlls = glob.glob(os.path.join(rust_folder, f"target/{debug_lower}/*.dll")) + \
                    glob.glob(os.path.join(rust_folder, f"target/{debug_lower}/*.pdb")) + \
                    glob.glob(os.path.join(rust_folder, f"target/{debug_lower}/deps/*.pdb"))
        rust_libs = glob.glob(os.path.join(rust_folder, f"target/{debug_lower}/*.lib"))
    else:
        rust_dlls = glob.glob(os.path.join(rust_folder, f"target/{debug_lower}/*.so"))
        rust_libs = rust_dlls
    rust_libs_list = ";".join(rust_libs)

    swig_location = shutil.which("swig")
    cmake_args = ["cmake",
                  "-S", cpp_folder,
                  "-B", build_folder,
                  *tool,
                  f"-DSWIG_EXECUTABLE={swig_location}",
                  f"-DRUST=True",
                  f"-DRUST_HEADER_PATH={os.path.join(rust_folder, f'target/{debug_lower}/include')}",
                  f"-DRUST_SWIG_PATH={os.path.join(rust_folder, f'swig')}",
                  f"-DRUST_LINK_LIBRARIES={rust_libs_list}",
                  f"-DCMAKE_BUILD_TYPE={debug_str}",
                  f"-DCMAKE_TOOLCHAIN_FILE={os.path.join(cpp_folder, 'build-vcpkg/scripts/buildsystems/vcpkg.cmake')}"
                  ] + extra_args
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
    if native.platform == native.Platform.Windows:
        old_libraries = glob.glob(os.path.join(unity_native_dll_folder, f"*.dll")) + glob.glob(
            os.path.join(unity_native_dll_folder, f"*.pdb"))
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
    if native.platform == native.Platform.Windows:
        if ninja:
            libraries = glob.glob(os.path.join(build_folder, f"bin/*.dll")) + glob.glob(
                os.path.join(build_folder, f"bin/*.pdb"))
        else:
            libraries = glob.glob(os.path.join(build_folder, f"bin/{debug_str}/*.dll")) + glob.glob(
                os.path.join(build_folder, f"bin/{debug_str}/*.pdb"))
    else:
        libraries = glob.glob(os.path.join(build_folder, "bin/*.so"))
    libraries += rust_dlls

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


def main(args):
    if "--debug" in args:
        debug = True
        args.remove("--debug")
    else:
        debug = False
    if "--ninja" in args:
        ninja = True
        args.remove("--ninja")
    elif native.platform != native.Platform.Windows:
        ninja = True
    else:
        ninja = False
    if "--verbose" in args:
        verbose = True
        args.remove("--verbose")
    else:
        verbose = False
    if "--help" in args:
        print("build.py [--debug] <EXTRA_CMAKE_ARGS>")
        exit(1)

    version_checks(ninja)

    native.main(["--vcpkg-dir", os.path.join(cpp_folder, "build-vcpkg"), cpp_folder])

    invoke_cargo(debug)
    invoke_cbindgen(debug)
    invoke_cmake(debug, ninja, verbose, args)


if __name__ == "__main__":
    main(sys.argv[1:])
