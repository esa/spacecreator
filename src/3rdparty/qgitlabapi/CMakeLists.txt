cmake_minimum_required(VERSION 3.14)
set(LIB_NAME QGitlabAPI)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
remove_definitions(-DQT_NO_KEYWORDS)

add_library(QGitlabAPI STATIC)

target_sources(${LIB_NAME} PRIVATE
  QGitlabAPI_global.h
  qgitlabclient.cpp
  qgitlabclient.h
  Issue.h
  Issue.cpp
  UrlComposer.h
  UrlComposer.cpp
  Label.h
  Label.cpp
)

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME} ${QT_NETWORK} ${QT_CORE} ${QT_WIDGETS})
target_compile_definitions(${LIB_NAME} PUBLIC QGITLABAPI_LIBRARY QT_DEBUG_OUTPUT)
