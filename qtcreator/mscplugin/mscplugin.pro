include(../../esa.pri)
#include(../../depend_asn1library.pri)
#include(../../depend_mscwidgets.pri)
#include(../../depend_msclibrary.pri)


INCLUDEPATH += \
    $$PWD/../../libs/asn1editor \
    $$PWD/../../libs/mscwidgets \
    $$PWD/../../libs/msclibrary


DEPENDPATH += \
    $$PWD/../../libs/asn1editor \
    $$PWD/../../libs/mscwidgets \
    $$PWD/../../libs/msclibrary


BuildType = release
CONFIG(debug, debug|release) {
    BuildType = debug
} # Profiling, etc (if any) are defaulted to Release

LibSuffix = a
LibPrefix = lib

win32:!win32-g++ {
    LibSuffix = lib
    LibPrefix = ""
}

win32 {
    PRE_TARGETDEPS += \
        $$OUT_PWD/../../libs/asn1editor/$$BuildType/$${LibPrefix}asn1editor.$$LibSuffix \
        $$OUT_PWD/../../libs/mscwidgets/$$BuildType/$${LibPrefix}mscwidgets.$$LibSuffix \
        $$OUT_PWD/../../libs/msclibrary/$$BuildType/$${LibPrefix}msclibrary.$$LibSuffix

    LIBS += \
        -L$$OUT_PWD/../../libs/asn1editor/$$BuildType/ -lasn1editor \
        -L$$OUT_PWD/../../libs/mscwidgets/$$BuildType/ -lmscwidgets \
        -L$$OUT_PWD/../../libs/msclibrary/$$BuildType/ -lmsclibrary

    QMAKE_LFLAGS_RELEASE += /LTCG
} else:unix {
    PRE_TARGETDEPS += \
        $$OUT_PWD/../../libs/asn1editor/$${LibPrefix}asn1editor.$$LibSuffix \
        $$OUT_PWD/../../libs/mscwidgets/$${LibPrefix}mscwidgets.$$LibSuffix \
        $$OUT_PWD/../../libs/msclibrary/$${LibPrefix}msclibrary.$$LibSuffix

    LIBS += \
        -L$$OUT_PWD/../../libs/asn1editor/ -lasn1editor \
        -L$$OUT_PWD/../../libs/msclibrary/ -lmsclibrary \
        -L$$OUT_PWD/../../libs/mscwidgets/ -lmscwidgets
}

# Add ANTLR runtime
include("../../link_antlr_rt.pri")



DEFINES += MSCPLUGIN_LIBRARY

# MscPlugin files

SOURCES += \
        mscpluginplugin.cpp

HEADERS += \
        mscpluginplugin.h \
        mscplugin_global.h \
        mscpluginconstants.h

# Qt Creator linking

## Either set the IDE_SOURCE_TREE when running qmake,
## or set the QTC_SOURCE environment variable, to override the default setting
isEmpty(IDE_SOURCE_TREE): IDE_SOURCE_TREE = $$(QTC_SOURCE)
isEmpty(IDE_SOURCE_TREE): IDE_SOURCE_TREE = "$$(HOME)/Qt/qt-creator"

## Either set the IDE_BUILD_TREE when running qmake,
## or set the QTC_BUILD environment variable, to override the default setting
isEmpty(IDE_BUILD_TREE): IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE): IDE_BUILD_TREE = "/opt/Qt/Tools/QtCreator"

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on OS X
# !isEmpty($$(QTC_DEPLOY_USER_DIR)): USE_USER_DESTDIR = yes
USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = MscPlugin
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$IDE_SOURCE_TREE/src/qtcreatorplugin.pri)
