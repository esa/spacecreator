TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES +=  $(HOME)/tool-inst/share/taste-types/taste-types.asn
DISTFILES += dekker1.msc
DISTFILES += interfaceview.xml
DISTFILES += work/binaries/*.msc
DISTFILES += work/binaries/coverage/index.html
DISTFILES += work/binaries/filters

DISTFILES += deploymentview.dv.xml
DISTFILES += dekker1.asn
DISTFILES += dekker1.acn
include(work/taste.pro)
message($$DISTFILES)

