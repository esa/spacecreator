#!/usr/bin/env python3

import argparse
import shutil
import os.path
import subprocess

import utils
from utils import join_dir, copy_content_of_dir_to_other_dir

"""
When the spacecreator plugin has been build, it needs to be copied to the plugin folder of the
QtCreator that is to load it. To be able to create a project and populate the project with files, QtCreator
needs a project and a file template definition copied to its 
spacecreator.AppDir/share/qtcreator/templates/wizards/files
and 
spacecreator.AppDir/share/qtcreator/templates/wizards/project
folders.
This script handles all this and can be called on the commandline or from the QtCreator/Other IDE used to develop in.
In QtCreator the run settings could be a custom Deploy Configuration with the command python3 an the arguments:
 %{ActiveProject:Path}/scripts/postbuild.py --project_dir=%{ActiveProject:Path} --build_dir=%{buildDir} --env_dir=/home/<user>/opt/qtcreatorenv/
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
    script_dir = os.path.dirname(os.path.realpath(__file__))
    default_project_dir = join_dir(script_dir, '..')

    # Parse arguments
    parser = argparse.ArgumentParser(prog='postbuild',
                                     epilog='python3 ./scripts/postbuild.py '
                                            '--project_dir=/home/<user>/projects/spacecreator/ '
                                            '--build_dir=/home/<user>/projects/spacecreator/build '
                                            '--env_dir=/home/<user>/opt/qtcreatorenv ')

    parser.add_argument('--project_dir', dest='project_dir', type=str, required=False,
                        help='Path to the folder where spacecreator project is')
    parser.add_argument('--build_dir', dest='build_dir', type=str, required=False,
                        help='Path to the folder where spacecreator was build')
    parser.add_argument('--env_dir', dest='env_dir', type=str, required=True,
                        help='Path to the folder that contains the build environment')
    args = parser.parse_args()

    if args.project_dir:
        project_dir = args.project_dir
        print("Project dir is {}".format(project_dir))
    else:
        project_dir = default_project_dir
        print("Defaulting to project dir {}".format(project_dir))

    if args.build_dir:
        build_dir = args.build_dir
        print("build dir is {}".format(build_dir))
    else:
        build_dir = join_dir(project_dir, 'build')
        print("Defaulting to build dir {}".format(build_dir))

    env_dir = args.env_dir
    plugin_build_dir = join_dir(build_dir, 'lib', 'qtcreator', 'plugins')
    plugin_install_dir = join_dir(env_dir, 'spacecreator.AppDir', 'lib', 'qtcreator', 'plugins')
    copy_plugins_to_plugin_dir(plugin_build_dir, plugin_install_dir)

    wizards_dir = join_dir(project_dir, 'wizards')
    wizards_install_dir = join_dir(env_dir, 'spacecreator.AppDir', 'share', 'qtcreator', 'templates', 'wizards')
    copy_wizards(wizards_dir, wizards_install_dir)
