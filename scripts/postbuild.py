#!/usr/bin/env python3

import argparse
import os.path
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
 %{ActiveProject:Path}/scripts/postbuild.py --project_dir %{ActiveProject:Path} --build_dir %{buildDir} --env_dir /home/<user>/opt/spacecreatorenv6/
 
On the command line:
python3 ./scripts/postbuild.py --project_dir ~/projects/spacecreator --build_dir ~/projects/spacecreator/build --env_dir ~/opt/spacecreatorenv6
"""


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


def copy_plugins(build_plugins_dir: str, app_dir_plugins_dir: str) -> None:
    if not os.path.exists(build_plugins_dir):
        print("postbuild.py: Could not find build dir: {}". format(build_plugins_dir))
        exit(1)
    if not os.path.exists(app_dir_plugins_dir):
        print("postbuild.py: Could not find app dir: {}".format(app_dir_plugins_dir))
        exit(2)
    print("postbuild.py: Copying plugins from {} to {}".format(build_plugins_dir, app_dir_plugins_dir))
    utils.copy_content_of_dir_to_other_dir(build_plugins_dir, app_dir_plugins_dir)


def copy_wizards(wizards_dir: str, wizards_install_dir: str) -> None:
    if not os.path.exists(wizards_dir):
        print("postbuild.py: Could not find wizards dir: {}". format(wizards_dir))
        exit(1)

    # File wizards
    files_dir = join_dir(wizards_dir, 'files')
    files_install_dir = join_dir(wizards_install_dir, 'files')

    copy_content_of_dir_to_other_dir(files_dir, files_install_dir)

    # Projects wizards
    projects_dir = join_dir(wizards_dir, 'projects')
    projects_install_dir = join_dir(wizards_install_dir, 'projects')
    print("postbuild.py: Copying wizards from {} to {}".format(wizards_dir, wizards_install_dir))
    copy_content_of_dir_to_other_dir(projects_dir, projects_install_dir)


def copy_highlighter_files(generic_highlighter_dir: str, generic_highlighter_install_dir: str) -> None:
    if not os.path.exists(generic_highlighter_dir):
        print("prebuild.py: Could not find wizards dir: {}".format(generic_highlighter_dir))
        exit(1)
    print("prebuild.py: Copying generic highlighter files from {} to {}".format(generic_highlighter_dir, generic_highlighter_install_dir))
    copy_content_of_dir_to_other_dir(generic_highlighter_dir, generic_highlighter_install_dir)


def copy_snippets(snippets_dir: str, snippets_install_dir: str) -> None:
    if not os.path.exists(snippets_dir):
        print("prebuild.py: Could not find snippets dir {}".format(snippets_dir))
        exit(1)
    print("prebuild.py: Copying snippets from {} to {}".format(snippets_dir, snippets_install_dir))
    copy_content_of_dir_to_other_dir(snippets_dir, snippets_install_dir)


def main():
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
        print("postbuild.py: Project dir is {}".format(project_dir))
    else:
        project_dir = default_project_dir
        print("postbuild.py: Defaulting to project dir {}".format(project_dir))

    if args.build_dir:
        build_dir = args.build_dir
        print("postbuild.py: build dir is {}".format(build_dir))
    else:
        build_dir = join_dir(project_dir, 'build')
        print("postbuild.py: Defaulting to build dir {}".format(build_dir))

    app_dir = args.app_dir

    # Copy applications from build tree to AppDir tree
    copy_applications(build_dir, app_dir)

    # Copy plugins from build tree to AppDir tree
    build_plugins_dir = join_dir(build_dir, 'lib', 'qtcreator', 'plugins')
    app_dir_plugins_dir = join_dir(app_dir, 'lib', 'qtcreator', 'plugins')
    copy_plugins(build_plugins_dir, app_dir_plugins_dir)

    # Copy the wizards from source tree to AppDir tree
    wizards_dir = join_dir(project_dir, 'wizards')
    wizards_install_dir = join_dir(app_dir, 'share', 'qtcreator', 'templates', 'wizards')
    copy_wizards(wizards_dir, wizards_install_dir)

    # Copy syntax highlighter files from asn1plugin and spacecreatorplugin
    asn1plugin_generic_highlighter_dir = join_dir(project_dir, 'src', 'qtcreator', 'asn1plugin',
                                                  'generic-highlighter')
    scl_files_spacecreatorplugin_generic_highlighter_dir = join_dir(project_dir, 'src', 'qtcreator',
                                                                    'spacecreatorplugin', 'scl',
                                                                    'generic-highlighter')
    generic_highlighter_install_dir = join_dir(app_dir, 'share', 'qtcreator', 'generic-highlighter')
    copy_highlighter_files(asn1plugin_generic_highlighter_dir, generic_highlighter_install_dir)
    copy_highlighter_files(scl_files_spacecreatorplugin_generic_highlighter_dir, generic_highlighter_install_dir)

    # Copy snippets from asn1plugin
    snippets_dir = join_dir(project_dir, 'src', 'qtcreator', 'asn1plugin', 'snippets')
    snippets_install_dir = join_dir(app_dir, 'share', 'qtcreator', 'snippets')
    copy_snippets(snippets_dir, snippets_install_dir)


if __name__ == '__main__':
    main()
