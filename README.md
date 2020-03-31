# taste3

ESA's TASTE (https://taste.tools/) evolution (v3?)

Grantlee dependency
===================

It is required Grantlee as a Qt implementation of string template engine of Django syntax.
Sources of Grantlee are avalable from here https://github.com/steveire/grantlee

    git clone https://github.com/steveire/grantlee.git

Linux build
-----------

To build Grantlee, you need at least Qt5.3 (with development packages), CMake 3.1 and
some additional optional packages:

    sudo apt install cmake lcov cccc doxygen

The current Grantlee release (v.5.2.0) has a problem with escape sequences.
Therefore you must apply a patch before:

    cp <TASTE3_DIR>/grantlee/fix_escape.diff <GRANTLEE_DIR>
    cd <GRANTLEE_DIR>
    git apply fix_escape.diff

To build Taste3 correctly Grantlee should be installed into QTDIR directory:

    export QTDIR=$(qmake -query QT_INSTALL_PREFIX)
    export CMAKE_PREFIX_PATH=$QTDIR:$CMAKE_PREFIX_PATH

    mkdir build
    cd build

    cmake -DCMAKE_INSTALL_PREFIX=$QTDIR ..
    cmake --build .
    cmake --build . --target install

The last command can require root permissions. Then use the 'sudo' command as follows:

    sudo cmake --build . --target install

Windows build
-------------

Make sure cmake, qmake and Visual Studio are installed and avalable in PATH.
Following instructions require git bash.

    cp <TASTE3_DIR>/grantlee/fix_escape.diff <GRANTLEE_DIR>
    cd <GRANTLEE_DIR>
    git apply --ignore-space-change --ignore-whitespace fix_escape.diff

To build Taste3 correctly Grantlee should be installed into QTDIR directory:

    export QTDIR=$(qmake -query QT_INSTALL_PREFIX)
    export CMAKE_PREFIX_PATH=$QTDIR:$CMAKE_PREFIX_PATH

    mkdir build
    cd build

    cmake -DCMAKE_INSTALL_PREFIX=$QTDIR -A x64 ..
    cmake --build . --config Release
    cmake --build . --config Release --target install

Taste3 build
============

Install dependendcies
---------------------
Nothing to do if pre-built Qt is used.
Otherwise it is required to install QtSvg module from debian packages.

    sudo apt install libqt5svg5-dev

Clone sources
-------------

    git clone ssh://git@git.vikingsoftware.com:2222/esa/taste3.git

Build
-----

    mkdir build
    cd build
    qmake ../taste3
    make -j<number_of_build_threads>
