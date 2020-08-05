set(LIB_NAME aadl)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    aadlcommonprops.cpp
    aadlcommonprops.h
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
    basicdatatype.cpp
    basicdatatype.h
    connectioncreationvalidator.cpp
    connectioncreationvalidator.h
    datatypesstorage.cpp
    datatypesstorage.h
)

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME}
    asn1editor
    shared
    Qt5::Core
    Qt5::Gui
)
