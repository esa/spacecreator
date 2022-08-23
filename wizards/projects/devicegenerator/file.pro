TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES += %{IVFile}
DISTFILES += %{AsnFile}
DISTFILES += %{AcnFile}

include(work/taste.pro)
message($$DISTFILES)