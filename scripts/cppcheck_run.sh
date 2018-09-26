#!/bin/sh

CHECK_DIRS="libs/msclibrary/*.cpp libs/msclibrary/*.h libs/mscwidgets msceditor tests"
CHECKS="warning,style,performance,portability,information"

cppcheck -j4 --error-exitcode=1 --quiet \
    --language=c++ \
    --std=c++14  \
    --library=qt \
    --enable=$CHECKS  \
    --suppress=*:libs/msclibrary/parser/*.h \
    $CHECK_DIRS
