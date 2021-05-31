set(LIB_NAME dvcore)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    dvbinding.cpp
    dvbinding.h
    dvboard.cpp
    dvboard.h
    dvboardreader.cpp
    dvboardreader.h
    dvbus.cpp
    dvbus.h
    dvcommonprops.cpp
    dvcommonprops.h
    dvconnection.cpp
    dvconnection.h
    dvdevice.cpp
    dvdevice.h
    dvfunction.cpp
    dvfunction.h
    dvinterface.cpp
    dvinterface.h
    dvmodel.cpp
    dvmodel.h
    dvnode.cpp
    dvnode.h
    dvobject.cpp
    dvobject.h
    dvpartition.cpp
    dvpartition.h
    dvprocessor.cpp
    dvprocessor.h
    dvxmlreader.cpp
    dvxmlreader.h
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    shared
    qobjectlistmodel
    ${QT_CORE}
    ${QT_XML}
)
