set(LIB_NAME deploy)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    deployobject.cpp
    deployobject.h
    deployobjectsmodel.cpp
    deployobjectsmodel.h
    deploynode.cpp
    deploynode.h
    deployprocessor.cpp
    deployprocessor.h
    deploypartition.cpp
    deploypartition.h
    deploydevice.cpp
    deploydevice.h
    deploybus.cpp
    deploybus.h
    deploylink.cpp
    deploylink.h
    deploybinding.cpp
    deploybinding.h
    prop.cpp
    prop.h
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    shared
    Qt5::Core
    Qt5::Gui
    Qt5::Xml
)
