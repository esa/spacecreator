#!/usr/bin/env python3

import argparse
import os.path
import shutil
import subprocess
import urllib.request
import tarfile
import py7zr


from utils import join_dir, print_cmd, ensure_dir, check_cmake_version, copy_content_of_dir_to_other_dir, copy_file_pattern_to_dir
from git.repo import Repo

'''
This script prepares a build environment in a given folder for SpaceCreator. SpaceCreator is a plugin for QtCreator
and the following is needed to compile it:

Qt
GrantLee - A template library that arguments Qt.
Qt Creator with header files for plugin development.

Requires aqt to be installed. (pip3 install aqtinstall).

Installing Qt 5.12.12 with QtCreator 4.8.2:
python3 ./scripts/prebuild.py --output_dir ~/opt/spacecreatorenv5 --qt_version=5.12.12 --qtcreator_version=4.8.2

Installing Qt 6.3.1 with QtCreator 8.0.1:
python3 ./scripts/prebuild.py --output_dir ~/opt/spacecreatorenv6 --qt_version=6.3.1 --qtcreator_version=8.0.1
'''


def build_path_object(env_path: str, qt_version: str):
    """
    Builds an object with named paths for files and folders.
    :param qt_version: Version of Qt to be used. Format is X.Y.Z
    :param env_path: path to build environment.
    :return: an object with attributes named after files and folders.
    """

    class Paths:
        env_dir = os.path.abspath(env_path)
        env_qt_install_dir = join_dir(env_dir, 'Qt')
        env_qt_dir = join_dir(env_dir, 'Qt', qt_version, 'gcc_64')
        env_app_dir = join_dir(env_dir, 'spacecreator.AppDir')  # This is where we put QtCreator and the SpaceCreator plugin
    _paths = Paths()
    return _paths


def download_qt(env_qt_path: str, env_qt_version: str) -> None:
    """
    Downloads the specified version of Qt to the specified environment folder.
    Extra modules installed: qtwebsockets, qt5compat
    """
    build_with_qt6 = env_qt_version.split('.')[0] == '6'
    print("Downloading Qt {} to {}".format(env_qt_version, env_qt_path))
    download_qt_command = ['aqt', 'install-qt', '--outputdir', env_qt_path,
                           '--base', 'https://download.qt.io/',
                           'linux', 'desktop', env_qt_version]
    if build_with_qt6:
        download_qt_command += ['--modules', 'qtwebsockets', 'qt5compat']

    print_cmd(download_qt_command)
    completed_process = subprocess.run(download_qt_command)
    if not completed_process.returncode == 0:
        print("Downloading Qt {} failed".format(env_qt_version))
        exit(1)


def download_qtcreator(env_path: str, env_qtc_version: str, env_app_dir, is_qt6: bool) -> None:
    """
    Downloads QtCreator in the specified version and the corresponding source files for plugin development
    and place them in the specified folder.
    :param env_path path to folder
    :param env_qtc_version version number in format X.Y.Z
    :param env_app_dir the dir
    :param is_qt6 is this QtCreator part of Qt6
    """
    # Download the 7z'ed QtCreator in the specified version
    version_list = env_qtc_version.split('.')
    version_short = '.'.join(version_list[:2])  # version_short is now in the format X.Y

    if is_qt6:
        base_url = 'https://download.qt.io/official_releases/qtcreator/' + \
                   version_short + '/' + env_qtc_version + '/installer_source/linux_x64/'
    else:
        base_url = 'https://download.qt.io/archive/qtcreator/' + \
                   version_short + '/' + env_qtc_version + '/installer_source/linux_gcc_64_rhel72/'
    bin_url = base_url + 'qtcreator.7z'
    qtcreator7z = join_dir(env_path, 'qtcreator.7z')
    print("Downloading {} to {}".format(bin_url, qtcreator7z))
    try:
        urllib.request.urlretrieve(bin_url, qtcreator7z)  # download qtcreator.7z to the root of the env folder
    except:
        print("Could not download QtCreator 7z file {}".format(bin_url))
        exit(2)

    with py7zr.SevenZipFile(qtcreator7z, mode='r') as z:
        z.extractall(env_app_dir)  # uncompress qtcreator into AppDir because qtcreator IS the app

    # Download the header files for building plugins for QtCreator and extract them to where qtcreator is
    dev_url = base_url + 'qtcreator_dev.7z'
    qtcreatordev7z = join_dir(env_path, 'qtcreator_dev.7z')
    print("Downloading {} to {}".format(dev_url, qtcreatordev7z))
    try:
        urllib.request.urlretrieve(dev_url, qtcreatordev7z)  # download qtcreator.7z to the root of the env folder
    except:
        print('Could not download QtCreator dev {}'.format(dev_url))
        exit(3)
    with py7zr.SevenZipFile(qtcreatordev7z, mode='r') as zdev:
        zdev.extractall(env_app_dir)  # uncompress qtcreator into AppDir because qtcreator IS the app


def download_grantlee(env_dir: str) -> None:
    """
    Clone grantlees template library
    """
    gitlab_url = "https://gitrepos.estec.esa.int/taste/grantlee.git"
    target_dir = join_dir(env_dir, 'grantlee')
    print('Cloning grantlee from {}'.format(gitlab_url))
    repository = Repo.clone_from(gitlab_url, target_dir)


