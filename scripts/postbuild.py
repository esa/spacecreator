#!/usr/bin/env python3

import argparse
import os.path
import subprocess
import utils
from utils import join_dir, copy_content_of_dir_to_other_dir, print_cmd

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
 %{ActiveProject:Path}/scripts/postbuild.py --project_dir %{ActiveProject:Path} --build_dir %{buildDir} --env_dir /home/<user>/opt/spacecreatorenv6/
 
On the command line:
python3 ./scripts/postbuild.py --project_dir ~/projects/spacecreator --build_dir ~/projects/spacecreator/build --env_dir ~/opt/spacecreatorenv6
"""


def copy_wizards(wizards_dir: str, wizards_install_dir: str) -> None:
    if not os.path.exists(wizards_dir):
        print("postbuild.py: Could not find wizards dir: {}". format(wizards_dir))
        exit(1)
    if not os.path.exists(wizards_install_dir):
        print("postbuild.py: Could not find wizards install dir: {}".format(wizards_install_dir))
        exit(2)

    # File wizards
    files_dir = join_dir(wizards_dir, 'files')
    files_install_dir = join_dir(wizards_install_dir, 'files')

    utils.copy_content_of_dir_to_other_dir(files_dir, files_install_dir)

    # Projects wizards
    projects_dir = join_dir(wizards_dir, 'projects')
    projects_install_dir = join_dir(wizards_install_dir, 'projects')
    print("postbuild.py: Copying wizards from {} to {}".format(wizards_dir, wizards_install_dir))
    utils.copy_content_of_dir_to_other_dir(projects_dir, projects_install_dir)


def copy_applications(build_dir: str, app_dir: str) -> None:
    if not os.path.exists(build_dir):
        print("postbuild.py: Could not find build dir: {}". format(build_dir))
        exit(1)
    if not os.path.exists(app_dir):
        print("postbuild.py: Could not find app dir: {}".format(app_dir))
        exit(2)

    build_bin_dir = join_dir(build_dir, 'bin')
    app_bin_dir = join_dir(app_dir, 'bin')
    print("postbuild.py: Copying applications from {} to {}".format(build_bin_dir, app_bin_dir))
    utils.copy_content_of_dir_to_other_dir(build_bin_dir, app_bin_dir)


if __name__ == '__main__':
    script_dir = os.path.dirname(os.path.realpath(__file__))
    default_project_dir = join_dir(script_dir, '..')

    # Parse arguments
    parser = argparse.ArgumentParser(prog='postbuild')

    parser.add_argument('--project_dir', dest='project_dir', type=str, required=False,
                        help='Path to the folder where spacecreator project is')
    parser.add_argument('--build_dir', dest='build_dir', type=str, required=False,
                        help='Path to the folder where spacecreator was build')
    parser.add_argument('--app_dir', dest='app_dir', type=str, required=True,
                        help='Path to the folder that contains AppDir')

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

    app_dir = args.app_dir

    # Copy plugins from build tree to AppDir tree
    build_plugins_dir = join_dir(build_dir, 'lib', 'qtcreator', 'plugins')
    app_dir_plugins_dir = join_dir(app_dir, 'lib', 'qtcreator', 'plugins')
    print("postbuild.py: Copying plugins from {} to {}".format(build_plugins_dir, app_dir_plugins_dir))
    utils.copy_content_of_dir_to_other_dir(build_plugins_dir, app_dir_plugins_dir)

    




