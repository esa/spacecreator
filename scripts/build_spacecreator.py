#!/usr/bin/env python3

import argparse
import os.path
import subprocess

from utils import join_dir, print_cmd, exit_if_not_exists, check_cmake_version, qt_dir_from_env_dir
from utils import start_time, delta_time

"""
Builds the SpaceCreator Project.
Prerequisites: A spacecreator build environment produced by the prebuild.py script.

Build SpaceCreator with Qt6:
python3 ./scripts/build_spacecreator.py 
    --project_dir ~/projects/spacecreator
    --build_dir ~/projects/spacecreator/build
    --env_dir ~/opt/spacecreatorenv6
    --env_qt_dir ~/opt/spacecreatorenv6/Qt/6.3.1/gcc_64/ 
"""


def build_spacecreator(project_dir: str, build_dir: str, app_dir: str, build_type: str, env_dir: str, build_asn1plugin: bool, patch_version: int) -> None:
    env_qt_dir = qt_dir_from_env_dir(env_dir)
    env_qmake_bin = join_dir(env_qt_dir, 'bin', 'qmake')
    qtc_install = app_dir
    env_qt_cmake_dir = join_dir(env_qt_dir, 'lib', 'cmake')

    exit_if_not_exists(project_dir)
    exit_if_not_exists(env_dir)
    exit_if_not_exists(env_qt_dir)
    exit_if_not_exists(env_qmake_bin)
    exit_if_not_exists(qtc_install)
    exit_if_not_exists(env_qt_cmake_dir)

    cmake_prefix_path = env_qt_cmake_dir
    ninja_cmd = ['cmake',
                 '-GNinja',
                 '-S', project_dir,
                 '-B', build_dir,
                 '-DCMAKE_BUILD_TYPE=' + build_type,
                 '-DBUILD_PATCH_NUMBER=' + str(patch_version),
                 '-DCMAKE_PREFIX_PATH=' + cmake_prefix_path,
                 '-DQT_QMAKE_EXECUTABLE:STRING=' + env_qmake_bin,
                 '-DQTC_INSTALL=' + qtc_install,
                 '-DQTC_SOURCE=' + qtc_install,
                 '-DBUILD_PLUGIN_ASN1=' + ('True' if build_asn1plugin else 'False')
                 ]
    print_cmd(ninja_cmd)
    completed_process = subprocess.run(ninja_cmd)
    if not completed_process.returncode == 0:
        print("build_spacecreator.py: CMake failed")
        exit(1)

    # Build SpaceCreator using ninja
    build_cmd = ['cmake',
                 '--build', build_dir,
                 '--target', 'all']
    print_cmd(build_cmd)
    completed_process = subprocess.run(build_cmd)
    if not completed_process.returncode == 0:
        print("build_spacecreator.py: Building Spacecreator failed")
        exit(2)


if __name__ == '__main__':
    start_time = start_time()
    script_dir = os.path.dirname(os.path.realpath(__file__))
    default_project_dir = join_dir(script_dir, '..')

    # Parse arguments
    parser = argparse.ArgumentParser(prog='build_spacecreator')

    parser.add_argument('--project_dir', dest='project_dir', type=str, required=False, default=default_project_dir,
                        help='Path to the folder where spacecreator is located')
    parser.add_argument('--build_dir', dest='build_dir', type=str, required=True,
                        help='Path to the build directory where spacecreator is to be build')
    parser.add_argument('--app_dir', dest='app_dir', type=str, required=False,
                        help='Path to the folder that contains AppDir')
    parser.add_argument('--build_type', dest='build_type', type=str, required=False,
                        help='Release or Debug')
    parser.add_argument('--env_dir', dest='env_dir', type=str, required=True,
                        help='Path to the build environment dir created by prebuild.py')
    parser.add_argument('--no_build_asn1plugin', dest='no_build_asn1plugin', action='store_true')
    parser.add_argument('--patch_version', dest='patch_version', type=int, required=False,
                        help='Version number after major and minor version')

    args = parser.parse_args()

    if args.project_dir:
        project_dir = args.project_dir
        print("build_spacecreator.py: Project dir is {}".format(project_dir))
    else:
        project_dir = default_project_dir
        print("build_spacecreator.py: Defaulting to project dir {}".format(project_dir))

    if args.build_dir:
        build_dir = args.build_dir
        print("build_spacecreator.py: build dir is {}".format(build_dir))
    else:
        build_dir = join_dir(project_dir, 'build')
        print("build_spacecreator.py: Defaulting to build dir {}".format(build_dir))

    env_dir = args.env_dir

    if args.app_dir:
        app_dir = args.app_dir
    else:
        app_dir = join_dir(args.env_dir, 'spacecreator.AppDir')
    print("build_spacecreator.py: app_dir is {}".format(app_dir))

    if args.build_type:
        build_type = args.build_type
        print("build_spacecreator.py:  Build type {}".format(build_type))
    else:
        build_type = 'Release'
        print("build_spacecreator.py:  Defaulting to build type {}".format(build_type))

    if args.no_build_asn1plugin:
        build_asn1plugin = False
        print("build_spacecreator.py:  Skipping ASN1Plugin")
    else:
        build_asn1plugin = True
        print("build_spacecreator.py:  Building ASN1Plugin")

    if args.patch_version:
        patch_version = args.patch_version
        print("build_spacecreator.py: Patch version is {}".format(patch_version))
    else:
        patch_version = 0
        print("build_spacecreator.py: Defaulting patch version {}".format(patch_version))

    check_cmake_version(3, 16, 0)

    build_spacecreator(project_dir, build_dir, app_dir, build_type, env_dir, build_asn1plugin, patch_version)
    print("build_spacecreator.py: Total time was: {}".format(delta_time(start_time)))
