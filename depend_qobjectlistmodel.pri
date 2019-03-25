!isEmpty(QOBJECT_LIST_MODEL_PRI_INCLUDED):error("depend_qobjectlistmodel.pri already included")
QOBJECT_LIST_MODEL_PRI_INCLUDED = 1

include(depend_conf_utils.pri)

sub_dir = $$_PRO_FILE_PWD_
sub_dir ~= s,^$$re_escape($$PWD),,
BUILD_TREE = $$clean_path($$OUT_PWD)
BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,

OUT_PWD_BASE = $$BUILD_TREE/libs/qobjectlistmodel
win32: LIBS += -L$$clean_path($$OUT_PWD_BASE/$$BuildType/) -lqobjectlistmodel
else:unix: LIBS += -L$$clean_path($$OUT_PWD_BASE/) -lqobjectlistmodel

INCLUDEPATH += $$PWD/libs/qobjectlistmodel
DEPENDPATH += $$PWD/libs/qobjectlistmodel

win32:PRE_TARGETDEPS += $$OUT_PWD_BASE/$$BuildType/$${LibPrefix}qobjectlistmodel.$$LibSuffix
else:unix: PRE_TARGETDEPS += $$OUT_PWD_BASE/$${LibPrefix}qobjectlistmodel.$$LibSuffix
