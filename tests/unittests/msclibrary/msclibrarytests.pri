BuildType = Release
CONFIG(debug, debug|release) {
    BuildType = Debug
} # Profiling, etc (if any) are defaulted to Release
BuildTypeLower = $$lower($$BuildType)

OUT_PWD_BASE = $$OUT_PWD/../../../../libs/msclibrary
win32: LIBS += -L$$clean_path($$OUT_PWD_BASE/$$BuildTypeLower/) -lmsclibrary
else:unix: LIBS += -L$$clean_path($$OUT_PWD_BASE/) -lmsclibrary

INCLUDEPATH += $$PWD/../../../libs/msclibrary
DEPENDPATH += $$PWD/../../../libs/msclibrary

LibSuffix = a
LibPrefix = lib

win32:!win32-g++ {
    LibSuffix = lib
    LibPrefix = ""
}

win32:PRE_TARGETDEPS += $$OUT_PWD_BASE/$$BuildTypeLower/$$(LibPrefix)msclibrary.$$LibSuffix
else:unix: PRE_TARGETDEPS += $$OUT_PWD_BASE/$$(LibPrefix)msclibrary.$$LibSuffix

# Add ANTLR runtime
include("$$PWD/../../../link_antlr_rt.pri")
