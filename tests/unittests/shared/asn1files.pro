TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES += $$PWD/taps.asn
DISTFILES += $$PWD/taps.acn
DISTFILES += $$PWD/interfaceview.xml
DISTFILES += $(HOME)/tool-inst/share/taste-types/taste-types.asn
DISTFILES += $(HOME)/tool-inst/share/taste-types/taste-types.asn1

DISTFILES += \
    $$PWD/taps_multiline.asn \
    $$PWD/taps_multiline.acn \
    $$PWD/interfaceview_multiline.xml \
    $(HOME)/tool-inst/share/taste-types/taste-types_multiline.acn \
    $(HOME)/tool-inst/share/taste-types/taste-types_multiline.asn1

DISTFILES += $$PWD/taps_singleline.asn $$PWD/taps_singleline.acn $$PWD/interfaceview_singleline.xml $(HOME)/tool-inst/share/taste-types/taste-types_singleline.acn $(HOME)/tool-inst/share/taste-types/taste-types_singleline.asn1
