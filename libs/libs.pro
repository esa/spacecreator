TEMPLATE = subdirs

SUBDIRS = \
    antrl4runtime \
    asn1editor \
    qobjectlistmodel \
    msclibrary \
    mscwidgets \

msclibrary.depends = antrl4runtime
msclibrary.depends = asn1editor
msclibrary.depends = qobjectlistmodel
mscwidgets.depends = msclibrary

CONFIG += ordered
