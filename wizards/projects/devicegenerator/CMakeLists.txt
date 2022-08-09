cmake_minimum_required(VERSION 2.8)

project(%{JS: '%{ProjectName}'})

set(IVSOURCES %{IVFile})

include(asn1sccSettings.cmake)
include(createFilesList.cmake)

set(SOURCES ${ASN1ACNSOURCES} ${ASN1SCC_PRODUCTS} ${IVSOURCES})

if(SOURCES)
    set(TARGET_NAME ${PROJECT_NAME})
    add_library(${TARGET_NAME} ${SOURCES})
endif()

include(configureAsn1SccTarget.cmake)