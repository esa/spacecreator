#!/usr/bin/env python3
import argparse
import subprocess
import os
import re
import platform
import urllib.request

from utils import join_dir, print_cmd


def download_app_image_tool(env_dir: str) -> None:
    app_image_tool = join_dir(env_dir, 'appimagetool')
    if os.path.exists(app_image_tool):
        print('No need to download appimagetool, found in ', app_image_tool)
        return

    def detect_architecture() -> str:
        uname_machine = platform.uname().machine
        if uname_machine in ['x86_64', 'amd64']:
            return 'x86_64'
        if re.search("i?86", uname_machine):
            return 'i686'
        if uname_machine in ['unknown', 'AuthenticAMD', 'GenuineIntel']:
            return ''

    arch = detect_architecture()
    url = "https://github.com/AppImage/AppImageKit/releases/download/13/appimagetool-{}.AppImage".format(arch)

    app_image_tool = join_dir(env_dir, 'appimagetool')
    print('Downloading {} to {}'.format(url, app_image_tool))
    try:
        urllib.request.urlretrieve(url, app_image_tool)  # download qtcreator.7z to the root of the env folder
    except:
        print("Could not download AppImage tool from {}".format(url))
        exit(4)
    os.chmod(app_image_tool, 0o0755)


def create_app_image(appimagetool_dir: str, app_dir: str, output_dir: str, version: str):
    """
    Creates an AppImage file from a directory with the
    :param appimagetool_dir: Dir in which to find the AppImage-tool
    :param app_dir: Dir with the content to turn into an AppImage
    :param output_dir: Where to put the resulting AppImage
    :param version: The version of SpaceCreator to make an AppImage out of
    """
    app_image_tool = join_dir(appimagetool_dir, 'appimagetool')
    app_image = join_dir(output_dir, 'spacecreator-x86_64-{}.AppImage'.format(version))
    options = []
    # This is useful when builing spacecreator in a Dockerfile
    options.append('--appimage-extract-and-run')
    app_image_tool_cmd = [app_image_tool, *options, app_dir, app_image]
    print_cmd(app_image_tool_cmd)

    completed_process = subprocess.run(app_image_tool_cmd, env={'ARCH': 'x86_64'})
    if not completed_process.returncode == 0:
        print("AppImage tool failed")
        exit(1)


def main():
    script_dir = os.path.dirname(os.path.realpath(__file__))
    default_project_dir = join_dir(script_dir, '..')

    # Parse arguments
    parser = argparse.ArgumentParser(prog='create_appimage.py')
    parser.add_argument('--project_dir', dest='project_dir', type=str, required=False,
                        help='Path to the folder where spacecreator project is')
    parser.add_argument('--app_dir', dest='app_dir', type=str, required=True,
                        help='Path to the folder that contains AppDir')
    parser.add_argument('--env_dir', dest='env_dir', type=str, required=True,
                        help='Path to the folder that contains the build environment')
    parser.add_argument('--project_version', dest='version', type=str, required=False,
                        help='Version number of spacecreator in the format X.Y.Z')
    parser.add_argument('--output_dir', dest='output_dir', type=str, required=False,
                        help='Path to the folder where the')
    args = parser.parse_args()

    if args.project_dir:
        project_dir = args.project_dir
        print("Project dir is {}".format(project_dir))
    else:
        project_dir = default_project_dir
        print("Defaulting to project dir {}".format(project_dir))

    if args.version:
        version = args.version
        print("Version is {}".format(version))
    else:
        version = '0.0.0'
        print("Defaulting to version 0.0.0")

    if args.output_dir:
        output_dir = args.output_dir
        print("Output dir is {}".format(output_dir))
    else:
        output_dir = join_dir(project_dir, '..')
        print("Defaulting to output dir {}".format(output_dir))

    app_dir = args.app_dir
    env_dir = args.env_dir

    download_app_image_tool(env_dir)
    create_app_image(env_dir, app_dir, output_dir, version)


if __name__ == '__main__':
    main()
