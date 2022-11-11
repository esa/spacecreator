#!/usr/bin/env python3

import argparse
import os.path
import subprocess
import utils
from utils import join_dir, copy_content_of_dir_to_other_dir, print_cmd
from utils import start_time, delta_time

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


def copy_ans1scc_fuzzer(build_dir: str, app_dir: str) -> None:
    """
    This method copies asn1scc-fuzzer tool from build_dir/asn1scc_bin/libexec to the app_dir.
    Ideally this method should not be needed as project_dir/asn1scc/CMakeFile.txt should install it.

    :param build_dir:
    :param app_dir:
    """
    build_libexec_dir = join_dir(build_dir, 'asn1scc_bin', 'libexec')
    app_libexec_dir = join_dir(app_dir, 'libexec')
    print("postbuild.py: Copying ans1scc-fuzzer from {} to {}".format(build_libexec_dir, app_libexec_dir))
    utils.copy_content_of_dir_to_other_dir(build_libexec_dir, app_libexec_dir)


def copy_asn1acn(build_dir: str, app_dir: str) -> None:
    """
    This method copies asn1acn lib from build_dir/asn1scc_bin/share to the app_dir.
    Ideally this method should not be needed as project_dir/asn1scc/CMakeFile.txt should install it.

    :param build_dir:
    :param app_dir:
    """
    build_share_dir = join_dir(build_dir, 'asn1scc_bin', 'share')
    app_share_dir = join_dir(app_dir, 'share')
    print("postbuild.py: Copying ans1acn from {} to {}".format(build_share_dir, app_share_dir))
    utils.copy_content_of_dir_to_other_dir(build_share_dir, app_share_dir)


if __name__ == '__main__':
    start_time = start_time()
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

    # Copy applications from build tree to AppDir tree
    copy_applications(build_dir, app_dir)

    # Copy plugins from build tree to AppDir tree
    build_plugins_dir = join_dir(build_dir, 'lib', 'qtcreator', 'plugins')
    app_dir_plugins_dir = join_dir(app_dir, 'lib', 'qtcreator', 'plugins')
    copy_plugins(build_plugins_dir, app_dir_plugins_dir)

    # ToDo: These two methods should be made superfluous
    copy_ans1scc_fuzzer(build_dir, app_dir)
    copy_asn1acn(build_dir, app_dir)

    print("post_build.py: Total time was: {}".format(delta_time(start_time)))




