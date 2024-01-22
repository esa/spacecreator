#!/usr/bin/env python3

import argparse
import os.path
import subprocess

from utils import join_dir, print_cmd, qt_dir_from_env_dir

def build_breadcrumbs(project_dir: str,
                      build_dir: str,
                      env_qt_cmake_dir: str) -> None:

    ninja_cmd = ['cmake',
                   '-GNinja',
                   '-S', project_dir,
                   '-B', build_dir,
                   '-DCMAKE_PREFIX_PATH:PATH=' + env_qt_cmake_dir,
                   '-DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE' # Print what command is being genereated by CMake
                   ]
    print_cmd(ninja_cmd)
    completed_process = subprocess.run(ninja_cmd)
    if not completed_process.returncode == 0:
        print("build_breadcrumbs.py: CMake failed")
        exit(1)

    # Build Breadcrumbs using ninja
    build_cmd = ['cmake',
                 '--build', build_dir,
                 '--target', 'all']
    print_cmd(build_cmd)
    completed_process = subprocess.run(build_cmd)
    if not completed_process.returncode == 0:
        print("build_breadcrumbs.py: Building breadcrumbs failed")
        exit(2)
        
def install_library(build_dir: str) -> None:
    install_cmd = ['cmake', '--install', build_dir]
    print_cmd(install_cmd)
    completed_process = subprocess.run(install_cmd)
    if not completed_process.returncode == 0:
        print("build_breadcrumbs.py: Installing breadcrumbs failed")
        exit(3)

def main():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    default_project_dir = join_dir(script_dir, '..')

    # Parse arguments
    parser = argparse.ArgumentParser(prog='build_breadcrumbs')

    parser.add_argument('--project_dir', dest='project_dir', type=str, required=False, default=default_project_dir,
                       help='Path to the folder where breadcrumbs is located')
    parser.add_argument('--build_dir', dest='build_dir', type=str, required=False,
                       help='Path to the build directory where breadcrumbs is to be build')
    parser.add_argument('--qt_cmake_dir', dest='qt_cmake_dir', type=str, required=False,
                       help='Path to cmake dir in the Qt installation to be used')
    args = parser.parse_args()

    if args.project_dir:
        project_dir = args.project_dir
        print("build_breadcrumbs.py: Project dir is {}".format(project_dir))
    else:
        project_dir = default_project_dir
        print("build_breadcrumbs.py: Defaulting to project dir {}".format(project_dir))

    if args.build_dir:
        build_dir = args.build_dir
        print("build_breadcrumbs.py: build dir is {}".format(build_dir))
    else:
        build_dir = join_dir(project_dir, 'build')
        print("build_breadcrumbs.py: Defaulting to build dir {}".format(build_dir))

    if args.qt_cmake_dir:
        qt_cmake_dir = args.qt_cmake_dir
    else:
        env_dir = join_dir(project_dir, '..', 'breadcrumbsenv')
        qt_cmake_dir = qt_dir_from_env_dir(env_dir)

    build_breadcrumbs(project_dir, build_dir, qt_cmake_dir)
    install_library(build_dir)
    print("build_breadcrumbs: breadcrumbs done")


if __name__ == '__main__':
    main()
