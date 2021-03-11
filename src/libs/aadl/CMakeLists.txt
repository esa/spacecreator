set(LIB_NAME aadl)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    aadlcommonprops.cpp
    aadlcommonprops.h
    aadlconnectionchain.cpp
    aadlconnectionchain.h
    aadlinterfacechain.cpp
    aadlinterfacechain.h
    aadlnamevalidator.cpp
    aadlnamevalidator.h
    aadlobject.cpp
    aadlobject.h
    aadlcomment.cpp
    aadlcomment.h
    aadlconnection.cpp
    aadlconnection.h
    aadlconnectiongroup.cpp
    aadlconnectiongroup.h
    aadlfunction.cpp
    aadlfunction.h
    aadlfunctiontype.cpp
    aadlfunctiontype.h
    aadliface.cpp
    aadliface.h
    aadlifacegroup.cpp
    aadlifacegroup.h
    aadlmodel.cpp
    aadlmodel.h
    aadlparameter.cpp
    aadlparameter.h
    aadltestutils.cpp
    aadltestutils.h
    aadlxmlreader.cpp
    aadlxmlreader.h
    connectioncreationvalidator.cpp
    connectioncreationvalidator.h
    propertytemplate.cpp
    propertytemplate.h
    propertytemplateconfig.cpp
    propertytemplateconfig.h
    aadllibrary.cpp
    aadllibrary.h
    aadlresources.qrc
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    asn1editor
    asn1library
    shared
    ${QT_CORE}
    ${QT_XML}
)

source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")
