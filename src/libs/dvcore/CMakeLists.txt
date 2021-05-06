set(LIB_NAME dvcore)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    dvobject.cpp
    dvobject.h
    dvmodel.cpp
    dvmodel.h
    dvnode.cpp
    dvnode.h
    dvprocessor.cpp
    dvprocessor.h
    dvpartition.cpp
    dvpartition.h
    dvdevice.cpp
    dvdevice.h
    dvbus.cpp
    dvbus.h
    dvconnection.cpp
    dvconnection.h
    dvbinding.cpp
    dvbinding.h
    dvxmlreader.cpp
    dvxmlreader.h
    dvcommonprops.cpp
    dvcommonprops.h
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    shared
    ${QT_CORE}
    ${QT_XML}
)
