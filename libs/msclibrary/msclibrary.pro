TEMPLATE = lib
TARGET = msclibrary

include(../../esa.pri)

CONFIG += static

HEADERS += \
    mscfile.h

SOURCES += \
    mscfile.cpp

linux {
    message("Creating parser out of antlr grammar file")
    system(cd $$PWD/../../ ; ./scripts/create_msc_parser.sh)
}
win32 {
    message("TBD!!! create parser out of antlr grammar file")
    system(cd $$PWD/../../ ; scripts/create_msc_parser.bat)
}
osx {
    message("Creating parser out of antlr grammar file")
    system(cd $$PWD/../../ ; ./scripts/create_msc_parser.sh)
}
