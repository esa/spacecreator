TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES += paramlessProcedure.asn
DISTFILES += interfaceview.xml
DISTFILES += work/binaries/*.msc
DISTFILES += work/binaries/coverage/index.html
DISTFILES += work/binaries/filters
include(work/taste.pro)
message($$DISTFILES)

