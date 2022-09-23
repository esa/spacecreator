#!/usr/bin/env python3
import argparse
import subprocess
import tarfile
import os
import re
import platform
import urllib.request

from utils import join_dir, print_cmd, copy_content_of_dir_to_other_dir, ensure_dir, copy_file_pattern_to_dir


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


def create_app_dir(app_dir: str, project_dir: str, build_dir: str, env_dir: str, env_qt_dir: str) -> None:
    """
    Prepare a directory with the content needed to create an AppImage
    :param app_dir: Directory that will be packaged to an AppImage
    :param project_dir: Directory where spacecreator has been cloned
    :param build_dir: Directory where spacecreator was build
    :param env_dir:
    :param env_qt_dir:
    :return: None
    """
    # Copy spacecreator.AppDir from build environment
    ensure_dir(app_dir)
    app_dir_template_dir = join_dir(env_dir, 'spacecreator.AppDir')
    copy_content_of_dir_to_other_dir(app_dir_template_dir, app_dir)
    print('App dir created {}'.format(app_dir))
    # Asn1
    copy_content_of_dir_to_other_dir(join_dir(build_dir, 'bin'), join_dir(app_dir, 'bin'))
    copy_content_of_dir_to_other_dir(join_dir(build_dir, 'asn1scc_bin'), app_dir)
    copy_content_of_dir_to_other_dir(
        join_dir(project_dir, 'src', 'qtcreator', 'asn1plugin', 'generic-highlighter', 'syntax'),
        join_dir(app_dir, 'share', 'qtcreator', 'generic-highlighter')
    )
    copy_content_of_dir_to_other_dir(
        join_dir(project_dir, 'src', 'qtcreator', 'asn1plugin', 'snippets'),
        join_dir(app_dir, 'share', 'qtcreator', 'snippets')
    )
    # Grantlee templates from Grantlee build dir in environment dir
    grantlee_build_dir = join_dir(env_dir, 'build')
    grantlee_templates_pattern = join_dir(grantlee_build_dir, 'templates', 'lib', 'libGrantlee_Templates.*')
    copy_file_pattern_to_dir(grantlee_templates_pattern, join_dir(app_dir, 'lib', 'Qt', 'lib'))
 
    # Qt files from environment
    qt_web_sockets_pattern = join_dir(env_qt_dir, 'lib', 'libQt*WebSockets*')
    copy_file_pattern_to_dir(qt_web_sockets_pattern, join_dir(app_dir, 'lib', 'Qt', 'lib'))

    # AppImage files SpaceCreator.desktop and AppRun
    copy_content_of_dir_to_other_dir(join_dir(project_dir, 'install', 'appimage'), app_dir)

    # libzxb-util
    libzxbutil_tar_gz = join_dir(project_dir, 'install', 'libzxb-util.tar.gz')
    with tarfile.open(libzxbutil_tar_gz, 'r:gz') as libzxbutil_file:
        libzxbutil_file.extractall(join_dir(app_dir, 'lib'))


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
    app_image_tool_cmd = [app_image_tool, app_dir, app_image]
    print_cmd(app_image_tool_cmd)

    completed_process = subprocess.run(app_image_tool_cmd, env={'ARCH': 'x86_64'})
    if not completed_process.returncode == 0:
        print("AppImage tool failed")
        exit(1)


if __name__ == '__main__':
    script_dir = os.path.dirname(os.path.realpath(__file__))
    default_project_dir = join_dir(script_dir, '..')
    root_dir = join_dir(script_dir, '..', '..')

    # Parse arguments
    parser = argparse.ArgumentParser(prog='create_appimage.py',
                                     epilog='python3 ./scripts/create_appimage.py '
                                            '--project_dir /home/<user>/projects/spacecreator/ '
                                            '--build_dir /home/<user>/projects/spacecreator/build '
                                            '--env_dir /home/<user>/opt/spacecreatorenv6 '
                                            '--env_qt_dir /home/<user>/opt/qtcreatorenv/Qt/6.3.1/gcc_64/ '
                                            '--project_version 1.2.3')

    parser.add_argument('--project_dir', dest='project_dir', type=str, required=False,
                        help='Path to the folder where spacecreator project is')
    parser.add_argument('--build_dir', dest='build_dir', type=str, required=False,
                        help='Path to the folder where spacecreator was build')
    parser.add_argument('--env_dir', dest='env_dir', type=str, required=True,
                        help='Path to the folder that contains the build environment')
    parser.add_argument('--env_qt_dir', dest='env_qt_dir', type=str, required=True,
                        help='Path to the Qt distribution (./Qt/6.3.1/gcc_64/)')
    parser.add_argument('--project_version', dest='version', type=str, required=True,
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

    if args.build_dir:
        build_dir = args.build_dir
        print("build dir is {}".format(build_dir))
    else:
        build_dir = join_dir(project_dir, 'build')
        print("Defaulting to build dir {}".format(build_dir))

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
        output_dir = build_dir
        print("Defaulting to output dir {}".format(output_dir))

    env_dir = args.env_dir
    env_qt_dir = args.env_qt_dir
    version = args.version

    download_app_image_tool(env_dir)

    # AppDir
    app_dir = join_dir(root_dir, 'spacecreator.AppDir')
    create_app_dir(app_dir, project_dir, build_dir, env_dir, env_qt_dir)

    create_app_image(env_dir, app_dir, output_dir, version)
