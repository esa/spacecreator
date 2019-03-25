!isEmpty(MSC_LIBRARY_PRI_INCLUDED):error("depend_msclibrary.pri already included")
MSC_LIBRARY_PRI_INCLUDED = 1

include(depend_conf_utils.pri)

sub_dir = $$_PRO_FILE_PWD_
sub_dir ~= s,^$$re_escape($$PWD),,
BUILD_TREE = $$clean_path($$OUT_PWD)
BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,

OUT_PWD_BASE = $$BUILD_TREE/libs/msclibrary
win32: LIBS += -L$$clean_path($$OUT_PWD_BASE/$$BuildType/) -lmsclibrary
else:unix: LIBS += -L$$clean_path($$OUT_PWD_BASE/) -lmsclibrary

INCLUDEPATH += $$PWD/libs/msclibrary
DEPENDPATH += $$PWD/libs/msclibrary

win32:PRE_TARGETDEPS += $$OUT_PWD_BASE/$$BuildType/$${LibPrefix}msclibrary.$$LibSuffix
else:unix: PRE_TARGETDEPS += $$OUT_PWD_BASE/$${LibPrefix}msclibrary.$$LibSuffix

# Add ANTLR runtime
include("$$PWD/link_antlr_rt.pri")

# add qobjectlistmodel
include (depend_qobjectlistmodel.pri)
