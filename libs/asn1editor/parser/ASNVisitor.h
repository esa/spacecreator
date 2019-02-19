
// Generated from ASN.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"
#include "ASNParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by ASNParser.
 */
class  ASNVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by ASNParser.
   */
    virtual antlrcpp::Any visitModuleDefinition(ASNParser::ModuleDefinitionContext *context) = 0;

    virtual antlrcpp::Any visitTagDefault(ASNParser::TagDefaultContext *context) = 0;

    virtual antlrcpp::Any visitExtensionDefault(ASNParser::ExtensionDefaultContext *context) = 0;

    virtual antlrcpp::Any visitModuleBody(ASNParser::ModuleBodyContext *context) = 0;

    virtual antlrcpp::Any visitExports(ASNParser::ExportsContext *context) = 0;

    virtual antlrcpp::Any visitSymbolsExported(ASNParser::SymbolsExportedContext *context) = 0;

    virtual antlrcpp::Any visitImports(ASNParser::ImportsContext *context) = 0;

    virtual antlrcpp::Any visitSymbolsImported(ASNParser::SymbolsImportedContext *context) = 0;

    virtual antlrcpp::Any visitSymbolsFromModuleList(ASNParser::SymbolsFromModuleListContext *context) = 0;

    virtual antlrcpp::Any visitSymbolsFromModule(ASNParser::SymbolsFromModuleContext *context) = 0;

    virtual antlrcpp::Any visitGlobalModuleReference(ASNParser::GlobalModuleReferenceContext *context) = 0;

    virtual antlrcpp::Any visitAssignedIdentifier(ASNParser::AssignedIdentifierContext *context) = 0;

    virtual antlrcpp::Any visitSymbolList(ASNParser::SymbolListContext *context) = 0;

    virtual antlrcpp::Any visitSymbol(ASNParser::SymbolContext *context) = 0;

    virtual antlrcpp::Any visitAssignmentList(ASNParser::AssignmentListContext *context) = 0;

    virtual antlrcpp::Any visitAssignment(ASNParser::AssignmentContext *context) = 0;

    virtual antlrcpp::Any visitSequenceType(ASNParser::SequenceTypeContext *context) = 0;

    virtual antlrcpp::Any visitExtensionAndException(ASNParser::ExtensionAndExceptionContext *context) = 0;

    virtual antlrcpp::Any visitOptionalExtensionMarker(ASNParser::OptionalExtensionMarkerContext *context) = 0;

    virtual antlrcpp::Any visitComponentTypeLists(ASNParser::ComponentTypeListsContext *context) = 0;

    virtual antlrcpp::Any visitRootComponentTypeList(ASNParser::RootComponentTypeListContext *context) = 0;

    virtual antlrcpp::Any visitComponentTypeList(ASNParser::ComponentTypeListContext *context) = 0;

    virtual antlrcpp::Any visitComponentType(ASNParser::ComponentTypeContext *context) = 0;

    virtual antlrcpp::Any visitExtensionAdditions(ASNParser::ExtensionAdditionsContext *context) = 0;

    virtual antlrcpp::Any visitExtensionAdditionList(ASNParser::ExtensionAdditionListContext *context) = 0;

    virtual antlrcpp::Any visitExtensionAddition(ASNParser::ExtensionAdditionContext *context) = 0;

    virtual antlrcpp::Any visitExtensionAdditionGroup(ASNParser::ExtensionAdditionGroupContext *context) = 0;

    virtual antlrcpp::Any visitVersionNumber(ASNParser::VersionNumberContext *context) = 0;

    virtual antlrcpp::Any visitSequenceOfType(ASNParser::SequenceOfTypeContext *context) = 0;

    virtual antlrcpp::Any visitSizeConstraint(ASNParser::SizeConstraintContext *context) = 0;

    virtual antlrcpp::Any visitParameterizedAssignment(ASNParser::ParameterizedAssignmentContext *context) = 0;

    virtual antlrcpp::Any visitParameterList(ASNParser::ParameterListContext *context) = 0;

    virtual antlrcpp::Any visitParameter(ASNParser::ParameterContext *context) = 0;

    virtual antlrcpp::Any visitParamGovernor(ASNParser::ParamGovernorContext *context) = 0;

    virtual antlrcpp::Any visitGovernor(ASNParser::GovernorContext *context) = 0;

    virtual antlrcpp::Any visitObjectClassAssignment(ASNParser::ObjectClassAssignmentContext *context) = 0;

    virtual antlrcpp::Any visitObjectClass(ASNParser::ObjectClassContext *context) = 0;

    virtual antlrcpp::Any visitDefinedObjectClass(ASNParser::DefinedObjectClassContext *context) = 0;

    virtual antlrcpp::Any visitUsefulObjectClassReference(ASNParser::UsefulObjectClassReferenceContext *context) = 0;

    virtual antlrcpp::Any visitExternalObjectClassReference(ASNParser::ExternalObjectClassReferenceContext *context) = 0;

    virtual antlrcpp::Any visitObjectClassDefn(ASNParser::ObjectClassDefnContext *context) = 0;

    virtual antlrcpp::Any visitWithSyntaxSpec(ASNParser::WithSyntaxSpecContext *context) = 0;

    virtual antlrcpp::Any visitSyntaxList(ASNParser::SyntaxListContext *context) = 0;

    virtual antlrcpp::Any visitTokenOrGroupSpec(ASNParser::TokenOrGroupSpecContext *context) = 0;

    virtual antlrcpp::Any visitOptionalGroup(ASNParser::OptionalGroupContext *context) = 0;

    virtual antlrcpp::Any visitRequiredToken(ASNParser::RequiredTokenContext *context) = 0;

    virtual antlrcpp::Any visitLiteral(ASNParser::LiteralContext *context) = 0;

    virtual antlrcpp::Any visitPrimitiveFieldName(ASNParser::PrimitiveFieldNameContext *context) = 0;

    virtual antlrcpp::Any visitFieldSpec(ASNParser::FieldSpecContext *context) = 0;

    virtual antlrcpp::Any visitTypeFieldSpec(ASNParser::TypeFieldSpecContext *context) = 0;

    virtual antlrcpp::Any visitTypeOptionalitySpec(ASNParser::TypeOptionalitySpecContext *context) = 0;

    virtual antlrcpp::Any visitFixedTypeValueFieldSpec(ASNParser::FixedTypeValueFieldSpecContext *context) = 0;

    virtual antlrcpp::Any visitValueOptionalitySpec(ASNParser::ValueOptionalitySpecContext *context) = 0;

    virtual antlrcpp::Any visitVariableTypeValueFieldSpec(ASNParser::VariableTypeValueFieldSpecContext *context) = 0;

    virtual antlrcpp::Any visitFixedTypeValueSetFieldSpec(ASNParser::FixedTypeValueSetFieldSpecContext *context) = 0;

    virtual antlrcpp::Any visitValueSetOptionalitySpec(ASNParser::ValueSetOptionalitySpecContext *context) = 0;

    virtual antlrcpp::Any visitObject(ASNParser::ObjectContext *context) = 0;

    virtual antlrcpp::Any visitParameterizedObject(ASNParser::ParameterizedObjectContext *context) = 0;

    virtual antlrcpp::Any visitDefinedObject(ASNParser::DefinedObjectContext *context) = 0;

    virtual antlrcpp::Any visitObjectSet(ASNParser::ObjectSetContext *context) = 0;

    virtual antlrcpp::Any visitObjectSetSpec(ASNParser::ObjectSetSpecContext *context) = 0;

    virtual antlrcpp::Any visitFieldName(ASNParser::FieldNameContext *context) = 0;

    virtual antlrcpp::Any visitValueSet(ASNParser::ValueSetContext *context) = 0;

    virtual antlrcpp::Any visitElementSetSpecs(ASNParser::ElementSetSpecsContext *context) = 0;

    virtual antlrcpp::Any visitRootElementSetSpec(ASNParser::RootElementSetSpecContext *context) = 0;

    virtual antlrcpp::Any visitAdditionalElementSetSpec(ASNParser::AdditionalElementSetSpecContext *context) = 0;

    virtual antlrcpp::Any visitElementSetSpec(ASNParser::ElementSetSpecContext *context) = 0;

    virtual antlrcpp::Any visitUnions(ASNParser::UnionsContext *context) = 0;

    virtual antlrcpp::Any visitExclusions(ASNParser::ExclusionsContext *context) = 0;

    virtual antlrcpp::Any visitIntersections(ASNParser::IntersectionsContext *context) = 0;

    virtual antlrcpp::Any visitUnionMark(ASNParser::UnionMarkContext *context) = 0;

    virtual antlrcpp::Any visitIntersectionMark(ASNParser::IntersectionMarkContext *context) = 0;

    virtual antlrcpp::Any visitElements(ASNParser::ElementsContext *context) = 0;

    virtual antlrcpp::Any visitObjectSetElements(ASNParser::ObjectSetElementsContext *context) = 0;

    virtual antlrcpp::Any visitIntersectionElements(ASNParser::IntersectionElementsContext *context) = 0;

    virtual antlrcpp::Any visitSubtypeElements(ASNParser::SubtypeElementsContext *context) = 0;

    virtual antlrcpp::Any visitVariableTypeValueSetFieldSpec(ASNParser::VariableTypeValueSetFieldSpecContext *context) = 0;

    virtual antlrcpp::Any visitObjectFieldSpec(ASNParser::ObjectFieldSpecContext *context) = 0;

    virtual antlrcpp::Any visitObjectOptionalitySpec(ASNParser::ObjectOptionalitySpecContext *context) = 0;

    virtual antlrcpp::Any visitObjectSetFieldSpec(ASNParser::ObjectSetFieldSpecContext *context) = 0;

    virtual antlrcpp::Any visitObjectSetOptionalitySpec(ASNParser::ObjectSetOptionalitySpecContext *context) = 0;

    virtual antlrcpp::Any visitTypeAssignment(ASNParser::TypeAssignmentContext *context) = 0;

    virtual antlrcpp::Any visitValueAssignment(ASNParser::ValueAssignmentContext *context) = 0;

    virtual antlrcpp::Any visitAsnType(ASNParser::AsnTypeContext *context) = 0;

    virtual antlrcpp::Any visitBuiltinType(ASNParser::BuiltinTypeContext *context) = 0;

    virtual antlrcpp::Any visitObjectClassFieldType(ASNParser::ObjectClassFieldTypeContext *context) = 0;

    virtual antlrcpp::Any visitSetType(ASNParser::SetTypeContext *context) = 0;

    virtual antlrcpp::Any visitSetOfType(ASNParser::SetOfTypeContext *context) = 0;

    virtual antlrcpp::Any visitReferencedType(ASNParser::ReferencedTypeContext *context) = 0;

    virtual antlrcpp::Any visitDefinedType(ASNParser::DefinedTypeContext *context) = 0;

    virtual antlrcpp::Any visitConstraint(ASNParser::ConstraintContext *context) = 0;

    virtual antlrcpp::Any visitConstraintSpec(ASNParser::ConstraintSpecContext *context) = 0;

    virtual antlrcpp::Any visitUserDefinedConstraint(ASNParser::UserDefinedConstraintContext *context) = 0;

    virtual antlrcpp::Any visitGeneralConstraint(ASNParser::GeneralConstraintContext *context) = 0;

    virtual antlrcpp::Any visitUserDefinedConstraintParameter(ASNParser::UserDefinedConstraintParameterContext *context) = 0;

    virtual antlrcpp::Any visitTableConstraint(ASNParser::TableConstraintContext *context) = 0;

    virtual antlrcpp::Any visitSimpleTableConstraint(ASNParser::SimpleTableConstraintContext *context) = 0;

    virtual antlrcpp::Any visitContentsConstraint(ASNParser::ContentsConstraintContext *context) = 0;

    virtual antlrcpp::Any visitSubtypeConstraint(ASNParser::SubtypeConstraintContext *context) = 0;

    virtual antlrcpp::Any visitValue(ASNParser::ValueContext *context) = 0;

    virtual antlrcpp::Any visitBuiltinValue(ASNParser::BuiltinValueContext *context) = 0;

    virtual antlrcpp::Any visitObjectIdentifierValue(ASNParser::ObjectIdentifierValueContext *context) = 0;

    virtual antlrcpp::Any visitObjIdComponentsList(ASNParser::ObjIdComponentsListContext *context) = 0;

    virtual antlrcpp::Any visitObjIdComponents(ASNParser::ObjIdComponentsContext *context) = 0;

    virtual antlrcpp::Any visitIntegerValue(ASNParser::IntegerValueContext *context) = 0;

    virtual antlrcpp::Any visitRealValue(ASNParser::RealValueContext *context) = 0;

    virtual antlrcpp::Any visitChoiceValue(ASNParser::ChoiceValueContext *context) = 0;

    virtual antlrcpp::Any visitEnumeratedValue(ASNParser::EnumeratedValueContext *context) = 0;

    virtual antlrcpp::Any visitSignedNumber(ASNParser::SignedNumberContext *context) = 0;

    virtual antlrcpp::Any visitChoiceType(ASNParser::ChoiceTypeContext *context) = 0;

    virtual antlrcpp::Any visitAlternativeTypeLists(ASNParser::AlternativeTypeListsContext *context) = 0;

    virtual antlrcpp::Any visitExtensionAdditionAlternatives(ASNParser::ExtensionAdditionAlternativesContext *context) = 0;

    virtual antlrcpp::Any visitExtensionAdditionAlternativesList(ASNParser::ExtensionAdditionAlternativesListContext *context) = 0;

    virtual antlrcpp::Any visitExtensionAdditionAlternative(ASNParser::ExtensionAdditionAlternativeContext *context) = 0;

    virtual antlrcpp::Any visitExtensionAdditionAlternativesGroup(ASNParser::ExtensionAdditionAlternativesGroupContext *context) = 0;

    virtual antlrcpp::Any visitRootAlternativeTypeList(ASNParser::RootAlternativeTypeListContext *context) = 0;

    virtual antlrcpp::Any visitAlternativeTypeList(ASNParser::AlternativeTypeListContext *context) = 0;

    virtual antlrcpp::Any visitNamedType(ASNParser::NamedTypeContext *context) = 0;

    virtual antlrcpp::Any visitEnumeratedType(ASNParser::EnumeratedTypeContext *context) = 0;

    virtual antlrcpp::Any visitEnumerations(ASNParser::EnumerationsContext *context) = 0;

    virtual antlrcpp::Any visitRootEnumeration(ASNParser::RootEnumerationContext *context) = 0;

    virtual antlrcpp::Any visitEnumeration(ASNParser::EnumerationContext *context) = 0;

    virtual antlrcpp::Any visitEnumerationItem(ASNParser::EnumerationItemContext *context) = 0;

    virtual antlrcpp::Any visitNamedNumber(ASNParser::NamedNumberContext *context) = 0;

    virtual antlrcpp::Any visitNamedRealNumber(ASNParser::NamedRealNumberContext *context) = 0;

    virtual antlrcpp::Any visitDefinedValue(ASNParser::DefinedValueContext *context) = 0;

    virtual antlrcpp::Any visitParameterizedValue(ASNParser::ParameterizedValueContext *context) = 0;

    virtual antlrcpp::Any visitSimpleDefinedValue(ASNParser::SimpleDefinedValueContext *context) = 0;

    virtual antlrcpp::Any visitActualParameterList(ASNParser::ActualParameterListContext *context) = 0;

    virtual antlrcpp::Any visitActualParameter(ASNParser::ActualParameterContext *context) = 0;

    virtual antlrcpp::Any visitExceptionSpec(ASNParser::ExceptionSpecContext *context) = 0;

    virtual antlrcpp::Any visitExceptionIdentification(ASNParser::ExceptionIdentificationContext *context) = 0;

    virtual antlrcpp::Any visitAdditionalEnumeration(ASNParser::AdditionalEnumerationContext *context) = 0;

    virtual antlrcpp::Any visitIntegerType(ASNParser::IntegerTypeContext *context) = 0;

    virtual antlrcpp::Any visitRealType(ASNParser::RealTypeContext *context) = 0;

    virtual antlrcpp::Any visitBooleanType(ASNParser::BooleanTypeContext *context) = 0;

    virtual antlrcpp::Any visitNamedNumberList(ASNParser::NamedNumberListContext *context) = 0;

    virtual antlrcpp::Any visitNamedRealNumberList(ASNParser::NamedRealNumberListContext *context) = 0;

    virtual antlrcpp::Any visitObjectidentifiertype(ASNParser::ObjectidentifiertypeContext *context) = 0;

    virtual antlrcpp::Any visitComponentRelationConstraint(ASNParser::ComponentRelationConstraintContext *context) = 0;

    virtual antlrcpp::Any visitAtNotation(ASNParser::AtNotationContext *context) = 0;

    virtual antlrcpp::Any visitLevel(ASNParser::LevelContext *context) = 0;

    virtual antlrcpp::Any visitComponentIdList(ASNParser::ComponentIdListContext *context) = 0;

    virtual antlrcpp::Any visitOctetStringType(ASNParser::OctetStringTypeContext *context) = 0;

    virtual antlrcpp::Any visitBitStringType(ASNParser::BitStringTypeContext *context) = 0;

    virtual antlrcpp::Any visitNamedBitList(ASNParser::NamedBitListContext *context) = 0;

    virtual antlrcpp::Any visitNamedBit(ASNParser::NamedBitContext *context) = 0;

    virtual antlrcpp::Any visitBooleanValue(ASNParser::BooleanValueContext *context) = 0;


};

