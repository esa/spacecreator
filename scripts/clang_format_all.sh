#!/bin/sh

find libs/ -iname *.cpp -o -iname *.h | xargs clang-format -style=file -i
find msceditor/ -iname *.cpp -o -iname *.h | xargs clang-format -style=file -i
find tests/ -iname *.cpp -o -iname *.h | xargs clang-format -style=file -i
