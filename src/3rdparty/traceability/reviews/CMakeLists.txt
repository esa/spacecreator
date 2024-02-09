set(LIB_NAME reviews)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    gitlab/gitlabreviews.h gitlab/gitlabreviews.cpp
    review.h review.cpp
    reviewsmodel.h reviewsmodel.cpp
    reviewswidget.h reviewswidget.cpp reviewswidget.ui
    reviewsmanager.h reviewsmanager.cpp
)

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME}
    PUBLIC Qt6::Core Qt6::Widgets tracecommon
    PRIVATE QGitlabAPI)
