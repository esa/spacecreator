set(LIB_NAME asn1library)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    asn1const.h
    asn1scc.cpp
    asn1scc.h
    asn1modelstorage.cpp
    asn1modelstorage.h
    asn1valueparser.cpp
    asn1valueparser.h
    asn1reader.cpp
    asn1reader.h
    asn1systemchecks.cpp
    asn1systemchecks.h
    astxmlconstraintparser.h
    astxmlparser.cpp
    astxmlparser.h
    asn1/acnargument.h
    asn1/acnparameter.h
    asn1/acnsequencecomponent.cpp
    asn1/acnsequencecomponent.h
    asn1/asn1model.h
    asn1/asn1model.cpp
    asn1/asnsequencecomponent.cpp
    asn1/asnsequencecomponent.h
    asn1/choicevalue.cpp
    asn1/choicevalue.h
    asn1/definitions.cpp
    asn1/definitions.h
    asn1/errormessage.h
    asn1/file.cpp
    asn1/file.h
    asn1/importedtype.h
    asn1/importedvalue.h
    asn1/multiplevalue.cpp
    asn1/multiplevalue.h
    asn1/mutatingvisitor.h
    asn1/mutatingvisitor.cpp
    asn1/namedvalue.cpp
    asn1/namedvalue.h
    asn1/node.cpp
    asn1/node.h
    asn1/project.cpp
    asn1/project.h
    asn1/range.h
    asn1/rangelist.h
    asn1/root.cpp
    asn1/root.h
    asn1/sequencecomponent.h
    asn1/sequencecomponentvisitor.h
    asn1/singlevalue.cpp
    asn1/singlevalue.h
    asn1/sourcelocation.cpp
    asn1/sourcelocation.h
    asn1/typeassignment.cpp
    asn1/typeassignment.h
    asn1/typereference.h
    asn1/constraints/constraint.h
    asn1/constraints/constraintlist.h
    asn1/constraints/constraintvisitor.h
    asn1/constraints/fromconstraint.h
    asn1/constraints/logicoperators.h
    asn1/constraints/printingvisitor.h
    asn1/constraints/rangeconstraint.h
    asn1/constraints/rangelistingvisitor.h
    asn1/constraints/sizeconstraint.h
    asn1/constraints/withconstraints.h
    asn1/patchersnippet.h
    asn1/types/acnparameterizablecomposite.h
    asn1/types/asciistringacnparameters.cpp
    asn1/types/asciistringacnparameters.h
    asn1/types/bitstring.cpp
    asn1/types/bitstring.h
    asn1/types/boolean.cpp
    asn1/types/boolean.h
    asn1/types/choice.cpp
    asn1/types/choice.h
    asn1/types/enumerated.cpp
    asn1/types/enumerated.h
    asn1/types/ia5string.cpp
    asn1/types/ia5string.h
    asn1/types/integer.cpp
    asn1/types/integer.h
    asn1/types/integeracnparams.cpp
    asn1/types/integeracnparams.h
    asn1/types/labeltype.cpp
    asn1/types/labeltype.h
    asn1/types/null.cpp
    asn1/types/null.h
    asn1/types/numericstring.cpp
    asn1/types/numericstring.h
    asn1/types/octetstring.cpp
    asn1/types/octetstring.h
    asn1/types/real.cpp
    asn1/types/real.h
    asn1/types/sequence.cpp
    asn1/types/sequence.h
    asn1/types/sequenceof.cpp
    asn1/types/sequenceof.h
    asn1/types/string.h
    asn1/types/type.cpp
    asn1/types/type.h
    asn1/types/typefactory.cpp
    asn1/types/typefactory.h
    asn1/types/typemutatingvisitor.cpp
    asn1/types/typemutatingvisitor.h
    asn1/types/typereadingvisitor.h
    asn1/types/typevisitorwithvalue.h
    asn1/types/userdefinedtype.cpp
    asn1/types/userdefinedtype.h
    asn1/value.h
    asn1/valueassignment.cpp
    asn1/valueassignment.h
    asn1/values.h
    asn1/visitor.cpp
    asn1/visitor.h
    asn1/visitorwithvalue.h
    errormessageparser.cpp
    errormessageparser.h
)

target_include_directories(${LIB_NAME}
    PUBLIC .
    PUBLIC asn1
    PRIVATE ${CMAKE_SOURCE_DIR}/src/libs
)

target_link_libraries(${LIB_NAME}
    PUBLIC shared
           ${QT_CORE}
)

if(OPTIONS_NO_DEPRECATED)
    target_compile_options(${LIB_NAME} PRIVATE -Wno-deprecated -Wno-deprecated-declarations)
endif()

add_format_target(${LIB_NAME})

source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")
