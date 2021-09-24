project(PromelaExporter VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
  promelaexporter.cpp
  visitors/promelamodelvisitor.cpp
  visitors/utypevisitor.cpp
  visitors/declarationvisitor.cpp
  visitors/datatypeprefixvisitor.cpp
  visitors/datatypesuffixvisitor.cpp
  )
set(INCLUDES
  PromelaOptions/options.h
  promelaexporter.h
  visitors/promelamodelvisitor.h
  visitors/utypevisitor.h
  visitors/declarationvisitor.h
  visitors/datatypeprefixvisitor.h
  visitors/datatypesuffixvisitor.h
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
                      TMC::PromelaModel
                      ${QT_CORE})

add_format_target(PromelaExporter)

add_library(Conversion::PromelaExporter ALIAS PromelaExporter)
