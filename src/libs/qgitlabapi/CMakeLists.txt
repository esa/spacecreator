cmake_minimum_required(VERSION 3.14)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
remove_definitions(-DQT_NO_KEYWORDS)
add_library(QGitlabAPI STATIC
  QGitlabAPI_global.h
  qgitlabclient.cpp
  qgitlabclient.h
  GitLabTransport.cpp
  GitLabTransport.h
  Issue.h
  Issue.cpp
  UrlComposer.h
  UrlComposer.cpp
  Label.h
  Label.cpp
)

target_link_libraries(QGitlabAPI PRIVATE ${QT_NETWORK} ${QT_CORE})

target_compile_definitions(QGitlabAPI PRIVATE QGITLABAPI_LIBRARY QT_DEBUG_OUTPUT)
