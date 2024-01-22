set(LIB_NAME requirements)

remove_definitions(-DQT_NO_KEYWORDS)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
  checkedfilterproxymodel.cpp
  checkedfilterproxymodel.h
  gitlab/gitlabrequirements.cpp
  gitlab/gitlabrequirements.h
  requirement.cpp
  requirement.h
  requirementslibrary.cpp
  requirementslibrary.h
  requirementsmanager.cpp
  requirementsmanager.h
  requirementsmodelbase.cpp
  requirementsmodelbase.h
  requirementtextproxymodel.cpp
  requirementtextproxymodel.h
  tagfilterproxymodel.cpp
  tagfilterproxymodel.h
  ui/requirementswidget.cpp
  ui/requirementswidget.h
  ui/requirementswidget.ui
  ui/addnewrequirementdialog.h
  ui/addnewrequirementdialog.cpp
  ui/addnewrequirementdialog.ui
  ui/widgetbar.h
  ui/widgetbar.cpp
  ui/widgetbar.ui
  requirementsresources.qrc
)

target_include_directories(${LIB_NAME} PUBLIC .)
target_link_libraries(${LIB_NAME}
    PUBLIC Qt6::Core Qt6::Widgets
    PRIVATE QGitlabAPI tracecommon)
target_compile_definitions(${LIB_NAME} PUBLIC QGITLABAPI_LIBRARY QT_DEBUG_OUTPUT)
