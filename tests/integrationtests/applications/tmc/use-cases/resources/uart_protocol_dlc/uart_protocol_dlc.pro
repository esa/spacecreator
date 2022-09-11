TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES +=  $(HOME)/tool-inst/share/taste-types/taste-types.asn \
    data_model/basic_types.asn \
    data_model/bilevel_types.asn \
    data_model/uart_dlc.asn
DISTFILES += uart_protocol_dlc.asn
DISTFILES += uart_protocol_dlc.acn
DISTFILES += uart_protocol_dlc.msc
DISTFILES += interfaceview.xml
DISTFILES += work/binaries/*.msc
DISTFILES += work/binaries/coverage/index.html
DISTFILES += work/binaries/filters
#include(handleAsn1AcnBuild.pri)
include(work/taste.pro)
message($$DISTFILES)

