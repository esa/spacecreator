#!/bin/sh

CHECK_DIRS="libs/msclibrary/*.cpp libs/msclibrary/*.h libs/mscwidgets msceditor tests"
#EXCLUDE_DIRS="libs/msclibrary/parser"
CHECKS="warning,style,performance,portability,information"

cppcheck -j4 --enable=$CHECKS --language=c++ --error-exitcode=1 --std=c++14 --quiet \
    --suppress=*:libs/msclibrary/parser/*.h \
    $CHECK_DIRS
