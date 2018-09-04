TEMPLATE = lib
TARGET = msclibrary

include(../../esa.pri)

CONFIG += static

HEADERS += \
    mscfile.h \
    parser/MscBaseVisitor.h \
    parser/MscLexer.h \
    parser/MscParser.h \
    parser/MscVisitor.h \
    mscparservisitor.h

SOURCES += \
    mscfile.cpp \
    parser/MscBaseVisitor.cpp \
    parser/MscLexer.cpp \
    parser/MscParser.cpp \
    parser/MscVisitor.cpp \
    mscparservisitor.cpp

message("Creating parser out of antlr grammar file")
linux {
    system(cd $$PWD/../../ ; ./scripts/create_msc_parser.sh)
}
win32 {
    system($$PWD/../../scripts/create_msc_parser.bat)
}
osx {
    system(cd $$PWD/../../ ; ./scripts/create_msc_parser.sh)
}

# Not using INCLUDEPATH, as the warnings should be disabled to the ANTLR runtime
QMAKE_CXXFLAGS += -isystem $$clean_path($$PWD/../../3rdparty/antlr/cpp_runtime/runtime/src)
