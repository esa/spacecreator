TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES +=  $(HOME)/tool-inst/share/taste-types/taste-types.asn
DISTFILES += uart_protocol_dlc.msc
DISTFILES += interfaceview.xml
DISTFILES += work/binaries/*.msc
DISTFILES += work/binaries/coverage/index.html
DISTFILES += work/binaries/filters

DISTFILES += data_model/basic_types.asn
DISTFILES += data_model/bilevel_types.asn
DISTFILES += data_model/uart_dlc.asn
DISTFILES += uart_protocol_dlc.asn
DISTFILES += uart_protocol_dlc.acn
include(work/taste.pro)
message($$DISTFILES)

