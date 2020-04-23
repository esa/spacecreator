TEMPLATE = subdirs

SUBDIRS += \
    TASTEeditor \
    doc

!CONFIG(no_tests):SUBDIRS += tests
