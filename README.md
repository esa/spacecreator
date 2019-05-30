# MSC-NG

New version of the MSC editor created by Viking SW

# Build instructions

There are several ways to build the software. For all of them, you need Qt and Java installed and the binaries added to your path.

## Generic build instructions with Qt Creator

This way is working on Linux with GCC, on Mac OS X with XCode, and on Windows with Visual Studio. For other build types, see below.

1. Checkout the sources in a directory.
2. Open the msceditor.pro in Qt Creator.
3. Set up the project to use a separate build directory (this is the default)
4. Build it and run it in Qt Creator.

## Make/GCC on Linux based build

This is the preferred way to build on Linux on the command line. This should work on Mac and Windows as well, but we don't guarantee it.

1. Check out the sources in a directory.
2. Create a build directory next to it with "mkdir build".
3. Run "cd build; qmake ../msceditor; make"

This will build the application. To run it, you need the Qt libraries in your library path and then just run "./msceditor"

## Visual Studio as IDE on Windows

The generic build on Windows uses Visual Studio as the compiler. If you prefer to use Visual Studio as the IDE as well, this is the way to do it.

1. Check out the sources.
2. Run "qmake -tp vc" in the source directory.
3. Open the generated msceditor.sln in Visual Studio.
4. Build and run in Visual Studio.

## Mac OS X

There is no special build for Mac. Use the generic build instructions. It's possible that the make based build works as well.
