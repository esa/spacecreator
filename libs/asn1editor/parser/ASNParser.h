
// Generated from ASN.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"




class  ASNParser : public antlr4::Parser {
public:
  enum {
    A_ROND = 1, STAR = 2, ASSIGN_OP = 3, BOOLEAN_LITERAL = 4, TRUE_LITERAL = 5, 
    FALSE_LITERAL = 6, DOT = 7, DOUBLE_DOT = 8, ELLIPSIS = 9, APOSTROPHE = 10, 
    AMPERSAND = 11, LESS_THAN = 12, GREATER_THAN = 13, LESS_THAN_SLASH = 14, 
    SLASH_GREATER_THAN = 15, TRUE_SMALL_LITERAL = 16, FALSE_SMALL_LITERAL = 17, 
    INTEGER_LITERAL = 18, L_BRACE = 19, R_BRACE = 20, COMMA = 21, L_PARAN = 22, 
    R_PARAN = 23, MINUS = 24, ENUMERATED_LITERAL = 25, REAL_LITERAL = 26, 
    PLUS_INFINITY_LITERAL = 27, MINUS_INFINITY_LITERAL = 28, BIT_LITERAL = 29, 
    STRING_LITERAL = 30, CONTAINING_LITERAL = 31, OCTET_LITERAL = 32, NULL_LITERAL = 33, 
    SEQUENCE_LITERAL = 34, OPTIONAL_LITERAL = 35, DEFAULT_LITERAL = 36, 
    COMPONENTS_LITERAL = 37, OF_LITERAL = 38, SET_LITERAL = 39, EXCLAM = 40, 
    ALL_LITERAL = 41, EXCEPT_LITERAL = 42, POWER = 43, PIPE = 44, UNION_LITERAL = 45, 
    INTERSECTION_LITERAL = 46, INCLUDES_LITERAL = 47, MIN_LITERAL = 48, 
    MAX_LITERAL = 49, SIZE_LITERAL = 50, FROM_LITERAL = 51, WITH_LITERAL = 52, 
    COMPONENT_LITERAL = 53, PRESENT_LITERAL = 54, ABSENT_LITERAL = 55, PATTERN_LITERAL = 56, 
    TYPE_IDENTIFIER_LITERAL = 57, ABSTRACT_SYNTAX_LITERAL = 58, CLASS_LITERAL = 59, 
    UNIQUE_LITERAL = 60, SYNTAX_LITERAL = 61, L_BRACKET = 62, R_BRACKET = 63, 
    INSTANCE_LITERAL = 64, SEMI_COLON = 65, IMPORTS_LITERAL = 66, EXPORTS_LITERAL = 67, 
    EXTENSIBILITY_LITERAL = 68, IMPLIED_LITERAL = 69, EXPLICIT_LITERAL = 70, 
    TAGS_LITERAL = 71, IMPLICIT_LITERAL = 72, AUTOMATIC_LITERAL = 73, DEFINITIONS_LITERAL = 74, 
    BEGIN_LITERAL = 75, END_LITERAL = 76, DOUBLE_L_BRACKET = 77, DOUBLE_R_BRACKET = 78, 
    COLON = 79, CHOICE_LITERAL = 80, UNIVERSAL_LITERAL = 81, APPLICATION_LITERAL = 82, 
    PRIVATE_LITERAL = 83, EMBEDDED_LITERAL = 84, PDV_LITERAL = 85, EXTERNAL_LITERAL = 86, 
    OBJECT_LITERAL = 87, IDENTIFIER_LITERAL = 88, RELATIVE_OID_LITERAL = 89, 
    CHARACTER_LITERAL = 90, CONSTRAINED_LITERAL = 91, BY_LITERAL = 92, A_ROND_DOT = 93, 
    ENCODED_LITERAL = 94, COMMENT = 95, UNRESTRICTEDCHARACTERSTRINGTYPE = 96, 
    EXTENSTIONENDMARKER = 97, NUMBER = 98, WS = 99, LINE_COMMENT = 100, 
    BSTRING = 101, HSTRING = 102, CSTRING = 103, IDENTIFIER = 104
  };

