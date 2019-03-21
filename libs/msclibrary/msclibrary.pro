TEMPLATE = lib
TARGET = msclibrary

include(../../esa.pri)
include(../../depend_asn1library.pri)
include(../../suppress_antlr_wrn.pri)

CONFIG += static
CONFIG += no_keywords

QT += core gui

HEADERS += \
    mscparameterlist.h \
    parser/MscBaseVisitor.h \
    parser/MscLexer.h \
    parser/MscParser.h \
    parser/MscVisitor.h \
    exceptions.h \
    mscaction.h \
    mscchart.h \
    mscdocument.h \
    mscentity.h \
    mscerrorlistener.h \
    msccondition.h \
    msccoregion.h \
    mscfile.h \
    mscgate.h \
    mscinstance.h \
    mscinstanceevent.h \
    mscmodel.h \
    mscmessage.h \
    mscparservisitor.h \
    msctimer.h \
    mscwriter.h \
    msccreate.h \
    cif/cifparser.h \
    cif/cifline.h \
    parserdebughelper_p.h \
    cif/cifblockfactory.h \
    cif/ciflines.h \
    cif/cifblock.h \
    cif/cifblocks.h

SOURCES += \
    mscparameterlist.cpp \
    parser/MscBaseVisitor.cpp \
    parser/MscLexer.cpp \
    parser/MscParser.cpp \
    parser/MscVisitor.cpp \
    exceptions.cpp \
    mscaction.cpp \
    mscchart.cpp \
    mscdocument.cpp \
    mscentity.cpp \
    mscerrorlistener.cpp \
    msccondition.cpp \
    msccoregion.cpp \
    mscfile.cpp \
    mscgate.cpp \
    mscinstance.cpp \
    mscinstanceevent.cpp \
    mscmessage.cpp \
    mscmodel.cpp \
    mscparservisitor.cpp \
    msctimer.cpp \
    mscwriter.cpp \
    msccreate.cpp \
    cif/cifparser.cpp \
    cif/cifline.cpp \
    parserdebughelper_p.cpp \
    cif/cifblockfactory.cpp \
    cif/ciflines.cpp \
    cif/cifblock.cpp \
    cif/cifblocks.cpp

antlrGen.target = antlr4-gen
ANTLR_RUNTIME_PATH=$$shell_path("$$_PRO_FILE_PWD_\..\..\3rdparty\antlr\cpp_runtime\runtime")

win32 {
    INCLUDEPATH += $$shell_path($$ANTLR_RUNTIME_PATH/src)
    antlrGen.commands = $$PWD/../../scripts/create_msc_parser.bat

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
    antlrGen.commands = $$clean_path(cd $$PWD/../../) ; $$clean_path(./scripts/create_msc_parser.sh)
    # Not using INCLUDEPATH, as the warnings should be disabled to the ANTLR runtime
    QMAKE_CXXFLAGS += -isystem $$clean_path($$ANTLR_RUNTIME_PATH/src)
}

message("Creating parser out of antlr grammar file")
system($$antlrGen.commands)

DISTFILES += \
    parser/Msc.g4
