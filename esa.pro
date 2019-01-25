ROOT_DIR = $$PWD
include(esa.pri)

TEMPLATE = subdirs

SUBDIRS += \
    doc \
    libs \
    msceditor \
    qtcreator \
    tests

CONFIG += ordered
