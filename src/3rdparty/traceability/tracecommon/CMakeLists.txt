set(LIB_NAME tracecommon)

add_library(${LIB_NAME} STATIC
    credentialwidget.h credentialwidget.cpp credentialwidget.ui
    issuesmanager.h issuesmanager.cpp
    issuesmanagerprivate.h issuesmanagerprivate.cpp
    tracecommonresources.qrc
    tracecommonlibrary.cpp
    tracecommonlibrary.h
    )

target_sources(${LIB_NAME} PRIVATE
)

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME}
    PUBLIC Qt6::Core Qt6::Widgets
    PRIVATE QGitlabAPI)
