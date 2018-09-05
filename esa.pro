ROOT_DIR = $$PWD
include(esa.pri)

TEMPLATE = subdirs

SUBDIRS += \
    doc \
    libs \
    msceditor \
    tests

CONFIG += ordered
