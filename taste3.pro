TEMPLATE = subdirs

SUBDIRS += \
    TASTEeditor

!CONFIG(no_tests) {
    SUBDIRS += tests
}
