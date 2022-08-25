TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES +=  $(HOME)/tool-inst/share/taste-types/taste-types.asn
DISTFILES += MscNeverSequenceBacktracking.msc
DISTFILES += interfaceview.xml
DISTFILES += work/binaries/*.msc
DISTFILES += work/binaries/coverage/index.html
DISTFILES += work/binaries/filters

DISTFILES += MscNeverSequenceBacktracking.asn
DISTFILES += MscNeverSequenceBacktracking.acn
include(work/taste.pro)
message($$DISTFILES)

