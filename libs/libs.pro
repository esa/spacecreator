TEMPLATE = subdirs

SUBDIRS = \
    msclibrary \
    mscwidgets \
    asn1editor 

mscwidgets.depends = msclibrary
