set(LIB_NAME QGitlabAPI)

add_library(QGitlabAPI STATIC)

target_sources(${LIB_NAME} PRIVATE
  QGitlabAPI_global.h
  issue.cpp
  issue.h
  issuerequestoptions.cpp
  issuerequestoptions.h
  label.cpp
  label.h
  qgitlabclient.cpp
  qgitlabclient.h
  urlcomposer.cpp
  urlcomposer.h
)

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME} ${QT_NETWORK} ${QT_CORE} ${QT_GUI})
target_compile_definitions(${LIB_NAME} PUBLIC QGITLABAPI_LIBRARY QT_DEBUG_OUTPUT)
