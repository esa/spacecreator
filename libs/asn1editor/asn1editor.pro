TEMPLATE = lib
TARGET = asn1editor

include(../../esa.pri)
include(../../suppress_antlr_wrn.pri)

CONFIG += static

QT += core gui widgets xml

HEADERS += \
    parser/ASNBaseVisitor.h \
    parser/ASNLexer.h \
    parser/ASNParser.h \
    parser/ASNVisitor.h \
    asn1editor.h \
    asn1file.h \
    asn1itemdelegate.h \
    asn1treeview.h \
    asn1xmlparser.h \
    asn1valueparser.h \
    asn1itemmodel.h \
    asn1const.h

SOURCES += \
    parser/ASNBaseVisitor.cpp \
    parser/ASNLexer.cpp \
    parser/ASNParser.cpp \
    parser/ASNVisitor.cpp \
    asn1editor.cpp \
    asn1file.cpp \
    asn1itemdelegate.cpp \
    asn1treeview.cpp \
    asn1xmlparser.cpp \
    asn1valueparser.cpp \
    asn1itemmodel.cpp

FORMS += asn1editor.ui

antlrGen.target = antlr4-gen
ANTLR_RUNTIME_PATH=$$shell_path("$$_PRO_FILE_PWD_\..\..\3rdparty\antlr\cpp_runtime\runtime")

win32 {
    INCLUDEPATH += $$shell_path($$ANTLR_RUNTIME_PATH/src)
    antlrGen.commands = $$PWD/../../scripts/create_asn1_parser.bat

    CONFIG(debug, debug|release) {
        antlrBin.targetBuild = Debug
    } else {
        antlrBin.targetBuild = Release
    }

    antlrBin.target = antlr4-rtl
    antlrBin.vcproj = $$shell_path($$ANTLR_RUNTIME_PATH/antlr4cpp-vs2017.vcxproj)
    #TODO: make it configurable:
    antlrBin.buildTool = "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/MSBuild/15.0/Bin/msbuild.exe"
    antlrBin.commands = \
    $$shell_quote($$antlrBin.buildTool) \
    $$antlrBin.vcproj /p:PlatformToolset=v141 /p:configuration=$$shell_quote($$antlrBin.targetBuild Static) /p:platform=x64

    QMAKE_EXTRA_TARGETS += antlrBin
    PRE_TARGETDEPS += $$antlrBin.target
} else { # It's the same for Linux & Mac
    antlrGen.commands = $$clean_path(cd $$PWD/../../) ; $$clean_path(./scripts/create_asn1_parser.sh)
    # Not using INCLUDEPATH, as the warnings should be disabled to the ANTLR runtime
    QMAKE_CXXFLAGS += -isystem $$clean_path($$ANTLR_RUNTIME_PATH/src)
}

message("Creating parser out of antlr grammar file")
system($$antlrGen.commands)

DISTFILES += \
    parser/ASN.g4
