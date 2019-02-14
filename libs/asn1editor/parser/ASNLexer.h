
// Generated from ASN.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"




class  ASNLexer : public antlr4::Lexer {
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

  ASNLexer(antlr4::CharStream *input);
  ~ASNLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

