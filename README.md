# MSC-NG

New version of the MSC editor created by Viking SW

# Build instructions

There are several ways to build the software. For all of them, you need Qt and Java installed and the binaries added to your path.

## Generic build instructions with Qt Creator

This way is working on Linux with GCC, on Mac OS X with XCode, and on Windows with Visual Studio. For other build types, see below.

1. Checkout the sources in a directory.
2. On Linux and Mac, run the script build_antlr.sh
3. Open the msceditor.pro in Qt Creator.
4. Set up the project to use a separate build directory (this is the default)
5. Build it and run it in Qt Creator.

## Make/GCC on Linux based build

This is the preferred way to build on Linux on the command line. This should work on Mac and Windows as well, but we don't guarantee it.

1. Check out the sources in a directory.
2. Run "cd msceditor; ./build_antlr.sh; cd .."
2. Create a build directory next to it with "mkdir build".
3. Run "cd build; qmake ../msceditor; make"

This will build the application. To run it, you need the Qt libraries in your library path and then just run "./msceditor"

## Visual Studio as IDE on Windows

The generic build on Windows uses Visual Studio as the compiler. If you prefer to use Visual Studio as the IDE as well, this is the way to do it.

1. Check out the sources.
2. Run "qmake -tp vc" in the source directory.
3. Open the generated msceditor.sln in Visual Studio.
4. Build and run in Visual Studio.

It's also possible to use nmake or jam on Windows if you prefer building from the command line, but this is not officially supported.

## Mac OS X

There is no special build for Mac. Use the generic build instructions. It's possible that the make based build works as well.

# Dependencies

This project only has two external dependencies.

## Qt

The Qt core library is used for the application itself. It is necessary for both building and at runtime. Qt Creator is usually used for building and development.

For building the API HTML documentation, Clang is necessary. Check [the Qt documentation page](https://doc.qt.io/qt-5/qdoc-guide-clang.html) for installation instructions.

The build has been tested with version 5.11.3 but any version from 5.11.0 to the end of version 5 should work.

## Java

Java SE is only used during the first build for creating the ANTLR parser.

This has been tested with version 8 update 211, but should work with many versions before or after.

## Compiler and build environment

Most decent C++ compilers should work. It has been tested with Visual Studio on Windows, GCC on Linux and XCode on Mac.

# API Documentation builds

After installing the necessary Clang llvm-config (see the Qt dependency above), CD to the source dir and just run this:

"qdoc doc/msceditor.qdocconf"
