#!/usr/bin/env python3

import os
import subprocess
import argparse

import distutils
from distutils import dir_util

import glob

from sys import platform, stdout

packages = [
    'abseil',
    'cli11',
    'concurrentqueue',
    'cppfs',
    'doctest',
    'eastl',
    'fmt',
    'foundational',
    'glm',
    'mapbox-variant',
    'ms-gsl',
    'nova-renderer',
    'rapidxml-ns',
]

if platform == "linux" or platform == "linux2":
    target = "x64-linux"
elif platform == "darwin":
    target = "x64-osx"
elif platform == "win32":
    target = "x64-windows"

def copy_tree(src, dest):
	print(f"Copying {src} to {dest}")
	stdout.flush()
	distutils.dir_util.copy_tree(src, dest)

def copy_ports(source_dir, vcpkg_dir):
	copy_tree("extern/ports/foundational/", os.path.join(vcpkg_dir, "ports/foundational/"))
	copy_tree("extern/ports/nova-renderer/", os.path.join(vcpkg_dir, "ports/nova-renderer/"))

def install_package(name, vcpkg_dir):
	if subprocess.run(["vcpkg", "install", f"{name}:{target}"]).returncode != 0:
		for file in glob.glob(os.path.join(vcpkg_dir, f"buildtrees/{name}/*.log")):
			print(f"Contents of file {file}")
			with open(file) as f:
				stdout.write(f.read())
				stdout.flush()

def main():
	parser = argparse.ArgumentParser()
	parser.add_argument("--source-dir", type=str, default=os.getcwd())
	parser.add_argument("--vcpkg-dir", type=str, required=True)
	parsed = parser.parse_args()

	copy_ports(parsed.source_dir, parsed.vcpkg_dir)

	subprocess.run(['vcpkg', 'update'])
	subprocess.run(['vcpkg', 'upgrade', '--no-dry-run'])

	for package in packages:
		install_package(package, parsed.vcpkg_dir)

if __name__ == '__main__':
	main()
