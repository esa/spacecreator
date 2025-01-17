#!/usr/bin/env python3

import argparse
import os.path
import subprocess
import urllib.request
import tarfile
import py7zr
import shutil

from utils import join_dir, print_cmd, ensure_dir, check_cmake_version, copy_content_of_dir_to_other_dir, copy_file_pattern_to_dir
from git import Git
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

Installing Qt 6.4.3 with QtCreator 11.0.3:
python3 ./scripts/prebuild.py --output_dir ~/opt/spacecreatorenv6 --qt_version=6.4.3 --qtcreator_version=11.0.3


'''


def __notify_satus(message: str):
    me = os.path.basename(__file__)
    print(f"{me}: {message}")


def __notify_action_started(action: str, arg_from, arg_to):
    __notify_satus(f"{action} '{arg_from}' to '{arg_to}'")


def __notify_download_started(url, destination):
    __notify_action_started("Downloading", url, destination)


def build_path_object(project_dir: str, env_path: str, qt_version: str):
    """
    Builds an object with named paths for files and folders.
    :param qt_version: Version of Qt to be used. Format is X.Y.Z
    :param env_path: path to build environment.
    :param project_dir: path to where the spacecreator project is cloned
    :return: an object with attributes named after files and folders.
    """

    class Paths:
        env_dir = os.path.abspath(env_path)
        env_qt_install_dir = join_dir(env_dir, 'Qt')
        env_qt_dir = join_dir(env_dir, 'Qt', qt_version, 'gcc_64')
        env_qt_libexec_dir = join_dir(env_qt_dir, 'libexec')
        env_qt_bin_dir = join_dir(env_qt_dir, 'bin')
        install_dir = join_dir(project_dir, 'install')

    _paths = Paths()
    return _paths


def download_qt(env_qt_path: str, env_qt_version: str) -> None:
    """
    Downloads the specified version of Qt to the specified environment folder.
    Extra modules installed: qtwebsockets, qt5compat
    """
    build_with_qt6 = env_qt_version.split('.')[0] == '6'
    __notify_download_started(env_qt_version, env_qt_path)
    download_qt_command = [
        'aqt',
        'install-qt',
        '--outputdir',
        env_qt_path,
        '--base',
        'https://download.qt.io/',
        'linux',
        'desktop',
        env_qt_version,
    ]
    if build_with_qt6:
        download_qt_command += ['--modules', 'qtwebsockets', 'qt5compat']

    print_cmd(download_qt_command)
    completed_process = subprocess.run(download_qt_command)
    if not completed_process.returncode == 0:
        __notify_satus(f"Downloading Qt {env_qt_version} failed")
        exit(1)


def download_qtcreator(env_path: str, env_qtc_version: str, env_app_dir) -> None:
    """
    Downloads QtCreator in the specified version and the corresponding source files for plugin development
    and place them in the specified folder.
    :param env_path path to folder
    :param env_qtc_version version number in format X.Y.Z
    :param env_app_dir the dir
    """
    # Download the 7z'ed QtCreator in the specified version
    version_list = env_qtc_version.split('.')
    version_short = '.'.join(version_list[:2])  # version_short is now in the format X.Y

    base_url = f"https://download.qt.io/official_releases/qtcreator/{version_short}/{env_qtc_version}/installer_source/linux_x64/"

    bin_url = base_url + 'qtcreator.7z'
    qtcreator7z = join_dir(env_path, 'qtcreator.7z')
    __notify_download_started(bin_url, qtcreator7z)
    try:
        urllib.request.urlretrieve(bin_url, qtcreator7z)  # download qtcreator.7z to the root of the env folder
    except:  # noqa E722
        __notify_satus(f"Could not download QtCreator 7z file '{bin_url}'")
        exit(2)

    with py7zr.SevenZipFile(qtcreator7z, mode='r') as z:
        z.extractall(env_app_dir)  # uncompress qtcreator into AppDir because qtcreator IS the app

    # Download the header files for building plugins for QtCreator and extract them to where qtcreator is
    dev_url = base_url + 'qtcreator_dev.7z'
    qtcreatordev7z = join_dir(env_path, 'qtcreator_dev.7z')
    __notify_download_started(dev_url, qtcreatordev7z)
    try:
        urllib.request.urlretrieve(dev_url, qtcreatordev7z)  # download qtcreator.7z to the root of the env folder
    except:  # noqa E722
        __notify_satus(f"Could not download QtCreator dev '{dev_url}'")
        exit(3)
    with py7zr.SevenZipFile(qtcreatordev7z, mode='r') as zdev:
        zdev.extractall(env_app_dir)  # uncompress qtcreator into AppDir because qtcreator IS the app


def download_grantlee(env_dir: str) -> None:
    """
    Clone grantlees template library
    """
    gitlab_url = "https://gitrepos.estec.esa.int/taste/grantlee.git"
    target_dir = join_dir(env_dir, 'grantlee')
    grantlee_tag = "v5.3.1"
    __notify_satus(f"Cloning grantlee from '{gitlab_url}' the tag '{grantlee_tag}'")
    Repo.clone_from(gitlab_url, target_dir)
    repo = Git(target_dir)
    repo.checkout(grantlee_tag)


def build_grantlee(env_dir: str, env_qt_dir: str) -> None:

    cmake_build_dir = join_dir(env_dir, 'build')
    cmake_source_dir = join_dir(env_dir, 'grantlee')
    qmake_dir = join_dir(env_qt_dir, 'bin', 'qmake')

    __notify_satus("Building grantlee")
    # Make ninja.build
    ninja_cmd = [
        'cmake',
        '-GNinja',
        '-DCMAKE_PREFIX_PATH:STRING=' + env_qt_dir,
        '-DQT_QMAKE_EXECUTABLE:STRING=' + qmake_dir,
        '-DCMAKE_BUILD_TYPE=Release',
        '-DCMAKE_INSTALL_PREFIX=' + env_qt_dir,
        '-B',
        cmake_build_dir,
        '-S',
        cmake_source_dir,
        '-DGRANTLEE_BUILD_WITH_QT6=ON',
        '-Wmaybe-uninitialized',
    ]

    print_cmd(ninja_cmd)
    completed_process = subprocess.run(ninja_cmd)
    if not completed_process.returncode == 0:
        __notify_satus("Could not make ninja.build for grantlee")
        exit(4)

    # Build Grantlee using ninja
    build_cmd = [
        'cmake',
        '--build',
        cmake_build_dir,
    ]
    print_cmd(build_cmd)
    completed_process = subprocess.run(build_cmd)
    if not completed_process.returncode == 0:
        __notify_satus("Could not build grantlee")
        exit(4)


def install_grantlee(env_dir: str, app_dir: str) -> None:
    """
    Install grantlee into the Qt SDK, for build time usage (i.e. ~/opt/spacecreatorenv6/Qt)
    In addition copy grantlee to the spacecreator build environment, to be used at
    run time (~/opt/spacecreatorenv6/spacecreator.AppDir/Qt)

    Grantlee is traditionally installed on top of a Qt installation instead of adding the location of
    grantlee to CMAKE_PREFIX_PATH, so we do that as well.

    :param env_dir: path to the build environment (i.e ~/opt/spacecreatorenv6)
    """
    cmake_build_dir = join_dir(env_dir, 'build')
    install_cmd = [
        'cmake',
        '--build',
        cmake_build_dir,
        '--target',
        'install',
    ]
    print_cmd(install_cmd)
    completed_process = subprocess.run(install_cmd)
    if not completed_process.returncode == 0:
        __notify_satus(f"Could not install grantlee in '{cmake_build_dir}'")
        exit(5)

    # Install grantlee in spacecreator.AppDir
    grantlee_lib_dir = join_dir(cmake_build_dir, 'grantlee')  # source of .so files
    spacecreator_qt_plugins_dir = join_dir(app_dir, 'lib', 'Qt', 'plugins')
    spacecreator_grantlee_plugins_dir = join_dir(spacecreator_qt_plugins_dir, 'grantlee')  # dest of .so files
    ensure_dir(spacecreator_grantlee_plugins_dir)
    copy_content_of_dir_to_other_dir(grantlee_lib_dir, spacecreator_grantlee_plugins_dir)

    spacecreator_qt_lib_dir = join_dir(app_dir, 'lib', 'Qt', 'lib')
    templates_lib_dir = join_dir(cmake_build_dir, 'templates', 'lib')
    pattern = join_dir(templates_lib_dir, 'libGrantlee_*.so*')
    copy_file_pattern_to_dir(pattern, spacecreator_qt_lib_dir)
    textdocument_lib_dir = join_dir(cmake_build_dir, 'textdocument', 'lib')
    pattern = join_dir(textdocument_lib_dir, 'libGrantlee_*.so*')
    copy_file_pattern_to_dir(pattern, spacecreator_qt_lib_dir)


def download_asn1scc(env_dir: str) -> None:
    """
    Downloads asn1scc archive. In the build step it will we compiled into the asn1scc-fuzzer executable
    :param env_dir: The dir in which asn1scc is extracted
    """
    asn_url = "https://github.com/maxime-esa/asn1scc/releases/download/4.2.4.7f/asn1scc-bin-4.2.4.7f.tar.bz2"
    asn_tarbz2 = join_dir(env_dir, 'asn1scc-bin-4.2.4.7f.tar.bz2')
    __notify_download_started(asn_url, asn_tarbz2)
    try:
        urllib.request.urlretrieve(asn_url, asn_tarbz2)  # download qtcreator.7z to the root of the env folder
    except:  # noqa E722
        __notify_satus(f"Could not download asn1scc from '{asn_url}'")
        exit(4)
    __notify_action_started("Extracting", asn_tarbz2, env_dir)
    with tarfile.open(asn_tarbz2, 'r:bz2') as asn_tarbz2_file:
        asn_tarbz2_file.extractall(env_dir)


def download_asn_fuzzer(env_dir: str, app_dir: str) -> None:
    """
    Downloads the asn fuzzer
    """
    fuzzer_url = "https://github.com/n7space/asn1scc.Fuzzer/releases/download/0.9/asn1scc-Fuzzer-0.9-linux-x64.tar.gz"
    fuzzer_targz = join_dir(env_dir, 'asn1scc-Fuzzer-0.9-linux-x64.tar.gz')
    __notify_download_started(fuzzer_url, fuzzer_targz)
    try:
        urllib.request.urlretrieve(fuzzer_url, fuzzer_targz)
    except:  # noqa E722
        __notify_satus(f"Could not download asn fuzzer from '{fuzzer_url}'")
        exit(4)
    fuzzer_target = join_dir(app_dir, "libexec", "qtcreator")
    __notify_action_started("Extracting", fuzzer_targz, fuzzer_target)
    with tarfile.open(fuzzer_targz, 'r:gz') as fuzzer_targz_file:
        fuzzer_targz_file.extractall(fuzzer_target)


def download_pus_c(env_dir: str, app_dir: str) -> None:
    """
    Downloads pus-C library
    """
    pusc_url = "https://github.com/n7space/asn1-pusc-lib/releases/download/1.1.0/Asn1Acn-PusC-Library-1.1.0.7z"
    pusc_7z = join_dir(env_dir, 'Asn1Acn-PusC-Library-1.1.0.7z')
    __notify_download_started(pusc_url, pusc_7z)
    try:
        urllib.request.urlretrieve(pusc_url, pusc_7z)
    except:  # noqa E722
        __notify_satus(f"Could not download asn fuzzer from '{pusc_url}'")
        exit(4)
    pusc_target = join_dir(app_dir, "share", "qtcreator", "asn1acn", "libs", "PUS-C")
    ensure_dir(pusc_target)
    __notify_action_started("Extracting", pusc_7z, pusc_target)

    with py7zr.SevenZipFile(pusc_7z, mode='r') as z:
        z.extractall(pusc_target)


def build_asn1scc_language_server(env_dir: str) -> None:
    makefile = join_dir(env_dir, 'Makefile.debian')
    if not os.path.exists(makefile):
        __notify_satus(f"No Makefile.debian found in '{makefile}'")
        exit(5)
    make_cmd = ['make', '-f', makefile]
    __notify_satus("Building Language Server")
    print_cmd(make_cmd)
    completed_process = subprocess.run(make_cmd)
    if not completed_process.returncode == 0:
        __notify_satus("Could not build asn1scc")
        exit(6)
    server = join_dir(env_dir, 'asn1scc', 'lsp', 'Server', 'Server', 'bin', 'Release', 'net6.0', 'Server')
    if os.path.exists(server):
        __notify_satus(f"Successfully build '{server}'")
    else:
        __notify_satus(f"Failed building language server. File not build: '{server}'")
        exit(7)


def download_asn1scc_language_server(env_dir: str) -> None:
    url = "https://github.com/maxime-esa/asn1scc/releases/download/4.3.1.1/asn1scc_lsp_linux-x64-4.3.1.1.tar.bz2"
    asn1cc_lsp_tarbz2 = join_dir(env_dir, 'asn1scc-lsp.tar.bz2')
    __notify_download_started(url, asn1cc_lsp_tarbz2)
    try:
        urllib.request.urlretrieve(url, asn1cc_lsp_tarbz2)  # download qtcreator.7z to the root of the env folder
    except:  # noqa E722
        __notify_satus(f"Could not download asn1scc language server from '{url}'")
        exit(4)
    __notify_action_started("Extracting", asn1cc_lsp_tarbz2, env_dir)
    with tarfile.open(asn1cc_lsp_tarbz2, 'r:bz2') as ans_lsp_tarbz2_file:
        ans_lsp_tarbz2_file.extractall(env_dir)


def copy_additional_qt_modules(env_qt_dir: str, app_dir: str) -> None:
    if not os.path.exists(env_qt_dir):
        __notify_satus(f"Could not find env qt dir: '{env_qt_dir}'")
        exit(1)
    if not os.path.exists(app_dir):
        __notify_satus(f"Could not find env app dir: '{app_dir}'")
        exit(2)

    env_qt_lib_dir = join_dir(env_qt_dir, 'lib')
    app_lib_dir = join_dir(app_dir, 'lib', 'Qt', 'lib')
    __notify_action_started("Copying additional qt modules", env_qt_lib_dir, app_lib_dir)
    pattern = join_dir(env_qt_lib_dir, 'libQt*WebSockets*')
    copy_file_pattern_to_dir(pattern, app_lib_dir)


def extract_extraLibraries(install_dir: str, lib_dir: str) -> None:
    extra_libs = ['libzxb-util.tar.gz', 'libxcb-cursor.tar.gz', 'libssl3.tar.gz']
    for extra_lib_name in extra_libs:
        extra_lib = join_dir(install_dir, extra_lib_name)
        __notify_action_started("Extracting", extra_lib, lib_dir)
        with tarfile.open(extra_lib, 'r:gz') as archive:
            archive.extractall(lib_dir)


def copy_highlighter_files(generic_highlighter_dir: str, generic_highlighter_install_dir: str) -> None:
    if not os.path.exists(generic_highlighter_dir):
        __notify_satus(f"Could not find wizards dir: '{generic_highlighter_dir}'")
        exit(1)
    __notify_action_started("Copying generic highlighter files", generic_highlighter_dir, generic_highlighter_install_dir)
    copy_content_of_dir_to_other_dir(generic_highlighter_dir, generic_highlighter_install_dir)


def copy_snippets(snippets_dir: str, snippets_install_dir: str) -> None:
    if not os.path.exists(snippets_dir):
        __notify_satus(f"Could not find snippets dir '{snippets_dir}'")
        exit(1)
    __notify_action_started("Copying snippets", snippets_dir, snippets_install_dir)
    copy_content_of_dir_to_other_dir(snippets_dir, snippets_install_dir)


def copy_qhelpgenerator(qhelpgenerator_dir: str, target_libexec_dir: str) -> None:
    if not os.path.exists(qhelpgenerator_dir):
        __notify_satus(f"Could not find qhelpgenerator in '{qhelpgenerator_dir}'")
        exit(1)
    if not os.path.exists(target_libexec_dir):
        os.makedirs(target_libexec_dir)  # in QtC 4 this folder does not exist
    qhelpgenerator = join_dir(qhelpgenerator_dir, 'qhelpgenerator')
    __notify_action_started("Copying qhelpgenerator", qhelpgenerator_dir, target_libexec_dir)
    shutil.copy2(qhelpgenerator, target_libexec_dir)


def main():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    default_project_dir = join_dir(script_dir, '..')

    # Parse arguments
    parser = argparse.ArgumentParser(prog='prebuild')
    parser.add_argument('--output_dir',
                        dest='env_path',
                        type=str,
                        required=True,
                        help='Where to put the build environment . This means the '
                        'specified version of Qt, GrantLee lib and the spacecrator.AppDir which'
                        ' is the final application')
    parser.add_argument('--project_dir',
                        dest='project_dir',
                        type=str,
                        required=False,
                        help='Path to the folder where spacecreator project is')
    parser.add_argument('--qt_version',
                        dest='qt_version',
                        type=str,
                        required=True,
                        help='Version of Qt to download to the build environment. Format X.Y.Z')
    parser.add_argument('--qtcreator_version',
                        dest='qtcreator_version',
                        type=str,
                        required=True,
                        help='Version of Qt Creator to download. Format X.Y.Z')
    parser.add_argument('--app_dir',
                        dest='app_dir',
                        type=str,
                        required=False,
                        help="Path to the folder that contains AppDir. Defaults to output_dir/spacecreator.AppDir")
    args = parser.parse_args()

    # Build the paths object
    if args.project_dir:
        project_dir = args.project_dir
    else:
        project_dir = default_project_dir
        __notify_satus(f"project_dir defaults to '{project_dir}'")

    env_dir = args.env_path
    if args.app_dir:
        app_dir = args.app_dir
    else:
        app_dir = join_dir(env_dir, 'spacecreator.AppDir')

    qt_version = args.qt_version
    qtcreator_version = args.qtcreator_version
    paths = build_path_object(project_dir, env_dir, qt_version)

    __notify_satus(f"env_dir is '{env_dir}'")
    __notify_satus(f"qt_version is {qt_version}")
    __notify_satus(f"qtcreator_version is {qtcreator_version}")

    check_cmake_version(3, 18, 0)

    # Ensure dirs
    ensure_dir(paths.env_dir)
    ensure_dir(paths.env_qt_dir)
    ensure_dir(app_dir)

    # Setup Qt and QtCreator with plugin development files
    download_qt(paths.env_qt_install_dir, qt_version)
    download_qtcreator(env_dir, qtcreator_version, app_dir)
    copy_additional_qt_modules(paths.env_qt_dir, app_dir)

    download_asn1scc_language_server(env_dir)

    # Grant Lee Template Library
    download_grantlee(env_dir)
    build_grantlee(env_dir, paths.env_qt_dir)
    install_grantlee(env_dir, app_dir)

    # libzxb-util
    install_dir = paths.install_dir
    lib_dir = join_dir(app_dir, 'lib')
    extract_extraLibraries(install_dir, lib_dir)

    # Abstract Syntax Notation
    download_asn1scc(env_dir)
    download_asn_fuzzer(env_dir, app_dir)
    download_pus_c(env_dir, app_dir)

    # AppImage files SpaceCreator.desktop, AppRun and TasteLanguageClients.ini
    copy_content_of_dir_to_other_dir(join_dir(project_dir, 'install', 'appimage'), app_dir)

    # Copy qhelpgenerator
    qhelpgenerator_dir = paths.env_qt_libexec_dir
    copy_qhelpgenerator(qhelpgenerator_dir, join_dir(app_dir, 'libexec'))


if __name__ == '__main__':
    main()
