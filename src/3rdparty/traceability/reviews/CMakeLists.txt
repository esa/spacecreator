set(LIB_NAME reviews)

add_library(${LIB_NAME} STATIC
    gitlab/gitlabreviews.cpp
    gitlab/gitlabreviews.h
    review.cpp
    review.h
    reviewswidget.h reviewswidget.cpp reviewswidget.ui
    reviewsmanager.h reviewsmanager.cpp)

target_sources(${LIB_NAME} PRIVATE
)

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME}
    PUBLIC Qt6::Core Qt6::Widgets tracecommon
    PRIVATE QGitlabAPI)
