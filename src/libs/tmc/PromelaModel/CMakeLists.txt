project(PromelaModel VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  arraytype.cpp
  datatype.cpp
  declaration.cpp
  namedmtype.cpp
  promelamodel.cpp
  unsigneddatatype.cpp
  utype.cpp
  utyperef.cpp
  )
set(INCLUDES
  arraytype.cpp
  basictypes.h
  datatype.h
  declaration.h
  namedmtype.h
  promelamodel.h
  unsigneddatatype.h
  utype.h
  utyperef.h
  )

add_library(PromelaModel STATIC)
target_sources(PromelaModel
  PRIVATE "${SOURCES}"
  "${INCLUDES}")
target_include_directories(PromelaModel
  PUBLIC    .
  PRIVATE   ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(PromelaModel
  PUBLIC Conversion::Common)

add_format_target(PromelaModel)

add_library(TMC::PromelaModel ALIAS PromelaModel)