  enum {
    RuleModuleDefinition = 0, RuleTagDefault = 1, RuleExtensionDefault = 2, 
    RuleModuleBody = 3, RuleExports = 4, RuleSymbolsExported = 5, RuleImports = 6, 
    RuleSymbolsImported = 7, RuleSymbolsFromModuleList = 8, RuleSymbolsFromModule = 9, 
    RuleGlobalModuleReference = 10, RuleAssignedIdentifier = 11, RuleSymbolList = 12, 
    RuleSymbol = 13, RuleAssignmentList = 14, RuleAssignment = 15, RuleSequenceType = 16, 
    RuleExtensionAndException = 17, RuleOptionalExtensionMarker = 18, RuleComponentTypeLists = 19, 
    RuleRootComponentTypeList = 20, RuleComponentTypeList = 21, RuleComponentType = 22, 
    RuleExtensionAdditions = 23, RuleExtensionAdditionList = 24, RuleExtensionAddition = 25, 
    RuleExtensionAdditionGroup = 26, RuleVersionNumber = 27, RuleSequenceOfType = 28, 
    RuleSizeConstraint = 29, RuleParameterizedAssignment = 30, RuleParameterList = 31, 
    RuleParameter = 32, RuleParamGovernor = 33, RuleGovernor = 34, RuleObjectClassAssignment = 35, 
    RuleObjectClass = 36, RuleDefinedObjectClass = 37, RuleUsefulObjectClassReference = 38, 
    RuleExternalObjectClassReference = 39, RuleObjectClassDefn = 40, RuleWithSyntaxSpec = 41, 
    RuleSyntaxList = 42, RuleTokenOrGroupSpec = 43, RuleOptionalGroup = 44, 
    RuleRequiredToken = 45, RuleLiteral = 46, RulePrimitiveFieldName = 47, 
    RuleFieldSpec = 48, RuleTypeFieldSpec = 49, RuleTypeOptionalitySpec = 50, 
    RuleFixedTypeValueFieldSpec = 51, RuleValueOptionalitySpec = 52, RuleVariableTypeValueFieldSpec = 53, 
    RuleFixedTypeValueSetFieldSpec = 54, RuleValueSetOptionalitySpec = 55, 
    RuleObject = 56, RuleParameterizedObject = 57, RuleDefinedObject = 58, 
    RuleObjectSet = 59, RuleObjectSetSpec = 60, RuleFieldName = 61, RuleValueSet = 62, 
    RuleElementSetSpecs = 63, RuleRootElementSetSpec = 64, RuleAdditionalElementSetSpec = 65, 
    RuleElementSetSpec = 66, RuleUnions = 67, RuleExclusions = 68, RuleIntersections = 69, 
    RuleUnionMark = 70, RuleIntersectionMark = 71, RuleElements = 72, RuleObjectSetElements = 73, 
    RuleIntersectionElements = 74, RuleSubtypeElements = 75, RuleVariableTypeValueSetFieldSpec = 76, 
    RuleObjectFieldSpec = 77, RuleObjectOptionalitySpec = 78, RuleObjectSetFieldSpec = 79, 
    RuleObjectSetOptionalitySpec = 80, RuleTypeAssignment = 81, RuleValueAssignment = 82, 
    RuleAsnType = 83, RuleBuiltinType = 84, RuleObjectClassFieldType = 85, 
    RuleSetType = 86, RuleSetOfType = 87, RuleReferencedType = 88, RuleDefinedType = 89, 
    RuleConstraint = 90, RuleConstraintSpec = 91, RuleUserDefinedConstraint = 92, 
    RuleGeneralConstraint = 93, RuleUserDefinedConstraintParameter = 94, 
    RuleTableConstraint = 95, RuleSimpleTableConstraint = 96, RuleContentsConstraint = 97, 
    RuleSubtypeConstraint = 98, RuleValue = 99, RuleBuiltinValue = 100, 
    RuleObjectIdentifierValue = 101, RuleObjIdComponentsList = 102, RuleObjIdComponents = 103, 
    RuleIntegerValue = 104, RuleChoiceValue = 105, RuleEnumeratedValue = 106, 
    RuleSignedNumber = 107, RuleChoiceType = 108, RuleAlternativeTypeLists = 109, 
    RuleExtensionAdditionAlternatives = 110, RuleExtensionAdditionAlternativesList = 111, 
    RuleExtensionAdditionAlternative = 112, RuleExtensionAdditionAlternativesGroup = 113, 
    RuleRootAlternativeTypeList = 114, RuleAlternativeTypeList = 115, RuleNamedType = 116, 
    RuleEnumeratedType = 117, RuleEnumerations = 118, RuleRootEnumeration = 119, 
    RuleEnumeration = 120, RuleEnumerationItem = 121, RuleNamedNumber = 122, 
    RuleDefinedValue = 123, RuleParameterizedValue = 124, RuleSimpleDefinedValue = 125, 
    RuleActualParameterList = 126, RuleActualParameter = 127, RuleExceptionSpec = 128, 
    RuleExceptionIdentification = 129, RuleAdditionalEnumeration = 130, 
    RuleIntegerType = 131, RuleNamedNumberList = 132, RuleObjectidentifiertype = 133, 
    RuleComponentRelationConstraint = 134, RuleAtNotation = 135, RuleLevel = 136, 
    RuleComponentIdList = 137, RuleOctetStringType = 138, RuleBitStringType = 139, 
    RuleNamedBitList = 140, RuleNamedBit = 141, RuleBooleanValue = 142
  };

  ASNParser(antlr4::TokenStream *input);
  ~ASNParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class ModuleDefinitionContext;
  class TagDefaultContext;
  class ExtensionDefaultContext;
  class ModuleBodyContext;
  class ExportsContext;
  class SymbolsExportedContext;
  class ImportsContext;
  class SymbolsImportedContext;
  class SymbolsFromModuleListContext;
  class SymbolsFromModuleContext;
  class GlobalModuleReferenceContext;
  class AssignedIdentifierContext;
  class SymbolListContext;
  class SymbolContext;
  class AssignmentListContext;
  class AssignmentContext;
  class SequenceTypeContext;
  class ExtensionAndExceptionContext;
  class OptionalExtensionMarkerContext;
  class ComponentTypeListsContext;
  class RootComponentTypeListContext;
  class ComponentTypeListContext;
  class ComponentTypeContext;
  class ExtensionAdditionsContext;
  class ExtensionAdditionListContext;
  class ExtensionAdditionContext;
  class ExtensionAdditionGroupContext;
  class VersionNumberContext;
  class SequenceOfTypeContext;
  class SizeConstraintContext;
  class ParameterizedAssignmentContext;
  class ParameterListContext;
  class ParameterContext;
  class ParamGovernorContext;
  class GovernorContext;
  class ObjectClassAssignmentContext;
  class ObjectClassContext;
  class DefinedObjectClassContext;
  class UsefulObjectClassReferenceContext;
  class ExternalObjectClassReferenceContext;
  class ObjectClassDefnContext;
  class WithSyntaxSpecContext;
  class SyntaxListContext;
  class TokenOrGroupSpecContext;
  class OptionalGroupContext;
  class RequiredTokenContext;
  class LiteralContext;
  class PrimitiveFieldNameContext;
  class FieldSpecContext;
  class TypeFieldSpecContext;
  class TypeOptionalitySpecContext;
  class FixedTypeValueFieldSpecContext;
  class ValueOptionalitySpecContext;
  class VariableTypeValueFieldSpecContext;
  class FixedTypeValueSetFieldSpecContext;
  class ValueSetOptionalitySpecContext;
  class ObjectContext;
  class ParameterizedObjectContext;
  class DefinedObjectContext;
  class ObjectSetContext;
  class ObjectSetSpecContext;
  class FieldNameContext;
  class ValueSetContext;
  class ElementSetSpecsContext;
  class RootElementSetSpecContext;
  class AdditionalElementSetSpecContext;
  class ElementSetSpecContext;
  class UnionsContext;
  class ExclusionsContext;
  class IntersectionsContext;
  class UnionMarkContext;
  class IntersectionMarkContext;
  class ElementsContext;
  class ObjectSetElementsContext;
  class IntersectionElementsContext;
  class SubtypeElementsContext;
  class VariableTypeValueSetFieldSpecContext;
  class ObjectFieldSpecContext;
  class ObjectOptionalitySpecContext;
  class ObjectSetFieldSpecContext;
  class ObjectSetOptionalitySpecContext;
  class TypeAssignmentContext;
  class ValueAssignmentContext;
  class AsnTypeContext;
  class BuiltinTypeContext;
  class ObjectClassFieldTypeContext;
  class SetTypeContext;
  class SetOfTypeContext;
  class ReferencedTypeContext;
  class DefinedTypeContext;
  class ConstraintContext;
  class ConstraintSpecContext;
  class UserDefinedConstraintContext;
  class GeneralConstraintContext;
  class UserDefinedConstraintParameterContext;
  class TableConstraintContext;
  class SimpleTableConstraintContext;
  class ContentsConstraintContext;
  class SubtypeConstraintContext;
  class ValueContext;
  class BuiltinValueContext;
  class ObjectIdentifierValueContext;
  class ObjIdComponentsListContext;
  class ObjIdComponentsContext;
  class IntegerValueContext;
  class ChoiceValueContext;
  class EnumeratedValueContext;
  class SignedNumberContext;
  class ChoiceTypeContext;
  class AlternativeTypeListsContext;
  class ExtensionAdditionAlternativesContext;
  class ExtensionAdditionAlternativesListContext;
  class ExtensionAdditionAlternativeContext;
  class ExtensionAdditionAlternativesGroupContext;
  class RootAlternativeTypeListContext;
  class AlternativeTypeListContext;
  class NamedTypeContext;
  class EnumeratedTypeContext;
  class EnumerationsContext;
  class RootEnumerationContext;
  class EnumerationContext;
  class EnumerationItemContext;
  class NamedNumberContext;
  class DefinedValueContext;
  class ParameterizedValueContext;
  class SimpleDefinedValueContext;
  class ActualParameterListContext;
  class ActualParameterContext;
  class ExceptionSpecContext;
  class ExceptionIdentificationContext;
  class AdditionalEnumerationContext;
  class IntegerTypeContext;
  class NamedNumberListContext;
  class ObjectidentifiertypeContext;
  class ComponentRelationConstraintContext;
  class AtNotationContext;
  class LevelContext;
  class ComponentIdListContext;
  class OctetStringTypeContext;
  class BitStringTypeContext;
  class NamedBitListContext;
  class NamedBitContext;
  class BooleanValueContext; 