def build_grantlee(env_dir: str, env_qt_dir: str, build_with_qt6: bool) -> None:

    cmake_build_dir = join_dir(env_dir, 'build')
    cmake_source_dir = join_dir(env_dir, 'grantlee')
    qmake_dir = join_dir(env_qt_dir, 'bin', 'qmake')

    print('Building grantlee')
    # Make ninja.build
    ninja_cmd = ['cmake',
                 '-GNinja',
                 '-DCMAKE_PREFIX_PATH:STRING=' + env_qt_dir,
                 '-DQT_QMAKE_EXECUTABLE:STRING=' + qmake_dir,
                 '-DCMAKE_BUILD_TYPE=Release',
                 '-DCMAKE_INSTALL_PREFIX=' + env_qt_dir,
                 '-B', cmake_build_dir,
                 '-S', cmake_source_dir]
    if build_with_qt6:
        ninja_cmd += ['-DGRANTLEE_BUILD_WITH_QT6=ON']

    print_cmd(ninja_cmd)
    completed_process = subprocess.run(ninja_cmd)
    if not completed_process.returncode == 0:
        print("Could not make ninja.build for grantlee")
        exit(4)

    # Build Grantlee using ninja
    build_cmd = ['cmake',
                 '--build',
                 cmake_build_dir]
    print_cmd(build_cmd)
    completed_process = subprocess.run(build_cmd)
    if not completed_process.returncode == 0:
        print("Could not build grantlee")
        exit(4)


def install_grantlee(env_dir: str) -> None:
    """
    Install grantlee into the Qt SDK, for build time usage (i.e. ~/opt/spacecreatorenv6/Qt)
    In addition copy grantlee to the spacecreator build environment, to be used at
    run time (~/opt/spacecreatorenv6/spacecreator.AppDir/Qt)

    Grantlee is traditionally installed on top of a Qt installation instead of adding the location of
    grantlee to CMAKE_PREFIX_PATH so we do that as well.

    :param env_dir: path to the build environment (i.e ~/opt/spacecreatorenv6)
    """
    cmake_build_dir = join_dir(env_dir, 'build')
    install_cmd = ['cmake',
                   '--build',
                   cmake_build_dir,
                   '--target',
                   'install']
    print_cmd(install_cmd)
    completed_process = subprocess.run(install_cmd)
    if not completed_process.returncode == 0:
        print("Could not install grantlee in {}".format(cmake_build_dir))
        exit(5)

    # Install grantlee in spacecreator.AppDir
    grantlee_lib_dir = join_dir(cmake_build_dir, 'grantlee')  # source of .so files
    spacecreator_qt_lib_dir = join_dir(env_dir, 'spacecreator.AppDir', 'lib', 'Qt', 'lib')
    spacecreator_grantlee_lib_dir = join_dir(spacecreator_qt_lib_dir, 'grantlee')  # dest of .so files
    ensure_dir(spacecreator_grantlee_lib_dir)
    shutil.copytree(grantlee_lib_dir, spacecreator_grantlee_lib_dir, dirs_exist_ok=True)

    templates_lib_dir = join_dir(cmake_build_dir, 'templates', 'lib')
    pattern = join_dir(templates_lib_dir, 'libGrantlee_Templates.so*')
    copy_file_pattern_to_dir(pattern, spacecreator_qt_lib_dir)


def download_asn1scc(env_dir: str) -> None:
    asn_url = "https://github.com/ttsiodras/asn1scc/releases/download/4.2.4.7f/asn1scc-bin-4.2.4.7f.tar.bz2"
    ans_tarbz2 = join_dir(env_dir, 'asn1scc-bin-4.2.4.7f.tar.bz2')
    print('Downloading {} to {}'.format(asn_url, ans_tarbz2))
    try:
        urllib.request.urlretrieve(asn_url, ans_tarbz2)  # download qtcreator.7z to the root of the env folder
    except:
        print("Could not download asn1scc from {}".format(asn_url))
        exit(4)
    print('Extracting {}'.format(ans_tarbz2))
    with tarfile.open(ans_tarbz2, 'r:bz2') as ans_tarbz2_file:
        ans_tarbz2_file.extractall(env_dir)


if __name__ == '__main__':
    # Parse arguments
    parser = argparse.ArgumentParser(prog='prebuild',
                                     epilog="Example: python3 ./prebuild.py --output_dir=/home/<user>/opt/qtcreatorenv "
                                            "--qt_version='6.3.1' "
                                            "--qtcreator_version='8.0.1'")
    parser.add_argument('--output_dir', dest='env_path', type=str, required=True,
                        help='Where to put the build environment t . This means the '
                             'specified version of Qt, GrantLee lib and the spacecrator.AppDir which'
                             ' is the final application')
    parser.add_argument('--qt_version', dest='qt_version', type=str, required=True,
                        help='Version of Qt to download to the build environment. Format X.Y.Z')
    parser.add_argument('--qtcreator_version', dest='qtcreator_version', type=str, required=True,
                        help='Version of Qt Creator to download. Format X.Y.Z')
    args = parser.parse_args()

    # Build the paths object
    env_dir = args.env_path
    qt_version = args.qt_version
    qtcreator_version = args.qtcreator_version
    paths = build_path_object(env_dir, qt_version)

    is_qt6 = qt_version.split('.')[0] == '6'
    print('qt_version was {}. Building with qt6 is {}'.format(qt_version, is_qt6))

    check_cmake_version(3, 16, 0)

    # Ensure dirs
    ensure_dir(paths.env_dir)
    ensure_dir(paths.env_qt_dir)
    ensure_dir(paths.env_app_dir)

    # Setup Qt and QtCreator with plugin development files
    download_qt(paths.env_qt_install_dir, qt_version)
    download_qtcreator(env_dir, qtcreator_version, paths.env_app_dir, is_qt6)

    # Grant Lee Template Library
    download_grantlee(env_dir)
    build_grantlee(env_dir, paths.env_qt_dir, is_qt6)
    install_grantlee(env_dir)

    # Abstract Syntax Notation
    download_asn1scc(env_dir)

