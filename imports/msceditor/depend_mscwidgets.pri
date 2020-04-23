isEmpty(MSC_WIDGETS_PRI_INCLUDED) {
    MSC_WIDGETS_PRI_INCLUDED = 1

    include(depend_conf_utils.pri)

    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    BUILD_TREE = $$clean_path($$OUT_PWD)
    BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,

    OUT_PWD_BASE = $$BUILD_TREE/libs/mscwidgets
    win32: LIBS += -L$$clean_path($$OUT_PWD_BASE/$$BuildType/) -lmscwidgets
    else:unix: LIBS += -L$$clean_path($$OUT_PWD_BASE/) -lmscwidgets

    INCLUDEPATH += $$PWD/libs/mscwidgets
    DEPENDPATH += $$PWD/libs/mscwidgets

    win32:PRE_TARGETDEPS += $$OUT_PWD_BASE/$$BuildType/$${LibPrefix}mscwidgets.$$LibSuffix
    else:unix: PRE_TARGETDEPS += $$OUT_PWD_BASE/$${LibPrefix}mscwidgets.$$LibSuffix

    include(depend_msclibrary.pri)
    include(depend_asn1library.pri)
} else {
#    message("depend_mscwidgets.pri already included")
}
