set(LIB_NAME deploy)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    deploymentobject.cpp
    deploymentobject.h
    deploymentobjectsmodel.cpp
    deploymentobjectsmodel.h
    deploymentnode.cpp
    deploymentnode.h
    deploymentprocessor.cpp
    deploymentprocessor.h
    deploymentpartition.cpp
    deploymentpartition.h
    deploymentdevice.cpp
    deploymentdevice.h
    deploymentbus.cpp
    deploymentbus.h
    deploymentlink.cpp
    deploymentlink.h
    deploymentbinding.cpp
    deploymentbinding.h
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
