cmake_minimum_required(VERSION 2.8)

project(%{JS: '%{ProjectName}'})

set(ASN1ACNSOURCES %{AsnFile})
set(MSCSOURCES %{MSCFile})
set(IVSOURCES %{IVFile})

include(asn1sccSettings.cmake)
include(createFilesList.cmake)

set(SOURCES ${ASN1ACNSOURCES} ${ASN1SCC_PRODUCTS} ${MSCSOURCES} ${IVSOURCES} ${DVSOURCES})

if(SOURCES)
    set(TARGET_NAME ${PROJECT_NAME})
    add_executable(${TARGET_NAME})
    target_sources(${TARGET_NAME} PRIVATE ${SOURCES})
endif()

include(configureAsn1SccTarget.cmake)
