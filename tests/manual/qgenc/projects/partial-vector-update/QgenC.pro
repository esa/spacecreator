TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES +=  $(HOME)/tool-inst/share/taste-types/taste-types.asn \
    INTERFACEBASIC.acn \
    INTERFACEBASIC.asn \
    MATLAB-STANDARD-DATATYPES.acn \
    MATLAB-STANDARD-DATATYPES.asn \
    linux.dv.xml
DISTFILES += QgenC.msc
DISTFILES += interfaceview.xml
DISTFILES += work/binaries/*.msc
DISTFILES += work/binaries/coverage/index.html
DISTFILES += work/binaries/filters
DISTFILES += work/system.asn

DISTFILES += QgenC.asn
DISTFILES += QgenC.acn
include(work/taste.pro)
message($$DISTFILES)

