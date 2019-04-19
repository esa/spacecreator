TEMPLATE = subdirs

SUBDIRS = \
    antrl4runtime \
    asn1editor \
    qobjectlistmodel \
    msclibrary \
    mscwidgets \
    application \

msclibrary.depends = antrl4runtime
msclibrary.depends = asn1editor
msclibrary.depends = qobjectlistmodel
mscwidgets.depends = msclibrary
application.depends = mscwidgets

CONFIG += ordered
