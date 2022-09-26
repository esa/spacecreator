project(PromelaModel VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  arraytype.cpp
  assertcall.cpp
  assignment.cpp
  binaryexpression.cpp
  booleanconstant.cpp
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
  goto.cpp
  initproctype.cpp
  inlinecall.cpp
  inlinedef.cpp
  label.cpp
  mtyperef.cpp
  namedmtype.cpp
  proctype.cpp
  proctypeelement.cpp
  promeladatamodel.cpp
  promelamodel.cpp
  promelasystemmodel.cpp
  select.cpp
  sequence.cpp
  typealias.cpp
  utype.cpp
  utyperef.cpp
  valuedefinition.cpp
  variableref.cpp
  )
set(INCLUDES
  arraytype.h
  assertcall.h
  assignment.h
  basictypes.h
  binaryexpression.h
  booleanconstant.h
  channelinit.h
  channelrecv.h
  channelsend.h
  conditional.h
  constant.h
  datatype.h
  declaration.h
  doloop.h
  elsestatement.h
  exitloop.h
  expression.h
  forloop.h
  goto.h
  initproctype.h
  inlinecall.cpp
  inlinedef.h
  label.h
  mtyperef.h
  namedmtype.h
  proctype.h
  proctypeelement.h
  promeladatamodel.h
  promelamodel.h
  promelasystemmodel.h
  select.h
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
