set(LIB_NAME asn1library)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    asn1const.h
    asn1library.cpp
    asn1library.h
    asn1modelstorage.cpp
    asn1modelstorage.h
    asn1resources.qrc
    asn1valueparser.cpp
    asn1valueparser.h
    asn1reader.cpp
    asn1reader.h
    astxmlparser.cpp
    astxmlparser.h
    asn1/definitions.cpp
    asn1/definitions.h
    asn1/errormessage.h
    asn1/file.cpp
    asn1/file.h
    asn1/importedtype.h
    asn1/importedvalue.h
    asn1/node.cpp
    asn1/node.h
    asn1/project.cpp
    asn1/project.h
    asn1/root.cpp
    asn1/root.h
    asn1/sourcelocation.cpp
    asn1/sourcelocation.h
    asn1/typeassignment.cpp
    asn1/typeassignment.h
    asn1/typereference.cpp
    asn1/typereference.h
    asn1/types/builtintypes.cpp
    asn1/types/builtintypes.h
    asn1/types/labeltype.cpp
    asn1/types/labeltype.h
    asn1/types/type.cpp
    asn1/types/type.h
    asn1/types/userdefinedtype.cpp
    asn1/types/userdefinedtype.h
    asn1/valueassignment.cpp
    asn1/valueassignment.h
    asn1/visitor.cpp
    asn1/visitor.h
    asn1/visitorwithvalue.h
)

target_include_directories(${LIB_NAME}
    PUBLIC .
    PUBLIC asn1
)

target_link_libraries(${LIB_NAME}
    shared
    Qt5::Core
)

source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")
