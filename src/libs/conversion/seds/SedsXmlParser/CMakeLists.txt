project(SedsParser VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES exceptions.cpp
            parser.cpp
            specialized/componentactivitiesparser.cpp
            specialized/componentprimitivesparser.cpp
            specialized/componentstatesparser.cpp
            specialized/componentsparser.cpp
            specialized/coreparser.cpp
            specialized/datatypesparser.cpp
            specialized/devicesparser.cpp
            specialized/interfacesparser.cpp
            specialized/genericsparser.cpp)
set(INCLUDES exceptions.h
             parser.h
             specialized/componentactivitiesparser.h
             specialized/componentprimitivesparser.h
             specialized/componentstatesparser.h
             specialized/componentsparser.h
             specialized/coreparser.h
             specialized/datatypesparser.h
             specialized/devicesparser.h
             specialized/interfacesparser.h
             specialized/genericsparser.h)

add_library(SedsParser STATIC)
target_sources(SedsParser
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsParser
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs/conversion)
target_link_libraries(SedsParser
    PUBLIC      SEDS::Model
                Qt5::Core)

# add_format_target(SedsParser)

add_library(SEDS::Parser ALIAS SedsParser)
