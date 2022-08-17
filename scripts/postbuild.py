#!/usr/bin/env python3

import argparse
import shutil
import os.path
import subprocess

import utils
from utils import join_dir, print_cmd, ensure_dir, copy_content_of_dir_to_other_dir

"""

"""


def copy_plugins_to_plugin_dir(plugin_build_dir: str, qtcreator_app_plugin_dir: str) -> None:
    if not os.path.exists(plugin_build_dir):
        print("Could not find plugin dir: {}", format(plugin_build_dir))
        exit(1)
    if not os.path.exists(qtcreator_app_plugin_dir):
        print("Could not find QtCreators plugin dir: {}", format(plugin_build_dir))
        exit(2)

    print("Copying plugins from {} to {}".format(plugin_build_dir, qtcreator_app_plugin_dir))
    libspacecreatorplugin = join_dir(plugin_build_dir, "libSpaceCreatorPlugin.so")
    shutil.copy2(libspacecreatorplugin, qtcreator_app_plugin_dir, )


def copy_wizards(wizards_dir: str, wizards_install_dir: str) -> None:
    if not os.path.exists(wizards_dir):
        print("Could not find wizards dir: {}", format(wizards_dir))
        exit(1)
    if not os.path.exists(wizards_install_dir):
        print("Could not find wizards install dir: {}", format(wizards_install_dir))
        exit(2)

    # File wizards
    files_dir = join_dir(wizards_dir, 'files')
    files_install_dir = join_dir(wizards_install_dir, 'files')

    utils.copy_content_of_dir_to_other_dir(files_dir, files_install_dir)

    # Projects wizards
    projects_dir = join_dir(wizards_dir, 'projects')
    projects_install_dir = join_dir(wizards_install_dir, 'projects')
    utils.copy_content_of_dir_to_other_dir(projects_dir, projects_install_dir)

    print("Copying wizards from {} to {}".format(wizards_dir, wizards_install_dir))


if __name__ == '__main__':
    # Parse arguments
    parser = argparse.ArgumentParser(prog='postbuild')

    parser.add_argument('--project_dir', dest='project_dir', type=str, required=True,
                        help='Path to the folder where spacecreator project is')
    parser.add_argument('--build_dir', dest='build_dir', type=str, required=True,
                        help='Path to the folder where spacecreator was build')
    parser.add_argument('--env_dir', dest='env_dir', type=str, required=True,
                        help='Path to the folder that contains the build environment')
    args = parser.parse_args()

    build_dir = args.build_dir
    env_dir = args.env_dir
    project_dir = args.project_dir
    plugin_build_dir = join_dir(build_dir, 'lib', 'qtcreator', 'plugins')
    plugin_install_dir = join_dir(env_dir, 'spacecreator.AppDir', 'lib', 'qtcreator', 'plugins')
    copy_plugins_to_plugin_dir(plugin_build_dir, plugin_install_dir)

    wizards_dir = join_dir(project_dir, 'wizards')
    wizards_install_dir = join_dir(env_dir, 'spacecreator.AppDir', 'share', 'qtcreator', 'templates', 'wizards')
    copy_wizards(wizards_dir, wizards_install_dir)

    # python3 ./scripts/postbuild.py --project_dir=/home/fisker/projects/spacecreator/ --build_dir=/home/fisker/projects/spacecreator/build --env_dir=/home/fisker/opt/qtcreatorenv
