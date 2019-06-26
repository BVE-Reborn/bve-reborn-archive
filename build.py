import os
import sys
import inspect
import math
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

def invoke_cmake(debug, extra_args):
    if (native.platform == native.Platform.Windows):
        print("Not yet")
        exit(1)

    debug_str = 'Debug' if debug else 'Release'
    debug_lower = 'debug' if debug else 'release'
    build_folder = os.path.join(native_folder, f"build-auto-{debug_lower}")
    os.makedirs(build_folder, exist_ok=True)
    
    cmake_args = ["cmake", "-S", native_folder, "-B", build_folder, "-GNinja", f"-DCMAKE_BUILD_TYPE={debug_str}", f"-DCMAKE_TOOLCHAIN_FILE={os.path.join(native_folder, 'build-vcpkg/scripts/buildsystems/vcpkg.cmake')}"] + extra_args
    result = subprocess.run(cmake_args)

    if result.returncode != 0:
        arg_string = '"' + '" "'.join(cmake_args) + '"'
        print(f"cmake returned {result.returncode}. Arguments: {arg_string}")
        exit(result.returncode)
    
    cpus = os.cpu_count()
    cores_in_build = int(math.ceil(cpus * 1.25))

    build_args = ["cmake", "--build", f"-j{cores_in_build}", build_folder]
    result2 = subprocess.run(build_args)
    if result2.returncode != 0:
        arg_string = '"' + '" "'.join(build_args) + '"'
        print(f'cmake build returned {result2.returncode}. Arguments: {arg_string}')
        exit(result2.returncode)

    # Remove old libraries
    if (native.platform == native.Platform.Windows):
        old_libraries = glob.glob(os.path.join(unity_native_folder, f"*.dll"), os.path.join(unity_native_folder, f"*.pdb"))
    else:
        old_libraries = glob.glob(os.path.join(unity_native_folder, "*.so"))

    for lib in old_libraries:
        print(f"Removing {lib}")
        os.remove(lib)
    if os.path.exists(unity_native_bindings_folder):
        print(f"Removing folder {unity_native_bindings_folder}")
        shutil.rmtree(unity_native_bindings_folder)

    # Copy current libraries
    if (native.platform == native.Platform.Windows):
        libraries = glob.glob(os.path.join(build_folder, f"bin/{debug_str}/*.dll"), os.path.join(build_folder, f"bin/{debug_str}/*.pdb"))
    else:
        libraries = glob.glob(os.path.join(build_folder, "bin/*.so"))

    os.makedirs(unity_native_bindings_folder, exist_ok=True)
    print(f"Copying shared libraries to {unity_native_folder}")
    for lib in libraries:
        print(f"Copying {os.path.basename(lib)}")
        shutil.copy(lib, unity_native_folder)

    bindings = glob.glob(os.path.join(build_folder, f"csharp/*.cs"))
    print(f"Copying c# binding files to {unity_native_bindings_folder}")
    for cs in bindings:
        print(f"Copying {os.path.basename(cs)}")
        shutil.copy(cs, unity_native_bindings_folder)

def main(args):
    if ("--debug" in args):
        debug = True
        args.remove("--debug")
    else:
        debug = False
    if ("--help" in args):
        print("build.py [--debug] <EXTRA_CMAKE_ARGS>")
        exit(1)

    native.main(["--vcpkg-dir", os.path.join(native_folder, "build-vcpkg"), native_folder])

    invoke_cmake(debug, args)

if __name__ == "__main__":
    main(sys.argv[1:])
