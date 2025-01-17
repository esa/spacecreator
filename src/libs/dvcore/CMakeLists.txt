set(LIB_NAME dvcore)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    abstractsystemchecks.cpp
    abstractsystemchecks.h
    connectionvalidator.cpp
    connectionvalidator.h
    dvboard.cpp
    dvboard.h
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
    dvhwlibraryreader.cpp
    dvhwlibraryreader.h
    dvlibrary.cpp
    dvlibrary.h
    dvmessage.cpp
    dvmessage.h
    dvmodel.cpp
    dvmodel.h
    dvnamevalidator.cpp
    dvnamevalidator.h
    dvnode.cpp
    dvnode.h
    dvobject.cpp
    dvobject.h
    dvpartition.cpp
    dvpartition.h
    dvport.cpp
    dvport.h
    dvpropertytemplate.cpp
    dvpropertytemplate.h
    dvpropertytemplateconfig.cpp
    dvpropertytemplateconfig.h
    dvresources.qrc
    dvsystemfunction.cpp
    dvsystemfunction.h
    dvsysteminterface.cpp
    dvsysteminterface.h
    dvxmlreader.cpp
    dvxmlreader.h
)

target_include_directories(${LIB_NAME}
    PUBLIC .
           ${CMAKE_SOURCE_DIR}/src/libs
)

target_link_libraries(${LIB_NAME} PUBLIC
    shared
    qobjectlistmodel
    ${QT_CORE}
    ${QT_XML}
)

if(OPTIONS_NO_DEPRECATED)
    target_compile_options(${LIB_NAME} PRIVATE -Wno-deprecated)
endif()
