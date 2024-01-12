set(LIB_NAME reviews)

add_library(${LIB_NAME} STATIC
    reviewswidget.h reviewswidget.cpp reviewswidget.ui)

target_sources(${LIB_NAME} PRIVATE
)

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME}
    PUBLIC ${QT_CORE} ${QT_WIDGETS}
    PRIVATE QGitlabAPI tracecommon)
