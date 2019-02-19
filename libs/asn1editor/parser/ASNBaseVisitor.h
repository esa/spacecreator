
// Generated from ASN.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"
#include "ASNVisitor.h"


/**
 * This class provides an empty implementation of ASNVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  ASNBaseVisitor : public ASNVisitor {
public:

  virtual antlrcpp::Any visitModuleDefinition(ASNParser::ModuleDefinitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTagDefault(ASNParser::TagDefaultContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExtensionDefault(ASNParser::ExtensionDefaultContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitModuleBody(ASNParser::ModuleBodyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExports(ASNParser::ExportsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSymbolsExported(ASNParser::SymbolsExportedContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitImports(ASNParser::ImportsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSymbolsImported(ASNParser::SymbolsImportedContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSymbolsFromModuleList(ASNParser::SymbolsFromModuleListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSymbolsFromModule(ASNParser::SymbolsFromModuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGlobalModuleReference(ASNParser::GlobalModuleReferenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignedIdentifier(ASNParser::AssignedIdentifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSymbolList(ASNParser::SymbolListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSymbol(ASNParser::SymbolContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignmentList(ASNParser::AssignmentListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignment(ASNParser::AssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSequenceType(ASNParser::SequenceTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExtensionAndException(ASNParser::ExtensionAndExceptionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOptionalExtensionMarker(ASNParser::OptionalExtensionMarkerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComponentTypeLists(ASNParser::ComponentTypeListsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRootComponentTypeList(ASNParser::RootComponentTypeListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComponentTypeList(ASNParser::ComponentTypeListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComponentType(ASNParser::ComponentTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExtensionAdditions(ASNParser::ExtensionAdditionsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExtensionAdditionList(ASNParser::ExtensionAdditionListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExtensionAddition(ASNParser::ExtensionAdditionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExtensionAdditionGroup(ASNParser::ExtensionAdditionGroupContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVersionNumber(ASNParser::VersionNumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSequenceOfType(ASNParser::SequenceOfTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSizeConstraint(ASNParser::SizeConstraintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParameterizedAssignment(ASNParser::ParameterizedAssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParameterList(ASNParser::ParameterListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParameter(ASNParser::ParameterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParamGovernor(ASNParser::ParamGovernorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGovernor(ASNParser::GovernorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjectClassAssignment(ASNParser::ObjectClassAssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjectClass(ASNParser::ObjectClassContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDefinedObjectClass(ASNParser::DefinedObjectClassContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUsefulObjectClassReference(ASNParser::UsefulObjectClassReferenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExternalObjectClassReference(ASNParser::ExternalObjectClassReferenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjectClassDefn(ASNParser::ObjectClassDefnContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWithSyntaxSpec(ASNParser::WithSyntaxSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSyntaxList(ASNParser::SyntaxListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTokenOrGroupSpec(ASNParser::TokenOrGroupSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOptionalGroup(ASNParser::OptionalGroupContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRequiredToken(ASNParser::RequiredTokenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLiteral(ASNParser::LiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrimitiveFieldName(ASNParser::PrimitiveFieldNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFieldSpec(ASNParser::FieldSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTypeFieldSpec(ASNParser::TypeFieldSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTypeOptionalitySpec(ASNParser::TypeOptionalitySpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFixedTypeValueFieldSpec(ASNParser::FixedTypeValueFieldSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValueOptionalitySpec(ASNParser::ValueOptionalitySpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVariableTypeValueFieldSpec(ASNParser::VariableTypeValueFieldSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFixedTypeValueSetFieldSpec(ASNParser::FixedTypeValueSetFieldSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValueSetOptionalitySpec(ASNParser::ValueSetOptionalitySpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObject(ASNParser::ObjectContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParameterizedObject(ASNParser::ParameterizedObjectContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDefinedObject(ASNParser::DefinedObjectContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjectSet(ASNParser::ObjectSetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjectSetSpec(ASNParser::ObjectSetSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFieldName(ASNParser::FieldNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValueSet(ASNParser::ValueSetContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitElementSetSpecs(ASNParser::ElementSetSpecsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRootElementSetSpec(ASNParser::RootElementSetSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAdditionalElementSetSpec(ASNParser::AdditionalElementSetSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitElementSetSpec(ASNParser::ElementSetSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnions(ASNParser::UnionsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExclusions(ASNParser::ExclusionsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIntersections(ASNParser::IntersectionsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnionMark(ASNParser::UnionMarkContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIntersectionMark(ASNParser::IntersectionMarkContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitElements(ASNParser::ElementsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjectSetElements(ASNParser::ObjectSetElementsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIntersectionElements(ASNParser::IntersectionElementsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSubtypeElements(ASNParser::SubtypeElementsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVariableTypeValueSetFieldSpec(ASNParser::VariableTypeValueSetFieldSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjectFieldSpec(ASNParser::ObjectFieldSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjectOptionalitySpec(ASNParser::ObjectOptionalitySpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjectSetFieldSpec(ASNParser::ObjectSetFieldSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjectSetOptionalitySpec(ASNParser::ObjectSetOptionalitySpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTypeAssignment(ASNParser::TypeAssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValueAssignment(ASNParser::ValueAssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAsnType(ASNParser::AsnTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBuiltinType(ASNParser::BuiltinTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjectClassFieldType(ASNParser::ObjectClassFieldTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSetType(ASNParser::SetTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSetOfType(ASNParser::SetOfTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReferencedType(ASNParser::ReferencedTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDefinedType(ASNParser::DefinedTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConstraint(ASNParser::ConstraintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConstraintSpec(ASNParser::ConstraintSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUserDefinedConstraint(ASNParser::UserDefinedConstraintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGeneralConstraint(ASNParser::GeneralConstraintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUserDefinedConstraintParameter(ASNParser::UserDefinedConstraintParameterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTableConstraint(ASNParser::TableConstraintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSimpleTableConstraint(ASNParser::SimpleTableConstraintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitContentsConstraint(ASNParser::ContentsConstraintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSubtypeConstraint(ASNParser::SubtypeConstraintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValue(ASNParser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBuiltinValue(ASNParser::BuiltinValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjectIdentifierValue(ASNParser::ObjectIdentifierValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjIdComponentsList(ASNParser::ObjIdComponentsListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjIdComponents(ASNParser::ObjIdComponentsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIntegerValue(ASNParser::IntegerValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRealValue(ASNParser::RealValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitChoiceValue(ASNParser::ChoiceValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEnumeratedValue(ASNParser::EnumeratedValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSignedNumber(ASNParser::SignedNumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitChoiceType(ASNParser::ChoiceTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAlternativeTypeLists(ASNParser::AlternativeTypeListsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExtensionAdditionAlternatives(ASNParser::ExtensionAdditionAlternativesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExtensionAdditionAlternativesList(ASNParser::ExtensionAdditionAlternativesListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExtensionAdditionAlternative(ASNParser::ExtensionAdditionAlternativeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExtensionAdditionAlternativesGroup(ASNParser::ExtensionAdditionAlternativesGroupContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRootAlternativeTypeList(ASNParser::RootAlternativeTypeListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAlternativeTypeList(ASNParser::AlternativeTypeListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNamedType(ASNParser::NamedTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEnumeratedType(ASNParser::EnumeratedTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEnumerations(ASNParser::EnumerationsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRootEnumeration(ASNParser::RootEnumerationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEnumeration(ASNParser::EnumerationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEnumerationItem(ASNParser::EnumerationItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNamedNumber(ASNParser::NamedNumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNamedRealNumber(ASNParser::NamedRealNumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDefinedValue(ASNParser::DefinedValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParameterizedValue(ASNParser::ParameterizedValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSimpleDefinedValue(ASNParser::SimpleDefinedValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitActualParameterList(ASNParser::ActualParameterListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitActualParameter(ASNParser::ActualParameterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExceptionSpec(ASNParser::ExceptionSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExceptionIdentification(ASNParser::ExceptionIdentificationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAdditionalEnumeration(ASNParser::AdditionalEnumerationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIntegerType(ASNParser::IntegerTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRealType(ASNParser::RealTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBooleanType(ASNParser::BooleanTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNamedNumberList(ASNParser::NamedNumberListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNamedRealNumberList(ASNParser::NamedRealNumberListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitObjectidentifiertype(ASNParser::ObjectidentifiertypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComponentRelationConstraint(ASNParser::ComponentRelationConstraintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAtNotation(ASNParser::AtNotationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLevel(ASNParser::LevelContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComponentIdList(ASNParser::ComponentIdListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOctetStringType(ASNParser::OctetStringTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBitStringType(ASNParser::BitStringTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNamedBitList(ASNParser::NamedBitListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNamedBit(ASNParser::NamedBitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBooleanValue(ASNParser::BooleanValueContext *ctx) override {
    return visitChildren(ctx);
  }


};

