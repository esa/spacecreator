TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES += %{AsnFile}
DISTFILES += %{MSCFile}
DISTFILES += %{DVFile}
DISTFILES += %{IVFile}

include(handleAsn1AcnBuild.pri)
