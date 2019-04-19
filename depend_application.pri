isEmpty(APPLICATION_PRI_INCLUDED) {
    APPLICATION_PRI_INCLUDED = 1

    include(depend_conf_utils.pri)

    QT += core gui widgets

    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    BUILD_TREE = $$clean_path($$OUT_PWD)
    BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,

    OUT_PWD_BASE = $$BUILD_TREE/libs/application
    win32: LIBS += -L$$clean_path($$OUT_PWD_BASE/$$BuildType/) -lapplication
    else:unix: LIBS += -L$$clean_path($$OUT_PWD_BASE/) -lapplication

    INCLUDEPATH += $$PWD/libs/application
    DEPENDPATH += $$PWD/libs/application

    win32:PRE_TARGETDEPS += $$OUT_PWD_BASE/$$BuildType/$${LibPrefix}application.$$LibSuffix
    else:unix: PRE_TARGETDEPS += $$OUT_PWD_BASE/$${LibPrefix}application.$$LibSuffix

    include(depend_mscwidgets.pri)
    include(depend_msclibrary.pri)
    include(depend_asn1library.pri)
} else {
#    error("depend_application.pri already included")
}
