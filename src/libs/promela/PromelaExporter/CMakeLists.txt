project(PromelaExporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  promelaexporter.cpp
  visitors/basictypegenerator.cpp
  visitors/datatypeprefixvisitor.cpp
  visitors/datatypesuffixvisitor.cpp
  visitors/declarationvisitor.cpp
  visitors/expressionvisitor.cpp
  visitors/inlinecallvisitor.cpp
  visitors/proctypeelementvisitor.cpp
  visitors/promelamodelvisitor.cpp
  visitors/sequencevisitor.cpp
  visitors/typealiasvisitor.cpp
  visitors/utypevisitor.cpp
  visitors/variablerefvisitor.cpp
  )
set(INCLUDES
  promelaexporter.h
  visitors/basictypegenerator.h
  visitors/datatypeprefixvisitor.h
  visitors/datatypesuffixvisitor.h
  visitors/declarationvisitor.h
  visitors/expressionvisitor.h
  visitors/inlinecallvisitor.h
  visitors/proctypeelementvisitor.h
  visitors/promelamodelvisitor.h
  visitors/sequencevisitor.h
  visitors/typealiasvisitor.h
  visitors/utypevisitor.h
  visitors/variablerefvisitor.h
  )

add_library(PromelaExporter STATIC)
target_sources(PromelaExporter
  PRIVATE      "${SOURCES}"
               "${INCLUDES}")
target_include_directories(PromelaExporter
  PUBLIC                   .
  PRIVATE                  ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(PromelaExporter
  PUBLIC              Conversion::Common
                      Promela::Model
                      ${QT_CORE}
  PRIVATE             CommonProjectWarnings)

add_format_target(PromelaExporter)
add_tidy_target(PromelaExporter)

add_library(Promela::Exporter ALIAS PromelaExporter)
