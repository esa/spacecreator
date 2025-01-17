project(SedsModel VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
    datasheet.cpp
    packagefile.cpp
    sedsmodel.cpp
    base/description.cpp
    base/fieldref.cpp
    base/literalvalue.cpp
    base/name.cpp
    base/namedentity.cpp
    base/positivelong.cpp
    base/qualifiedname.cpp
    components/component.cpp
    components/componentimplementation.cpp
    components/entryref.cpp
    components/interface.cpp
    components/parameteractivitymap.cpp
    components/parametermap.cpp
    components/parametermapdata.cpp
    components/variable.cpp
    components/variableref.cpp
    components/activities/activity.cpp
    components/activities/activityinvocation.cpp
    components/activities/andedconditions.cpp
    components/activities/assignment.cpp
    components/activities/basecondition.cpp
    components/activities/basemathoperation.cpp
    components/activities/body.cpp
    components/activities/booleanexpression.cpp
    components/activities/calibration.cpp
    components/activities/comparison.cpp
    components/activities/comparisonoperator.cpp
    components/activities/conditional.cpp
    components/activities/coremathoperator.cpp
    components/activities/iteration.cpp
    components/activities/mathoperation.cpp
    components/activities/operand.cpp
    components/activities/operator.cpp
    components/activities/oredconditions.cpp
    components/activities/polynomial.cpp
    components/activities/polynomialterm.cpp
    components/activities/splinecalibrator.cpp
    components/activities/splinepoint.cpp
    components/activities/statement.cpp
    components/activities/typecheck.cpp
    components/activities/valueoperand.cpp
    components/activities/variablerefoperand.cpp
    components/primitives/commandrefattributes.cpp
    components/primitives/namedargumentvalue.cpp
    components/primitives/oncommandprimitive.cpp
    components/primitives/onparameterprimitive.cpp
    components/primitives/parameteroperation.cpp
    components/primitives/parameterrefattributes.cpp
    components/primitives/primitivesink.cpp
    components/primitives/primitivesource.cpp
    components/primitives/sendcommandprimitive.cpp
    components/primitives/sendparameterprimitive.cpp
    components/primitives/sinkargumentvalue.cpp
    components/primitives/timersink.cpp
    components/states/state.cpp
    components/states/statemachine.cpp
    components/states/statemachineelement.cpp
    components/states/stateref.cpp
    components/states/transition.cpp
    device/device.cpp
    device/metadata/datemetadatavalue.cpp
    device/metadata/floatmetadatavalue.cpp
    device/metadata/integermetadatavalue.cpp
    device/metadata/metadata.cpp
    device/metadata/metadatacategory.cpp
    device/metadata/metadatavalue.cpp
    device/metadata/stringmetadatavalue.cpp
    generics/genericalternate.cpp
    generics/genericalternateset.cpp
    generics/generictype.cpp
    generics/generictypemap.cpp
    generics/generictypemapset.cpp
    interfaces/argumentscombination.cpp
    interfaces/commandargument.cpp
    interfaces/commandargumentmode.cpp
    interfaces/interfacecommand.cpp
    interfaces/interfacecommandmode.cpp
    interfaces/interfacedeclaration.cpp
    interfaces/interfacedeclarationref.cpp
    interfaces/interfacelevel.cpp
    interfaces/interfaceparameter.cpp
    interfaces/interfaceparametermode.cpp
    interfaces/interfaceref.cpp
    package/package.cpp
    types/arraydatatype.cpp
    types/binarydatatype.cpp
    types/booleandatatype.cpp
    types/compositedatatype.cpp
    types/containerdatatype.cpp
    types/datatype.cpp
    types/datatyperef.cpp
    types/dimensionsize.cpp
    types/enumerateddatatype.cpp
    types/externalfield.cpp
    types/field.cpp
    types/floatdatatype.cpp
    types/integerdatatype.cpp
    types/numericdatatype.cpp
    types/rootdatatype.cpp
    types/scalardatatype.cpp
    types/stringdatatype.cpp
    types/subrangedatatype.cpp
    types/valueenumeration.cpp
    types/constraints/containerconstraint.cpp
    types/constraints/containerrangeconstraint.cpp
    types/constraints/containertypeconstraint.cpp
    types/constraints/containervalueconstraint.cpp
    types/encodings/booleandataencoding.cpp
    types/encodings/byteorder.cpp
    types/encodings/coreencodingandprecision.cpp
    types/encodings/coreintegerencoding.cpp
    types/encodings/corestringencoding.cpp
    types/encodings/dataencoding.cpp
    types/encodings/falsevalue.cpp
    types/encodings/floatdataencoding.cpp
    types/encodings/integerdataencoding.cpp
    types/encodings/stringdataencoding.cpp
    types/entries/coreerrorcontrol.cpp
    types/entries/entrytype.cpp
    types/entries/errorcontrolentry.cpp
    types/entries/fixedvalueentry.cpp
    types/entries/listentry.cpp
    types/entries/paddingentry.cpp
    types/ranges/enumerateddatatyperange.cpp
    types/ranges/floatprecisionrange.cpp
    types/ranges/minmaxrange.cpp
    types/ranges/rangetype.cpp)
