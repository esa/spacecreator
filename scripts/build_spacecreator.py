#!/usr/bin/env python3

import argparse
import os.path
import subprocess
from utils import join_dir, print_cmd, exit_if_not_exists, check_cmake_version

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


def build_spacecreator(project_dir: str, build_dir: str, build_type: str, env_dir: str, env_qt_dir: str, build_asn1plugin: bool) -> None:
    env_qmake_bin = join_dir(env_qt_dir, 'bin', 'qmake')
    qtc_install = join_dir(env_dir, 'spacecreator.AppDir')
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
                 '-DBUILD_PATCH_NUMBER=0',
                 '-DCMAKE_PREFIX_PATH=' + cmake_prefix_path,
                 '-DQT_QMAKE_EXECUTABLE:STRING=' + env_qmake_bin,
                 '-DENABLE_FORMAT_CHECK=OFF',
                 '-DQTC_INSTALL=' + qtc_install,
                 '-DQTC_SOURCE=' + qtc_install,
                 '-DBUILD_ASN1PLUGIN=' + ('True' if build_asn1plugin else 'False')
                 ]
    print_cmd(ninja_cmd)
    completed_process = subprocess.run(ninja_cmd)
    if not completed_process.returncode == 0:
        print("CMake failed")
        exit(1)

    # Build SpaceCreator using ninja
    build_cmd = ['cmake',
                 '--build', build_dir,
                 '--target', 'all']
    print_cmd(build_cmd)
    completed_process = subprocess.run(build_cmd)
    if not completed_process.returncode == 0:
        print("Building Spacecreator failed")
        exit(2)


if __name__ == '__main__':
    script_dir = os.path.dirname(os.path.realpath(__file__))
    default_project_dir = join_dir(script_dir, '..')

    # Parse arguments
    parser = argparse.ArgumentParser(prog='build_spacecreator')

    parser.add_argument('--project_dir', dest='project_dir', type=str, required=False, default=default_project_dir,
                        help='Path to the folder where spacecreator is located')
    parser.add_argument('--build_dir', dest='build_dir', type=str, required=True,
                        help='Path to the build directory where spacecreator is to be build')
    parser.add_argument('--build_type', dest='build_type', type=str, required=False,
                        help='Release or Debug')
    parser.add_argument('--env_dir', dest='env_dir', type=str, required=True,
                        help='Path to the build environment dir created by prebuild.py')
    parser.add_argument('--env_qt_dir', dest='env_qt_dir', type=str, required=True,
                        help='Path to the Qt distribution (./Qt/6.3.1/gcc_64/)')
    parser.add_argument('--no_build_asn1plugin', dest='no_build_asn1plugin', action='store_true')

    args = parser.parse_args()

    if args.project_dir:
        project_dir = args.project_dir
        print("build_spacecreator: Project dir is {}".format(project_dir))
    else:
        project_dir = default_project_dir
        print("Defaulting to project dir {}".format(project_dir))

    if args.build_dir:
        build_dir = args.build_dir
        print("build_spacecreator: build dir is {}".format(build_dir))
    else:
        build_dir = join_dir(project_dir, 'build')
        print("build_spacecreator: Defaulting to build dir {}".format(build_dir))

    if args.build_type:
        build_type = args.build_type
        print("build_spacecreator: Build type {}".format(build_type))
    else:
        build_type = 'Release'
        print("build_spacecreator: Defaulting to build type {}".format(build_type))

    if args.no_build_asn1plugin:
        build_asn1plugin = False
        print("build_spacecreator: Skipping ASN1Plugin")
    else:
        build_asn1plugin = True
        print("build_spacecreator: Building ASN1Plugin")

    env_dir = args.env_dir
    env_qt_dir = args.env_qt_dir

    print("build_spacecreator.py: project_dir is {}".format(project_dir))
    print("build_spacecreator.py: build_dir is {}".format(build_dir))
    print("build_spacecreator.py: build_type is {}".format(build_type))
    print("build_spacecreator.py: env_dir is {}".format(env_dir))
    print("build_spacecreator.py: env_qt_dir is {}".format(env_qt_dir))
    print("build_spacecreator.py: build_asn1plugin is {}".format(build_asn1plugin))

    check_cmake_version(3, 16, 0)

    build_spacecreator(project_dir, build_dir, build_type, env_dir, env_qt_dir, build_asn1plugin)
