!isEmpty(MSC_WIDGETS_PRI_INCLUDED):error("depend_mscwidgets.pri already included")
MSC_WIDGETS_PRI_INCLUDED = 1

sub_dir = $$_PRO_FILE_PWD_
sub_dir ~= s,^$$re_escape($$PWD),,
BUILD_TREE = $$clean_path($$OUT_PWD)
BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,

BuildType = Release
CONFIG(debug, debug|release) {
    BuildType = Debug
} # Profiling, etc (if any) are defaulted to Release
BuildTypeLower = $$lower($$BuildType)

OUT_PWD_BASE = $$BUILD_TREE/libs/mscwidgets
win32: LIBS += -L$$clean_path($$OUT_PWD_BASE/$$BuildTypeLower/) -lmscwidgets
else:unix: LIBS += -L$$clean_path($$OUT_PWD_BASE/) -lmscwidgets

INCLUDEPATH += $$PWD/libs/mscwidgets
DEPENDPATH += $$PWD/libs/mscwidgets

LibSuffix = a
LibPrefix = lib

win32:!win32-g++ {
    LibSuffix = lib
    LibPrefix = ""
}

win32:PRE_TARGETDEPS += $$OUT_PWD_BASE/$$BuildTypeLower/$${LibPrefix}mscwidgets.$$LibSuffix
else:unix: PRE_TARGETDEPS += $$OUT_PWD_BASE/$${LibPrefix}mscwidgets.$$LibSuffix
