include(../../TASTEeditor_sources.pri)

TEMPLATE = app

INCLUDEPATH += ../

SOURCES += \
        ../xmlcommon.cpp \
        tst_aadlxmlreader.cpp

RESOURCES += \
    rsc.qrc

HEADERS += \
    ../xmlcommon.h

