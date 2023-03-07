TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES +=  $(HOME)/tool-inst/share/taste-types/taste-types.asn
DISTFILES += sporadic_many_to_one_observer.msc
DISTFILES += interfaceview.xml
DISTFILES += work/binaries/*.msc
DISTFILES += work/binaries/coverage/index.html
DISTFILES += work/binaries/filters
DISTFILES += work/system.asn

DISTFILES += sporadic_many_to_one_observer.asn
DISTFILES += sporadic_many_to_one_observer.acn
include(work/taste.pro)
message($$DISTFILES)