  class  ModuleDefinitionContext : public antlr4::ParserRuleContext {
  public:
    ModuleDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    antlr4::tree::TerminalNode *DEFINITIONS_LITERAL();
    TagDefaultContext *tagDefault();
    ExtensionDefaultContext *extensionDefault();
    antlr4::tree::TerminalNode *ASSIGN_OP();
    antlr4::tree::TerminalNode *BEGIN_LITERAL();
    ModuleBodyContext *moduleBody();
    antlr4::tree::TerminalNode *END_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    antlr4::tree::TerminalNode *R_BRACE();
    std::vector<antlr4::tree::TerminalNode *> L_PARAN();
    antlr4::tree::TerminalNode* L_PARAN(size_t i);
    std::vector<antlr4::tree::TerminalNode *> NUMBER();
    antlr4::tree::TerminalNode* NUMBER(size_t i);
    std::vector<antlr4::tree::TerminalNode *> R_PARAN();
    antlr4::tree::TerminalNode* R_PARAN(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ModuleDefinitionContext* moduleDefinition();

  class  TagDefaultContext : public antlr4::ParserRuleContext {
  public:
    TagDefaultContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TAGS_LITERAL();
    antlr4::tree::TerminalNode *EXPLICIT_LITERAL();
    antlr4::tree::TerminalNode *IMPLICIT_LITERAL();
    antlr4::tree::TerminalNode *AUTOMATIC_LITERAL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TagDefaultContext* tagDefault();

  class  ExtensionDefaultContext : public antlr4::ParserRuleContext {
  public:
    ExtensionDefaultContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXTENSIBILITY_LITERAL();
    antlr4::tree::TerminalNode *IMPLIED_LITERAL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionDefaultContext* extensionDefault();

  class  ModuleBodyContext : public antlr4::ParserRuleContext {
  public:
    ModuleBodyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExportsContext *exports();
    ImportsContext *imports();
    AssignmentListContext *assignmentList();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ModuleBodyContext* moduleBody();

  class  ExportsContext : public antlr4::ParserRuleContext {
  public:
    ExportsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXPORTS_LITERAL();
    SymbolsExportedContext *symbolsExported();
    antlr4::tree::TerminalNode *SEMI_COLON();
    antlr4::tree::TerminalNode *ALL_LITERAL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExportsContext* exports();

  class  SymbolsExportedContext : public antlr4::ParserRuleContext {
  public:
    SymbolsExportedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SymbolListContext *symbolList();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SymbolsExportedContext* symbolsExported();

  class  ImportsContext : public antlr4::ParserRuleContext {
  public:
    ImportsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IMPORTS_LITERAL();
    SymbolsImportedContext *symbolsImported();
    antlr4::tree::TerminalNode *SEMI_COLON();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ImportsContext* imports();

  class  SymbolsImportedContext : public antlr4::ParserRuleContext {
  public:
    SymbolsImportedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SymbolsFromModuleListContext *symbolsFromModuleList();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SymbolsImportedContext* symbolsImported();

  class  SymbolsFromModuleListContext : public antlr4::ParserRuleContext {
  public:
    SymbolsFromModuleListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SymbolsFromModuleContext *> symbolsFromModule();
    SymbolsFromModuleContext* symbolsFromModule(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SymbolsFromModuleListContext* symbolsFromModuleList();

  class  SymbolsFromModuleContext : public antlr4::ParserRuleContext {
  public:
    SymbolsFromModuleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SymbolListContext *symbolList();
    antlr4::tree::TerminalNode *FROM_LITERAL();
    GlobalModuleReferenceContext *globalModuleReference();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SymbolsFromModuleContext* symbolsFromModule();

  class  GlobalModuleReferenceContext : public antlr4::ParserRuleContext {
  public:
    GlobalModuleReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    AssignedIdentifierContext *assignedIdentifier();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GlobalModuleReferenceContext* globalModuleReference();

  class  AssignedIdentifierContext : public antlr4::ParserRuleContext {
  public:
    AssignedIdentifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignedIdentifierContext* assignedIdentifier();

  class  SymbolListContext : public antlr4::ParserRuleContext {
  public:
    SymbolListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SymbolContext *> symbol();
    SymbolContext* symbol(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SymbolListContext* symbolList();

  class  SymbolContext : public antlr4::ParserRuleContext {
  public:
    SymbolContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *L_BRACE();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SymbolContext* symbol();

  class  AssignmentListContext : public antlr4::ParserRuleContext {
  public:
    AssignmentListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AssignmentContext *> assignment();
    AssignmentContext* assignment(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignmentListContext* assignmentList();

  class  AssignmentContext : public antlr4::ParserRuleContext {
  public:
    AssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    ValueAssignmentContext *valueAssignment();
    TypeAssignmentContext *typeAssignment();
    ParameterizedAssignmentContext *parameterizedAssignment();
    ObjectClassAssignmentContext *objectClassAssignment();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignmentContext* assignment();

  class  SequenceTypeContext : public antlr4::ParserRuleContext {
  public:
    SequenceTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SEQUENCE_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    antlr4::tree::TerminalNode *R_BRACE();
    ExtensionAndExceptionContext *extensionAndException();
    OptionalExtensionMarkerContext *optionalExtensionMarker();
    ComponentTypeListsContext *componentTypeLists();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SequenceTypeContext* sequenceType();

  class  ExtensionAndExceptionContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAndExceptionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ELLIPSIS();
    ExceptionSpecContext *exceptionSpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAndExceptionContext* extensionAndException();

  class  OptionalExtensionMarkerContext : public antlr4::ParserRuleContext {
  public:
    OptionalExtensionMarkerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *ELLIPSIS();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptionalExtensionMarkerContext* optionalExtensionMarker();

  class  ComponentTypeListsContext : public antlr4::ParserRuleContext {
  public:
    ComponentTypeListsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<RootComponentTypeListContext *> rootComponentTypeList();
    RootComponentTypeListContext* rootComponentTypeList(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    ExtensionAndExceptionContext *extensionAndException();
    ExtensionAdditionsContext *extensionAdditions();
    OptionalExtensionMarkerContext *optionalExtensionMarker();
    antlr4::tree::TerminalNode *EXTENSTIONENDMARKER();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentTypeListsContext* componentTypeLists();

  class  RootComponentTypeListContext : public antlr4::ParserRuleContext {
  public:
    RootComponentTypeListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ComponentTypeListContext *componentTypeList();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootComponentTypeListContext* rootComponentTypeList();

  class  ComponentTypeListContext : public antlr4::ParserRuleContext {
  public:
    ComponentTypeListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ComponentTypeContext *> componentType();
    ComponentTypeContext* componentType(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentTypeListContext* componentTypeList();

  class  ComponentTypeContext : public antlr4::ParserRuleContext {
  public:
    ComponentTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NamedTypeContext *namedType();
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    ValueContext *value();
    antlr4::tree::TerminalNode *COMPONENTS_LITERAL();
    antlr4::tree::TerminalNode *OF_LITERAL();
    AsnTypeContext *asnType();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentTypeContext* componentType();

  class  ExtensionAdditionsContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COMMA();
    ExtensionAdditionListContext *extensionAdditionList();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionsContext* extensionAdditions();

  class  ExtensionAdditionListContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExtensionAdditionContext *> extensionAddition();
    ExtensionAdditionContext* extensionAddition(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionListContext* extensionAdditionList();

  class  ExtensionAdditionContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ComponentTypeContext *componentType();
    ExtensionAdditionGroupContext *extensionAdditionGroup();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionContext* extensionAddition();

  class  ExtensionAdditionGroupContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionGroupContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOUBLE_L_BRACKET();
    VersionNumberContext *versionNumber();
    ComponentTypeListContext *componentTypeList();
    antlr4::tree::TerminalNode *DOUBLE_R_BRACKET();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionGroupContext* extensionAdditionGroup();

  class  VersionNumberContext : public antlr4::ParserRuleContext {
  public:
    VersionNumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *COLON();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VersionNumberContext* versionNumber();

  class  SequenceOfTypeContext : public antlr4::ParserRuleContext {
  public:
    SequenceOfTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SEQUENCE_LITERAL();
    antlr4::tree::TerminalNode *OF_LITERAL();
    AsnTypeContext *asnType();
    NamedTypeContext *namedType();
    antlr4::tree::TerminalNode *L_PARAN();
    antlr4::tree::TerminalNode *R_PARAN();
    ConstraintContext *constraint();
    SizeConstraintContext *sizeConstraint();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SequenceOfTypeContext* sequenceOfType();

  class  SizeConstraintContext : public antlr4::ParserRuleContext {
  public:
    SizeConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SIZE_LITERAL();
    ConstraintContext *constraint();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SizeConstraintContext* sizeConstraint();

  class  ParameterizedAssignmentContext : public antlr4::ParserRuleContext {
  public:
    ParameterizedAssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ParameterListContext *parameterList();
    antlr4::tree::TerminalNode *ASSIGN_OP();
    AsnTypeContext *asnType();
    ValueContext *value();
    ValueSetContext *valueSet();
    DefinedObjectClassContext *definedObjectClass();
    ObjectContext *object();
    ObjectClassContext *objectClass();
    ObjectSetContext *objectSet();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterizedAssignmentContext* parameterizedAssignment();

  class  ParameterListContext : public antlr4::ParserRuleContext {
  public:
    ParameterListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    std::vector<ParameterContext *> parameter();
    ParameterContext* parameter(size_t i);
    antlr4::tree::TerminalNode *R_BRACE();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterListContext* parameterList();

  class  ParameterContext : public antlr4::ParserRuleContext {
  public:
    ParameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    ParamGovernorContext *paramGovernor();
    antlr4::tree::TerminalNode *COLON();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterContext* parameter();

  class  ParamGovernorContext : public antlr4::ParserRuleContext {
  public:
    ParamGovernorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GovernorContext *governor();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParamGovernorContext* paramGovernor();

  class  GovernorContext : public antlr4::ParserRuleContext {
  public:
    GovernorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AsnTypeContext *asnType();
    DefinedObjectClassContext *definedObjectClass();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GovernorContext* governor();

  class  ObjectClassAssignmentContext : public antlr4::ParserRuleContext {
  public:
    ObjectClassAssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASSIGN_OP();
    ObjectClassContext *objectClass();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectClassAssignmentContext* objectClassAssignment();

  class  ObjectClassContext : public antlr4::ParserRuleContext {
  public:
    ObjectClassContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DefinedObjectClassContext *definedObjectClass();
    ObjectClassDefnContext *objectClassDefn();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectClassContext* objectClass();

  class  DefinedObjectClassContext : public antlr4::ParserRuleContext {
  public:
    DefinedObjectClassContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *TYPE_IDENTIFIER_LITERAL();
    antlr4::tree::TerminalNode *ABSTRACT_SYNTAX_LITERAL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DefinedObjectClassContext* definedObjectClass();

  class  UsefulObjectClassReferenceContext : public antlr4::ParserRuleContext {
  public:
    UsefulObjectClassReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TYPE_IDENTIFIER_LITERAL();
    antlr4::tree::TerminalNode *ABSTRACT_SYNTAX_LITERAL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UsefulObjectClassReferenceContext* usefulObjectClassReference();

  class  ExternalObjectClassReferenceContext : public antlr4::ParserRuleContext {
  public:
    ExternalObjectClassReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    antlr4::tree::TerminalNode *DOT();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExternalObjectClassReferenceContext* externalObjectClassReference();

  class  ObjectClassDefnContext : public antlr4::ParserRuleContext {
  public:
    ObjectClassDefnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CLASS_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    std::vector<FieldSpecContext *> fieldSpec();
    FieldSpecContext* fieldSpec(size_t i);
    antlr4::tree::TerminalNode *R_BRACE();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    WithSyntaxSpecContext *withSyntaxSpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectClassDefnContext* objectClassDefn();

  class  WithSyntaxSpecContext : public antlr4::ParserRuleContext {
  public:
    WithSyntaxSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WITH_LITERAL();
    antlr4::tree::TerminalNode *SYNTAX_LITERAL();
    SyntaxListContext *syntaxList();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WithSyntaxSpecContext* withSyntaxSpec();

  class  SyntaxListContext : public antlr4::ParserRuleContext {
  public:
    SyntaxListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    antlr4::tree::TerminalNode *R_BRACE();
    std::vector<TokenOrGroupSpecContext *> tokenOrGroupSpec();
    TokenOrGroupSpecContext* tokenOrGroupSpec(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SyntaxListContext* syntaxList();

  class  TokenOrGroupSpecContext : public antlr4::ParserRuleContext {
  public:
    TokenOrGroupSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RequiredTokenContext *requiredToken();
    OptionalGroupContext *optionalGroup();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TokenOrGroupSpecContext* tokenOrGroupSpec();

  class  OptionalGroupContext : public antlr4::ParserRuleContext {
  public:
    OptionalGroupContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACKET();
    antlr4::tree::TerminalNode *R_BRACKET();
    std::vector<TokenOrGroupSpecContext *> tokenOrGroupSpec();
    TokenOrGroupSpecContext* tokenOrGroupSpec(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptionalGroupContext* optionalGroup();

  class  RequiredTokenContext : public antlr4::ParserRuleContext {
  public:
    RequiredTokenContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LiteralContext *literal();
    PrimitiveFieldNameContext *primitiveFieldName();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RequiredTokenContext* requiredToken();

  class  LiteralContext : public antlr4::ParserRuleContext {
  public:
    LiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *COMMA();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LiteralContext* literal();

  class  PrimitiveFieldNameContext : public antlr4::ParserRuleContext {
  public:
    PrimitiveFieldNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrimitiveFieldNameContext* primitiveFieldName();

  class  FieldSpecContext : public antlr4::ParserRuleContext {
  public:
    FieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    AsnTypeContext *asnType();
    FieldNameContext *fieldName();
    DefinedObjectClassContext *definedObjectClass();
    TypeOptionalitySpecContext *typeOptionalitySpec();
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    ValueSetOptionalitySpecContext *valueSetOptionalitySpec();
    antlr4::tree::TerminalNode *UNIQUE_LITERAL();
    ValueOptionalitySpecContext *valueOptionalitySpec();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    ValueSetContext *valueSet();
    ValueContext *value();
    ObjectSetContext *objectSet();
    ObjectContext *object();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FieldSpecContext* fieldSpec();

  class  TypeFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    TypeFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    TypeOptionalitySpecContext *typeOptionalitySpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeFieldSpecContext* typeFieldSpec();

  class  TypeOptionalitySpecContext : public antlr4::ParserRuleContext {
  public:
    TypeOptionalitySpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    AsnTypeContext *asnType();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeOptionalitySpecContext* typeOptionalitySpec();

  class  FixedTypeValueFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    FixedTypeValueFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    AsnTypeContext *asnType();
    antlr4::tree::TerminalNode *UNIQUE_LITERAL();
    ValueOptionalitySpecContext *valueOptionalitySpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedTypeValueFieldSpecContext* fixedTypeValueFieldSpec();

  class  ValueOptionalitySpecContext : public antlr4::ParserRuleContext {
  public:
    ValueOptionalitySpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    ValueContext *value();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueOptionalitySpecContext* valueOptionalitySpec();

  class  VariableTypeValueFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    VariableTypeValueFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    FieldNameContext *fieldName();
    ValueOptionalitySpecContext *valueOptionalitySpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VariableTypeValueFieldSpecContext* variableTypeValueFieldSpec();

  class  FixedTypeValueSetFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    FixedTypeValueSetFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    AsnTypeContext *asnType();
    ValueSetOptionalitySpecContext *valueSetOptionalitySpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedTypeValueSetFieldSpecContext* fixedTypeValueSetFieldSpec();

  class  ValueSetOptionalitySpecContext : public antlr4::ParserRuleContext {
  public:
    ValueSetOptionalitySpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    ValueSetContext *valueSet();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueSetOptionalitySpecContext* valueSetOptionalitySpec();

  class  ObjectContext : public antlr4::ParserRuleContext {
  public:
    ObjectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DefinedObjectContext *definedObject();
    ParameterizedObjectContext *parameterizedObject();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectContext* object();

  class  ParameterizedObjectContext : public antlr4::ParserRuleContext {
  public:
    ParameterizedObjectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DefinedObjectContext *definedObject();
    ActualParameterListContext *actualParameterList();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterizedObjectContext* parameterizedObject();

  class  DefinedObjectContext : public antlr4::ParserRuleContext {
  public:
    DefinedObjectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *DOT();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DefinedObjectContext* definedObject();

  class  ObjectSetContext : public antlr4::ParserRuleContext {
  public:
    ObjectSetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    ObjectSetSpecContext *objectSetSpec();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectSetContext* objectSet();

  class  ObjectSetSpecContext : public antlr4::ParserRuleContext {
  public:
    ObjectSetSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RootElementSetSpecContext *rootElementSetSpec();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *ELLIPSIS();
    AdditionalElementSetSpecContext *additionalElementSetSpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectSetSpecContext* objectSetSpec();

  class  FieldNameContext : public antlr4::ParserRuleContext {
  public:
    FieldNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> AMPERSAND();
    antlr4::tree::TerminalNode* AMPERSAND(size_t i);
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FieldNameContext* fieldName();

  class  ValueSetContext : public antlr4::ParserRuleContext {
  public:
    ValueSetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    ElementSetSpecsContext *elementSetSpecs();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueSetContext* valueSet();

  class  ElementSetSpecsContext : public antlr4::ParserRuleContext {
  public:
    ElementSetSpecsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RootElementSetSpecContext *rootElementSetSpec();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *ELLIPSIS();
    AdditionalElementSetSpecContext *additionalElementSetSpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementSetSpecsContext* elementSetSpecs();

  class  RootElementSetSpecContext : public antlr4::ParserRuleContext {
  public:
    RootElementSetSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementSetSpecContext *elementSetSpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootElementSetSpecContext* rootElementSetSpec();

  class  AdditionalElementSetSpecContext : public antlr4::ParserRuleContext {
  public:
    AdditionalElementSetSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementSetSpecContext *elementSetSpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AdditionalElementSetSpecContext* additionalElementSetSpec();

  class  ElementSetSpecContext : public antlr4::ParserRuleContext {
  public:
    ElementSetSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnionsContext *unions();
    antlr4::tree::TerminalNode *ALL_LITERAL();
    ExclusionsContext *exclusions();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementSetSpecContext* elementSetSpec();

  class  UnionsContext : public antlr4::ParserRuleContext {
  public:
    UnionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IntersectionsContext *> intersections();
    IntersectionsContext* intersections(size_t i);
    std::vector<UnionMarkContext *> unionMark();
    UnionMarkContext* unionMark(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnionsContext* unions();

  class  ExclusionsContext : public antlr4::ParserRuleContext {
  public:
    ExclusionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXCEPT_LITERAL();
    ElementsContext *elements();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExclusionsContext* exclusions();

  class  IntersectionsContext : public antlr4::ParserRuleContext {
  public:
    IntersectionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IntersectionElementsContext *> intersectionElements();
    IntersectionElementsContext* intersectionElements(size_t i);
    std::vector<IntersectionMarkContext *> intersectionMark();
    IntersectionMarkContext* intersectionMark(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntersectionsContext* intersections();

  class  UnionMarkContext : public antlr4::ParserRuleContext {
  public:
    UnionMarkContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PIPE();
    antlr4::tree::TerminalNode *UNION_LITERAL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnionMarkContext* unionMark();

  class  IntersectionMarkContext : public antlr4::ParserRuleContext {
  public:
    IntersectionMarkContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *POWER();
    antlr4::tree::TerminalNode *INTERSECTION_LITERAL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntersectionMarkContext* intersectionMark();

  class  ElementsContext : public antlr4::ParserRuleContext {
  public:
    ElementsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SubtypeElementsContext *subtypeElements();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementsContext* elements();

  class  ObjectSetElementsContext : public antlr4::ParserRuleContext {
  public:
    ObjectSetElementsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ObjectContext *object();
    DefinedObjectContext *definedObject();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectSetElementsContext* objectSetElements();

  class  IntersectionElementsContext : public antlr4::ParserRuleContext {
  public:
    IntersectionElementsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementsContext *elements();
    ExclusionsContext *exclusions();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntersectionElementsContext* intersectionElements();

  class  SubtypeElementsContext : public antlr4::ParserRuleContext {
  public:
    SubtypeElementsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOUBLE_DOT();
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);
    antlr4::tree::TerminalNode *MIN_LITERAL();
    antlr4::tree::TerminalNode *MAX_LITERAL();
    std::vector<antlr4::tree::TerminalNode *> LESS_THAN();
    antlr4::tree::TerminalNode* LESS_THAN(size_t i);
    SizeConstraintContext *sizeConstraint();
    antlr4::tree::TerminalNode *PATTERN_LITERAL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SubtypeElementsContext* subtypeElements();

  class  VariableTypeValueSetFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    VariableTypeValueSetFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    FieldNameContext *fieldName();
    ValueSetOptionalitySpecContext *valueSetOptionalitySpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VariableTypeValueSetFieldSpecContext* variableTypeValueSetFieldSpec();

  class  ObjectFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    ObjectFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    DefinedObjectClassContext *definedObjectClass();
    ObjectOptionalitySpecContext *objectOptionalitySpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectFieldSpecContext* objectFieldSpec();

  class  ObjectOptionalitySpecContext : public antlr4::ParserRuleContext {
  public:
    ObjectOptionalitySpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    ObjectContext *object();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectOptionalitySpecContext* objectOptionalitySpec();

  class  ObjectSetFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    ObjectSetFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    DefinedObjectClassContext *definedObjectClass();
    ObjectSetOptionalitySpecContext *objectSetOptionalitySpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectSetFieldSpecContext* objectSetFieldSpec();

  class  ObjectSetOptionalitySpecContext : public antlr4::ParserRuleContext {
  public:
    ObjectSetOptionalitySpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    ObjectSetContext *objectSet();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectSetOptionalitySpecContext* objectSetOptionalitySpec();

  class  TypeAssignmentContext : public antlr4::ParserRuleContext {
  public:
    TypeAssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASSIGN_OP();
    AsnTypeContext *asnType();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeAssignmentContext* typeAssignment();

  class  ValueAssignmentContext : public antlr4::ParserRuleContext {
  public:
    ValueAssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AsnTypeContext *asnType();
    antlr4::tree::TerminalNode *ASSIGN_OP();
    ValueContext *value();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueAssignmentContext* valueAssignment();

  class  AsnTypeContext : public antlr4::ParserRuleContext {
  public:
    AsnTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BuiltinTypeContext *builtinType();
    ReferencedTypeContext *referencedType();
    std::vector<ConstraintContext *> constraint();
    ConstraintContext* constraint(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AsnTypeContext* asnType();

  class  BuiltinTypeContext : public antlr4::ParserRuleContext {
  public:
    BuiltinTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    OctetStringTypeContext *octetStringType();
    BitStringTypeContext *bitStringType();
    ChoiceTypeContext *choiceType();
    EnumeratedTypeContext *enumeratedType();
    IntegerTypeContext *integerType();
    SequenceTypeContext *sequenceType();
    SequenceOfTypeContext *sequenceOfType();
    SetTypeContext *setType();
    SetOfTypeContext *setOfType();
    ObjectidentifiertypeContext *objectidentifiertype();
    ObjectClassFieldTypeContext *objectClassFieldType();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BuiltinTypeContext* builtinType();

  class  ObjectClassFieldTypeContext : public antlr4::ParserRuleContext {
  public:
    ObjectClassFieldTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DefinedObjectClassContext *definedObjectClass();
    antlr4::tree::TerminalNode *DOT();
    FieldNameContext *fieldName();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectClassFieldTypeContext* objectClassFieldType();

  class  SetTypeContext : public antlr4::ParserRuleContext {
  public:
    SetTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SET_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    antlr4::tree::TerminalNode *R_BRACE();
    ExtensionAndExceptionContext *extensionAndException();
    OptionalExtensionMarkerContext *optionalExtensionMarker();
    ComponentTypeListsContext *componentTypeLists();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetTypeContext* setType();

  class  SetOfTypeContext : public antlr4::ParserRuleContext {
  public:
    SetOfTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SET_LITERAL();
    antlr4::tree::TerminalNode *OF_LITERAL();
    AsnTypeContext *asnType();
    NamedTypeContext *namedType();
    ConstraintContext *constraint();
    SizeConstraintContext *sizeConstraint();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetOfTypeContext* setOfType();

  class  ReferencedTypeContext : public antlr4::ParserRuleContext {
  public:
    ReferencedTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DefinedTypeContext *definedType();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReferencedTypeContext* referencedType();

  class  DefinedTypeContext : public antlr4::ParserRuleContext {
  public:
    DefinedTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    antlr4::tree::TerminalNode *DOT();
    ActualParameterListContext *actualParameterList();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DefinedTypeContext* definedType();

  class  ConstraintContext : public antlr4::ParserRuleContext {
  public:
    ConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_PARAN();
    ConstraintSpecContext *constraintSpec();
    antlr4::tree::TerminalNode *R_PARAN();
    ExceptionSpecContext *exceptionSpec();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstraintContext* constraint();

  class  ConstraintSpecContext : public antlr4::ParserRuleContext {
  public:
    ConstraintSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GeneralConstraintContext *generalConstraint();
    SubtypeConstraintContext *subtypeConstraint();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstraintSpecContext* constraintSpec();

  class  UserDefinedConstraintContext : public antlr4::ParserRuleContext {
  public:
    UserDefinedConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CONSTRAINED_LITERAL();
    antlr4::tree::TerminalNode *BY_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    std::vector<UserDefinedConstraintParameterContext *> userDefinedConstraintParameter();
    UserDefinedConstraintParameterContext* userDefinedConstraintParameter(size_t i);
    antlr4::tree::TerminalNode *R_BRACE();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UserDefinedConstraintContext* userDefinedConstraint();

  class  GeneralConstraintContext : public antlr4::ParserRuleContext {
  public:
    GeneralConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UserDefinedConstraintContext *userDefinedConstraint();
    TableConstraintContext *tableConstraint();
    ContentsConstraintContext *contentsConstraint();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GeneralConstraintContext* generalConstraint();

  class  UserDefinedConstraintParameterContext : public antlr4::ParserRuleContext {
  public:
    UserDefinedConstraintParameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GovernorContext *governor();
    antlr4::tree::TerminalNode *COLON();
    ValueContext *value();
    ValueSetContext *valueSet();
    ObjectContext *object();
    ObjectSetContext *objectSet();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UserDefinedConstraintParameterContext* userDefinedConstraintParameter();

  class  TableConstraintContext : public antlr4::ParserRuleContext {
  public:
    TableConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ComponentRelationConstraintContext *componentRelationConstraint();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableConstraintContext* tableConstraint();

  class  SimpleTableConstraintContext : public antlr4::ParserRuleContext {
  public:
    SimpleTableConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ObjectSetContext *objectSet();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleTableConstraintContext* simpleTableConstraint();

  class  ContentsConstraintContext : public antlr4::ParserRuleContext {
  public:
    ContentsConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CONTAINING_LITERAL();
    AsnTypeContext *asnType();
    antlr4::tree::TerminalNode *ENCODED_LITERAL();
    antlr4::tree::TerminalNode *BY_LITERAL();
    ValueContext *value();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ContentsConstraintContext* contentsConstraint();

  class  SubtypeConstraintContext : public antlr4::ParserRuleContext {
  public:
    SubtypeConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementSetSpecsContext *elementSetSpecs();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SubtypeConstraintContext* subtypeConstraint();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BuiltinValueContext *builtinValue();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueContext* value();

  class  BuiltinValueContext : public antlr4::ParserRuleContext {
  public:
    BuiltinValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EnumeratedValueContext *enumeratedValue();
    IntegerValueContext *integerValue();
    ChoiceValueContext *choiceValue();
    ObjectIdentifierValueContext *objectIdentifierValue();
    BooleanValueContext *booleanValue();
    antlr4::tree::TerminalNode *CSTRING();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BuiltinValueContext* builtinValue();

  class  ObjectIdentifierValueContext : public antlr4::ParserRuleContext {
  public:
    ObjectIdentifierValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    ObjIdComponentsListContext *objIdComponentsList();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectIdentifierValueContext* objectIdentifierValue();

  class  ObjIdComponentsListContext : public antlr4::ParserRuleContext {
  public:
    ObjIdComponentsListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ObjIdComponentsContext *> objIdComponents();
    ObjIdComponentsContext* objIdComponents(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjIdComponentsListContext* objIdComponentsList();

  class  ObjIdComponentsContext : public antlr4::ParserRuleContext {
  public:
    ObjIdComponentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *L_PARAN();
    antlr4::tree::TerminalNode *R_PARAN();
    DefinedValueContext *definedValue();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjIdComponentsContext* objIdComponents();

  class  IntegerValueContext : public antlr4::ParserRuleContext {
  public:
    IntegerValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SignedNumberContext *signedNumber();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntegerValueContext* integerValue();

  class  ChoiceValueContext : public antlr4::ParserRuleContext {
  public:
    ChoiceValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *COLON();
    ValueContext *value();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ChoiceValueContext* choiceValue();

  class  EnumeratedValueContext : public antlr4::ParserRuleContext {
  public:
    EnumeratedValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EnumeratedValueContext* enumeratedValue();

  class  SignedNumberContext : public antlr4::ParserRuleContext {
  public:
    SignedNumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *MINUS();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SignedNumberContext* signedNumber();

  class  ChoiceTypeContext : public antlr4::ParserRuleContext {
  public:
    ChoiceTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CHOICE_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    AlternativeTypeListsContext *alternativeTypeLists();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ChoiceTypeContext* choiceType();

  class  AlternativeTypeListsContext : public antlr4::ParserRuleContext {
  public:
    AlternativeTypeListsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RootAlternativeTypeListContext *rootAlternativeTypeList();
    antlr4::tree::TerminalNode *COMMA();
    ExtensionAndExceptionContext *extensionAndException();
    ExtensionAdditionAlternativesContext *extensionAdditionAlternatives();
    OptionalExtensionMarkerContext *optionalExtensionMarker();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AlternativeTypeListsContext* alternativeTypeLists();

  class  ExtensionAdditionAlternativesContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionAlternativesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COMMA();
    ExtensionAdditionAlternativesListContext *extensionAdditionAlternativesList();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionAlternativesContext* extensionAdditionAlternatives();

  class  ExtensionAdditionAlternativesListContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionAlternativesListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExtensionAdditionAlternativeContext *> extensionAdditionAlternative();
    ExtensionAdditionAlternativeContext* extensionAdditionAlternative(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionAlternativesListContext* extensionAdditionAlternativesList();

  class  ExtensionAdditionAlternativeContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionAlternativeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExtensionAdditionAlternativesGroupContext *extensionAdditionAlternativesGroup();
    NamedTypeContext *namedType();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionAlternativeContext* extensionAdditionAlternative();

  class  ExtensionAdditionAlternativesGroupContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionAlternativesGroupContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOUBLE_L_BRACKET();
    VersionNumberContext *versionNumber();
    AlternativeTypeListContext *alternativeTypeList();
    antlr4::tree::TerminalNode *DOUBLE_R_BRACKET();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionAlternativesGroupContext* extensionAdditionAlternativesGroup();

  class  RootAlternativeTypeListContext : public antlr4::ParserRuleContext {
  public:
    RootAlternativeTypeListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AlternativeTypeListContext *alternativeTypeList();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootAlternativeTypeListContext* rootAlternativeTypeList();

  class  AlternativeTypeListContext : public antlr4::ParserRuleContext {
  public:
    AlternativeTypeListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<NamedTypeContext *> namedType();
    NamedTypeContext* namedType(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AlternativeTypeListContext* alternativeTypeList();

  class  NamedTypeContext : public antlr4::ParserRuleContext {
  public:
    NamedTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    AsnTypeContext *asnType();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamedTypeContext* namedType();

  class  EnumeratedTypeContext : public antlr4::ParserRuleContext {
  public:
    EnumeratedTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ENUMERATED_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    EnumerationsContext *enumerations();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EnumeratedTypeContext* enumeratedType();

  class  EnumerationsContext : public antlr4::ParserRuleContext {
  public:
    EnumerationsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RootEnumerationContext *rootEnumeration();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *ELLIPSIS();
    ExceptionSpecContext *exceptionSpec();
    AdditionalEnumerationContext *additionalEnumeration();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EnumerationsContext* enumerations();

  class  RootEnumerationContext : public antlr4::ParserRuleContext {
  public:
    RootEnumerationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EnumerationContext *enumeration();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootEnumerationContext* rootEnumeration();

  class  EnumerationContext : public antlr4::ParserRuleContext {
  public:
    EnumerationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<EnumerationItemContext *> enumerationItem();
    EnumerationItemContext* enumerationItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EnumerationContext* enumeration();

  class  EnumerationItemContext : public antlr4::ParserRuleContext {
  public:
    EnumerationItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    NamedNumberContext *namedNumber();
    ValueContext *value();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EnumerationItemContext* enumerationItem();

  class  NamedNumberContext : public antlr4::ParserRuleContext {
  public:
    NamedNumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *L_PARAN();
    antlr4::tree::TerminalNode *R_PARAN();
    SignedNumberContext *signedNumber();
    DefinedValueContext *definedValue();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamedNumberContext* namedNumber();

  class  DefinedValueContext : public antlr4::ParserRuleContext {
  public:
    DefinedValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ParameterizedValueContext *parameterizedValue();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DefinedValueContext* definedValue();

  class  ParameterizedValueContext : public antlr4::ParserRuleContext {
  public:
    ParameterizedValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimpleDefinedValueContext *simpleDefinedValue();
    ActualParameterListContext *actualParameterList();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterizedValueContext* parameterizedValue();

  class  SimpleDefinedValueContext : public antlr4::ParserRuleContext {
  public:
    SimpleDefinedValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    antlr4::tree::TerminalNode *DOT();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleDefinedValueContext* simpleDefinedValue();

  class  ActualParameterListContext : public antlr4::ParserRuleContext {
  public:
    ActualParameterListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    std::vector<ActualParameterContext *> actualParameter();
    ActualParameterContext* actualParameter(size_t i);
    antlr4::tree::TerminalNode *R_BRACE();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ActualParameterListContext* actualParameterList();

  class  ActualParameterContext : public antlr4::ParserRuleContext {
  public:
    ActualParameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AsnTypeContext *asnType();
    ValueContext *value();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ActualParameterContext* actualParameter();

  class  ExceptionSpecContext : public antlr4::ParserRuleContext {
  public:
    ExceptionSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXCLAM();
    ExceptionIdentificationContext *exceptionIdentification();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExceptionSpecContext* exceptionSpec();

  class  ExceptionIdentificationContext : public antlr4::ParserRuleContext {
  public:
    ExceptionIdentificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SignedNumberContext *signedNumber();
    DefinedValueContext *definedValue();
    AsnTypeContext *asnType();
    antlr4::tree::TerminalNode *COLON();
    ValueContext *value();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExceptionIdentificationContext* exceptionIdentification();

  class  AdditionalEnumerationContext : public antlr4::ParserRuleContext {
  public:
    AdditionalEnumerationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EnumerationContext *enumeration();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AdditionalEnumerationContext* additionalEnumeration();

  class  IntegerTypeContext : public antlr4::ParserRuleContext {
  public:
    IntegerTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    NamedNumberListContext *namedNumberList();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntegerTypeContext* integerType();

  class  NamedNumberListContext : public antlr4::ParserRuleContext {
  public:
    NamedNumberListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<NamedNumberContext *> namedNumber();
    NamedNumberContext* namedNumber(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamedNumberListContext* namedNumberList();

  class  ObjectidentifiertypeContext : public antlr4::ParserRuleContext {
  public:
    ObjectidentifiertypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OBJECT_LITERAL();
    antlr4::tree::TerminalNode *IDENTIFIER_LITERAL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectidentifiertypeContext* objectidentifiertype();

  class  ComponentRelationConstraintContext : public antlr4::ParserRuleContext {
  public:
    ComponentRelationConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> L_BRACE();
    antlr4::tree::TerminalNode* L_BRACE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> R_BRACE();
    antlr4::tree::TerminalNode* R_BRACE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    std::vector<AtNotationContext *> atNotation();
    AtNotationContext* atNotation(size_t i);
    antlr4::tree::TerminalNode *DOT();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentRelationConstraintContext* componentRelationConstraint();

  class  AtNotationContext : public antlr4::ParserRuleContext {
  public:
    AtNotationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ComponentIdListContext *componentIdList();
    antlr4::tree::TerminalNode *A_ROND();
    antlr4::tree::TerminalNode *A_ROND_DOT();
    LevelContext *level();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AtNotationContext* atNotation();

  class  LevelContext : public antlr4::ParserRuleContext {
  public:
    LevelContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOT();
    LevelContext *level();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LevelContext* level();

  class  ComponentIdListContext : public antlr4::ParserRuleContext {
  public:
    ComponentIdListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentIdListContext* componentIdList();

  class  OctetStringTypeContext : public antlr4::ParserRuleContext {
  public:
    OctetStringTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OCTET_LITERAL();
    antlr4::tree::TerminalNode *STRING_LITERAL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OctetStringTypeContext* octetStringType();

  class  BitStringTypeContext : public antlr4::ParserRuleContext {
  public:
    BitStringTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BIT_LITERAL();
    antlr4::tree::TerminalNode *STRING_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    NamedBitListContext *namedBitList();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BitStringTypeContext* bitStringType();

  class  NamedBitListContext : public antlr4::ParserRuleContext {
  public:
    NamedBitListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<NamedBitContext *> namedBit();
    NamedBitContext* namedBit(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamedBitListContext* namedBitList();

  class  NamedBitContext : public antlr4::ParserRuleContext {
  public:
    NamedBitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *L_PARAN();
    antlr4::tree::TerminalNode *R_PARAN();
    antlr4::tree::TerminalNode *NUMBER();
    DefinedValueContext *definedValue();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamedBitContext* namedBit();

  class  BooleanValueContext : public antlr4::ParserRuleContext {
  public:
    BooleanValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TRUE_LITERAL();
    antlr4::tree::TerminalNode *FALSE_LITERAL();
    antlr4::tree::TerminalNode *TRUE_SMALL_LITERAL();
    antlr4::tree::TerminalNode *FALSE_SMALL_LITERAL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BooleanValueContext* booleanValue();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

