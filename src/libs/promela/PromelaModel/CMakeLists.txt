project(PromelaModel VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  arraytype.cpp
  datatype.cpp
  declaration.cpp
  namedmtype.cpp
  promelamodel.cpp
  utype.cpp
  utyperef.cpp
  mtyperef.cpp
  typealias.cpp
  valuedefinition.cpp
  channelinit.cpp
  proctype.cpp
  initproctype.cpp
  channelsend.cpp
  channelrecv.cpp
  expression.cpp
  proctypeelement.cpp
  constant.cpp
  variableref.cpp
  doloop.cpp
  sequence.cpp
  assignment.cpp
  )
set(INCLUDES
  arraytype.h
  basictypes.h
  datatype.h
  declaration.h
  namedmtype.h
  promelamodel.h
  unsigneddatatype.h
  utype.h
  utyperef.h
  mtyperef.h
  typealias.h
  valuedefinition.h
  channelinit.h
  proctype.h
  initproctype.h
  channelsend.h
  channelrecv.h
  expression.h
  proctypeelement.h
  constant.h
  variableref.h
  doloop.h
  sequence.h
  assignment.h
  )

add_library(PromelaModel STATIC)
target_sources(PromelaModel
  PRIVATE "${SOURCES}"
  "${INCLUDES}")
target_include_directories(PromelaModel
  PUBLIC    .
  PRIVATE   ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(PromelaModel
  PUBLIC     Conversion::Common
  PRIVATE    CommonProjectWarnings)

add_format_target(PromelaModel)
add_tidy_target(PromelaModel)

add_library(Promela::Model ALIAS PromelaModel)
