import os.path
import os
import subprocess
import shutil
import re
import glob


def join_dir(*subdir):
    return os.path.abspath(os.path.join(*subdir))


def print_cmd(cmd) -> None:
    """
    Prints a list of strings using spaces as seperator.
    """
    print(' '.join(cmd))


def ensure_dir(dir_str: str) -> None:
    """
    Ensure the given dir exists and is empty
    :param dir_str:
    """
    if os.path.exists(dir_str):
        rm_dir = ['rm', '-rf', dir_str]
        print_cmd(rm_dir)
        return_code = subprocess.call(rm_dir)
        if return_code:
            exit(1)
    os.makedirs(dir_str, exist_ok=True)


def exit_if_not_exists(path: str):
    if not os.path.exists(path):
        print("Path {} does not exist. Exiting".format(path))
        exit(1)


def copy_content_of_dir_to_other_dir(src_dir: str, dst_dir: str) -> None:
    """
    Copy all files and dirs in src_dir to dst_dir. Creates dirs in dst_dir if need be.
    :param src_dir: source dir
    :param dst_dir: destination dir
    """
    def ensure_dst_dir(path: str) -> None:
        if not os.path.exists(path):
            os.makedirs(path)

    for item_name in os.listdir(src_dir):
        item = join_dir(src_dir, item_name)
        if os.path.isfile(item):
            shutil.copy2(item, dst_dir)
        elif os.path.isdir(item):
            dst_dir_item = join_dir(dst_dir, item_name)
            ensure_dst_dir(dst_dir_item)
            copy_content_of_dir_to_other_dir(item, dst_dir_item)
        else:
            print("Unknown content ", item)


def copy_file_pattern_to_dir(pattern: str, dst_dir: str) -> None:
    if not os.path.exists(dst_dir):
        os.makedirs(dst_dir)
    files = glob.glob(pattern, recursive=False)
    for file_to_copy in files:
        #print('copied {} to {}'.format(file_to_copy, dst_dir))
        shutil.copy2(file_to_copy, dst_dir)


def check_cmake_version(min_major: int, min_minor: int, min_patch: int) -> None:
    cmake_cmd = ['cmake', '--version']
    completed_process = subprocess.run(cmake_cmd, capture_output=True)
    if not completed_process.returncode == 0:
        print("Could not call CMake. Is it installed?")
        exit(1)
    output = completed_process.stdout.decode('utf-8')
    match = re.search("([0-9]+).([0-9]+).([0-9]+)", output)
    if not match:
        print("Could not find version number in ", output)
        exit(2)
    groups = match.groups()
    if not len(groups) == 3:
        print("Could not find version number in format X.Y.Z in ", output)
        exit(3)
    major = int(groups[0])
    minor = int(groups[1])
    patch = int(groups[2])
    minimum_version_required = "{}.{}.{}".format(min_major, min_minor, min_patch)
    actual_version = "{}.{}.{}".format(major, minor, patch)
    actual_version_to_small = min_major > major or min_minor > minor or min_patch > patch
    if actual_version_to_small:
        print("Found CMake version is {} but minimum required version is {}. Consult quickstart.md for how to upgrade CMake".format(actual_version, minimum_version_required))
        exit(4)
    print("Found CMake version {} which is greater than or equal to required version {}".format(actual_version, minimum_version_required))