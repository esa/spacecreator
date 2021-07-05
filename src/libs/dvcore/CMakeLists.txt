set(LIB_NAME dvcore)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    dvboard.cpp
    dvboard.h
    dvhwlibraryreader.cpp
    dvhwlibraryreader.h
    dvcommonprops.cpp
    dvcommonprops.h
    dvconnection.cpp
    dvconnection.h
    dvdevice.cpp
    dvdevice.h
    dvfunction.cpp
    dvfunction.h
    dvmodel.cpp
    dvmodel.h
    dvnode.cpp
    dvnode.h
    dvobject.cpp
    dvobject.h
    dvpartition.cpp
    dvpartition.h
    dvxmlreader.cpp
    dvxmlreader.h
    connectioncreationvalidator.cpp
    connectioncreationvalidator.h
    dvpropertytemplateconfig.cpp
    dvpropertytemplateconfig.h
    dvpropertytemplate.cpp
    dvpropertytemplate.h
    dvlibrary.cpp
    dvlibrary.h
    dvresources.qrc
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    shared
    qobjectlistmodel
    ${QT_CORE}
    ${QT_XML}
)
