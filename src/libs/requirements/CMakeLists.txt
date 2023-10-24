cmake_minimum_required(VERSION 3.14)
set(LIB_NAME requirements)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
remove_definitions(-DQT_NO_KEYWORDS)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
  requirement.cpp
  requirement.h
  requirementsfiltermodel.cpp
  requirementsfiltermodel.h
  requirementsmanager.cpp
  requirementsmanager.h
  requirementsmodel.cpp
  requirementsmodel.h
  ui/gitlabrequirements.cpp
  ui/gitlabrequirements.h
  ui/gitlabrequirements.ui
)

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME} ${QT_NETWORK} ${QT_CORE} ${QT_WIDGETS} QGitlabAPI libiveditor)
target_compile_definitions(${LIB_NAME} PUBLIC QGITLABAPI_LIBRARY QT_DEBUG_OUTPUT)
