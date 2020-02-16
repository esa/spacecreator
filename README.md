# taste3

ESA's TASTE (https://taste.tools/) evolution (v3?)

It is required Grantlee as a Qt implementation of string template engine of Django syntax.
Sources of Grantlee are avalable from here https://github.com/steveire/grantlee
To build Grantlee, you need at least Qt5.3 (with development packages), CMake 3.1 and
some additional packages:

    sudo apt install lcov cccc doxygen cmake

The current Grantlee release (v.5.2.0) has a problem with escape sequences.
Therefore you must apply a patch before:

    cp <TASTE_DIR>/grantlee/fix_escape.diff <GRANTLEE_DIR>
    cd <GRANTLEE_DIR>
    git apply grantlee.diff

To build Taste correctly Grantlee should be installed into QTDIR directory:

    export QTDIR=$(qmake -query QT_INSTALL_PREFIX)
    export CMAKE_PREFIX_PATH=$QTDIR:$CMAKE_PREFIX_PATH

    mkdir build
    cd build

    cmake -DCMAKE_INSTALL_PREFIX=$QTDIR ..
    cmake --build .
    cmake --build . --target install
