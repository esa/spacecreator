set(LIB_NAME aadl)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    aadlcommonprops.cpp
    aadlcommonprops.h
    aadlconnectionchain.cpp
    aadlconnectionchain.h
    aadlnamevalidator.cpp
    aadlnamevalidator.h
    aadlobject.cpp
    aadlobject.h
    aadlobjectcomment.cpp
    aadlobjectcomment.h
    aadlobjectconnection.cpp
    aadlobjectconnection.h
    aadlobjectfunction.cpp
    aadlobjectfunction.h
    aadlobjectfunctiontype.cpp
    aadlobjectfunctiontype.h
    aadlobjectiface.cpp
    aadlobjectiface.h
    aadlobjectsmodel.cpp
    aadlobjectsmodel.h
    aadlparameter.cpp
    aadlparameter.h
    aadltestutils.cpp
    aadltestutils.h
    aadlxmlreader.cpp
    aadlxmlreader.h
    connectioncreationvalidator.cpp
    connectioncreationvalidator.h
    aadlobjectconnectiongroup.cpp
    aadlobjectconnectiongroup.h
    aadlobjectifacegroup.cpp
    aadlobjectifacegroup.h
    dynamicproperty.cpp
    dynamicproperty.h
    dynamicpropertyconfig.cpp
    dynamicpropertyconfig.h
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    asn1editor
    asn1library
    shared
    Qt5::Core
    Qt5::Gui
    Qt5::Xml
)
