project(PromelaModel VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  arraytype.cpp
  assignment.cpp
  channelinit.cpp
  channelrecv.cpp
  channelsend.cpp
  conditional.cpp
  constant.cpp
  datatype.cpp
  declaration.cpp
  doloop.cpp
  expression.cpp
  forloop.cpp
  initproctype.cpp
  inlinecall.cpp
  inlinedef.cpp
  mtyperef.cpp
  namedmtype.cpp
  proctype.cpp
  proctypeelement.cpp
  promelamodel.cpp
  sequence.cpp
  typealias.cpp
  utype.cpp
  utyperef.cpp
  valuedefinition.cpp
  variableref.cpp
  )
set(INCLUDES
  arraytype.h
  assignment.h
  basictypes.h
  channelinit.h
  channelrecv.h
  channelsend.h
  conditional.h
  constant.h
  datatype.h
  declaration.h
  doloop.h
  expression.h
  forloop.h
  initproctype.h
  inlinecall.cpp
  inlinedef.h
  mtyperef.h
  namedmtype.h
  proctype.h
  proctypeelement.h
  promelamodel.h
  sequence.h
  skip.h
  typealias.h
  unsigneddatatype.h
  utype.h
  utyperef.h
  valuedefinition.h
  variableref.h
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
