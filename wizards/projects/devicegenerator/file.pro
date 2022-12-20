TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES += %{IVFile}
DISTFILES += %{AsnFile}
DISTFILES += %{AcnFile}
DISTFILES += %{AsnConfigFile}
DISTFILES += %{AcnConfigFile}
DISTFILES += %{AsnPrivateDataFile}
DISTFILES += %{AcnPrivateDataFile}

include(work/taste.pro)
message($$DISTFILES)