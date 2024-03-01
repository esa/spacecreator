TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES +=  $(HOME)/tool-inst/share/taste-types/taste-types.asn
DISTFILES += MscWhenThenNotSequenceParametersFailure.msc
DISTFILES += interfaceview.xml
DISTFILES += work/binaries/*.msc
DISTFILES += work/binaries/coverage/index.html
DISTFILES += work/binaries/filters
DISTFILES += work/system.asn

DISTFILES += MscWhenThenNotSequenceParametersFailure.asn
DISTFILES += MscWhenThenNotSequenceParametersFailure.acn
include(work/taste.pro)
message($$DISTFILES)

