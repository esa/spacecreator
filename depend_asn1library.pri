!isEmpty(ASN1_PRI_INCLUDED):error("depend_asn1library.pri already included")
ASN1_PRI_INCLUDED = 1

include(depend_conf_utils.pri)

sub_dir = $$_PRO_FILE_PWD_
sub_dir ~= s,^$$re_escape($$PWD),,
BUILD_TREE = $$clean_path($$OUT_PWD)
BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,


OUT_PWD_BASE = $$BUILD_TREE/libs/asn1editor
win32: LIBS += -L$$clean_path($$OUT_PWD_BASE/$$BuildType/) -lasn1editor
else:unix: LIBS += -L$$clean_path($$OUT_PWD_BASE/) -lasn1editor

INCLUDEPATH += $$PWD/libs/asn1editor
DEPENDPATH += $$PWD/libs/asn1editor

win32:PRE_TARGETDEPS += $$OUT_PWD_BASE/$$BuildType/$${LibPrefix}asn1editor.$$LibSuffix
else:unix: PRE_TARGETDEPS += $$OUT_PWD_BASE/$${LibPrefix}asn1editor.$$LibSuffix
