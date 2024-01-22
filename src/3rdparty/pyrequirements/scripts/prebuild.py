#!/usr/bin/env python3
import os
import subprocess
from utils import join_dir, print_cmd, ensure_dir
from git import Git
from git.repo import Repo


def download_qt(env_qt_dir: str, env_qt_version: str) -> None:
    """
    Downloads the specified version of Qt to the specified environment folder.
    Extra modules installed: qtwebsockets, qt5compat
    """
    print("Downloading Qt {} to {}".format(env_qt_version, env_qt_dir))
    download_qt_command = ['aqt', 'install-qt', '--outputdir', env_qt_dir,
                           '--base', 'https://download.qt.io/',
                           'linux', 'desktop', env_qt_version,
                           '--modules', 'qtwebsockets'
                           ]

    print_cmd(download_qt_command)
    completed_process = subprocess.run(download_qt_command)
    if not completed_process.returncode == 0:
        print("Downloading Qt {} failed".format(env_qt_version))
        exit(1)


def main():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    project_dir = join_dir(script_dir, '..')
    env_dir = join_dir(project_dir, '..', 'breadcrumbsenv')
    env_qt_install_dir = join_dir(env_dir, 'Qt')
    pyside_dir = join_dir(env_dir, 'pyside')
    qt_version = "6.3.0"
    qtpaths_dir = join_dir(env_qt_install_dir, qt_version, 'gcc_64', 'bin', 'qtpaths6')
    pyside_version = "dev"

    ensure_dir(env_dir)

    download_qt(env_qt_install_dir, qt_version)

if __name__ == '__main__':
    main()
