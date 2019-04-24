include(../../esa.pri)
include(../../depend_application.pri)
include(../../depend_mscwidgets.pri)
include(../../depend_msclibrary.pri)
include(../../depend_asn1library.pri)

DEFINES += MSCPLUGIN_LIBRARY

# MscPlugin files

SOURCES += \
    mainwidget.cpp \
    msccontext.cpp \
    msceditordata.cpp \
    msceditordocument.cpp \
    msceditorfactory.cpp \
    msceditorstack.cpp \
    mscpluginplugin.cpp \
    msctexteditor.cpp \

HEADERS += \
    mainwidget.h \
    msccontext.h \
    msceditordata.h \
    msceditordocument.h \
    msceditorfactory.h \
    msceditorstack.h \
    mscplugin_global.h \
    mscpluginconstants.h \
    mscpluginplugin.h \
    msctexteditor.h \

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

QTC_PLUGIN_DEPENDS += \
    coreplugin

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$IDE_SOURCE_TREE/src/qtcreatorplugin.pri)
