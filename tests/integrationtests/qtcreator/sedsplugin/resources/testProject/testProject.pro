TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES += testProject.asn
DISTFILES += testProject.acn
DISTFILES += testProject.msc
DISTFILES += interfaceview.xml
DISTFILES += work/binaries/*.msc
DISTFILES += work/binaries/coverage/index.html
DISTFILES += work/binaries/filters
#include(handleAsn1AcnBuild.pri)
include(work/taste.pro)
message($$DISTFILES)

