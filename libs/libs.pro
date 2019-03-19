TEMPLATE = subdirs

SUBDIRS = \
    antrl4runtime \
    asn1editor \
    msclibrary \
    mscwidgets \

asn1editor.depends = antrl4runtime
msclibrary.depends = antrl4runtime
msclibrary.depends = asn1editor
mscwidgets.depends = msclibrary
