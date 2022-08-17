#!/usr/bin/env python3

import argparse
import os.path
import subprocess
import urllib.request

import py7zr
import zipfile
from utils import join_dir, print_cmd, ensure_dir


def build_spacecreator(project_dir: str, build_dir: str, build_type: str, env_dir: str, env_qt_distribution_dir: str) -> None:
    env_qmake_bin = join_dir(env_qt_distribution_dir, 'bin', 'qmake')
    qtc_install = join_dir(env_dir, 'spacecreator.AppDir')
    ninja_cmd = ['cmake',
                 '-GNinja',
                 '-S', project_dir,
                 '-B', build_dir,
                 '-DCMAKE_BUILD_TYPE=' + build_type,
                 '-DBUILD_PATCH_NUMBER=0',
                 '-DCMAKE_PREFIX_PATH:STRING=' + env_qt_distribution_dir,
                 '-DQT_QMAKE_EXECUTABLE:STRING=' + env_qmake_bin,
                 '-DENABLE_FORMAT_CHECK=OFF',
                 '-DQTC_INSTALL=' + qtc_install,
                 '-DQTC_SOURCE=' + qtc_install
                 ]
    print_cmd(ninja_cmd)
    return_code = subprocess.call(ninja_cmd)
    if return_code:
        exit(1)

    # Build SpaceCreator using ninja
    build_cmd = ['cmake',
                 '--build', build_dir,
                 '--target', 'all']
    print_cmd(build_cmd)
    return_code = subprocess.call(build_cmd)
    if return_code:
        exit(4)


if __name__ == '__main__':
    # Parse arguments
    parser = argparse.ArgumentParser(prog='build_spacecreator')

    parser.add_argument('--project_dir', dest='project_dir', type=str, required=True,
                        help='Path to the folder where spacecreator is located')
    parser.add_argument('--build_dir', dest='build_dir', type=str, required=True,
                        help='Path to the build directory where spacecreator is to be build')
    parser.add_argument('--build_type', dest='build_type', type=str, required=True,
                        help='Release or Debug')
    parser.add_argument('--env_dir', dest='env_dir', type=str, required=True,
                        help='Path to the build environment dir created by prebuild.py')
    parser.add_argument('--env_qt_distribution_dir', dest='env_qt_distribution_dir', type=str, required=True,
                        help='Path to the Qt distribution (./Qt/6.3.1/gcc_64/)')

    args = parser.parse_args()

    project_dir = args.project_dir
    build_dir = args.build_dir
    build_type = args.build_type
    env_dir = args.env_dir
    env_qt_distribution_dir = args.env_qt_distribution_dir

    build_spacecreator(project_dir, build_dir, build_type, env_dir, env_qt_distribution_dir)

# python3 ./scripts/build_spacecreator.py --project_dir=/home/fisker/projects/spacecreator --build_dir=/home/fisker/projects/spacecreator/build --build_type=Release --env_dir=/home/fisker/opt/qtcreatorenv --env_qt_distribution_dir=/home/fisker/opt/qtcreatorenv/Qt/6.3.1/gcc_64