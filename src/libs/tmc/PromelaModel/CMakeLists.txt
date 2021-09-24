project(PromelaModel VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  promelamodel.cpp
  namedmtype.cpp
  unsigneddatatype.cpp
  utyperef.cpp
  utype.cpp
  datatype.cpp
  declaration.cpp
  arraytype.cpp
  )
set(INCLUDES
  promelamodel.h
  namedmtype.h
  unsigneddatatype.h
  utyperef.h
  utype.h
  datatype.h
  declaration.h
  arraytype.cpp
  basictypes.h
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
