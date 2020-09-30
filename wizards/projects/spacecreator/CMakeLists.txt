cmake_minimum_required(VERSION 2.8)

project(%{JS: '%{ProjectName}'})

set(ASN1ACNSOURCES %{JS: if (%{AddAsnFile}) '%{AsnRelativePath}/%{AsnFile}'})
set(MSCSOURCES %{JS: if (%{AddMSCFile}) '%{MSCRelativePath}/%{MSCFile}'})
set(IVSOURCES %{JS: if (%{AddIVFile}) '%{IVRelativePath}/%{IVFile}'})

include(asn1sccSettings.cmake)
include(createFilesList.cmake)

set(SOURCES ${ASN1ACNSOURCES} ${ASN1SCC_PRODUCTS} ${MSCSOURCES} ${IVSOURCES})

if(SOURCES)
    set(TARGET_NAME ${PROJECT_NAME})
    add_executable(${TARGET_NAME})
    target_sources(${TARGET_NAME} ${SOURCES})
endif()

include(configureAsn1SccTarget.cmake)
