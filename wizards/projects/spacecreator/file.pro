TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

%{JS: if (%{AddAsnFile}) 'DISTFILES += %{AsnRelativePath}/%{AsnFile}'}
%{JS: if (%{AddMSCFile}) 'DISTFILES += %{MSCRelativePath}/%{MSCFile}'}
%{JS: if (%{AddIVFile}) 'DISTFILES += %{IVRelativePath}/%{IVFile}'}

include(handleAsn1AcnBuild.pri)
