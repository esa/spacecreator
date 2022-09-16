TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES +=  $(HOME)/tool-inst/share/taste-types/taste-types.asn \
    deploymentview.dv.xml
DISTFILES += Spaceman.msc
DISTFILES += interfaceview.xml
DISTFILES += work/binaries/*.msc
DISTFILES += work/binaries/coverage/index.html
DISTFILES += work/binaries/filters

DISTFILES += Spaceman.asn
DISTFILES += Spaceman.acn
include(work/taste.pro)
message($$DISTFILES)

