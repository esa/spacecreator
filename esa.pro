ROOT_DIR = $$PWD
include(esa.pri)

TEMPLATE = subdirs

SUBDIRS += \
    doc \
    libs \
    msceditor \
    qtcreator \

msceditor.depends = libs
qtcreator.depends = libs

CONFIG(no_tests) {
} else {
    SUBDIRS += tests
    tests.depends = libs
}
