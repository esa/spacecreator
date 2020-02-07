# taste3

ESA's TASTE (https://taste.tools/) evolution (v3?)

It is required Grantlee as a Qt implementation of string template engine of Django syntax.
Sources of Grantlee are avalable from here https://github.com/steveire/grantlee
To build Grantlee, you need at least Qt5.3 (with development packages), CMake 3.1 and
some additional packages:

    sudo apt install lcov cccc doxygen cmake

To build Taste correctly Grantlee should be installed into QTDIR directory:

    export QTDIR=$(qmake -query QT_INSTALL_PREFIX)
    export CMAKE_PREFIX_PATH=$QTDIR:$CMAKE_PREFIX_PATH

    mkdir build
    cd build

    cmake -DCMAKE_INSTALL_PREFIX=$QTDIR ..
    cmake --build .
    cmake --build . --target install
