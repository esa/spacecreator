set(LIB_NAME dvcore)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
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
    dvpseudofunction.cpp
    dvpseudofunction.h
    dvpseudointerface.cpp
    dvpseudointerface.h
    dvresources.qrc
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

if(OPTIONS_NO_DEPRECATED)
    target_compile_options(${LIB_NAME} PRIVATE -Wno-deprecated)
endif()
