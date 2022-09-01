import os.path
import os
import subprocess
import shutil


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
