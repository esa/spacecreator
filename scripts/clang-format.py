#!/usr/bin/env python3

import os
import subprocess
import argparse
from fnmatch import fnmatch


def clang_format(root_dir: str) -> None:
    clang_format_cmd = ['clang-format' '-i']
    for path, subdirs, files in os.walk(root_dir):
        for name in files:
            if fnmatch(name, "*.h") or fnmatch(name, "*.cpp"):
                file = os.path.join(path, name)
                clang_format_cmd.append(file)
    print(clang_format_cmd)
    if len(clang_format_cmd) == 1:
        return
    completed_process = subprocess.run(clang_format_cmd)
    if not completed_process.returncode == 0:
        print("clang-format failed")
        exit(1)


if __name__ == '__main__':
    # Parse arguments
    parser = argparse.ArgumentParser(prog='clangformat')
    parser.add_argument('root_dir', type=str)

    args = parser.parse_args()
    root_dir = args.root_dir

    print("Root dir is ", root_dir)
    clang_format(root_dir)

