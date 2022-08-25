import os.path
import os
import subprocess
import shutil


def join_dir(*subdir):
    return os.path.abspath(os.path.join(*subdir))


def print_cmd(cmd: list[str]) -> None:
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
    Copy all files and dirs in src_dir to dst_dir
    :param src_dir: source dir
    :param dst_dir: destination dir
    """
    for content in os.listdir(src_dir):
        if os.path.isdir(content):
            shutil.copytree(content, dst_dir)
        if os.path.isfile(content):
            shutil.copy2(content, dst_dir)
