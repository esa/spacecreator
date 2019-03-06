ROOT_DIR = $$PWD
include(esa.pri)

TEMPLATE = subdirs

SUBDIRS += \
    doc \
    libs \
    msceditor \
    qtcreator \
    tests

msceditor.depends = libs
qtcreator.depends = libs
tests.depends = libs