set(INCLUDES
    sedsmodel.h
    datasheet.h
    packagefile.h
    xincludeinfo.h
    base/description.h
    base/fieldref.h
    base/literalvalue.h
    base/name.h
    base/namedentity.h
    base/positivelong.h
    base/qualifiedname.h
    components/component.h
    components/componentimplementation.h
    components/entryref.h
    components/interface.h
    components/parameteractivitymap.h
    components/parametermap.h
    components/parametermapdata.h
    components/variable.h
    components/variableref.h
    components/activities/activity.h
    components/activities/activityargument.h
    components/activities/activityinvocation.h
    components/activities/andedconditions.h
    components/activities/assignment.h
    components/activities/basecondition.h
    components/activities/basemathoperation.h
    components/activities/body.h
    components/activities/booleanexpression.h
    components/activities/calibration.h
    components/activities/comparison.h
    components/activities/comparisonoperator.h
    components/activities/conditional.h
    components/activities/coremathoperator.h
    components/activities/iteration.h
    components/activities/mathoperation.h
    components/activities/mathoperator.h
    components/activities/operand.h
    components/activities/operator.h
    components/activities/oredconditions.h
    components/activities/polynomial.h
    components/activities/polynomialterm.h
    components/activities/splinecalibrator.h
    components/activities/splinepoint.h
    components/activities/statement.h
    components/activities/typecheck.h
    components/activities/valueoperand.h
    components/activities/variablerefoperand.h
    components/primitives/commandrefattributes.h
    components/primitives/namedargumentvalue.h
    components/primitives/oncommandprimitive.h
    components/primitives/onparameterprimitive.h
    components/primitives/parameteroperation.h
    components/primitives/parameterrefattributes.h
    components/primitives/primitivesink.h
    components/primitives/primitivesource.h
    components/primitives/sendcommandprimitive.h
    components/primitives/sendparameterprimitive.h
    components/primitives/sinkargumentvalue.h
    components/primitives/timersink.h
    components/states/entrystate.h
    components/states/exitstate.h
    components/states/state.h
    components/states/statemachine.h
    components/states/statemachineelement.h
    components/states/stateref.h
    components/states/transition.h
    device/device.h
    device/metadata/datemetadatavalue.h
    device/metadata/floatmetadatavalue.h
    device/metadata/integermetadatavalue.h
    device/metadata/metadata.h
    device/metadata/metadatacategory.h
    device/metadata/metadatavalue.h
    device/metadata/metadatavalueset.h
    device/metadata/stringmetadatavalue.h
    generics/genericalternate.h
    generics/genericalternateset.h
    generics/generictype.h
    generics/generictypemap.h
    generics/generictypemapset.h
    interfaces/argumentscombination.h
    interfaces/commandargument.h
    interfaces/commandargumentmode.h
    interfaces/interfacecommand.h
    interfaces/interfacecommandmode.h
    interfaces/interfacedeclaration.h
    interfaces/interfacedeclarationref.h
    interfaces/interfacelevel.h
    interfaces/interfaceparameter.h
    interfaces/interfaceparametermode.h
    interfaces/interfaceref.h
    package/package.h
    types/arraydatatype.h
    types/binarydatatype.h
    types/booleandatatype.h
    types/compositedatatype.h
    types/containerdatatype.h
    types/datatype.h
    types/datatyperef.h
    types/dimensionsize.h
    types/enumerateddatatype.h
    types/externalfield.h
    types/field.h
    types/floatdatatype.h
    types/integerdatatype.h
    types/numericdatatype.h
    types/rootdatatype.h
    types/scalardatatype.h
    types/stringdatatype.h
    types/subrangedatatype.h
    types/valueenumeration.h
    types/constraints/containerconstraint.h
    types/constraints/containerrangeconstraint.h
    types/constraints/containertypeconstraint.h
    types/constraints/containervalueconstraint.h
    types/encodings/booleandataencoding.h
    types/encodings/byteorder.h
    types/encodings/coreencodingandprecision.h
    types/encodings/coreintegerencoding.h
    types/encodings/corestringencoding.h
    types/encodings/dataencoding.h
    types/encodings/falsevalue.h
    types/encodings/floatdataencoding.h
    types/encodings/integerdataencoding.h
    types/encodings/stringdataencoding.h
    types/entries/coreerrorcontrol.h
    types/entries/entry.h
    types/entries/entrytype.h
    types/entries/errorcontrolentry.h
    types/entries/fixedvalueentry.h
    types/entries/lengthentry.h
    types/entries/listentry.h
    types/entries/paddingentry.h
    types/ranges/derivedtyperange.h
    types/ranges/enumerateddatatyperange.h
    types/ranges/floatprecisionrange.h
    types/ranges/minmaxrange.h
    types/ranges/rangetype.h)

add_library(SedsModel STATIC)
target_sources(SedsModel
    PRIVATE     "${SOURCES}"
                "${INCLUDES}")
target_include_directories(SedsModel
    PUBLIC      .
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(SedsModel
    PUBLIC     Conversion::Common
               shared
    PRIVATE    CommonProjectWarnings)

add_format_target(SedsModel)
add_tidy_target(SedsModel)

add_library(SEDS::Model ALIAS SedsModel)
