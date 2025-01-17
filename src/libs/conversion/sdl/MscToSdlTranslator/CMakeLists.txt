project(MscToSdlTranslator VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES     mscparametervalueparser.cpp
                tftable.cpp
                translator.cpp
                specialized/charttranslator.cpp
                specialized/documenttranslator.cpp
                specialized/neversequencetranslator.cpp
                specialized/sequencetranslator.cpp
                specialized/whensequencetranslator.cpp)
set(INCLUDES    mscparametervalueparser.h
                signalinfo.h
                tftable.h
                translator.h
                specialized/charttranslator.h
                specialized/documenttranslator.h
                specialized/neversequencetranslator.h
                specialized/sequencetranslator.h
                specialized/whensequencetranslator.h)

add_library(MscToSdlTranslator STATIC)
target_sources(MscToSdlTranslator
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(MscToSdlTranslator
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(MscToSdlTranslator
    PUBLIC      Conversion::Common
                asn1library
                ivcore
                msccore
                SdlModel
    PRIVATE     CommonProjectWarnings)

add_format_target(MscToSdlTranslator)
add_tidy_target(MscToSdlTranslator)

add_library(Conversion::MscToSdlTranslator ALIAS MscToSdlTranslator)
