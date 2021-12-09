project(PromelaExporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  promelaexporter.cpp
  visitors/promelamodelvisitor.cpp
  visitors/utypevisitor.cpp
  visitors/declarationvisitor.cpp
  visitors/datatypeprefixvisitor.cpp
  visitors/datatypesuffixvisitor.cpp
  visitors/typealiasvisitor.cpp
  visitors/basictypegenerator.cpp
  visitors/proctypeelementvisitor.cpp
  visitors/expressionvisitor.cpp
  visitors/variablerefvisitor.cpp
  visitors/sequencevisitor.cpp
  )
set(INCLUDES
  promelaexporter.h
  visitors/promelamodelvisitor.h
  visitors/utypevisitor.h
  visitors/declarationvisitor.h
  visitors/datatypeprefixvisitor.h
  visitors/datatypesuffixvisitor.h
  visitors/typealiasvisitor.h
  visitors/basictypegenerator.h
  visitors/proctypeelementvisitor.h
  visitors/expressionvisitor.h
  visitors/variablerefvisitor.h
  visitors/sequencevisitor.h
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
