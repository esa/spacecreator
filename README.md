# Dependency list

This repository depends on these parts:

* Qt
* The Qt SVG module if it did not come with Qt (on Debian: sudo apt install libqt5svg5-dev)
* Grantlee (see below)
* Java SE 8
* On Windows: Visual Studio 2017 or 2019
* CMake
* ASN1 compiler (binary build from https://github.com/ttsiodras/asn1scc/releases)

Custom Qt Creator build (for plugins) also needs:

* The QtScrip module (marked as deprecated now)
* clang-dev

Grantlee is a Qt implementation of string template engine of Django syntax.
Sources of Grantlee are avalable from here https://github.com/steveire/grantlee

    git clone https://github.com/steveire/grantlee.git

### Tested version combinations

On Windows, the build is tested with Qt 5.14.1 (binary msvc_2017_64 package), Visual Studio 2019, cmake 3.17.1 and jdk 8.0.211.

On Linux: TODO.

#### Grantlee Linux build

To build Grantlee, you need at least Qt5.3 (with development packages), CMake 3.1 and
some additional optional packages:

    sudo apt install cmake lcov cccc doxygen

The current Grantlee release (v.5.2.0) has a problem with escape sequences.
Therefore you must apply a patch before:

    cp SpaceCreator/patches/grantlee_fix_escape.diff <GRANTLEE_DIR>
    cd <GRANTLEE_DIR>
    git apply grantlee_fix_escape.diff

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

#### Grantlee Windows build

Make sure cmake, qmake and Visual Studio are installed and avalable in PATH.
Following instructions require git bash.

    cp SpaceCreator/patches/grantlee_fix_escape.diff <GRANTLEE_DIR>
    cd <GRANTLEE_DIR>
    git apply --ignore-space-change --ignore-whitespace grantlee_fix_escape.diff

To build Taste3 correctly Grantlee should be installed into QTDIR directory (if CMAKE_PREFIX_PATH is already set, expand it instead of setting):

    set QTDIR=C:\Qt\5.14.1\msvc2017_64
    set CMAKE_PREFIX_PATH=%QTDIR%

    mkdir build
    cd build

    cmake -DCMAKE_INSTALL_PREFIX=%QTDIR% -A x64 ..
    cmake --build . --config Release
    cmake --build . --config Release --target install
    cmake --build . --config Debug
    cmake --build . --config Debug --target install

# SpaceCreator build

## Standalone applications

When working on SpaceCreator, just open the CMakeLists.txt in Qt Creator and run the build.

On the command line, do this:

    mkdir build-spacecreator
    cd build-spacecreator
    cmake ../SpaceCreator
    make -j`nproc` #<number_of_build_threads>

## Qt Creator plugins

### Using prebuilt binaries of QtC

TBD

### Using own build of QtC

```
git clone http://code.qt.io/qt-creator/qt-creator.git && \
cd qt-creator && \
git checkout 4.8 && \
git submodule init && \
git submodule update --recursive
```

1. Get the source code as shown above. Remember the `qt-creator' directory path, we'll need it at step 5;
1. Use the same Qt version for all projects (*SpaceCreator*, *QtC*, *Grantlee*). Ensure its **QtScript** module (deprecated now) is installed;
1. Install **clang** development files (`sudo apt install libclang-dev`);
1. build the QtC;
1. Setup build environment variables for the *SpaceCreator*: `QTC_SOURCE=/path/to/qt-creator/from/step/#1` and `QTC_INSTALL=/path/to/the/build/dir/from/step/#4`;
1. If CMake fails with "Could not detect project version number from..." error, clear CMake configuration and restart the QtC instance to fix it.

# Grantlee notes

The best source of documentation is in the grantlee website http://www.grantlee.org. Examples of use are found here: http://www.grantlee.org/apidox/examples.html.
The template language is documented in the Django framework https://docs.djangoproject.com/en/3.0/ref/templates/language.

**Steps to integrate the string template engine**<br>
The current implementation is based on the [Grantlee string template library](https://github.com/steveire/grantlee).
*  Build and install *Grantlee* (please look at README.md).
*  Add the next lines into your *.pro file:<br>
`GRANTLEE_LIB_DIR = $$[QT_INSTALL_LIBS]`<br>
`DEFINES += GRANTLEE_LIB_DIR='\\"$$GRANTLEE_LIB_DIR\\"'`<br>
`GRANTLEE_LIB=Grantlee_Templates`<br>
`win32: CONFIG(debug, debug|release):GRANTLEE_LIB = $$join(GRANTLEE_LIB,,,d)`<br>
`LIBS += -L$$GRANTLEE_LIB_DIR -l$$GRANTLEE_LIB`<br>
`macx: INCLUDEPATH += $$[QT_INSTALL_HEADERS]`
*  Add next files from *src/libs/libiveditor/templating/xml_templates/* to your project and set them in CMakeLists.txt file:<br>
`HEADERS += \`<br>
`        ...`<br>
`        templating/abstractexportableobject.h \`<br>
`        templating/exportableproperty.h \`<br>
`        templating/stringtemplate.h \`<br>
`        templating/templateeditor.h \`<br>
`        templating/templatehighlighter.h \`<br>
`        templating/xmlhighlighter.h \`<br>
`        templating/templatesyntaxhelpdialog.h`<br>
`SOURCES += \`<br>
`        ...`<br>
`        templating/stringtemplate.cpp \`<br>
`        templating/templateeditor.cpp \`<br>
`        templating/templatehighlighter.cpp \`<br>
`        templating/xmlhighlighter.cpp \`<br>
`        templating/templatesyntaxhelpdialog.cpp`
*  Write a successor of `AbstractExportableObject` as a lightweight wrapper to export your QObject-derived objects. Use implementation of `ExportableIVObject` and its successors as a reference.
*  Create exportable an object, cast it to `QVariant` and append it in `QHash<QString, QVariantList>` by using a `groupName()` as a key. Then invoke `StringTemplate::parseFile()` for parsing a template. Use implementation of `IVExporter` from *src/libs/libiveditor/* as a reference.

# Import of existing editors

## Import of MSC editor:

For the integration the tag 'gollum_integration' is used (hash: 'f34f255f5cf84adb93d1b3a92adb02df936a8835')

'git subtree add -P imports/msceditor ssh://git@git.vikingsoftware.com:2222/esa/msceditor.git gollum_integration'

## Import of Interface View editor:

For the integration the tag 'gollum_integration' is used (hash: '9a6f93b9c6dae7d0d2e5a276b6fb5a6370558cc5')

'git subtree add -P imports/iveditor ssh://git@git.vikingsoftware.com:2222/esa/taste3.git gollum_integration'

## Import of ASN1 editor:

For the integration the hash '752de71b9c9fea971da0cccccfba563325e17b7d' is used

'git subtree add -P imports/asn1scc.IDE https://github.com/n7space/asn1scc.IDE.git master'

# Development workflow

For easier work on the QtCreator plugins, add a custom deploy step and a custom target for execution like this:

![QtCreator_project_setup.png](doc/images/QtCreator_project_setup.png)

That way the target QtCreator is started and can be debugged directly.
