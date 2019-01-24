!isEmpty(ASN1_PRI_INCLUDED):error("depend_asn1library.pri already included")
ASN1_PRI_INCLUDED = 1

sub_dir = $$_PRO_FILE_PWD_
sub_dir ~= s,^$$re_escape($$PWD),,
BUILD_TREE = $$clean_path($$OUT_PWD)
BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,

BuildType = Release
CONFIG(debug, debug|release) {
    BuildType = Debug
} # Profiling, etc (if any) are defaulted to Release
BuildTypeLower = $$lower($$BuildType)

OUT_PWD_BASE = $$BUILD_TREE/libs/asn1editor
win32: LIBS += -L$$clean_path($$OUT_PWD_BASE/$$BuildTypeLower/) -lasn1editor
else:unix: LIBS += -L$$clean_path($$OUT_PWD_BASE/) -lasn1editor

INCLUDEPATH += $$PWD/libs/asn1editor
DEPENDPATH += $$PWD/libs/asn1editor

LibSuffix = a
LibPrefix = lib

win32:!win32-g++ {
    LibSuffix = lib
    LibPrefix = ""
}

win32:PRE_TARGETDEPS += $$OUT_PWD_BASE/$$BuildTypeLower/$${LibPrefix}asn1editor.$$LibSuffix
else:unix: PRE_TARGETDEPS += $$OUT_PWD_BASE/$${LibPrefix}asn1editor.$$LibSuffix
