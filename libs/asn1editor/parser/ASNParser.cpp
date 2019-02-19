
// Generated from ASN.g4 by ANTLR 4.7.1


#include "ASNVisitor.h"

#include "ASNParser.h"


using namespace antlrcpp;
using namespace antlr4;

ASNParser::ASNParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

ASNParser::~ASNParser() {
  delete _interpreter;
}

std::string ASNParser::getGrammarFileName() const {
  return "ASN.g4";
}

const std::vector<std::string>& ASNParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& ASNParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ModuleDefinitionContext ------------------------------------------------------------------

ASNParser::ModuleDefinitionContext::ModuleDefinitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ASNParser::ModuleDefinitionContext::IDENTIFIER() {
  return getTokens(ASNParser::IDENTIFIER);
}

tree::TerminalNode* ASNParser::ModuleDefinitionContext::IDENTIFIER(size_t i) {
  return getToken(ASNParser::IDENTIFIER, i);
}

tree::TerminalNode* ASNParser::ModuleDefinitionContext::DEFINITIONS_LITERAL() {
  return getToken(ASNParser::DEFINITIONS_LITERAL, 0);
}

ASNParser::TagDefaultContext* ASNParser::ModuleDefinitionContext::tagDefault() {
  return getRuleContext<ASNParser::TagDefaultContext>(0);
}

ASNParser::ExtensionDefaultContext* ASNParser::ModuleDefinitionContext::extensionDefault() {
  return getRuleContext<ASNParser::ExtensionDefaultContext>(0);
}

tree::TerminalNode* ASNParser::ModuleDefinitionContext::ASSIGN_OP() {
  return getToken(ASNParser::ASSIGN_OP, 0);
}

tree::TerminalNode* ASNParser::ModuleDefinitionContext::BEGIN_LITERAL() {
  return getToken(ASNParser::BEGIN_LITERAL, 0);
}

ASNParser::ModuleBodyContext* ASNParser::ModuleDefinitionContext::moduleBody() {
  return getRuleContext<ASNParser::ModuleBodyContext>(0);
}

tree::TerminalNode* ASNParser::ModuleDefinitionContext::END_LITERAL() {
  return getToken(ASNParser::END_LITERAL, 0);
}

tree::TerminalNode* ASNParser::ModuleDefinitionContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

tree::TerminalNode* ASNParser::ModuleDefinitionContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}

std::vector<tree::TerminalNode *> ASNParser::ModuleDefinitionContext::L_PARAN() {
  return getTokens(ASNParser::L_PARAN);
}

tree::TerminalNode* ASNParser::ModuleDefinitionContext::L_PARAN(size_t i) {
  return getToken(ASNParser::L_PARAN, i);
}

std::vector<tree::TerminalNode *> ASNParser::ModuleDefinitionContext::NUMBER() {
  return getTokens(ASNParser::NUMBER);
}

tree::TerminalNode* ASNParser::ModuleDefinitionContext::NUMBER(size_t i) {
  return getToken(ASNParser::NUMBER, i);
}

std::vector<tree::TerminalNode *> ASNParser::ModuleDefinitionContext::R_PARAN() {
  return getTokens(ASNParser::R_PARAN);
}

tree::TerminalNode* ASNParser::ModuleDefinitionContext::R_PARAN(size_t i) {
  return getToken(ASNParser::R_PARAN, i);
}


size_t ASNParser::ModuleDefinitionContext::getRuleIndex() const {
  return ASNParser::RuleModuleDefinition;
}

antlrcpp::Any ASNParser::ModuleDefinitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitModuleDefinition(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ModuleDefinitionContext* ASNParser::moduleDefinition() {
  ModuleDefinitionContext *_localctx = _tracker.createInstance<ModuleDefinitionContext>(_ctx, getState());
  enterRule(_localctx, 0, ASNParser::RuleModuleDefinition);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(296);
    match(ASNParser::IDENTIFIER);
    setState(308);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::L_BRACE) {
      setState(297);
      match(ASNParser::L_BRACE);
      setState(304);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ASNParser::IDENTIFIER) {
        setState(298);
        match(ASNParser::IDENTIFIER);
        setState(299);
        match(ASNParser::L_PARAN);
        setState(300);
        match(ASNParser::NUMBER);
        setState(301);
        match(ASNParser::R_PARAN);
        setState(306);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(307);
      match(ASNParser::R_BRACE);
    }
    setState(310);
    match(ASNParser::DEFINITIONS_LITERAL);
    setState(311);
    tagDefault();
    setState(312);
    extensionDefault();
    setState(313);
    match(ASNParser::ASSIGN_OP);
    setState(314);
    match(ASNParser::BEGIN_LITERAL);
    setState(315);
    moduleBody();
    setState(316);
    match(ASNParser::END_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TagDefaultContext ------------------------------------------------------------------

ASNParser::TagDefaultContext::TagDefaultContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::TagDefaultContext::TAGS_LITERAL() {
  return getToken(ASNParser::TAGS_LITERAL, 0);
}

tree::TerminalNode* ASNParser::TagDefaultContext::EXPLICIT_LITERAL() {
  return getToken(ASNParser::EXPLICIT_LITERAL, 0);
}

tree::TerminalNode* ASNParser::TagDefaultContext::IMPLICIT_LITERAL() {
  return getToken(ASNParser::IMPLICIT_LITERAL, 0);
}

tree::TerminalNode* ASNParser::TagDefaultContext::AUTOMATIC_LITERAL() {
  return getToken(ASNParser::AUTOMATIC_LITERAL, 0);
}


size_t ASNParser::TagDefaultContext::getRuleIndex() const {
  return ASNParser::RuleTagDefault;
}

antlrcpp::Any ASNParser::TagDefaultContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitTagDefault(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::TagDefaultContext* ASNParser::tagDefault() {
  TagDefaultContext *_localctx = _tracker.createInstance<TagDefaultContext>(_ctx, getState());
  enterRule(_localctx, 2, ASNParser::RuleTagDefault);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(320);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 70) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 70)) & ((1ULL << (ASNParser::EXPLICIT_LITERAL - 70))
      | (1ULL << (ASNParser::IMPLICIT_LITERAL - 70))
      | (1ULL << (ASNParser::AUTOMATIC_LITERAL - 70)))) != 0)) {
      setState(318);
      _la = _input->LA(1);
      if (!(((((_la - 70) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 70)) & ((1ULL << (ASNParser::EXPLICIT_LITERAL - 70))
        | (1ULL << (ASNParser::IMPLICIT_LITERAL - 70))
        | (1ULL << (ASNParser::AUTOMATIC_LITERAL - 70)))) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(319);
      match(ASNParser::TAGS_LITERAL);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExtensionDefaultContext ------------------------------------------------------------------

ASNParser::ExtensionDefaultContext::ExtensionDefaultContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ExtensionDefaultContext::EXTENSIBILITY_LITERAL() {
  return getToken(ASNParser::EXTENSIBILITY_LITERAL, 0);
}

tree::TerminalNode* ASNParser::ExtensionDefaultContext::IMPLIED_LITERAL() {
  return getToken(ASNParser::IMPLIED_LITERAL, 0);
}


size_t ASNParser::ExtensionDefaultContext::getRuleIndex() const {
  return ASNParser::RuleExtensionDefault;
}

antlrcpp::Any ASNParser::ExtensionDefaultContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExtensionDefault(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExtensionDefaultContext* ASNParser::extensionDefault() {
  ExtensionDefaultContext *_localctx = _tracker.createInstance<ExtensionDefaultContext>(_ctx, getState());
  enterRule(_localctx, 4, ASNParser::RuleExtensionDefault);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(324);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::EXTENSIBILITY_LITERAL) {
      setState(322);
      match(ASNParser::EXTENSIBILITY_LITERAL);
      setState(323);
      match(ASNParser::IMPLIED_LITERAL);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ModuleBodyContext ------------------------------------------------------------------

ASNParser::ModuleBodyContext::ModuleBodyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ExportsContext* ASNParser::ModuleBodyContext::exports() {
  return getRuleContext<ASNParser::ExportsContext>(0);
}

ASNParser::ImportsContext* ASNParser::ModuleBodyContext::imports() {
  return getRuleContext<ASNParser::ImportsContext>(0);
}

ASNParser::AssignmentListContext* ASNParser::ModuleBodyContext::assignmentList() {
  return getRuleContext<ASNParser::AssignmentListContext>(0);
}


size_t ASNParser::ModuleBodyContext::getRuleIndex() const {
  return ASNParser::RuleModuleBody;
}

antlrcpp::Any ASNParser::ModuleBodyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitModuleBody(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ModuleBodyContext* ASNParser::moduleBody() {
  ModuleBodyContext *_localctx = _tracker.createInstance<ModuleBodyContext>(_ctx, getState());
  enterRule(_localctx, 6, ASNParser::RuleModuleBody);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(330);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 66) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 66)) & ((1ULL << (ASNParser::IMPORTS_LITERAL - 66))
      | (1ULL << (ASNParser::EXPORTS_LITERAL - 66))
      | (1ULL << (ASNParser::IDENTIFIER - 66)))) != 0)) {
      setState(326);
      exports();
      setState(327);
      imports();
      setState(328);
      assignmentList();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExportsContext ------------------------------------------------------------------

ASNParser::ExportsContext::ExportsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ExportsContext::EXPORTS_LITERAL() {
  return getToken(ASNParser::EXPORTS_LITERAL, 0);
}

ASNParser::SymbolsExportedContext* ASNParser::ExportsContext::symbolsExported() {
  return getRuleContext<ASNParser::SymbolsExportedContext>(0);
}

tree::TerminalNode* ASNParser::ExportsContext::SEMI_COLON() {
  return getToken(ASNParser::SEMI_COLON, 0);
}

tree::TerminalNode* ASNParser::ExportsContext::ALL_LITERAL() {
  return getToken(ASNParser::ALL_LITERAL, 0);
}


size_t ASNParser::ExportsContext::getRuleIndex() const {
  return ASNParser::RuleExports;
}

antlrcpp::Any ASNParser::ExportsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExports(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExportsContext* ASNParser::exports() {
  ExportsContext *_localctx = _tracker.createInstance<ExportsContext>(_ctx, getState());
  enterRule(_localctx, 8, ASNParser::RuleExports);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(339);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      setState(332);
      match(ASNParser::EXPORTS_LITERAL);
      setState(333);
      symbolsExported();
      setState(334);
      match(ASNParser::SEMI_COLON);
      break;
    }

    case 2: {
      setState(336);
      match(ASNParser::EXPORTS_LITERAL);
      setState(337);
      match(ASNParser::ALL_LITERAL);
      setState(338);
      match(ASNParser::SEMI_COLON);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SymbolsExportedContext ------------------------------------------------------------------

ASNParser::SymbolsExportedContext::SymbolsExportedContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::SymbolListContext* ASNParser::SymbolsExportedContext::symbolList() {
  return getRuleContext<ASNParser::SymbolListContext>(0);
}


size_t ASNParser::SymbolsExportedContext::getRuleIndex() const {
  return ASNParser::RuleSymbolsExported;
}

antlrcpp::Any ASNParser::SymbolsExportedContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSymbolsExported(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SymbolsExportedContext* ASNParser::symbolsExported() {
  SymbolsExportedContext *_localctx = _tracker.createInstance<SymbolsExportedContext>(_ctx, getState());
  enterRule(_localctx, 10, ASNParser::RuleSymbolsExported);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(342);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::IDENTIFIER) {
      setState(341);
      symbolList();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ImportsContext ------------------------------------------------------------------

ASNParser::ImportsContext::ImportsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ImportsContext::IMPORTS_LITERAL() {
  return getToken(ASNParser::IMPORTS_LITERAL, 0);
}

ASNParser::SymbolsImportedContext* ASNParser::ImportsContext::symbolsImported() {
  return getRuleContext<ASNParser::SymbolsImportedContext>(0);
}

tree::TerminalNode* ASNParser::ImportsContext::SEMI_COLON() {
  return getToken(ASNParser::SEMI_COLON, 0);
}


size_t ASNParser::ImportsContext::getRuleIndex() const {
  return ASNParser::RuleImports;
}

antlrcpp::Any ASNParser::ImportsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitImports(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ImportsContext* ASNParser::imports() {
  ImportsContext *_localctx = _tracker.createInstance<ImportsContext>(_ctx, getState());
  enterRule(_localctx, 12, ASNParser::RuleImports);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(348);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::IMPORTS_LITERAL) {
      setState(344);
      match(ASNParser::IMPORTS_LITERAL);
      setState(345);
      symbolsImported();
      setState(346);
      match(ASNParser::SEMI_COLON);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SymbolsImportedContext ------------------------------------------------------------------

ASNParser::SymbolsImportedContext::SymbolsImportedContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::SymbolsFromModuleListContext* ASNParser::SymbolsImportedContext::symbolsFromModuleList() {
  return getRuleContext<ASNParser::SymbolsFromModuleListContext>(0);
}


size_t ASNParser::SymbolsImportedContext::getRuleIndex() const {
  return ASNParser::RuleSymbolsImported;
}

antlrcpp::Any ASNParser::SymbolsImportedContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSymbolsImported(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SymbolsImportedContext* ASNParser::symbolsImported() {
  SymbolsImportedContext *_localctx = _tracker.createInstance<SymbolsImportedContext>(_ctx, getState());
  enterRule(_localctx, 14, ASNParser::RuleSymbolsImported);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(351);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::IDENTIFIER) {
      setState(350);
      symbolsFromModuleList();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SymbolsFromModuleListContext ------------------------------------------------------------------

ASNParser::SymbolsFromModuleListContext::SymbolsFromModuleListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::SymbolsFromModuleContext *> ASNParser::SymbolsFromModuleListContext::symbolsFromModule() {
  return getRuleContexts<ASNParser::SymbolsFromModuleContext>();
}

ASNParser::SymbolsFromModuleContext* ASNParser::SymbolsFromModuleListContext::symbolsFromModule(size_t i) {
  return getRuleContext<ASNParser::SymbolsFromModuleContext>(i);
}


size_t ASNParser::SymbolsFromModuleListContext::getRuleIndex() const {
  return ASNParser::RuleSymbolsFromModuleList;
}

antlrcpp::Any ASNParser::SymbolsFromModuleListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSymbolsFromModuleList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SymbolsFromModuleListContext* ASNParser::symbolsFromModuleList() {
  SymbolsFromModuleListContext *_localctx = _tracker.createInstance<SymbolsFromModuleListContext>(_ctx, getState());
  enterRule(_localctx, 16, ASNParser::RuleSymbolsFromModuleList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(353);
    symbolsFromModule();
    setState(357);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::IDENTIFIER) {
      setState(354);
      symbolsFromModule();
      setState(359);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SymbolsFromModuleContext ------------------------------------------------------------------

ASNParser::SymbolsFromModuleContext::SymbolsFromModuleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::SymbolListContext* ASNParser::SymbolsFromModuleContext::symbolList() {
  return getRuleContext<ASNParser::SymbolListContext>(0);
}

tree::TerminalNode* ASNParser::SymbolsFromModuleContext::FROM_LITERAL() {
  return getToken(ASNParser::FROM_LITERAL, 0);
}

ASNParser::GlobalModuleReferenceContext* ASNParser::SymbolsFromModuleContext::globalModuleReference() {
  return getRuleContext<ASNParser::GlobalModuleReferenceContext>(0);
}


size_t ASNParser::SymbolsFromModuleContext::getRuleIndex() const {
  return ASNParser::RuleSymbolsFromModule;
}

antlrcpp::Any ASNParser::SymbolsFromModuleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSymbolsFromModule(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SymbolsFromModuleContext* ASNParser::symbolsFromModule() {
  SymbolsFromModuleContext *_localctx = _tracker.createInstance<SymbolsFromModuleContext>(_ctx, getState());
  enterRule(_localctx, 18, ASNParser::RuleSymbolsFromModule);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(360);
    symbolList();
    setState(361);
    match(ASNParser::FROM_LITERAL);
    setState(362);
    globalModuleReference();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GlobalModuleReferenceContext ------------------------------------------------------------------

ASNParser::GlobalModuleReferenceContext::GlobalModuleReferenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::GlobalModuleReferenceContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

ASNParser::AssignedIdentifierContext* ASNParser::GlobalModuleReferenceContext::assignedIdentifier() {
  return getRuleContext<ASNParser::AssignedIdentifierContext>(0);
}


size_t ASNParser::GlobalModuleReferenceContext::getRuleIndex() const {
  return ASNParser::RuleGlobalModuleReference;
}

antlrcpp::Any ASNParser::GlobalModuleReferenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitGlobalModuleReference(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::GlobalModuleReferenceContext* ASNParser::globalModuleReference() {
  GlobalModuleReferenceContext *_localctx = _tracker.createInstance<GlobalModuleReferenceContext>(_ctx, getState());
  enterRule(_localctx, 20, ASNParser::RuleGlobalModuleReference);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(364);
    match(ASNParser::IDENTIFIER);
    setState(365);
    assignedIdentifier();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignedIdentifierContext ------------------------------------------------------------------

ASNParser::AssignedIdentifierContext::AssignedIdentifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ASNParser::AssignedIdentifierContext::getRuleIndex() const {
  return ASNParser::RuleAssignedIdentifier;
}

antlrcpp::Any ASNParser::AssignedIdentifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitAssignedIdentifier(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::AssignedIdentifierContext* ASNParser::assignedIdentifier() {
  AssignedIdentifierContext *_localctx = _tracker.createInstance<AssignedIdentifierContext>(_ctx, getState());
  enterRule(_localctx, 22, ASNParser::RuleAssignedIdentifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);

   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SymbolListContext ------------------------------------------------------------------

ASNParser::SymbolListContext::SymbolListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::SymbolContext *> ASNParser::SymbolListContext::symbol() {
  return getRuleContexts<ASNParser::SymbolContext>();
}

ASNParser::SymbolContext* ASNParser::SymbolListContext::symbol(size_t i) {
  return getRuleContext<ASNParser::SymbolContext>(i);
}

std::vector<tree::TerminalNode *> ASNParser::SymbolListContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::SymbolListContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}


size_t ASNParser::SymbolListContext::getRuleIndex() const {
  return ASNParser::RuleSymbolList;
}

antlrcpp::Any ASNParser::SymbolListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSymbolList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SymbolListContext* ASNParser::symbolList() {
  SymbolListContext *_localctx = _tracker.createInstance<SymbolListContext>(_ctx, getState());
  enterRule(_localctx, 24, ASNParser::RuleSymbolList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(369);
    symbol();
    setState(374);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::COMMA) {
      setState(370);
      match(ASNParser::COMMA);
      setState(371);
      symbol();
      setState(376);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SymbolContext ------------------------------------------------------------------

ASNParser::SymbolContext::SymbolContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::SymbolContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

tree::TerminalNode* ASNParser::SymbolContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

tree::TerminalNode* ASNParser::SymbolContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}


size_t ASNParser::SymbolContext::getRuleIndex() const {
  return ASNParser::RuleSymbol;
}

antlrcpp::Any ASNParser::SymbolContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSymbol(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SymbolContext* ASNParser::symbol() {
  SymbolContext *_localctx = _tracker.createInstance<SymbolContext>(_ctx, getState());
  enterRule(_localctx, 26, ASNParser::RuleSymbol);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(377);
    match(ASNParser::IDENTIFIER);
    setState(380);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::L_BRACE) {
      setState(378);
      match(ASNParser::L_BRACE);
      setState(379);
      match(ASNParser::R_BRACE);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentListContext ------------------------------------------------------------------

ASNParser::AssignmentListContext::AssignmentListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::AssignmentContext *> ASNParser::AssignmentListContext::assignment() {
  return getRuleContexts<ASNParser::AssignmentContext>();
}

ASNParser::AssignmentContext* ASNParser::AssignmentListContext::assignment(size_t i) {
  return getRuleContext<ASNParser::AssignmentContext>(i);
}


size_t ASNParser::AssignmentListContext::getRuleIndex() const {
  return ASNParser::RuleAssignmentList;
}

antlrcpp::Any ASNParser::AssignmentListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitAssignmentList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::AssignmentListContext* ASNParser::assignmentList() {
  AssignmentListContext *_localctx = _tracker.createInstance<AssignmentListContext>(_ctx, getState());
  enterRule(_localctx, 28, ASNParser::RuleAssignmentList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(382);
    assignment();
    setState(386);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::IDENTIFIER) {
      setState(383);
      assignment();
      setState(388);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentContext ------------------------------------------------------------------

ASNParser::AssignmentContext::AssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::AssignmentContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

ASNParser::ValueAssignmentContext* ASNParser::AssignmentContext::valueAssignment() {
  return getRuleContext<ASNParser::ValueAssignmentContext>(0);
}

ASNParser::TypeAssignmentContext* ASNParser::AssignmentContext::typeAssignment() {
  return getRuleContext<ASNParser::TypeAssignmentContext>(0);
}

ASNParser::ParameterizedAssignmentContext* ASNParser::AssignmentContext::parameterizedAssignment() {
  return getRuleContext<ASNParser::ParameterizedAssignmentContext>(0);
}

ASNParser::ObjectClassAssignmentContext* ASNParser::AssignmentContext::objectClassAssignment() {
  return getRuleContext<ASNParser::ObjectClassAssignmentContext>(0);
}


size_t ASNParser::AssignmentContext::getRuleIndex() const {
  return ASNParser::RuleAssignment;
}

antlrcpp::Any ASNParser::AssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitAssignment(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::AssignmentContext* ASNParser::assignment() {
  AssignmentContext *_localctx = _tracker.createInstance<AssignmentContext>(_ctx, getState());
  enterRule(_localctx, 30, ASNParser::RuleAssignment);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(389);
    match(ASNParser::IDENTIFIER);
    setState(394);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx)) {
    case 1: {
      setState(390);
      valueAssignment();
      break;
    }

    case 2: {
      setState(391);
      typeAssignment();
      break;
    }

    case 3: {
      setState(392);
      parameterizedAssignment();
      break;
    }

    case 4: {
      setState(393);
      objectClassAssignment();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SequenceTypeContext ------------------------------------------------------------------

ASNParser::SequenceTypeContext::SequenceTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::SequenceTypeContext::SEQUENCE_LITERAL() {
  return getToken(ASNParser::SEQUENCE_LITERAL, 0);
}

tree::TerminalNode* ASNParser::SequenceTypeContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

tree::TerminalNode* ASNParser::SequenceTypeContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}

ASNParser::ExtensionAndExceptionContext* ASNParser::SequenceTypeContext::extensionAndException() {
  return getRuleContext<ASNParser::ExtensionAndExceptionContext>(0);
}

ASNParser::OptionalExtensionMarkerContext* ASNParser::SequenceTypeContext::optionalExtensionMarker() {
  return getRuleContext<ASNParser::OptionalExtensionMarkerContext>(0);
}

ASNParser::ComponentTypeListsContext* ASNParser::SequenceTypeContext::componentTypeLists() {
  return getRuleContext<ASNParser::ComponentTypeListsContext>(0);
}


size_t ASNParser::SequenceTypeContext::getRuleIndex() const {
  return ASNParser::RuleSequenceType;
}

antlrcpp::Any ASNParser::SequenceTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSequenceType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SequenceTypeContext* ASNParser::sequenceType() {
  SequenceTypeContext *_localctx = _tracker.createInstance<SequenceTypeContext>(_ctx, getState());
  enterRule(_localctx, 32, ASNParser::RuleSequenceType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(396);
    match(ASNParser::SEQUENCE_LITERAL);
    setState(397);
    match(ASNParser::L_BRACE);
    setState(402);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      setState(398);
      extensionAndException();
      setState(399);
      optionalExtensionMarker();
      break;
    }

    case 2: {
      setState(401);
      componentTypeLists();
      break;
    }

    }
    setState(404);
    match(ASNParser::R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExtensionAndExceptionContext ------------------------------------------------------------------

ASNParser::ExtensionAndExceptionContext::ExtensionAndExceptionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ExtensionAndExceptionContext::ELLIPSIS() {
  return getToken(ASNParser::ELLIPSIS, 0);
}

ASNParser::ExceptionSpecContext* ASNParser::ExtensionAndExceptionContext::exceptionSpec() {
  return getRuleContext<ASNParser::ExceptionSpecContext>(0);
}


size_t ASNParser::ExtensionAndExceptionContext::getRuleIndex() const {
  return ASNParser::RuleExtensionAndException;
}

antlrcpp::Any ASNParser::ExtensionAndExceptionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExtensionAndException(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExtensionAndExceptionContext* ASNParser::extensionAndException() {
  ExtensionAndExceptionContext *_localctx = _tracker.createInstance<ExtensionAndExceptionContext>(_ctx, getState());
  enterRule(_localctx, 34, ASNParser::RuleExtensionAndException);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(406);
    match(ASNParser::ELLIPSIS);
    setState(408);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::EXCLAM) {
      setState(407);
      exceptionSpec();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OptionalExtensionMarkerContext ------------------------------------------------------------------

ASNParser::OptionalExtensionMarkerContext::OptionalExtensionMarkerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::OptionalExtensionMarkerContext::COMMA() {
  return getToken(ASNParser::COMMA, 0);
}

tree::TerminalNode* ASNParser::OptionalExtensionMarkerContext::ELLIPSIS() {
  return getToken(ASNParser::ELLIPSIS, 0);
}


size_t ASNParser::OptionalExtensionMarkerContext::getRuleIndex() const {
  return ASNParser::RuleOptionalExtensionMarker;
}

antlrcpp::Any ASNParser::OptionalExtensionMarkerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitOptionalExtensionMarker(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::OptionalExtensionMarkerContext* ASNParser::optionalExtensionMarker() {
  OptionalExtensionMarkerContext *_localctx = _tracker.createInstance<OptionalExtensionMarkerContext>(_ctx, getState());
  enterRule(_localctx, 36, ASNParser::RuleOptionalExtensionMarker);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(412);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::COMMA) {
      setState(410);
      match(ASNParser::COMMA);
      setState(411);
      match(ASNParser::ELLIPSIS);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComponentTypeListsContext ------------------------------------------------------------------

ASNParser::ComponentTypeListsContext::ComponentTypeListsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::RootComponentTypeListContext *> ASNParser::ComponentTypeListsContext::rootComponentTypeList() {
  return getRuleContexts<ASNParser::RootComponentTypeListContext>();
}

ASNParser::RootComponentTypeListContext* ASNParser::ComponentTypeListsContext::rootComponentTypeList(size_t i) {
  return getRuleContext<ASNParser::RootComponentTypeListContext>(i);
}

std::vector<tree::TerminalNode *> ASNParser::ComponentTypeListsContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::ComponentTypeListsContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}

ASNParser::ExtensionAndExceptionContext* ASNParser::ComponentTypeListsContext::extensionAndException() {
  return getRuleContext<ASNParser::ExtensionAndExceptionContext>(0);
}

ASNParser::ExtensionAdditionsContext* ASNParser::ComponentTypeListsContext::extensionAdditions() {
  return getRuleContext<ASNParser::ExtensionAdditionsContext>(0);
}

ASNParser::OptionalExtensionMarkerContext* ASNParser::ComponentTypeListsContext::optionalExtensionMarker() {
  return getRuleContext<ASNParser::OptionalExtensionMarkerContext>(0);
}

tree::TerminalNode* ASNParser::ComponentTypeListsContext::EXTENSTIONENDMARKER() {
  return getToken(ASNParser::EXTENSTIONENDMARKER, 0);
}


size_t ASNParser::ComponentTypeListsContext::getRuleIndex() const {
  return ASNParser::RuleComponentTypeLists;
}

antlrcpp::Any ASNParser::ComponentTypeListsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitComponentTypeLists(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ComponentTypeListsContext* ASNParser::componentTypeLists() {
  ComponentTypeListsContext *_localctx = _tracker.createInstance<ComponentTypeListsContext>(_ctx, getState());
  enterRule(_localctx, 38, ASNParser::RuleComponentTypeLists);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(434);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::COMPONENTS_LITERAL:
      case ASNParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(414);
        rootComponentTypeList();
        setState(424);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ASNParser::COMMA) {
          setState(415);
          match(ASNParser::COMMA);
          setState(416);
          extensionAndException();
          setState(417);
          extensionAdditions();
          setState(422);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case ASNParser::R_BRACE:
            case ASNParser::COMMA: {
              setState(418);
              optionalExtensionMarker();
              break;
            }

            case ASNParser::EXTENSTIONENDMARKER: {
              setState(419);
              match(ASNParser::EXTENSTIONENDMARKER);
              setState(420);
              match(ASNParser::COMMA);
              setState(421);
              rootComponentTypeList();
              break;
            }

          default:
            throw NoViableAltException(this);
          }
        }
        break;
      }

      case ASNParser::ELLIPSIS: {
        enterOuterAlt(_localctx, 2);
        setState(426);
        extensionAndException();
        setState(427);
        extensionAdditions();
        setState(432);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case ASNParser::R_BRACE:
          case ASNParser::COMMA: {
            setState(428);
            optionalExtensionMarker();
            break;
          }

          case ASNParser::EXTENSTIONENDMARKER: {
            setState(429);
            match(ASNParser::EXTENSTIONENDMARKER);
            setState(430);
            match(ASNParser::COMMA);
            setState(431);
            rootComponentTypeList();
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RootComponentTypeListContext ------------------------------------------------------------------

ASNParser::RootComponentTypeListContext::RootComponentTypeListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ComponentTypeListContext* ASNParser::RootComponentTypeListContext::componentTypeList() {
  return getRuleContext<ASNParser::ComponentTypeListContext>(0);
}


size_t ASNParser::RootComponentTypeListContext::getRuleIndex() const {
  return ASNParser::RuleRootComponentTypeList;
}

antlrcpp::Any ASNParser::RootComponentTypeListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitRootComponentTypeList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::RootComponentTypeListContext* ASNParser::rootComponentTypeList() {
  RootComponentTypeListContext *_localctx = _tracker.createInstance<RootComponentTypeListContext>(_ctx, getState());
  enterRule(_localctx, 40, ASNParser::RuleRootComponentTypeList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(436);
    componentTypeList();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComponentTypeListContext ------------------------------------------------------------------

ASNParser::ComponentTypeListContext::ComponentTypeListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::ComponentTypeContext *> ASNParser::ComponentTypeListContext::componentType() {
  return getRuleContexts<ASNParser::ComponentTypeContext>();
}

ASNParser::ComponentTypeContext* ASNParser::ComponentTypeListContext::componentType(size_t i) {
  return getRuleContext<ASNParser::ComponentTypeContext>(i);
}

std::vector<tree::TerminalNode *> ASNParser::ComponentTypeListContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::ComponentTypeListContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}


size_t ASNParser::ComponentTypeListContext::getRuleIndex() const {
  return ASNParser::RuleComponentTypeList;
}

antlrcpp::Any ASNParser::ComponentTypeListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitComponentTypeList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ComponentTypeListContext* ASNParser::componentTypeList() {
  ComponentTypeListContext *_localctx = _tracker.createInstance<ComponentTypeListContext>(_ctx, getState());
  enterRule(_localctx, 42, ASNParser::RuleComponentTypeList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(438);
    componentType();
    setState(443);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(439);
        match(ASNParser::COMMA);
        setState(440);
        componentType(); 
      }
      setState(445);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComponentTypeContext ------------------------------------------------------------------

ASNParser::ComponentTypeContext::ComponentTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::NamedTypeContext* ASNParser::ComponentTypeContext::namedType() {
  return getRuleContext<ASNParser::NamedTypeContext>(0);
}

tree::TerminalNode* ASNParser::ComponentTypeContext::OPTIONAL_LITERAL() {
  return getToken(ASNParser::OPTIONAL_LITERAL, 0);
}

tree::TerminalNode* ASNParser::ComponentTypeContext::DEFAULT_LITERAL() {
  return getToken(ASNParser::DEFAULT_LITERAL, 0);
}

ASNParser::ValueContext* ASNParser::ComponentTypeContext::value() {
  return getRuleContext<ASNParser::ValueContext>(0);
}

tree::TerminalNode* ASNParser::ComponentTypeContext::COMPONENTS_LITERAL() {
  return getToken(ASNParser::COMPONENTS_LITERAL, 0);
}

tree::TerminalNode* ASNParser::ComponentTypeContext::OF_LITERAL() {
  return getToken(ASNParser::OF_LITERAL, 0);
}

ASNParser::AsnTypeContext* ASNParser::ComponentTypeContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}


size_t ASNParser::ComponentTypeContext::getRuleIndex() const {
  return ASNParser::RuleComponentType;
}

antlrcpp::Any ASNParser::ComponentTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitComponentType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ComponentTypeContext* ASNParser::componentType() {
  ComponentTypeContext *_localctx = _tracker.createInstance<ComponentTypeContext>(_ctx, getState());
  enterRule(_localctx, 44, ASNParser::RuleComponentType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(455);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(446);
        namedType();
        setState(450);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case ASNParser::OPTIONAL_LITERAL: {
            setState(447);
            match(ASNParser::OPTIONAL_LITERAL);
            break;
          }

          case ASNParser::DEFAULT_LITERAL: {
            setState(448);
            match(ASNParser::DEFAULT_LITERAL);
            setState(449);
            value();
            break;
          }

          case ASNParser::R_BRACE:
          case ASNParser::COMMA:
          case ASNParser::DOUBLE_R_BRACKET:
          case ASNParser::EXTENSTIONENDMARKER: {
            break;
          }

        default:
          break;
        }
        break;
      }

      case ASNParser::COMPONENTS_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(452);
        match(ASNParser::COMPONENTS_LITERAL);
        setState(453);
        match(ASNParser::OF_LITERAL);
        setState(454);
        asnType();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExtensionAdditionsContext ------------------------------------------------------------------

ASNParser::ExtensionAdditionsContext::ExtensionAdditionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ExtensionAdditionsContext::COMMA() {
  return getToken(ASNParser::COMMA, 0);
}

ASNParser::ExtensionAdditionListContext* ASNParser::ExtensionAdditionsContext::extensionAdditionList() {
  return getRuleContext<ASNParser::ExtensionAdditionListContext>(0);
}


size_t ASNParser::ExtensionAdditionsContext::getRuleIndex() const {
  return ASNParser::RuleExtensionAdditions;
}

antlrcpp::Any ASNParser::ExtensionAdditionsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExtensionAdditions(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExtensionAdditionsContext* ASNParser::extensionAdditions() {
  ExtensionAdditionsContext *_localctx = _tracker.createInstance<ExtensionAdditionsContext>(_ctx, getState());
  enterRule(_localctx, 46, ASNParser::RuleExtensionAdditions);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(459);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
    case 1: {
      setState(457);
      match(ASNParser::COMMA);
      setState(458);
      extensionAdditionList();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExtensionAdditionListContext ------------------------------------------------------------------

ASNParser::ExtensionAdditionListContext::ExtensionAdditionListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::ExtensionAdditionContext *> ASNParser::ExtensionAdditionListContext::extensionAddition() {
  return getRuleContexts<ASNParser::ExtensionAdditionContext>();
}

ASNParser::ExtensionAdditionContext* ASNParser::ExtensionAdditionListContext::extensionAddition(size_t i) {
  return getRuleContext<ASNParser::ExtensionAdditionContext>(i);
}

std::vector<tree::TerminalNode *> ASNParser::ExtensionAdditionListContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::ExtensionAdditionListContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}


size_t ASNParser::ExtensionAdditionListContext::getRuleIndex() const {
  return ASNParser::RuleExtensionAdditionList;
}

antlrcpp::Any ASNParser::ExtensionAdditionListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExtensionAdditionList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExtensionAdditionListContext* ASNParser::extensionAdditionList() {
  ExtensionAdditionListContext *_localctx = _tracker.createInstance<ExtensionAdditionListContext>(_ctx, getState());
  enterRule(_localctx, 48, ASNParser::RuleExtensionAdditionList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(461);
    extensionAddition();
    setState(466);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(462);
        match(ASNParser::COMMA);
        setState(463);
        extensionAddition(); 
      }
      setState(468);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 25, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExtensionAdditionContext ------------------------------------------------------------------

ASNParser::ExtensionAdditionContext::ExtensionAdditionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ComponentTypeContext* ASNParser::ExtensionAdditionContext::componentType() {
  return getRuleContext<ASNParser::ComponentTypeContext>(0);
}

ASNParser::ExtensionAdditionGroupContext* ASNParser::ExtensionAdditionContext::extensionAdditionGroup() {
  return getRuleContext<ASNParser::ExtensionAdditionGroupContext>(0);
}


size_t ASNParser::ExtensionAdditionContext::getRuleIndex() const {
  return ASNParser::RuleExtensionAddition;
}

antlrcpp::Any ASNParser::ExtensionAdditionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExtensionAddition(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExtensionAdditionContext* ASNParser::extensionAddition() {
  ExtensionAdditionContext *_localctx = _tracker.createInstance<ExtensionAdditionContext>(_ctx, getState());
  enterRule(_localctx, 50, ASNParser::RuleExtensionAddition);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(471);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::COMPONENTS_LITERAL:
      case ASNParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(469);
        componentType();
        break;
      }

      case ASNParser::DOUBLE_L_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(470);
        extensionAdditionGroup();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExtensionAdditionGroupContext ------------------------------------------------------------------

ASNParser::ExtensionAdditionGroupContext::ExtensionAdditionGroupContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ExtensionAdditionGroupContext::DOUBLE_L_BRACKET() {
  return getToken(ASNParser::DOUBLE_L_BRACKET, 0);
}

ASNParser::VersionNumberContext* ASNParser::ExtensionAdditionGroupContext::versionNumber() {
  return getRuleContext<ASNParser::VersionNumberContext>(0);
}

ASNParser::ComponentTypeListContext* ASNParser::ExtensionAdditionGroupContext::componentTypeList() {
  return getRuleContext<ASNParser::ComponentTypeListContext>(0);
}

tree::TerminalNode* ASNParser::ExtensionAdditionGroupContext::DOUBLE_R_BRACKET() {
  return getToken(ASNParser::DOUBLE_R_BRACKET, 0);
}


size_t ASNParser::ExtensionAdditionGroupContext::getRuleIndex() const {
  return ASNParser::RuleExtensionAdditionGroup;
}

antlrcpp::Any ASNParser::ExtensionAdditionGroupContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExtensionAdditionGroup(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExtensionAdditionGroupContext* ASNParser::extensionAdditionGroup() {
  ExtensionAdditionGroupContext *_localctx = _tracker.createInstance<ExtensionAdditionGroupContext>(_ctx, getState());
  enterRule(_localctx, 52, ASNParser::RuleExtensionAdditionGroup);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(473);
    match(ASNParser::DOUBLE_L_BRACKET);
    setState(474);
    versionNumber();
    setState(475);
    componentTypeList();
    setState(476);
    match(ASNParser::DOUBLE_R_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VersionNumberContext ------------------------------------------------------------------

ASNParser::VersionNumberContext::VersionNumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::VersionNumberContext::NUMBER() {
  return getToken(ASNParser::NUMBER, 0);
}

tree::TerminalNode* ASNParser::VersionNumberContext::COLON() {
  return getToken(ASNParser::COLON, 0);
}


size_t ASNParser::VersionNumberContext::getRuleIndex() const {
  return ASNParser::RuleVersionNumber;
}

antlrcpp::Any ASNParser::VersionNumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitVersionNumber(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::VersionNumberContext* ASNParser::versionNumber() {
  VersionNumberContext *_localctx = _tracker.createInstance<VersionNumberContext>(_ctx, getState());
  enterRule(_localctx, 54, ASNParser::RuleVersionNumber);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(480);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::NUMBER) {
      setState(478);
      match(ASNParser::NUMBER);
      setState(479);
      match(ASNParser::COLON);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SequenceOfTypeContext ------------------------------------------------------------------

ASNParser::SequenceOfTypeContext::SequenceOfTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::SequenceOfTypeContext::SEQUENCE_LITERAL() {
  return getToken(ASNParser::SEQUENCE_LITERAL, 0);
}

tree::TerminalNode* ASNParser::SequenceOfTypeContext::OF_LITERAL() {
  return getToken(ASNParser::OF_LITERAL, 0);
}

ASNParser::AsnTypeContext* ASNParser::SequenceOfTypeContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}

ASNParser::NamedTypeContext* ASNParser::SequenceOfTypeContext::namedType() {
  return getRuleContext<ASNParser::NamedTypeContext>(0);
}

tree::TerminalNode* ASNParser::SequenceOfTypeContext::L_PARAN() {
  return getToken(ASNParser::L_PARAN, 0);
}

tree::TerminalNode* ASNParser::SequenceOfTypeContext::R_PARAN() {
  return getToken(ASNParser::R_PARAN, 0);
}

ASNParser::ConstraintContext* ASNParser::SequenceOfTypeContext::constraint() {
  return getRuleContext<ASNParser::ConstraintContext>(0);
}

ASNParser::SizeConstraintContext* ASNParser::SequenceOfTypeContext::sizeConstraint() {
  return getRuleContext<ASNParser::SizeConstraintContext>(0);
}


size_t ASNParser::SequenceOfTypeContext::getRuleIndex() const {
  return ASNParser::RuleSequenceOfType;
}

antlrcpp::Any ASNParser::SequenceOfTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSequenceOfType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SequenceOfTypeContext* ASNParser::sequenceOfType() {
  SequenceOfTypeContext *_localctx = _tracker.createInstance<SequenceOfTypeContext>(_ctx, getState());
  enterRule(_localctx, 56, ASNParser::RuleSequenceOfType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(482);
    match(ASNParser::SEQUENCE_LITERAL);
    setState(490);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::L_PARAN) {
      setState(483);
      match(ASNParser::L_PARAN);
      setState(486);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case ASNParser::L_PARAN: {
          setState(484);
          constraint();
          break;
        }

        case ASNParser::SIZE_LITERAL: {
          setState(485);
          sizeConstraint();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(488);
      match(ASNParser::R_PARAN);
    }
    setState(492);
    match(ASNParser::OF_LITERAL);
    setState(495);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
    case 1: {
      setState(493);
      asnType();
      break;
    }

    case 2: {
      setState(494);
      namedType();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SizeConstraintContext ------------------------------------------------------------------

ASNParser::SizeConstraintContext::SizeConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::SizeConstraintContext::SIZE_LITERAL() {
  return getToken(ASNParser::SIZE_LITERAL, 0);
}

ASNParser::ConstraintContext* ASNParser::SizeConstraintContext::constraint() {
  return getRuleContext<ASNParser::ConstraintContext>(0);
}


size_t ASNParser::SizeConstraintContext::getRuleIndex() const {
  return ASNParser::RuleSizeConstraint;
}

antlrcpp::Any ASNParser::SizeConstraintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSizeConstraint(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SizeConstraintContext* ASNParser::sizeConstraint() {
  SizeConstraintContext *_localctx = _tracker.createInstance<SizeConstraintContext>(_ctx, getState());
  enterRule(_localctx, 58, ASNParser::RuleSizeConstraint);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(497);
    match(ASNParser::SIZE_LITERAL);
    setState(498);
    constraint();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterizedAssignmentContext ------------------------------------------------------------------

ASNParser::ParameterizedAssignmentContext::ParameterizedAssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ParameterListContext* ASNParser::ParameterizedAssignmentContext::parameterList() {
  return getRuleContext<ASNParser::ParameterListContext>(0);
}

tree::TerminalNode* ASNParser::ParameterizedAssignmentContext::ASSIGN_OP() {
  return getToken(ASNParser::ASSIGN_OP, 0);
}

ASNParser::AsnTypeContext* ASNParser::ParameterizedAssignmentContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}

ASNParser::ValueContext* ASNParser::ParameterizedAssignmentContext::value() {
  return getRuleContext<ASNParser::ValueContext>(0);
}

ASNParser::ValueSetContext* ASNParser::ParameterizedAssignmentContext::valueSet() {
  return getRuleContext<ASNParser::ValueSetContext>(0);
}

ASNParser::DefinedObjectClassContext* ASNParser::ParameterizedAssignmentContext::definedObjectClass() {
  return getRuleContext<ASNParser::DefinedObjectClassContext>(0);
}

ASNParser::ObjectContext* ASNParser::ParameterizedAssignmentContext::object() {
  return getRuleContext<ASNParser::ObjectContext>(0);
}

ASNParser::ObjectClassContext* ASNParser::ParameterizedAssignmentContext::objectClass() {
  return getRuleContext<ASNParser::ObjectClassContext>(0);
}

ASNParser::ObjectSetContext* ASNParser::ParameterizedAssignmentContext::objectSet() {
  return getRuleContext<ASNParser::ObjectSetContext>(0);
}


size_t ASNParser::ParameterizedAssignmentContext::getRuleIndex() const {
  return ASNParser::RuleParameterizedAssignment;
}

antlrcpp::Any ASNParser::ParameterizedAssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitParameterizedAssignment(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ParameterizedAssignmentContext* ASNParser::parameterizedAssignment() {
  ParameterizedAssignmentContext *_localctx = _tracker.createInstance<ParameterizedAssignmentContext>(_ctx, getState());
  enterRule(_localctx, 60, ASNParser::RuleParameterizedAssignment);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(514);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::L_BRACE: {
        enterOuterAlt(_localctx, 1);
        setState(500);
        parameterList();

        setState(501);
        match(ASNParser::ASSIGN_OP);
        setState(505);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
        case 1: {
          setState(502);
          asnType();
          break;
        }

        case 2: {
          setState(503);
          value();
          break;
        }

        case 3: {
          setState(504);
          valueSet();
          break;
        }

        }
        break;
      }

      case ASNParser::TYPE_IDENTIFIER_LITERAL:
      case ASNParser::ABSTRACT_SYNTAX_LITERAL:
      case ASNParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(507);
        definedObjectClass();
        setState(508);
        match(ASNParser::ASSIGN_OP);
        setState(512);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx)) {
        case 1: {
          setState(509);
          object();
          break;
        }

        case 2: {
          setState(510);
          objectClass();
          break;
        }

        case 3: {
          setState(511);
          objectSet();
          break;
        }

        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterListContext ------------------------------------------------------------------

ASNParser::ParameterListContext::ParameterListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ParameterListContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

std::vector<ASNParser::ParameterContext *> ASNParser::ParameterListContext::parameter() {
  return getRuleContexts<ASNParser::ParameterContext>();
}

ASNParser::ParameterContext* ASNParser::ParameterListContext::parameter(size_t i) {
  return getRuleContext<ASNParser::ParameterContext>(i);
}

tree::TerminalNode* ASNParser::ParameterListContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}

std::vector<tree::TerminalNode *> ASNParser::ParameterListContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::ParameterListContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}


size_t ASNParser::ParameterListContext::getRuleIndex() const {
  return ASNParser::RuleParameterList;
}

antlrcpp::Any ASNParser::ParameterListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitParameterList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ParameterListContext* ASNParser::parameterList() {
  ParameterListContext *_localctx = _tracker.createInstance<ParameterListContext>(_ctx, getState());
  enterRule(_localctx, 62, ASNParser::RuleParameterList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(516);
    match(ASNParser::L_BRACE);
    setState(517);
    parameter();
    setState(522);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::COMMA) {
      setState(518);
      match(ASNParser::COMMA);
      setState(519);
      parameter();
      setState(524);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(525);
    match(ASNParser::R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterContext ------------------------------------------------------------------

ASNParser::ParameterContext::ParameterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ParameterContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

ASNParser::ParamGovernorContext* ASNParser::ParameterContext::paramGovernor() {
  return getRuleContext<ASNParser::ParamGovernorContext>(0);
}

tree::TerminalNode* ASNParser::ParameterContext::COLON() {
  return getToken(ASNParser::COLON, 0);
}


size_t ASNParser::ParameterContext::getRuleIndex() const {
  return ASNParser::RuleParameter;
}

antlrcpp::Any ASNParser::ParameterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitParameter(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ParameterContext* ASNParser::parameter() {
  ParameterContext *_localctx = _tracker.createInstance<ParameterContext>(_ctx, getState());
  enterRule(_localctx, 64, ASNParser::RuleParameter);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(530);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx)) {
    case 1: {
      setState(527);
      paramGovernor();
      setState(528);
      match(ASNParser::COLON);
      break;
    }

    }
    setState(532);
    match(ASNParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamGovernorContext ------------------------------------------------------------------

ASNParser::ParamGovernorContext::ParamGovernorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::GovernorContext* ASNParser::ParamGovernorContext::governor() {
  return getRuleContext<ASNParser::GovernorContext>(0);
}

tree::TerminalNode* ASNParser::ParamGovernorContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}


size_t ASNParser::ParamGovernorContext::getRuleIndex() const {
  return ASNParser::RuleParamGovernor;
}

antlrcpp::Any ASNParser::ParamGovernorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitParamGovernor(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ParamGovernorContext* ASNParser::paramGovernor() {
  ParamGovernorContext *_localctx = _tracker.createInstance<ParamGovernorContext>(_ctx, getState());
  enterRule(_localctx, 66, ASNParser::RuleParamGovernor);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(536);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(534);
      governor();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(535);
      match(ASNParser::IDENTIFIER);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GovernorContext ------------------------------------------------------------------

ASNParser::GovernorContext::GovernorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::AsnTypeContext* ASNParser::GovernorContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}

ASNParser::DefinedObjectClassContext* ASNParser::GovernorContext::definedObjectClass() {
  return getRuleContext<ASNParser::DefinedObjectClassContext>(0);
}


size_t ASNParser::GovernorContext::getRuleIndex() const {
  return ASNParser::RuleGovernor;
}

antlrcpp::Any ASNParser::GovernorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitGovernor(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::GovernorContext* ASNParser::governor() {
  GovernorContext *_localctx = _tracker.createInstance<GovernorContext>(_ctx, getState());
  enterRule(_localctx, 68, ASNParser::RuleGovernor);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(540);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(538);
      asnType();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(539);
      definedObjectClass();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectClassAssignmentContext ------------------------------------------------------------------

ASNParser::ObjectClassAssignmentContext::ObjectClassAssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ObjectClassAssignmentContext::ASSIGN_OP() {
  return getToken(ASNParser::ASSIGN_OP, 0);
}

ASNParser::ObjectClassContext* ASNParser::ObjectClassAssignmentContext::objectClass() {
  return getRuleContext<ASNParser::ObjectClassContext>(0);
}


size_t ASNParser::ObjectClassAssignmentContext::getRuleIndex() const {
  return ASNParser::RuleObjectClassAssignment;
}

antlrcpp::Any ASNParser::ObjectClassAssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjectClassAssignment(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectClassAssignmentContext* ASNParser::objectClassAssignment() {
  ObjectClassAssignmentContext *_localctx = _tracker.createInstance<ObjectClassAssignmentContext>(_ctx, getState());
  enterRule(_localctx, 70, ASNParser::RuleObjectClassAssignment);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(542);
    match(ASNParser::ASSIGN_OP);
    setState(543);
    objectClass();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectClassContext ------------------------------------------------------------------

ASNParser::ObjectClassContext::ObjectClassContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::DefinedObjectClassContext* ASNParser::ObjectClassContext::definedObjectClass() {
  return getRuleContext<ASNParser::DefinedObjectClassContext>(0);
}

ASNParser::ObjectClassDefnContext* ASNParser::ObjectClassContext::objectClassDefn() {
  return getRuleContext<ASNParser::ObjectClassDefnContext>(0);
}


size_t ASNParser::ObjectClassContext::getRuleIndex() const {
  return ASNParser::RuleObjectClass;
}

antlrcpp::Any ASNParser::ObjectClassContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjectClass(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectClassContext* ASNParser::objectClass() {
  ObjectClassContext *_localctx = _tracker.createInstance<ObjectClassContext>(_ctx, getState());
  enterRule(_localctx, 72, ASNParser::RuleObjectClass);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(547);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::TYPE_IDENTIFIER_LITERAL:
      case ASNParser::ABSTRACT_SYNTAX_LITERAL:
      case ASNParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(545);
        definedObjectClass();
        break;
      }

      case ASNParser::CLASS_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(546);
        objectClassDefn();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DefinedObjectClassContext ------------------------------------------------------------------

ASNParser::DefinedObjectClassContext::DefinedObjectClassContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ASNParser::DefinedObjectClassContext::IDENTIFIER() {
  return getTokens(ASNParser::IDENTIFIER);
}

tree::TerminalNode* ASNParser::DefinedObjectClassContext::IDENTIFIER(size_t i) {
  return getToken(ASNParser::IDENTIFIER, i);
}

tree::TerminalNode* ASNParser::DefinedObjectClassContext::DOT() {
  return getToken(ASNParser::DOT, 0);
}

tree::TerminalNode* ASNParser::DefinedObjectClassContext::TYPE_IDENTIFIER_LITERAL() {
  return getToken(ASNParser::TYPE_IDENTIFIER_LITERAL, 0);
}

tree::TerminalNode* ASNParser::DefinedObjectClassContext::ABSTRACT_SYNTAX_LITERAL() {
  return getToken(ASNParser::ABSTRACT_SYNTAX_LITERAL, 0);
}


size_t ASNParser::DefinedObjectClassContext::getRuleIndex() const {
  return ASNParser::RuleDefinedObjectClass;
}

antlrcpp::Any ASNParser::DefinedObjectClassContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitDefinedObjectClass(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::DefinedObjectClassContext* ASNParser::definedObjectClass() {
  DefinedObjectClassContext *_localctx = _tracker.createInstance<DefinedObjectClassContext>(_ctx, getState());
  enterRule(_localctx, 74, ASNParser::RuleDefinedObjectClass);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(556);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(551);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
        case 1: {
          setState(549);
          match(ASNParser::IDENTIFIER);
          setState(550);
          match(ASNParser::DOT);
          break;
        }

        }
        setState(553);
        match(ASNParser::IDENTIFIER);
        break;
      }

      case ASNParser::TYPE_IDENTIFIER_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(554);
        match(ASNParser::TYPE_IDENTIFIER_LITERAL);
        break;
      }

      case ASNParser::ABSTRACT_SYNTAX_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(555);
        match(ASNParser::ABSTRACT_SYNTAX_LITERAL);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UsefulObjectClassReferenceContext ------------------------------------------------------------------

ASNParser::UsefulObjectClassReferenceContext::UsefulObjectClassReferenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::UsefulObjectClassReferenceContext::TYPE_IDENTIFIER_LITERAL() {
  return getToken(ASNParser::TYPE_IDENTIFIER_LITERAL, 0);
}

tree::TerminalNode* ASNParser::UsefulObjectClassReferenceContext::ABSTRACT_SYNTAX_LITERAL() {
  return getToken(ASNParser::ABSTRACT_SYNTAX_LITERAL, 0);
}


size_t ASNParser::UsefulObjectClassReferenceContext::getRuleIndex() const {
  return ASNParser::RuleUsefulObjectClassReference;
}

antlrcpp::Any ASNParser::UsefulObjectClassReferenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitUsefulObjectClassReference(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::UsefulObjectClassReferenceContext* ASNParser::usefulObjectClassReference() {
  UsefulObjectClassReferenceContext *_localctx = _tracker.createInstance<UsefulObjectClassReferenceContext>(_ctx, getState());
  enterRule(_localctx, 76, ASNParser::RuleUsefulObjectClassReference);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(558);
    _la = _input->LA(1);
    if (!(_la == ASNParser::TYPE_IDENTIFIER_LITERAL

    || _la == ASNParser::ABSTRACT_SYNTAX_LITERAL)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExternalObjectClassReferenceContext ------------------------------------------------------------------

ASNParser::ExternalObjectClassReferenceContext::ExternalObjectClassReferenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ASNParser::ExternalObjectClassReferenceContext::IDENTIFIER() {
  return getTokens(ASNParser::IDENTIFIER);
}

tree::TerminalNode* ASNParser::ExternalObjectClassReferenceContext::IDENTIFIER(size_t i) {
  return getToken(ASNParser::IDENTIFIER, i);
}

tree::TerminalNode* ASNParser::ExternalObjectClassReferenceContext::DOT() {
  return getToken(ASNParser::DOT, 0);
}


size_t ASNParser::ExternalObjectClassReferenceContext::getRuleIndex() const {
  return ASNParser::RuleExternalObjectClassReference;
}

antlrcpp::Any ASNParser::ExternalObjectClassReferenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExternalObjectClassReference(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExternalObjectClassReferenceContext* ASNParser::externalObjectClassReference() {
  ExternalObjectClassReferenceContext *_localctx = _tracker.createInstance<ExternalObjectClassReferenceContext>(_ctx, getState());
  enterRule(_localctx, 78, ASNParser::RuleExternalObjectClassReference);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(560);
    match(ASNParser::IDENTIFIER);
    setState(561);
    match(ASNParser::DOT);
    setState(562);
    match(ASNParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectClassDefnContext ------------------------------------------------------------------

ASNParser::ObjectClassDefnContext::ObjectClassDefnContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ObjectClassDefnContext::CLASS_LITERAL() {
  return getToken(ASNParser::CLASS_LITERAL, 0);
}

tree::TerminalNode* ASNParser::ObjectClassDefnContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

std::vector<ASNParser::FieldSpecContext *> ASNParser::ObjectClassDefnContext::fieldSpec() {
  return getRuleContexts<ASNParser::FieldSpecContext>();
}

ASNParser::FieldSpecContext* ASNParser::ObjectClassDefnContext::fieldSpec(size_t i) {
  return getRuleContext<ASNParser::FieldSpecContext>(i);
}

tree::TerminalNode* ASNParser::ObjectClassDefnContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}

std::vector<tree::TerminalNode *> ASNParser::ObjectClassDefnContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::ObjectClassDefnContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}

ASNParser::WithSyntaxSpecContext* ASNParser::ObjectClassDefnContext::withSyntaxSpec() {
  return getRuleContext<ASNParser::WithSyntaxSpecContext>(0);
}


size_t ASNParser::ObjectClassDefnContext::getRuleIndex() const {
  return ASNParser::RuleObjectClassDefn;
}

antlrcpp::Any ASNParser::ObjectClassDefnContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjectClassDefn(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectClassDefnContext* ASNParser::objectClassDefn() {
  ObjectClassDefnContext *_localctx = _tracker.createInstance<ObjectClassDefnContext>(_ctx, getState());
  enterRule(_localctx, 80, ASNParser::RuleObjectClassDefn);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(564);
    match(ASNParser::CLASS_LITERAL);
    setState(565);
    match(ASNParser::L_BRACE);
    setState(566);
    fieldSpec();
    setState(571);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::COMMA) {
      setState(567);
      match(ASNParser::COMMA);
      setState(568);
      fieldSpec();
      setState(573);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(574);
    match(ASNParser::R_BRACE);
    setState(576);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::WITH_LITERAL) {
      setState(575);
      withSyntaxSpec();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WithSyntaxSpecContext ------------------------------------------------------------------

ASNParser::WithSyntaxSpecContext::WithSyntaxSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::WithSyntaxSpecContext::WITH_LITERAL() {
  return getToken(ASNParser::WITH_LITERAL, 0);
}

tree::TerminalNode* ASNParser::WithSyntaxSpecContext::SYNTAX_LITERAL() {
  return getToken(ASNParser::SYNTAX_LITERAL, 0);
}

ASNParser::SyntaxListContext* ASNParser::WithSyntaxSpecContext::syntaxList() {
  return getRuleContext<ASNParser::SyntaxListContext>(0);
}


size_t ASNParser::WithSyntaxSpecContext::getRuleIndex() const {
  return ASNParser::RuleWithSyntaxSpec;
}

antlrcpp::Any ASNParser::WithSyntaxSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitWithSyntaxSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::WithSyntaxSpecContext* ASNParser::withSyntaxSpec() {
  WithSyntaxSpecContext *_localctx = _tracker.createInstance<WithSyntaxSpecContext>(_ctx, getState());
  enterRule(_localctx, 82, ASNParser::RuleWithSyntaxSpec);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(578);
    match(ASNParser::WITH_LITERAL);
    setState(579);
    match(ASNParser::SYNTAX_LITERAL);
    setState(580);
    syntaxList();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SyntaxListContext ------------------------------------------------------------------

ASNParser::SyntaxListContext::SyntaxListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::SyntaxListContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

tree::TerminalNode* ASNParser::SyntaxListContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}

std::vector<ASNParser::TokenOrGroupSpecContext *> ASNParser::SyntaxListContext::tokenOrGroupSpec() {
  return getRuleContexts<ASNParser::TokenOrGroupSpecContext>();
}

ASNParser::TokenOrGroupSpecContext* ASNParser::SyntaxListContext::tokenOrGroupSpec(size_t i) {
  return getRuleContext<ASNParser::TokenOrGroupSpecContext>(i);
}


size_t ASNParser::SyntaxListContext::getRuleIndex() const {
  return ASNParser::RuleSyntaxList;
}

antlrcpp::Any ASNParser::SyntaxListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSyntaxList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SyntaxListContext* ASNParser::syntaxList() {
  SyntaxListContext *_localctx = _tracker.createInstance<SyntaxListContext>(_ctx, getState());
  enterRule(_localctx, 84, ASNParser::RuleSyntaxList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(582);
    match(ASNParser::L_BRACE);
    setState(584); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(583);
      tokenOrGroupSpec();
      setState(586); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ASNParser::AMPERSAND)
      | (1ULL << ASNParser::COMMA)
      | (1ULL << ASNParser::L_BRACKET))) != 0) || _la == ASNParser::IDENTIFIER);
    setState(588);
    match(ASNParser::R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TokenOrGroupSpecContext ------------------------------------------------------------------

ASNParser::TokenOrGroupSpecContext::TokenOrGroupSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::RequiredTokenContext* ASNParser::TokenOrGroupSpecContext::requiredToken() {
  return getRuleContext<ASNParser::RequiredTokenContext>(0);
}

ASNParser::OptionalGroupContext* ASNParser::TokenOrGroupSpecContext::optionalGroup() {
  return getRuleContext<ASNParser::OptionalGroupContext>(0);
}


size_t ASNParser::TokenOrGroupSpecContext::getRuleIndex() const {
  return ASNParser::RuleTokenOrGroupSpec;
}

antlrcpp::Any ASNParser::TokenOrGroupSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitTokenOrGroupSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::TokenOrGroupSpecContext* ASNParser::tokenOrGroupSpec() {
  TokenOrGroupSpecContext *_localctx = _tracker.createInstance<TokenOrGroupSpecContext>(_ctx, getState());
  enterRule(_localctx, 86, ASNParser::RuleTokenOrGroupSpec);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(592);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::AMPERSAND:
      case ASNParser::COMMA:
      case ASNParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(590);
        requiredToken();
        break;
      }

      case ASNParser::L_BRACKET: {
        enterOuterAlt(_localctx, 2);
        setState(591);
        optionalGroup();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OptionalGroupContext ------------------------------------------------------------------

ASNParser::OptionalGroupContext::OptionalGroupContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::OptionalGroupContext::L_BRACKET() {
  return getToken(ASNParser::L_BRACKET, 0);
}

tree::TerminalNode* ASNParser::OptionalGroupContext::R_BRACKET() {
  return getToken(ASNParser::R_BRACKET, 0);
}

std::vector<ASNParser::TokenOrGroupSpecContext *> ASNParser::OptionalGroupContext::tokenOrGroupSpec() {
  return getRuleContexts<ASNParser::TokenOrGroupSpecContext>();
}

ASNParser::TokenOrGroupSpecContext* ASNParser::OptionalGroupContext::tokenOrGroupSpec(size_t i) {
  return getRuleContext<ASNParser::TokenOrGroupSpecContext>(i);
}


size_t ASNParser::OptionalGroupContext::getRuleIndex() const {
  return ASNParser::RuleOptionalGroup;
}

antlrcpp::Any ASNParser::OptionalGroupContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitOptionalGroup(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::OptionalGroupContext* ASNParser::optionalGroup() {
  OptionalGroupContext *_localctx = _tracker.createInstance<OptionalGroupContext>(_ctx, getState());
  enterRule(_localctx, 88, ASNParser::RuleOptionalGroup);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(594);
    match(ASNParser::L_BRACKET);
    setState(596); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(595);
      tokenOrGroupSpec();
      setState(598); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ASNParser::AMPERSAND)
      | (1ULL << ASNParser::COMMA)
      | (1ULL << ASNParser::L_BRACKET))) != 0) || _la == ASNParser::IDENTIFIER);
    setState(600);
    match(ASNParser::R_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RequiredTokenContext ------------------------------------------------------------------

ASNParser::RequiredTokenContext::RequiredTokenContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::LiteralContext* ASNParser::RequiredTokenContext::literal() {
  return getRuleContext<ASNParser::LiteralContext>(0);
}

ASNParser::PrimitiveFieldNameContext* ASNParser::RequiredTokenContext::primitiveFieldName() {
  return getRuleContext<ASNParser::PrimitiveFieldNameContext>(0);
}


size_t ASNParser::RequiredTokenContext::getRuleIndex() const {
  return ASNParser::RuleRequiredToken;
}

antlrcpp::Any ASNParser::RequiredTokenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitRequiredToken(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::RequiredTokenContext* ASNParser::requiredToken() {
  RequiredTokenContext *_localctx = _tracker.createInstance<RequiredTokenContext>(_ctx, getState());
  enterRule(_localctx, 90, ASNParser::RuleRequiredToken);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(604);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::COMMA:
      case ASNParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(602);
        literal();
        break;
      }

      case ASNParser::AMPERSAND: {
        enterOuterAlt(_localctx, 2);
        setState(603);
        primitiveFieldName();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LiteralContext ------------------------------------------------------------------

ASNParser::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::LiteralContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

tree::TerminalNode* ASNParser::LiteralContext::COMMA() {
  return getToken(ASNParser::COMMA, 0);
}


size_t ASNParser::LiteralContext::getRuleIndex() const {
  return ASNParser::RuleLiteral;
}

antlrcpp::Any ASNParser::LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitLiteral(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::LiteralContext* ASNParser::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 92, ASNParser::RuleLiteral);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(606);
    _la = _input->LA(1);
    if (!(_la == ASNParser::COMMA || _la == ASNParser::IDENTIFIER)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimitiveFieldNameContext ------------------------------------------------------------------

ASNParser::PrimitiveFieldNameContext::PrimitiveFieldNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::PrimitiveFieldNameContext::AMPERSAND() {
  return getToken(ASNParser::AMPERSAND, 0);
}

tree::TerminalNode* ASNParser::PrimitiveFieldNameContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}


size_t ASNParser::PrimitiveFieldNameContext::getRuleIndex() const {
  return ASNParser::RulePrimitiveFieldName;
}

antlrcpp::Any ASNParser::PrimitiveFieldNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitPrimitiveFieldName(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::PrimitiveFieldNameContext* ASNParser::primitiveFieldName() {
  PrimitiveFieldNameContext *_localctx = _tracker.createInstance<PrimitiveFieldNameContext>(_ctx, getState());
  enterRule(_localctx, 94, ASNParser::RulePrimitiveFieldName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(608);
    match(ASNParser::AMPERSAND);
    setState(609);
    match(ASNParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FieldSpecContext ------------------------------------------------------------------

ASNParser::FieldSpecContext::FieldSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::FieldSpecContext::AMPERSAND() {
  return getToken(ASNParser::AMPERSAND, 0);
}

tree::TerminalNode* ASNParser::FieldSpecContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

ASNParser::AsnTypeContext* ASNParser::FieldSpecContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}

ASNParser::FieldNameContext* ASNParser::FieldSpecContext::fieldName() {
  return getRuleContext<ASNParser::FieldNameContext>(0);
}

ASNParser::DefinedObjectClassContext* ASNParser::FieldSpecContext::definedObjectClass() {
  return getRuleContext<ASNParser::DefinedObjectClassContext>(0);
}

ASNParser::TypeOptionalitySpecContext* ASNParser::FieldSpecContext::typeOptionalitySpec() {
  return getRuleContext<ASNParser::TypeOptionalitySpecContext>(0);
}

tree::TerminalNode* ASNParser::FieldSpecContext::OPTIONAL_LITERAL() {
  return getToken(ASNParser::OPTIONAL_LITERAL, 0);
}

ASNParser::ValueSetOptionalitySpecContext* ASNParser::FieldSpecContext::valueSetOptionalitySpec() {
  return getRuleContext<ASNParser::ValueSetOptionalitySpecContext>(0);
}

tree::TerminalNode* ASNParser::FieldSpecContext::UNIQUE_LITERAL() {
  return getToken(ASNParser::UNIQUE_LITERAL, 0);
}

ASNParser::ValueOptionalitySpecContext* ASNParser::FieldSpecContext::valueOptionalitySpec() {
  return getRuleContext<ASNParser::ValueOptionalitySpecContext>(0);
}

tree::TerminalNode* ASNParser::FieldSpecContext::DEFAULT_LITERAL() {
  return getToken(ASNParser::DEFAULT_LITERAL, 0);
}

ASNParser::ValueSetContext* ASNParser::FieldSpecContext::valueSet() {
  return getRuleContext<ASNParser::ValueSetContext>(0);
}

ASNParser::ValueContext* ASNParser::FieldSpecContext::value() {
  return getRuleContext<ASNParser::ValueContext>(0);
}

ASNParser::ObjectSetContext* ASNParser::FieldSpecContext::objectSet() {
  return getRuleContext<ASNParser::ObjectSetContext>(0);
}

ASNParser::ObjectContext* ASNParser::FieldSpecContext::object() {
  return getRuleContext<ASNParser::ObjectContext>(0);
}


size_t ASNParser::FieldSpecContext::getRuleIndex() const {
  return ASNParser::RuleFieldSpec;
}

antlrcpp::Any ASNParser::FieldSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitFieldSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::FieldSpecContext* ASNParser::fieldSpec() {
  FieldSpecContext *_localctx = _tracker.createInstance<FieldSpecContext>(_ctx, getState());
  enterRule(_localctx, 96, ASNParser::RuleFieldSpec);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(611);
    match(ASNParser::AMPERSAND);
    setState(612);
    match(ASNParser::IDENTIFIER);
    setState(646);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 56, _ctx)) {
    case 1: {
      setState(614);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ASNParser::OPTIONAL_LITERAL

      || _la == ASNParser::DEFAULT_LITERAL) {
        setState(613);
        typeOptionalitySpec();
      }
      break;
    }

    case 2: {
      setState(616);
      asnType();
      setState(626);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 51, _ctx)) {
      case 1: {
        setState(618);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ASNParser::OPTIONAL_LITERAL

        || _la == ASNParser::DEFAULT_LITERAL) {
          setState(617);
          valueSetOptionalitySpec();
        }
        break;
      }

      case 2: {
        setState(621);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ASNParser::UNIQUE_LITERAL) {
          setState(620);
          match(ASNParser::UNIQUE_LITERAL);
        }
        setState(624);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ASNParser::OPTIONAL_LITERAL

        || _la == ASNParser::DEFAULT_LITERAL) {
          setState(623);
          valueOptionalitySpec();
        }
        break;
      }

      }
      break;
    }

    case 3: {
      setState(628);
      fieldName();
      setState(635);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case ASNParser::OPTIONAL_LITERAL: {
          setState(629);
          match(ASNParser::OPTIONAL_LITERAL);
          break;
        }

        case ASNParser::DEFAULT_LITERAL: {
          setState(630);
          match(ASNParser::DEFAULT_LITERAL);
          setState(633);
          _errHandler->sync(this);
          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 52, _ctx)) {
          case 1: {
            setState(631);
            valueSet();
            break;
          }

          case 2: {
            setState(632);
            value();
            break;
          }

          }
          break;
        }

        case ASNParser::R_BRACE:
        case ASNParser::COMMA: {
          break;
        }

      default:
        break;
      }
      break;
    }

    case 4: {
      setState(637);
      definedObjectClass();
      setState(644);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case ASNParser::OPTIONAL_LITERAL: {
          setState(638);
          match(ASNParser::OPTIONAL_LITERAL);
          break;
        }

        case ASNParser::DEFAULT_LITERAL: {
          setState(639);
          match(ASNParser::DEFAULT_LITERAL);
          setState(642);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case ASNParser::L_BRACE: {
              setState(640);
              objectSet();
              break;
            }

            case ASNParser::IDENTIFIER: {
              setState(641);
              object();
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          break;
        }

        case ASNParser::R_BRACE:
        case ASNParser::COMMA: {
          break;
        }

      default:
        break;
      }
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeFieldSpecContext ------------------------------------------------------------------

ASNParser::TypeFieldSpecContext::TypeFieldSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::TypeFieldSpecContext::AMPERSAND() {
  return getToken(ASNParser::AMPERSAND, 0);
}

tree::TerminalNode* ASNParser::TypeFieldSpecContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

ASNParser::TypeOptionalitySpecContext* ASNParser::TypeFieldSpecContext::typeOptionalitySpec() {
  return getRuleContext<ASNParser::TypeOptionalitySpecContext>(0);
}


size_t ASNParser::TypeFieldSpecContext::getRuleIndex() const {
  return ASNParser::RuleTypeFieldSpec;
}

antlrcpp::Any ASNParser::TypeFieldSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitTypeFieldSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::TypeFieldSpecContext* ASNParser::typeFieldSpec() {
  TypeFieldSpecContext *_localctx = _tracker.createInstance<TypeFieldSpecContext>(_ctx, getState());
  enterRule(_localctx, 98, ASNParser::RuleTypeFieldSpec);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(648);
    match(ASNParser::AMPERSAND);
    setState(649);
    match(ASNParser::IDENTIFIER);
    setState(651);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::OPTIONAL_LITERAL

    || _la == ASNParser::DEFAULT_LITERAL) {
      setState(650);
      typeOptionalitySpec();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeOptionalitySpecContext ------------------------------------------------------------------

ASNParser::TypeOptionalitySpecContext::TypeOptionalitySpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::TypeOptionalitySpecContext::OPTIONAL_LITERAL() {
  return getToken(ASNParser::OPTIONAL_LITERAL, 0);
}

tree::TerminalNode* ASNParser::TypeOptionalitySpecContext::DEFAULT_LITERAL() {
  return getToken(ASNParser::DEFAULT_LITERAL, 0);
}

ASNParser::AsnTypeContext* ASNParser::TypeOptionalitySpecContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}


size_t ASNParser::TypeOptionalitySpecContext::getRuleIndex() const {
  return ASNParser::RuleTypeOptionalitySpec;
}

antlrcpp::Any ASNParser::TypeOptionalitySpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitTypeOptionalitySpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::TypeOptionalitySpecContext* ASNParser::typeOptionalitySpec() {
  TypeOptionalitySpecContext *_localctx = _tracker.createInstance<TypeOptionalitySpecContext>(_ctx, getState());
  enterRule(_localctx, 100, ASNParser::RuleTypeOptionalitySpec);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(656);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::OPTIONAL_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(653);
        match(ASNParser::OPTIONAL_LITERAL);
        break;
      }

      case ASNParser::DEFAULT_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(654);
        match(ASNParser::DEFAULT_LITERAL);
        setState(655);
        asnType();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FixedTypeValueFieldSpecContext ------------------------------------------------------------------

ASNParser::FixedTypeValueFieldSpecContext::FixedTypeValueFieldSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::FixedTypeValueFieldSpecContext::AMPERSAND() {
  return getToken(ASNParser::AMPERSAND, 0);
}

tree::TerminalNode* ASNParser::FixedTypeValueFieldSpecContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

ASNParser::AsnTypeContext* ASNParser::FixedTypeValueFieldSpecContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}

tree::TerminalNode* ASNParser::FixedTypeValueFieldSpecContext::UNIQUE_LITERAL() {
  return getToken(ASNParser::UNIQUE_LITERAL, 0);
}

ASNParser::ValueOptionalitySpecContext* ASNParser::FixedTypeValueFieldSpecContext::valueOptionalitySpec() {
  return getRuleContext<ASNParser::ValueOptionalitySpecContext>(0);
}


size_t ASNParser::FixedTypeValueFieldSpecContext::getRuleIndex() const {
  return ASNParser::RuleFixedTypeValueFieldSpec;
}

antlrcpp::Any ASNParser::FixedTypeValueFieldSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitFixedTypeValueFieldSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::FixedTypeValueFieldSpecContext* ASNParser::fixedTypeValueFieldSpec() {
  FixedTypeValueFieldSpecContext *_localctx = _tracker.createInstance<FixedTypeValueFieldSpecContext>(_ctx, getState());
  enterRule(_localctx, 102, ASNParser::RuleFixedTypeValueFieldSpec);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(658);
    match(ASNParser::AMPERSAND);
    setState(659);
    match(ASNParser::IDENTIFIER);
    setState(660);
    asnType();
    setState(662);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::UNIQUE_LITERAL) {
      setState(661);
      match(ASNParser::UNIQUE_LITERAL);
    }
    setState(665);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::OPTIONAL_LITERAL

    || _la == ASNParser::DEFAULT_LITERAL) {
      setState(664);
      valueOptionalitySpec();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueOptionalitySpecContext ------------------------------------------------------------------

ASNParser::ValueOptionalitySpecContext::ValueOptionalitySpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ValueOptionalitySpecContext::OPTIONAL_LITERAL() {
  return getToken(ASNParser::OPTIONAL_LITERAL, 0);
}

tree::TerminalNode* ASNParser::ValueOptionalitySpecContext::DEFAULT_LITERAL() {
  return getToken(ASNParser::DEFAULT_LITERAL, 0);
}

ASNParser::ValueContext* ASNParser::ValueOptionalitySpecContext::value() {
  return getRuleContext<ASNParser::ValueContext>(0);
}


size_t ASNParser::ValueOptionalitySpecContext::getRuleIndex() const {
  return ASNParser::RuleValueOptionalitySpec;
}

antlrcpp::Any ASNParser::ValueOptionalitySpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitValueOptionalitySpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ValueOptionalitySpecContext* ASNParser::valueOptionalitySpec() {
  ValueOptionalitySpecContext *_localctx = _tracker.createInstance<ValueOptionalitySpecContext>(_ctx, getState());
  enterRule(_localctx, 104, ASNParser::RuleValueOptionalitySpec);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(670);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::OPTIONAL_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(667);
        match(ASNParser::OPTIONAL_LITERAL);
        break;
      }

      case ASNParser::DEFAULT_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(668);
        match(ASNParser::DEFAULT_LITERAL);
        setState(669);
        value();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariableTypeValueFieldSpecContext ------------------------------------------------------------------

ASNParser::VariableTypeValueFieldSpecContext::VariableTypeValueFieldSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::VariableTypeValueFieldSpecContext::AMPERSAND() {
  return getToken(ASNParser::AMPERSAND, 0);
}

tree::TerminalNode* ASNParser::VariableTypeValueFieldSpecContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

ASNParser::FieldNameContext* ASNParser::VariableTypeValueFieldSpecContext::fieldName() {
  return getRuleContext<ASNParser::FieldNameContext>(0);
}

ASNParser::ValueOptionalitySpecContext* ASNParser::VariableTypeValueFieldSpecContext::valueOptionalitySpec() {
  return getRuleContext<ASNParser::ValueOptionalitySpecContext>(0);
}


size_t ASNParser::VariableTypeValueFieldSpecContext::getRuleIndex() const {
  return ASNParser::RuleVariableTypeValueFieldSpec;
}

antlrcpp::Any ASNParser::VariableTypeValueFieldSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitVariableTypeValueFieldSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::VariableTypeValueFieldSpecContext* ASNParser::variableTypeValueFieldSpec() {
  VariableTypeValueFieldSpecContext *_localctx = _tracker.createInstance<VariableTypeValueFieldSpecContext>(_ctx, getState());
  enterRule(_localctx, 106, ASNParser::RuleVariableTypeValueFieldSpec);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(672);
    match(ASNParser::AMPERSAND);
    setState(673);
    match(ASNParser::IDENTIFIER);
    setState(674);
    fieldName();
    setState(676);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::OPTIONAL_LITERAL

    || _la == ASNParser::DEFAULT_LITERAL) {
      setState(675);
      valueOptionalitySpec();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FixedTypeValueSetFieldSpecContext ------------------------------------------------------------------

ASNParser::FixedTypeValueSetFieldSpecContext::FixedTypeValueSetFieldSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::FixedTypeValueSetFieldSpecContext::AMPERSAND() {
  return getToken(ASNParser::AMPERSAND, 0);
}

tree::TerminalNode* ASNParser::FixedTypeValueSetFieldSpecContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

ASNParser::AsnTypeContext* ASNParser::FixedTypeValueSetFieldSpecContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}

ASNParser::ValueSetOptionalitySpecContext* ASNParser::FixedTypeValueSetFieldSpecContext::valueSetOptionalitySpec() {
  return getRuleContext<ASNParser::ValueSetOptionalitySpecContext>(0);
}


size_t ASNParser::FixedTypeValueSetFieldSpecContext::getRuleIndex() const {
  return ASNParser::RuleFixedTypeValueSetFieldSpec;
}

antlrcpp::Any ASNParser::FixedTypeValueSetFieldSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitFixedTypeValueSetFieldSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::FixedTypeValueSetFieldSpecContext* ASNParser::fixedTypeValueSetFieldSpec() {
  FixedTypeValueSetFieldSpecContext *_localctx = _tracker.createInstance<FixedTypeValueSetFieldSpecContext>(_ctx, getState());
  enterRule(_localctx, 108, ASNParser::RuleFixedTypeValueSetFieldSpec);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(678);
    match(ASNParser::AMPERSAND);
    setState(679);
    match(ASNParser::IDENTIFIER);
    setState(680);
    asnType();
    setState(682);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::OPTIONAL_LITERAL

    || _la == ASNParser::DEFAULT_LITERAL) {
      setState(681);
      valueSetOptionalitySpec();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueSetOptionalitySpecContext ------------------------------------------------------------------

ASNParser::ValueSetOptionalitySpecContext::ValueSetOptionalitySpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ValueSetOptionalitySpecContext::OPTIONAL_LITERAL() {
  return getToken(ASNParser::OPTIONAL_LITERAL, 0);
}

tree::TerminalNode* ASNParser::ValueSetOptionalitySpecContext::DEFAULT_LITERAL() {
  return getToken(ASNParser::DEFAULT_LITERAL, 0);
}

ASNParser::ValueSetContext* ASNParser::ValueSetOptionalitySpecContext::valueSet() {
  return getRuleContext<ASNParser::ValueSetContext>(0);
}


size_t ASNParser::ValueSetOptionalitySpecContext::getRuleIndex() const {
  return ASNParser::RuleValueSetOptionalitySpec;
}

antlrcpp::Any ASNParser::ValueSetOptionalitySpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitValueSetOptionalitySpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ValueSetOptionalitySpecContext* ASNParser::valueSetOptionalitySpec() {
  ValueSetOptionalitySpecContext *_localctx = _tracker.createInstance<ValueSetOptionalitySpecContext>(_ctx, getState());
  enterRule(_localctx, 110, ASNParser::RuleValueSetOptionalitySpec);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(687);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::OPTIONAL_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(684);
        match(ASNParser::OPTIONAL_LITERAL);
        break;
      }

      case ASNParser::DEFAULT_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(685);
        match(ASNParser::DEFAULT_LITERAL);
        setState(686);
        valueSet();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectContext ------------------------------------------------------------------

ASNParser::ObjectContext::ObjectContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::DefinedObjectContext* ASNParser::ObjectContext::definedObject() {
  return getRuleContext<ASNParser::DefinedObjectContext>(0);
}

ASNParser::ParameterizedObjectContext* ASNParser::ObjectContext::parameterizedObject() {
  return getRuleContext<ASNParser::ParameterizedObjectContext>(0);
}


size_t ASNParser::ObjectContext::getRuleIndex() const {
  return ASNParser::RuleObject;
}

antlrcpp::Any ASNParser::ObjectContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObject(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectContext* ASNParser::object() {
  ObjectContext *_localctx = _tracker.createInstance<ObjectContext>(_ctx, getState());
  enterRule(_localctx, 112, ASNParser::RuleObject);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(691);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 65, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(689);
      definedObject();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(690);
      parameterizedObject();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterizedObjectContext ------------------------------------------------------------------

ASNParser::ParameterizedObjectContext::ParameterizedObjectContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::DefinedObjectContext* ASNParser::ParameterizedObjectContext::definedObject() {
  return getRuleContext<ASNParser::DefinedObjectContext>(0);
}

ASNParser::ActualParameterListContext* ASNParser::ParameterizedObjectContext::actualParameterList() {
  return getRuleContext<ASNParser::ActualParameterListContext>(0);
}


size_t ASNParser::ParameterizedObjectContext::getRuleIndex() const {
  return ASNParser::RuleParameterizedObject;
}

antlrcpp::Any ASNParser::ParameterizedObjectContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitParameterizedObject(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ParameterizedObjectContext* ASNParser::parameterizedObject() {
  ParameterizedObjectContext *_localctx = _tracker.createInstance<ParameterizedObjectContext>(_ctx, getState());
  enterRule(_localctx, 114, ASNParser::RuleParameterizedObject);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(693);
    definedObject();
    setState(694);
    actualParameterList();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DefinedObjectContext ------------------------------------------------------------------

ASNParser::DefinedObjectContext::DefinedObjectContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::DefinedObjectContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

tree::TerminalNode* ASNParser::DefinedObjectContext::DOT() {
  return getToken(ASNParser::DOT, 0);
}


size_t ASNParser::DefinedObjectContext::getRuleIndex() const {
  return ASNParser::RuleDefinedObject;
}

antlrcpp::Any ASNParser::DefinedObjectContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitDefinedObject(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::DefinedObjectContext* ASNParser::definedObject() {
  DefinedObjectContext *_localctx = _tracker.createInstance<DefinedObjectContext>(_ctx, getState());
  enterRule(_localctx, 116, ASNParser::RuleDefinedObject);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(696);
    match(ASNParser::IDENTIFIER);
    setState(698);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::DOT) {
      setState(697);
      match(ASNParser::DOT);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectSetContext ------------------------------------------------------------------

ASNParser::ObjectSetContext::ObjectSetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ObjectSetContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

ASNParser::ObjectSetSpecContext* ASNParser::ObjectSetContext::objectSetSpec() {
  return getRuleContext<ASNParser::ObjectSetSpecContext>(0);
}

tree::TerminalNode* ASNParser::ObjectSetContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}


size_t ASNParser::ObjectSetContext::getRuleIndex() const {
  return ASNParser::RuleObjectSet;
}

antlrcpp::Any ASNParser::ObjectSetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjectSet(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectSetContext* ASNParser::objectSet() {
  ObjectSetContext *_localctx = _tracker.createInstance<ObjectSetContext>(_ctx, getState());
  enterRule(_localctx, 118, ASNParser::RuleObjectSet);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(700);
    match(ASNParser::L_BRACE);
    setState(701);
    objectSetSpec();
    setState(702);
    match(ASNParser::R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectSetSpecContext ------------------------------------------------------------------

ASNParser::ObjectSetSpecContext::ObjectSetSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::RootElementSetSpecContext* ASNParser::ObjectSetSpecContext::rootElementSetSpec() {
  return getRuleContext<ASNParser::RootElementSetSpecContext>(0);
}

std::vector<tree::TerminalNode *> ASNParser::ObjectSetSpecContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::ObjectSetSpecContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}

tree::TerminalNode* ASNParser::ObjectSetSpecContext::ELLIPSIS() {
  return getToken(ASNParser::ELLIPSIS, 0);
}

ASNParser::AdditionalElementSetSpecContext* ASNParser::ObjectSetSpecContext::additionalElementSetSpec() {
  return getRuleContext<ASNParser::AdditionalElementSetSpecContext>(0);
}


size_t ASNParser::ObjectSetSpecContext::getRuleIndex() const {
  return ASNParser::RuleObjectSetSpec;
}

antlrcpp::Any ASNParser::ObjectSetSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjectSetSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectSetSpecContext* ASNParser::objectSetSpec() {
  ObjectSetSpecContext *_localctx = _tracker.createInstance<ObjectSetSpecContext>(_ctx, getState());
  enterRule(_localctx, 120, ASNParser::RuleObjectSetSpec);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(718);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::TRUE_LITERAL:
      case ASNParser::FALSE_LITERAL:
      case ASNParser::TRUE_SMALL_LITERAL:
      case ASNParser::FALSE_SMALL_LITERAL:
      case ASNParser::L_BRACE:
      case ASNParser::MINUS:
      case ASNParser::ALL_LITERAL:
      case ASNParser::MIN_LITERAL:
      case ASNParser::SIZE_LITERAL:
      case ASNParser::PATTERN_LITERAL:
      case ASNParser::NUMBER:
      case ASNParser::CSTRING:
      case ASNParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(704);
        rootElementSetSpec();
        setState(711);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ASNParser::COMMA) {
          setState(705);
          match(ASNParser::COMMA);
          setState(706);
          match(ASNParser::ELLIPSIS);
          setState(709);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == ASNParser::COMMA) {
            setState(707);
            match(ASNParser::COMMA);
            setState(708);
            additionalElementSetSpec();
          }
        }
        break;
      }

      case ASNParser::ELLIPSIS: {
        enterOuterAlt(_localctx, 2);
        setState(713);
        match(ASNParser::ELLIPSIS);
        setState(716);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ASNParser::COMMA) {
          setState(714);
          match(ASNParser::COMMA);
          setState(715);
          additionalElementSetSpec();
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FieldNameContext ------------------------------------------------------------------

ASNParser::FieldNameContext::FieldNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ASNParser::FieldNameContext::AMPERSAND() {
  return getTokens(ASNParser::AMPERSAND);
}

tree::TerminalNode* ASNParser::FieldNameContext::AMPERSAND(size_t i) {
  return getToken(ASNParser::AMPERSAND, i);
}

std::vector<tree::TerminalNode *> ASNParser::FieldNameContext::IDENTIFIER() {
  return getTokens(ASNParser::IDENTIFIER);
}

tree::TerminalNode* ASNParser::FieldNameContext::IDENTIFIER(size_t i) {
  return getToken(ASNParser::IDENTIFIER, i);
}

std::vector<tree::TerminalNode *> ASNParser::FieldNameContext::DOT() {
  return getTokens(ASNParser::DOT);
}

tree::TerminalNode* ASNParser::FieldNameContext::DOT(size_t i) {
  return getToken(ASNParser::DOT, i);
}


size_t ASNParser::FieldNameContext::getRuleIndex() const {
  return ASNParser::RuleFieldName;
}

antlrcpp::Any ASNParser::FieldNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitFieldName(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::FieldNameContext* ASNParser::fieldName() {
  FieldNameContext *_localctx = _tracker.createInstance<FieldNameContext>(_ctx, getState());
  enterRule(_localctx, 122, ASNParser::RuleFieldName);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(720);
    match(ASNParser::AMPERSAND);
    setState(721);
    match(ASNParser::IDENTIFIER);
    setState(728);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::AMPERSAND) {
      setState(723);
      match(ASNParser::AMPERSAND);
      setState(724);
      match(ASNParser::IDENTIFIER);
      setState(725);
      match(ASNParser::DOT);
      setState(730);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueSetContext ------------------------------------------------------------------

ASNParser::ValueSetContext::ValueSetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ValueSetContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

ASNParser::ElementSetSpecsContext* ASNParser::ValueSetContext::elementSetSpecs() {
  return getRuleContext<ASNParser::ElementSetSpecsContext>(0);
}

tree::TerminalNode* ASNParser::ValueSetContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}


size_t ASNParser::ValueSetContext::getRuleIndex() const {
  return ASNParser::RuleValueSet;
}

antlrcpp::Any ASNParser::ValueSetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitValueSet(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ValueSetContext* ASNParser::valueSet() {
  ValueSetContext *_localctx = _tracker.createInstance<ValueSetContext>(_ctx, getState());
  enterRule(_localctx, 124, ASNParser::RuleValueSet);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(731);
    match(ASNParser::L_BRACE);
    setState(732);
    elementSetSpecs();
    setState(733);
    match(ASNParser::R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ElementSetSpecsContext ------------------------------------------------------------------

ASNParser::ElementSetSpecsContext::ElementSetSpecsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::RootElementSetSpecContext* ASNParser::ElementSetSpecsContext::rootElementSetSpec() {
  return getRuleContext<ASNParser::RootElementSetSpecContext>(0);
}

std::vector<tree::TerminalNode *> ASNParser::ElementSetSpecsContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::ElementSetSpecsContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}

tree::TerminalNode* ASNParser::ElementSetSpecsContext::ELLIPSIS() {
  return getToken(ASNParser::ELLIPSIS, 0);
}

ASNParser::AdditionalElementSetSpecContext* ASNParser::ElementSetSpecsContext::additionalElementSetSpec() {
  return getRuleContext<ASNParser::AdditionalElementSetSpecContext>(0);
}


size_t ASNParser::ElementSetSpecsContext::getRuleIndex() const {
  return ASNParser::RuleElementSetSpecs;
}

antlrcpp::Any ASNParser::ElementSetSpecsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitElementSetSpecs(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ElementSetSpecsContext* ASNParser::elementSetSpecs() {
  ElementSetSpecsContext *_localctx = _tracker.createInstance<ElementSetSpecsContext>(_ctx, getState());
  enterRule(_localctx, 126, ASNParser::RuleElementSetSpecs);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(735);
    rootElementSetSpec();
    setState(742);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::COMMA) {
      setState(736);
      match(ASNParser::COMMA);
      setState(737);
      match(ASNParser::ELLIPSIS);
      setState(740);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ASNParser::COMMA) {
        setState(738);
        match(ASNParser::COMMA);
        setState(739);
        additionalElementSetSpec();
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RootElementSetSpecContext ------------------------------------------------------------------

ASNParser::RootElementSetSpecContext::RootElementSetSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ElementSetSpecContext* ASNParser::RootElementSetSpecContext::elementSetSpec() {
  return getRuleContext<ASNParser::ElementSetSpecContext>(0);
}


size_t ASNParser::RootElementSetSpecContext::getRuleIndex() const {
  return ASNParser::RuleRootElementSetSpec;
}

antlrcpp::Any ASNParser::RootElementSetSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitRootElementSetSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::RootElementSetSpecContext* ASNParser::rootElementSetSpec() {
  RootElementSetSpecContext *_localctx = _tracker.createInstance<RootElementSetSpecContext>(_ctx, getState());
  enterRule(_localctx, 128, ASNParser::RuleRootElementSetSpec);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(744);
    elementSetSpec();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AdditionalElementSetSpecContext ------------------------------------------------------------------

ASNParser::AdditionalElementSetSpecContext::AdditionalElementSetSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ElementSetSpecContext* ASNParser::AdditionalElementSetSpecContext::elementSetSpec() {
  return getRuleContext<ASNParser::ElementSetSpecContext>(0);
}


size_t ASNParser::AdditionalElementSetSpecContext::getRuleIndex() const {
  return ASNParser::RuleAdditionalElementSetSpec;
}

antlrcpp::Any ASNParser::AdditionalElementSetSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitAdditionalElementSetSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::AdditionalElementSetSpecContext* ASNParser::additionalElementSetSpec() {
  AdditionalElementSetSpecContext *_localctx = _tracker.createInstance<AdditionalElementSetSpecContext>(_ctx, getState());
  enterRule(_localctx, 130, ASNParser::RuleAdditionalElementSetSpec);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(746);
    elementSetSpec();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ElementSetSpecContext ------------------------------------------------------------------

ASNParser::ElementSetSpecContext::ElementSetSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::UnionsContext* ASNParser::ElementSetSpecContext::unions() {
  return getRuleContext<ASNParser::UnionsContext>(0);
}

tree::TerminalNode* ASNParser::ElementSetSpecContext::ALL_LITERAL() {
  return getToken(ASNParser::ALL_LITERAL, 0);
}

ASNParser::ExclusionsContext* ASNParser::ElementSetSpecContext::exclusions() {
  return getRuleContext<ASNParser::ExclusionsContext>(0);
}


size_t ASNParser::ElementSetSpecContext::getRuleIndex() const {
  return ASNParser::RuleElementSetSpec;
}

antlrcpp::Any ASNParser::ElementSetSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitElementSetSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ElementSetSpecContext* ASNParser::elementSetSpec() {
  ElementSetSpecContext *_localctx = _tracker.createInstance<ElementSetSpecContext>(_ctx, getState());
  enterRule(_localctx, 132, ASNParser::RuleElementSetSpec);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(751);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::TRUE_LITERAL:
      case ASNParser::FALSE_LITERAL:
      case ASNParser::TRUE_SMALL_LITERAL:
      case ASNParser::FALSE_SMALL_LITERAL:
      case ASNParser::L_BRACE:
      case ASNParser::MINUS:
      case ASNParser::MIN_LITERAL:
      case ASNParser::SIZE_LITERAL:
      case ASNParser::PATTERN_LITERAL:
      case ASNParser::NUMBER:
      case ASNParser::CSTRING:
      case ASNParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(748);
        unions();
        break;
      }

      case ASNParser::ALL_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(749);
        match(ASNParser::ALL_LITERAL);
        setState(750);
        exclusions();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnionsContext ------------------------------------------------------------------

ASNParser::UnionsContext::UnionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::IntersectionsContext *> ASNParser::UnionsContext::intersections() {
  return getRuleContexts<ASNParser::IntersectionsContext>();
}

ASNParser::IntersectionsContext* ASNParser::UnionsContext::intersections(size_t i) {
  return getRuleContext<ASNParser::IntersectionsContext>(i);
}

std::vector<ASNParser::UnionMarkContext *> ASNParser::UnionsContext::unionMark() {
  return getRuleContexts<ASNParser::UnionMarkContext>();
}

ASNParser::UnionMarkContext* ASNParser::UnionsContext::unionMark(size_t i) {
  return getRuleContext<ASNParser::UnionMarkContext>(i);
}


size_t ASNParser::UnionsContext::getRuleIndex() const {
  return ASNParser::RuleUnions;
}

antlrcpp::Any ASNParser::UnionsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitUnions(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::UnionsContext* ASNParser::unions() {
  UnionsContext *_localctx = _tracker.createInstance<UnionsContext>(_ctx, getState());
  enterRule(_localctx, 134, ASNParser::RuleUnions);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(753);
    intersections();
    setState(759);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::PIPE

    || _la == ASNParser::UNION_LITERAL) {
      setState(754);
      unionMark();
      setState(755);
      intersections();
      setState(761);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExclusionsContext ------------------------------------------------------------------

ASNParser::ExclusionsContext::ExclusionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ExclusionsContext::EXCEPT_LITERAL() {
  return getToken(ASNParser::EXCEPT_LITERAL, 0);
}

ASNParser::ElementsContext* ASNParser::ExclusionsContext::elements() {
  return getRuleContext<ASNParser::ElementsContext>(0);
}


size_t ASNParser::ExclusionsContext::getRuleIndex() const {
  return ASNParser::RuleExclusions;
}

antlrcpp::Any ASNParser::ExclusionsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExclusions(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExclusionsContext* ASNParser::exclusions() {
  ExclusionsContext *_localctx = _tracker.createInstance<ExclusionsContext>(_ctx, getState());
  enterRule(_localctx, 136, ASNParser::RuleExclusions);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(762);
    match(ASNParser::EXCEPT_LITERAL);
    setState(763);
    elements();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntersectionsContext ------------------------------------------------------------------

ASNParser::IntersectionsContext::IntersectionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::IntersectionElementsContext *> ASNParser::IntersectionsContext::intersectionElements() {
  return getRuleContexts<ASNParser::IntersectionElementsContext>();
}

ASNParser::IntersectionElementsContext* ASNParser::IntersectionsContext::intersectionElements(size_t i) {
  return getRuleContext<ASNParser::IntersectionElementsContext>(i);
}

std::vector<ASNParser::IntersectionMarkContext *> ASNParser::IntersectionsContext::intersectionMark() {
  return getRuleContexts<ASNParser::IntersectionMarkContext>();
}

ASNParser::IntersectionMarkContext* ASNParser::IntersectionsContext::intersectionMark(size_t i) {
  return getRuleContext<ASNParser::IntersectionMarkContext>(i);
}


size_t ASNParser::IntersectionsContext::getRuleIndex() const {
  return ASNParser::RuleIntersections;
}

antlrcpp::Any ASNParser::IntersectionsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitIntersections(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::IntersectionsContext* ASNParser::intersections() {
  IntersectionsContext *_localctx = _tracker.createInstance<IntersectionsContext>(_ctx, getState());
  enterRule(_localctx, 138, ASNParser::RuleIntersections);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(765);
    intersectionElements();
    setState(771);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::POWER

    || _la == ASNParser::INTERSECTION_LITERAL) {
      setState(766);
      intersectionMark();
      setState(767);
      intersectionElements();
      setState(773);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnionMarkContext ------------------------------------------------------------------

ASNParser::UnionMarkContext::UnionMarkContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::UnionMarkContext::PIPE() {
  return getToken(ASNParser::PIPE, 0);
}

tree::TerminalNode* ASNParser::UnionMarkContext::UNION_LITERAL() {
  return getToken(ASNParser::UNION_LITERAL, 0);
}


size_t ASNParser::UnionMarkContext::getRuleIndex() const {
  return ASNParser::RuleUnionMark;
}

antlrcpp::Any ASNParser::UnionMarkContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitUnionMark(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::UnionMarkContext* ASNParser::unionMark() {
  UnionMarkContext *_localctx = _tracker.createInstance<UnionMarkContext>(_ctx, getState());
  enterRule(_localctx, 140, ASNParser::RuleUnionMark);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(774);
    _la = _input->LA(1);
    if (!(_la == ASNParser::PIPE

    || _la == ASNParser::UNION_LITERAL)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntersectionMarkContext ------------------------------------------------------------------

ASNParser::IntersectionMarkContext::IntersectionMarkContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::IntersectionMarkContext::POWER() {
  return getToken(ASNParser::POWER, 0);
}

tree::TerminalNode* ASNParser::IntersectionMarkContext::INTERSECTION_LITERAL() {
  return getToken(ASNParser::INTERSECTION_LITERAL, 0);
}


size_t ASNParser::IntersectionMarkContext::getRuleIndex() const {
  return ASNParser::RuleIntersectionMark;
}

antlrcpp::Any ASNParser::IntersectionMarkContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitIntersectionMark(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::IntersectionMarkContext* ASNParser::intersectionMark() {
  IntersectionMarkContext *_localctx = _tracker.createInstance<IntersectionMarkContext>(_ctx, getState());
  enterRule(_localctx, 142, ASNParser::RuleIntersectionMark);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(776);
    _la = _input->LA(1);
    if (!(_la == ASNParser::POWER

    || _la == ASNParser::INTERSECTION_LITERAL)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ElementsContext ------------------------------------------------------------------

ASNParser::ElementsContext::ElementsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::SubtypeElementsContext* ASNParser::ElementsContext::subtypeElements() {
  return getRuleContext<ASNParser::SubtypeElementsContext>(0);
}


size_t ASNParser::ElementsContext::getRuleIndex() const {
  return ASNParser::RuleElements;
}

antlrcpp::Any ASNParser::ElementsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitElements(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ElementsContext* ASNParser::elements() {
  ElementsContext *_localctx = _tracker.createInstance<ElementsContext>(_ctx, getState());
  enterRule(_localctx, 144, ASNParser::RuleElements);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(778);
    subtypeElements();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectSetElementsContext ------------------------------------------------------------------

ASNParser::ObjectSetElementsContext::ObjectSetElementsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ObjectContext* ASNParser::ObjectSetElementsContext::object() {
  return getRuleContext<ASNParser::ObjectContext>(0);
}

ASNParser::DefinedObjectContext* ASNParser::ObjectSetElementsContext::definedObject() {
  return getRuleContext<ASNParser::DefinedObjectContext>(0);
}


size_t ASNParser::ObjectSetElementsContext::getRuleIndex() const {
  return ASNParser::RuleObjectSetElements;
}

antlrcpp::Any ASNParser::ObjectSetElementsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjectSetElements(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectSetElementsContext* ASNParser::objectSetElements() {
  ObjectSetElementsContext *_localctx = _tracker.createInstance<ObjectSetElementsContext>(_ctx, getState());
  enterRule(_localctx, 146, ASNParser::RuleObjectSetElements);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(782);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 77, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(780);
      object();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(781);
      definedObject();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntersectionElementsContext ------------------------------------------------------------------

ASNParser::IntersectionElementsContext::IntersectionElementsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ElementsContext* ASNParser::IntersectionElementsContext::elements() {
  return getRuleContext<ASNParser::ElementsContext>(0);
}

ASNParser::ExclusionsContext* ASNParser::IntersectionElementsContext::exclusions() {
  return getRuleContext<ASNParser::ExclusionsContext>(0);
}


size_t ASNParser::IntersectionElementsContext::getRuleIndex() const {
  return ASNParser::RuleIntersectionElements;
}

antlrcpp::Any ASNParser::IntersectionElementsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitIntersectionElements(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::IntersectionElementsContext* ASNParser::intersectionElements() {
  IntersectionElementsContext *_localctx = _tracker.createInstance<IntersectionElementsContext>(_ctx, getState());
  enterRule(_localctx, 148, ASNParser::RuleIntersectionElements);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(784);
    elements();
    setState(786);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::EXCEPT_LITERAL) {
      setState(785);
      exclusions();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SubtypeElementsContext ------------------------------------------------------------------

ASNParser::SubtypeElementsContext::SubtypeElementsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::SubtypeElementsContext::DOUBLE_DOT() {
  return getToken(ASNParser::DOUBLE_DOT, 0);
}

std::vector<ASNParser::ValueContext *> ASNParser::SubtypeElementsContext::value() {
  return getRuleContexts<ASNParser::ValueContext>();
}

ASNParser::ValueContext* ASNParser::SubtypeElementsContext::value(size_t i) {
  return getRuleContext<ASNParser::ValueContext>(i);
}

tree::TerminalNode* ASNParser::SubtypeElementsContext::MIN_LITERAL() {
  return getToken(ASNParser::MIN_LITERAL, 0);
}

tree::TerminalNode* ASNParser::SubtypeElementsContext::MAX_LITERAL() {
  return getToken(ASNParser::MAX_LITERAL, 0);
}

std::vector<tree::TerminalNode *> ASNParser::SubtypeElementsContext::LESS_THAN() {
  return getTokens(ASNParser::LESS_THAN);
}

tree::TerminalNode* ASNParser::SubtypeElementsContext::LESS_THAN(size_t i) {
  return getToken(ASNParser::LESS_THAN, i);
}

ASNParser::SizeConstraintContext* ASNParser::SubtypeElementsContext::sizeConstraint() {
  return getRuleContext<ASNParser::SizeConstraintContext>(0);
}

tree::TerminalNode* ASNParser::SubtypeElementsContext::PATTERN_LITERAL() {
  return getToken(ASNParser::PATTERN_LITERAL, 0);
}


size_t ASNParser::SubtypeElementsContext::getRuleIndex() const {
  return ASNParser::RuleSubtypeElements;
}

antlrcpp::Any ASNParser::SubtypeElementsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSubtypeElements(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SubtypeElementsContext* ASNParser::subtypeElements() {
  SubtypeElementsContext *_localctx = _tracker.createInstance<SubtypeElementsContext>(_ctx, getState());
  enterRule(_localctx, 150, ASNParser::RuleSubtypeElements);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(807);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 83, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(790);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case ASNParser::TRUE_LITERAL:
        case ASNParser::FALSE_LITERAL:
        case ASNParser::TRUE_SMALL_LITERAL:
        case ASNParser::FALSE_SMALL_LITERAL:
        case ASNParser::L_BRACE:
        case ASNParser::MINUS:
        case ASNParser::NUMBER:
        case ASNParser::CSTRING:
        case ASNParser::IDENTIFIER: {
          setState(788);
          value();
          break;
        }

        case ASNParser::MIN_LITERAL: {
          setState(789);
          match(ASNParser::MIN_LITERAL);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(793);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ASNParser::LESS_THAN) {
        setState(792);
        match(ASNParser::LESS_THAN);
      }
      setState(795);
      match(ASNParser::DOUBLE_DOT);
      setState(797);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ASNParser::LESS_THAN) {
        setState(796);
        match(ASNParser::LESS_THAN);
      }
      setState(801);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case ASNParser::TRUE_LITERAL:
        case ASNParser::FALSE_LITERAL:
        case ASNParser::TRUE_SMALL_LITERAL:
        case ASNParser::FALSE_SMALL_LITERAL:
        case ASNParser::L_BRACE:
        case ASNParser::MINUS:
        case ASNParser::NUMBER:
        case ASNParser::CSTRING:
        case ASNParser::IDENTIFIER: {
          setState(799);
          value();
          break;
        }

        case ASNParser::MAX_LITERAL: {
          setState(800);
          match(ASNParser::MAX_LITERAL);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(803);
      sizeConstraint();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(804);
      match(ASNParser::PATTERN_LITERAL);
      setState(805);
      value();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(806);
      value();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariableTypeValueSetFieldSpecContext ------------------------------------------------------------------

ASNParser::VariableTypeValueSetFieldSpecContext::VariableTypeValueSetFieldSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::VariableTypeValueSetFieldSpecContext::AMPERSAND() {
  return getToken(ASNParser::AMPERSAND, 0);
}

tree::TerminalNode* ASNParser::VariableTypeValueSetFieldSpecContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

ASNParser::FieldNameContext* ASNParser::VariableTypeValueSetFieldSpecContext::fieldName() {
  return getRuleContext<ASNParser::FieldNameContext>(0);
}

ASNParser::ValueSetOptionalitySpecContext* ASNParser::VariableTypeValueSetFieldSpecContext::valueSetOptionalitySpec() {
  return getRuleContext<ASNParser::ValueSetOptionalitySpecContext>(0);
}


size_t ASNParser::VariableTypeValueSetFieldSpecContext::getRuleIndex() const {
  return ASNParser::RuleVariableTypeValueSetFieldSpec;
}

antlrcpp::Any ASNParser::VariableTypeValueSetFieldSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitVariableTypeValueSetFieldSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::VariableTypeValueSetFieldSpecContext* ASNParser::variableTypeValueSetFieldSpec() {
  VariableTypeValueSetFieldSpecContext *_localctx = _tracker.createInstance<VariableTypeValueSetFieldSpecContext>(_ctx, getState());
  enterRule(_localctx, 152, ASNParser::RuleVariableTypeValueSetFieldSpec);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(809);
    match(ASNParser::AMPERSAND);
    setState(810);
    match(ASNParser::IDENTIFIER);
    setState(811);
    fieldName();
    setState(813);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::OPTIONAL_LITERAL

    || _la == ASNParser::DEFAULT_LITERAL) {
      setState(812);
      valueSetOptionalitySpec();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectFieldSpecContext ------------------------------------------------------------------

ASNParser::ObjectFieldSpecContext::ObjectFieldSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ObjectFieldSpecContext::AMPERSAND() {
  return getToken(ASNParser::AMPERSAND, 0);
}

tree::TerminalNode* ASNParser::ObjectFieldSpecContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

ASNParser::DefinedObjectClassContext* ASNParser::ObjectFieldSpecContext::definedObjectClass() {
  return getRuleContext<ASNParser::DefinedObjectClassContext>(0);
}

ASNParser::ObjectOptionalitySpecContext* ASNParser::ObjectFieldSpecContext::objectOptionalitySpec() {
  return getRuleContext<ASNParser::ObjectOptionalitySpecContext>(0);
}


size_t ASNParser::ObjectFieldSpecContext::getRuleIndex() const {
  return ASNParser::RuleObjectFieldSpec;
}

antlrcpp::Any ASNParser::ObjectFieldSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjectFieldSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectFieldSpecContext* ASNParser::objectFieldSpec() {
  ObjectFieldSpecContext *_localctx = _tracker.createInstance<ObjectFieldSpecContext>(_ctx, getState());
  enterRule(_localctx, 154, ASNParser::RuleObjectFieldSpec);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(815);
    match(ASNParser::AMPERSAND);
    setState(816);
    match(ASNParser::IDENTIFIER);
    setState(817);
    definedObjectClass();
    setState(819);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::OPTIONAL_LITERAL

    || _la == ASNParser::DEFAULT_LITERAL) {
      setState(818);
      objectOptionalitySpec();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectOptionalitySpecContext ------------------------------------------------------------------

ASNParser::ObjectOptionalitySpecContext::ObjectOptionalitySpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ObjectOptionalitySpecContext::OPTIONAL_LITERAL() {
  return getToken(ASNParser::OPTIONAL_LITERAL, 0);
}

tree::TerminalNode* ASNParser::ObjectOptionalitySpecContext::DEFAULT_LITERAL() {
  return getToken(ASNParser::DEFAULT_LITERAL, 0);
}

ASNParser::ObjectContext* ASNParser::ObjectOptionalitySpecContext::object() {
  return getRuleContext<ASNParser::ObjectContext>(0);
}


size_t ASNParser::ObjectOptionalitySpecContext::getRuleIndex() const {
  return ASNParser::RuleObjectOptionalitySpec;
}

antlrcpp::Any ASNParser::ObjectOptionalitySpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjectOptionalitySpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectOptionalitySpecContext* ASNParser::objectOptionalitySpec() {
  ObjectOptionalitySpecContext *_localctx = _tracker.createInstance<ObjectOptionalitySpecContext>(_ctx, getState());
  enterRule(_localctx, 156, ASNParser::RuleObjectOptionalitySpec);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(824);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::OPTIONAL_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(821);
        match(ASNParser::OPTIONAL_LITERAL);
        break;
      }

      case ASNParser::DEFAULT_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(822);
        match(ASNParser::DEFAULT_LITERAL);
        setState(823);
        object();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectSetFieldSpecContext ------------------------------------------------------------------

ASNParser::ObjectSetFieldSpecContext::ObjectSetFieldSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ObjectSetFieldSpecContext::AMPERSAND() {
  return getToken(ASNParser::AMPERSAND, 0);
}

tree::TerminalNode* ASNParser::ObjectSetFieldSpecContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

ASNParser::DefinedObjectClassContext* ASNParser::ObjectSetFieldSpecContext::definedObjectClass() {
  return getRuleContext<ASNParser::DefinedObjectClassContext>(0);
}

ASNParser::ObjectSetOptionalitySpecContext* ASNParser::ObjectSetFieldSpecContext::objectSetOptionalitySpec() {
  return getRuleContext<ASNParser::ObjectSetOptionalitySpecContext>(0);
}


size_t ASNParser::ObjectSetFieldSpecContext::getRuleIndex() const {
  return ASNParser::RuleObjectSetFieldSpec;
}

antlrcpp::Any ASNParser::ObjectSetFieldSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjectSetFieldSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectSetFieldSpecContext* ASNParser::objectSetFieldSpec() {
  ObjectSetFieldSpecContext *_localctx = _tracker.createInstance<ObjectSetFieldSpecContext>(_ctx, getState());
  enterRule(_localctx, 158, ASNParser::RuleObjectSetFieldSpec);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(826);
    match(ASNParser::AMPERSAND);
    setState(827);
    match(ASNParser::IDENTIFIER);
    setState(828);
    definedObjectClass();
    setState(830);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::OPTIONAL_LITERAL

    || _la == ASNParser::DEFAULT_LITERAL) {
      setState(829);
      objectSetOptionalitySpec();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectSetOptionalitySpecContext ------------------------------------------------------------------

ASNParser::ObjectSetOptionalitySpecContext::ObjectSetOptionalitySpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ObjectSetOptionalitySpecContext::OPTIONAL_LITERAL() {
  return getToken(ASNParser::OPTIONAL_LITERAL, 0);
}

tree::TerminalNode* ASNParser::ObjectSetOptionalitySpecContext::DEFAULT_LITERAL() {
  return getToken(ASNParser::DEFAULT_LITERAL, 0);
}

ASNParser::ObjectSetContext* ASNParser::ObjectSetOptionalitySpecContext::objectSet() {
  return getRuleContext<ASNParser::ObjectSetContext>(0);
}


size_t ASNParser::ObjectSetOptionalitySpecContext::getRuleIndex() const {
  return ASNParser::RuleObjectSetOptionalitySpec;
}

antlrcpp::Any ASNParser::ObjectSetOptionalitySpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjectSetOptionalitySpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectSetOptionalitySpecContext* ASNParser::objectSetOptionalitySpec() {
  ObjectSetOptionalitySpecContext *_localctx = _tracker.createInstance<ObjectSetOptionalitySpecContext>(_ctx, getState());
  enterRule(_localctx, 160, ASNParser::RuleObjectSetOptionalitySpec);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(835);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::OPTIONAL_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(832);
        match(ASNParser::OPTIONAL_LITERAL);
        break;
      }

      case ASNParser::DEFAULT_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(833);
        match(ASNParser::DEFAULT_LITERAL);
        setState(834);
        objectSet();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeAssignmentContext ------------------------------------------------------------------

ASNParser::TypeAssignmentContext::TypeAssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::TypeAssignmentContext::ASSIGN_OP() {
  return getToken(ASNParser::ASSIGN_OP, 0);
}

ASNParser::AsnTypeContext* ASNParser::TypeAssignmentContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}


size_t ASNParser::TypeAssignmentContext::getRuleIndex() const {
  return ASNParser::RuleTypeAssignment;
}

antlrcpp::Any ASNParser::TypeAssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitTypeAssignment(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::TypeAssignmentContext* ASNParser::typeAssignment() {
  TypeAssignmentContext *_localctx = _tracker.createInstance<TypeAssignmentContext>(_ctx, getState());
  enterRule(_localctx, 162, ASNParser::RuleTypeAssignment);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(837);
    match(ASNParser::ASSIGN_OP);
    setState(838);
    asnType();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueAssignmentContext ------------------------------------------------------------------

ASNParser::ValueAssignmentContext::ValueAssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::AsnTypeContext* ASNParser::ValueAssignmentContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}

tree::TerminalNode* ASNParser::ValueAssignmentContext::ASSIGN_OP() {
  return getToken(ASNParser::ASSIGN_OP, 0);
}

ASNParser::ValueContext* ASNParser::ValueAssignmentContext::value() {
  return getRuleContext<ASNParser::ValueContext>(0);
}


size_t ASNParser::ValueAssignmentContext::getRuleIndex() const {
  return ASNParser::RuleValueAssignment;
}

antlrcpp::Any ASNParser::ValueAssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitValueAssignment(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ValueAssignmentContext* ASNParser::valueAssignment() {
  ValueAssignmentContext *_localctx = _tracker.createInstance<ValueAssignmentContext>(_ctx, getState());
  enterRule(_localctx, 164, ASNParser::RuleValueAssignment);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(840);
    asnType();
    setState(841);
    match(ASNParser::ASSIGN_OP);
    setState(842);
    value();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AsnTypeContext ------------------------------------------------------------------

ASNParser::AsnTypeContext::AsnTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::BuiltinTypeContext* ASNParser::AsnTypeContext::builtinType() {
  return getRuleContext<ASNParser::BuiltinTypeContext>(0);
}

ASNParser::ReferencedTypeContext* ASNParser::AsnTypeContext::referencedType() {
  return getRuleContext<ASNParser::ReferencedTypeContext>(0);
}

std::vector<ASNParser::ConstraintContext *> ASNParser::AsnTypeContext::constraint() {
  return getRuleContexts<ASNParser::ConstraintContext>();
}

ASNParser::ConstraintContext* ASNParser::AsnTypeContext::constraint(size_t i) {
  return getRuleContext<ASNParser::ConstraintContext>(i);
}


size_t ASNParser::AsnTypeContext::getRuleIndex() const {
  return ASNParser::RuleAsnType;
}

antlrcpp::Any ASNParser::AsnTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitAsnType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::AsnTypeContext* ASNParser::asnType() {
  AsnTypeContext *_localctx = _tracker.createInstance<AsnTypeContext>(_ctx, getState());
  enterRule(_localctx, 166, ASNParser::RuleAsnType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(846);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 89, _ctx)) {
    case 1: {
      setState(844);
      builtinType();
      break;
    }

    case 2: {
      setState(845);
      referencedType();
      break;
    }

    }
    setState(851);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 90, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(848);
        constraint(); 
      }
      setState(853);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 90, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BuiltinTypeContext ------------------------------------------------------------------

ASNParser::BuiltinTypeContext::BuiltinTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::OctetStringTypeContext* ASNParser::BuiltinTypeContext::octetStringType() {
  return getRuleContext<ASNParser::OctetStringTypeContext>(0);
}

ASNParser::BitStringTypeContext* ASNParser::BuiltinTypeContext::bitStringType() {
  return getRuleContext<ASNParser::BitStringTypeContext>(0);
}

ASNParser::ChoiceTypeContext* ASNParser::BuiltinTypeContext::choiceType() {
  return getRuleContext<ASNParser::ChoiceTypeContext>(0);
}

ASNParser::EnumeratedTypeContext* ASNParser::BuiltinTypeContext::enumeratedType() {
  return getRuleContext<ASNParser::EnumeratedTypeContext>(0);
}

ASNParser::IntegerTypeContext* ASNParser::BuiltinTypeContext::integerType() {
  return getRuleContext<ASNParser::IntegerTypeContext>(0);
}

ASNParser::RealTypeContext* ASNParser::BuiltinTypeContext::realType() {
  return getRuleContext<ASNParser::RealTypeContext>(0);
}

ASNParser::BooleanTypeContext* ASNParser::BuiltinTypeContext::booleanType() {
  return getRuleContext<ASNParser::BooleanTypeContext>(0);
}

ASNParser::SequenceTypeContext* ASNParser::BuiltinTypeContext::sequenceType() {
  return getRuleContext<ASNParser::SequenceTypeContext>(0);
}

ASNParser::SequenceOfTypeContext* ASNParser::BuiltinTypeContext::sequenceOfType() {
  return getRuleContext<ASNParser::SequenceOfTypeContext>(0);
}

ASNParser::SetTypeContext* ASNParser::BuiltinTypeContext::setType() {
  return getRuleContext<ASNParser::SetTypeContext>(0);
}

ASNParser::SetOfTypeContext* ASNParser::BuiltinTypeContext::setOfType() {
  return getRuleContext<ASNParser::SetOfTypeContext>(0);
}

ASNParser::ObjectidentifiertypeContext* ASNParser::BuiltinTypeContext::objectidentifiertype() {
  return getRuleContext<ASNParser::ObjectidentifiertypeContext>(0);
}

ASNParser::ObjectClassFieldTypeContext* ASNParser::BuiltinTypeContext::objectClassFieldType() {
  return getRuleContext<ASNParser::ObjectClassFieldTypeContext>(0);
}


size_t ASNParser::BuiltinTypeContext::getRuleIndex() const {
  return ASNParser::RuleBuiltinType;
}

antlrcpp::Any ASNParser::BuiltinTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitBuiltinType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::BuiltinTypeContext* ASNParser::builtinType() {
  BuiltinTypeContext *_localctx = _tracker.createInstance<BuiltinTypeContext>(_ctx, getState());
  enterRule(_localctx, 168, ASNParser::RuleBuiltinType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(867);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 91, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(854);
      octetStringType();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(855);
      bitStringType();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(856);
      choiceType();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(857);
      enumeratedType();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(858);
      integerType();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(859);
      realType();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(860);
      booleanType();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(861);
      sequenceType();
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(862);
      sequenceOfType();
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(863);
      setType();
      break;
    }

    case 11: {
      enterOuterAlt(_localctx, 11);
      setState(864);
      setOfType();
      break;
    }

    case 12: {
      enterOuterAlt(_localctx, 12);
      setState(865);
      objectidentifiertype();
      break;
    }

    case 13: {
      enterOuterAlt(_localctx, 13);
      setState(866);
      objectClassFieldType();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectClassFieldTypeContext ------------------------------------------------------------------

ASNParser::ObjectClassFieldTypeContext::ObjectClassFieldTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::DefinedObjectClassContext* ASNParser::ObjectClassFieldTypeContext::definedObjectClass() {
  return getRuleContext<ASNParser::DefinedObjectClassContext>(0);
}

tree::TerminalNode* ASNParser::ObjectClassFieldTypeContext::DOT() {
  return getToken(ASNParser::DOT, 0);
}

ASNParser::FieldNameContext* ASNParser::ObjectClassFieldTypeContext::fieldName() {
  return getRuleContext<ASNParser::FieldNameContext>(0);
}


size_t ASNParser::ObjectClassFieldTypeContext::getRuleIndex() const {
  return ASNParser::RuleObjectClassFieldType;
}

antlrcpp::Any ASNParser::ObjectClassFieldTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjectClassFieldType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectClassFieldTypeContext* ASNParser::objectClassFieldType() {
  ObjectClassFieldTypeContext *_localctx = _tracker.createInstance<ObjectClassFieldTypeContext>(_ctx, getState());
  enterRule(_localctx, 170, ASNParser::RuleObjectClassFieldType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(869);
    definedObjectClass();
    setState(870);
    match(ASNParser::DOT);
    setState(871);
    fieldName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SetTypeContext ------------------------------------------------------------------

ASNParser::SetTypeContext::SetTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::SetTypeContext::SET_LITERAL() {
  return getToken(ASNParser::SET_LITERAL, 0);
}

tree::TerminalNode* ASNParser::SetTypeContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

tree::TerminalNode* ASNParser::SetTypeContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}

ASNParser::ExtensionAndExceptionContext* ASNParser::SetTypeContext::extensionAndException() {
  return getRuleContext<ASNParser::ExtensionAndExceptionContext>(0);
}

ASNParser::OptionalExtensionMarkerContext* ASNParser::SetTypeContext::optionalExtensionMarker() {
  return getRuleContext<ASNParser::OptionalExtensionMarkerContext>(0);
}

ASNParser::ComponentTypeListsContext* ASNParser::SetTypeContext::componentTypeLists() {
  return getRuleContext<ASNParser::ComponentTypeListsContext>(0);
}


size_t ASNParser::SetTypeContext::getRuleIndex() const {
  return ASNParser::RuleSetType;
}

antlrcpp::Any ASNParser::SetTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSetType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SetTypeContext* ASNParser::setType() {
  SetTypeContext *_localctx = _tracker.createInstance<SetTypeContext>(_ctx, getState());
  enterRule(_localctx, 172, ASNParser::RuleSetType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(873);
    match(ASNParser::SET_LITERAL);
    setState(874);
    match(ASNParser::L_BRACE);
    setState(879);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 92, _ctx)) {
    case 1: {
      setState(875);
      extensionAndException();
      setState(876);
      optionalExtensionMarker();
      break;
    }

    case 2: {
      setState(878);
      componentTypeLists();
      break;
    }

    }
    setState(881);
    match(ASNParser::R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SetOfTypeContext ------------------------------------------------------------------

ASNParser::SetOfTypeContext::SetOfTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::SetOfTypeContext::SET_LITERAL() {
  return getToken(ASNParser::SET_LITERAL, 0);
}

tree::TerminalNode* ASNParser::SetOfTypeContext::OF_LITERAL() {
  return getToken(ASNParser::OF_LITERAL, 0);
}

ASNParser::AsnTypeContext* ASNParser::SetOfTypeContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}

ASNParser::NamedTypeContext* ASNParser::SetOfTypeContext::namedType() {
  return getRuleContext<ASNParser::NamedTypeContext>(0);
}

ASNParser::ConstraintContext* ASNParser::SetOfTypeContext::constraint() {
  return getRuleContext<ASNParser::ConstraintContext>(0);
}

ASNParser::SizeConstraintContext* ASNParser::SetOfTypeContext::sizeConstraint() {
  return getRuleContext<ASNParser::SizeConstraintContext>(0);
}


size_t ASNParser::SetOfTypeContext::getRuleIndex() const {
  return ASNParser::RuleSetOfType;
}

antlrcpp::Any ASNParser::SetOfTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSetOfType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SetOfTypeContext* ASNParser::setOfType() {
  SetOfTypeContext *_localctx = _tracker.createInstance<SetOfTypeContext>(_ctx, getState());
  enterRule(_localctx, 174, ASNParser::RuleSetOfType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(883);
    match(ASNParser::SET_LITERAL);
    setState(886);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::L_PARAN: {
        setState(884);
        constraint();
        break;
      }

      case ASNParser::SIZE_LITERAL: {
        setState(885);
        sizeConstraint();
        break;
      }

      case ASNParser::OF_LITERAL: {
        break;
      }

    default:
      break;
    }
    setState(888);
    match(ASNParser::OF_LITERAL);
    setState(891);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 94, _ctx)) {
    case 1: {
      setState(889);
      asnType();
      break;
    }

    case 2: {
      setState(890);
      namedType();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReferencedTypeContext ------------------------------------------------------------------

ASNParser::ReferencedTypeContext::ReferencedTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::DefinedTypeContext* ASNParser::ReferencedTypeContext::definedType() {
  return getRuleContext<ASNParser::DefinedTypeContext>(0);
}


size_t ASNParser::ReferencedTypeContext::getRuleIndex() const {
  return ASNParser::RuleReferencedType;
}

antlrcpp::Any ASNParser::ReferencedTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitReferencedType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ReferencedTypeContext* ASNParser::referencedType() {
  ReferencedTypeContext *_localctx = _tracker.createInstance<ReferencedTypeContext>(_ctx, getState());
  enterRule(_localctx, 176, ASNParser::RuleReferencedType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(893);
    definedType();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DefinedTypeContext ------------------------------------------------------------------

ASNParser::DefinedTypeContext::DefinedTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ASNParser::DefinedTypeContext::IDENTIFIER() {
  return getTokens(ASNParser::IDENTIFIER);
}

tree::TerminalNode* ASNParser::DefinedTypeContext::IDENTIFIER(size_t i) {
  return getToken(ASNParser::IDENTIFIER, i);
}

tree::TerminalNode* ASNParser::DefinedTypeContext::DOT() {
  return getToken(ASNParser::DOT, 0);
}

ASNParser::ActualParameterListContext* ASNParser::DefinedTypeContext::actualParameterList() {
  return getRuleContext<ASNParser::ActualParameterListContext>(0);
}


size_t ASNParser::DefinedTypeContext::getRuleIndex() const {
  return ASNParser::RuleDefinedType;
}

antlrcpp::Any ASNParser::DefinedTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitDefinedType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::DefinedTypeContext* ASNParser::definedType() {
  DefinedTypeContext *_localctx = _tracker.createInstance<DefinedTypeContext>(_ctx, getState());
  enterRule(_localctx, 178, ASNParser::RuleDefinedType);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(895);
    match(ASNParser::IDENTIFIER);
    setState(898);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::DOT) {
      setState(896);
      match(ASNParser::DOT);
      setState(897);
      match(ASNParser::IDENTIFIER);
    }
    setState(901);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 96, _ctx)) {
    case 1: {
      setState(900);
      actualParameterList();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstraintContext ------------------------------------------------------------------

ASNParser::ConstraintContext::ConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ConstraintContext::L_PARAN() {
  return getToken(ASNParser::L_PARAN, 0);
}

ASNParser::ConstraintSpecContext* ASNParser::ConstraintContext::constraintSpec() {
  return getRuleContext<ASNParser::ConstraintSpecContext>(0);
}

tree::TerminalNode* ASNParser::ConstraintContext::R_PARAN() {
  return getToken(ASNParser::R_PARAN, 0);
}

ASNParser::ExceptionSpecContext* ASNParser::ConstraintContext::exceptionSpec() {
  return getRuleContext<ASNParser::ExceptionSpecContext>(0);
}


size_t ASNParser::ConstraintContext::getRuleIndex() const {
  return ASNParser::RuleConstraint;
}

antlrcpp::Any ASNParser::ConstraintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitConstraint(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ConstraintContext* ASNParser::constraint() {
  ConstraintContext *_localctx = _tracker.createInstance<ConstraintContext>(_ctx, getState());
  enterRule(_localctx, 180, ASNParser::RuleConstraint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(903);
    match(ASNParser::L_PARAN);
    setState(904);
    constraintSpec();
    setState(906);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::EXCLAM) {
      setState(905);
      exceptionSpec();
    }
    setState(908);
    match(ASNParser::R_PARAN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstraintSpecContext ------------------------------------------------------------------

ASNParser::ConstraintSpecContext::ConstraintSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::GeneralConstraintContext* ASNParser::ConstraintSpecContext::generalConstraint() {
  return getRuleContext<ASNParser::GeneralConstraintContext>(0);
}

ASNParser::SubtypeConstraintContext* ASNParser::ConstraintSpecContext::subtypeConstraint() {
  return getRuleContext<ASNParser::SubtypeConstraintContext>(0);
}


size_t ASNParser::ConstraintSpecContext::getRuleIndex() const {
  return ASNParser::RuleConstraintSpec;
}

antlrcpp::Any ASNParser::ConstraintSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitConstraintSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ConstraintSpecContext* ASNParser::constraintSpec() {
  ConstraintSpecContext *_localctx = _tracker.createInstance<ConstraintSpecContext>(_ctx, getState());
  enterRule(_localctx, 182, ASNParser::RuleConstraintSpec);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(912);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 98, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(910);
      generalConstraint();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(911);
      subtypeConstraint();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UserDefinedConstraintContext ------------------------------------------------------------------

ASNParser::UserDefinedConstraintContext::UserDefinedConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::UserDefinedConstraintContext::CONSTRAINED_LITERAL() {
  return getToken(ASNParser::CONSTRAINED_LITERAL, 0);
}

tree::TerminalNode* ASNParser::UserDefinedConstraintContext::BY_LITERAL() {
  return getToken(ASNParser::BY_LITERAL, 0);
}

tree::TerminalNode* ASNParser::UserDefinedConstraintContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

std::vector<ASNParser::UserDefinedConstraintParameterContext *> ASNParser::UserDefinedConstraintContext::userDefinedConstraintParameter() {
  return getRuleContexts<ASNParser::UserDefinedConstraintParameterContext>();
}

ASNParser::UserDefinedConstraintParameterContext* ASNParser::UserDefinedConstraintContext::userDefinedConstraintParameter(size_t i) {
  return getRuleContext<ASNParser::UserDefinedConstraintParameterContext>(i);
}

tree::TerminalNode* ASNParser::UserDefinedConstraintContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}

std::vector<tree::TerminalNode *> ASNParser::UserDefinedConstraintContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::UserDefinedConstraintContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}


size_t ASNParser::UserDefinedConstraintContext::getRuleIndex() const {
  return ASNParser::RuleUserDefinedConstraint;
}

antlrcpp::Any ASNParser::UserDefinedConstraintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitUserDefinedConstraint(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::UserDefinedConstraintContext* ASNParser::userDefinedConstraint() {
  UserDefinedConstraintContext *_localctx = _tracker.createInstance<UserDefinedConstraintContext>(_ctx, getState());
  enterRule(_localctx, 184, ASNParser::RuleUserDefinedConstraint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(914);
    match(ASNParser::CONSTRAINED_LITERAL);
    setState(915);
    match(ASNParser::BY_LITERAL);
    setState(916);
    match(ASNParser::L_BRACE);
    setState(917);
    userDefinedConstraintParameter();
    setState(922);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::COMMA) {
      setState(918);
      match(ASNParser::COMMA);
      setState(919);
      userDefinedConstraintParameter();
      setState(924);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(925);
    match(ASNParser::R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GeneralConstraintContext ------------------------------------------------------------------

ASNParser::GeneralConstraintContext::GeneralConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::UserDefinedConstraintContext* ASNParser::GeneralConstraintContext::userDefinedConstraint() {
  return getRuleContext<ASNParser::UserDefinedConstraintContext>(0);
}

ASNParser::TableConstraintContext* ASNParser::GeneralConstraintContext::tableConstraint() {
  return getRuleContext<ASNParser::TableConstraintContext>(0);
}

ASNParser::ContentsConstraintContext* ASNParser::GeneralConstraintContext::contentsConstraint() {
  return getRuleContext<ASNParser::ContentsConstraintContext>(0);
}


size_t ASNParser::GeneralConstraintContext::getRuleIndex() const {
  return ASNParser::RuleGeneralConstraint;
}

antlrcpp::Any ASNParser::GeneralConstraintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitGeneralConstraint(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::GeneralConstraintContext* ASNParser::generalConstraint() {
  GeneralConstraintContext *_localctx = _tracker.createInstance<GeneralConstraintContext>(_ctx, getState());
  enterRule(_localctx, 186, ASNParser::RuleGeneralConstraint);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(930);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::CONSTRAINED_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(927);
        userDefinedConstraint();
        break;
      }

      case ASNParser::L_BRACE: {
        enterOuterAlt(_localctx, 2);
        setState(928);
        tableConstraint();
        break;
      }

      case ASNParser::CONTAINING_LITERAL:
      case ASNParser::ENCODED_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(929);
        contentsConstraint();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UserDefinedConstraintParameterContext ------------------------------------------------------------------

ASNParser::UserDefinedConstraintParameterContext::UserDefinedConstraintParameterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::GovernorContext* ASNParser::UserDefinedConstraintParameterContext::governor() {
  return getRuleContext<ASNParser::GovernorContext>(0);
}

tree::TerminalNode* ASNParser::UserDefinedConstraintParameterContext::COLON() {
  return getToken(ASNParser::COLON, 0);
}

ASNParser::ValueContext* ASNParser::UserDefinedConstraintParameterContext::value() {
  return getRuleContext<ASNParser::ValueContext>(0);
}

ASNParser::ValueSetContext* ASNParser::UserDefinedConstraintParameterContext::valueSet() {
  return getRuleContext<ASNParser::ValueSetContext>(0);
}

ASNParser::ObjectContext* ASNParser::UserDefinedConstraintParameterContext::object() {
  return getRuleContext<ASNParser::ObjectContext>(0);
}

ASNParser::ObjectSetContext* ASNParser::UserDefinedConstraintParameterContext::objectSet() {
  return getRuleContext<ASNParser::ObjectSetContext>(0);
}


size_t ASNParser::UserDefinedConstraintParameterContext::getRuleIndex() const {
  return ASNParser::RuleUserDefinedConstraintParameter;
}

antlrcpp::Any ASNParser::UserDefinedConstraintParameterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitUserDefinedConstraintParameter(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::UserDefinedConstraintParameterContext* ASNParser::userDefinedConstraintParameter() {
  UserDefinedConstraintParameterContext *_localctx = _tracker.createInstance<UserDefinedConstraintParameterContext>(_ctx, getState());
  enterRule(_localctx, 188, ASNParser::RuleUserDefinedConstraintParameter);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(932);
    governor();
    setState(938);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 101, _ctx)) {
    case 1: {
      setState(933);
      match(ASNParser::COLON);
      setState(934);
      value();
      break;
    }

    case 2: {
      setState(935);
      valueSet();
      break;
    }

    case 3: {
      setState(936);
      object();
      break;
    }

    case 4: {
      setState(937);
      objectSet();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TableConstraintContext ------------------------------------------------------------------

ASNParser::TableConstraintContext::TableConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ComponentRelationConstraintContext* ASNParser::TableConstraintContext::componentRelationConstraint() {
  return getRuleContext<ASNParser::ComponentRelationConstraintContext>(0);
}


size_t ASNParser::TableConstraintContext::getRuleIndex() const {
  return ASNParser::RuleTableConstraint;
}

antlrcpp::Any ASNParser::TableConstraintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitTableConstraint(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::TableConstraintContext* ASNParser::tableConstraint() {
  TableConstraintContext *_localctx = _tracker.createInstance<TableConstraintContext>(_ctx, getState());
  enterRule(_localctx, 190, ASNParser::RuleTableConstraint);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(940);
    componentRelationConstraint();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleTableConstraintContext ------------------------------------------------------------------

ASNParser::SimpleTableConstraintContext::SimpleTableConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ObjectSetContext* ASNParser::SimpleTableConstraintContext::objectSet() {
  return getRuleContext<ASNParser::ObjectSetContext>(0);
}


size_t ASNParser::SimpleTableConstraintContext::getRuleIndex() const {
  return ASNParser::RuleSimpleTableConstraint;
}

antlrcpp::Any ASNParser::SimpleTableConstraintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSimpleTableConstraint(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SimpleTableConstraintContext* ASNParser::simpleTableConstraint() {
  SimpleTableConstraintContext *_localctx = _tracker.createInstance<SimpleTableConstraintContext>(_ctx, getState());
  enterRule(_localctx, 192, ASNParser::RuleSimpleTableConstraint);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(942);
    objectSet();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ContentsConstraintContext ------------------------------------------------------------------

ASNParser::ContentsConstraintContext::ContentsConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ContentsConstraintContext::CONTAINING_LITERAL() {
  return getToken(ASNParser::CONTAINING_LITERAL, 0);
}

ASNParser::AsnTypeContext* ASNParser::ContentsConstraintContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}

tree::TerminalNode* ASNParser::ContentsConstraintContext::ENCODED_LITERAL() {
  return getToken(ASNParser::ENCODED_LITERAL, 0);
}

tree::TerminalNode* ASNParser::ContentsConstraintContext::BY_LITERAL() {
  return getToken(ASNParser::BY_LITERAL, 0);
}

ASNParser::ValueContext* ASNParser::ContentsConstraintContext::value() {
  return getRuleContext<ASNParser::ValueContext>(0);
}


size_t ASNParser::ContentsConstraintContext::getRuleIndex() const {
  return ASNParser::RuleContentsConstraint;
}

antlrcpp::Any ASNParser::ContentsConstraintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitContentsConstraint(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ContentsConstraintContext* ASNParser::contentsConstraint() {
  ContentsConstraintContext *_localctx = _tracker.createInstance<ContentsConstraintContext>(_ctx, getState());
  enterRule(_localctx, 194, ASNParser::RuleContentsConstraint);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(955);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 102, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(944);
      match(ASNParser::CONTAINING_LITERAL);
      setState(945);
      asnType();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(946);
      match(ASNParser::ENCODED_LITERAL);
      setState(947);
      match(ASNParser::BY_LITERAL);
      setState(948);
      value();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(949);
      match(ASNParser::CONTAINING_LITERAL);
      setState(950);
      asnType();
      setState(951);
      match(ASNParser::ENCODED_LITERAL);
      setState(952);
      match(ASNParser::BY_LITERAL);
      setState(953);
      value();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SubtypeConstraintContext ------------------------------------------------------------------

ASNParser::SubtypeConstraintContext::SubtypeConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ElementSetSpecsContext* ASNParser::SubtypeConstraintContext::elementSetSpecs() {
  return getRuleContext<ASNParser::ElementSetSpecsContext>(0);
}


size_t ASNParser::SubtypeConstraintContext::getRuleIndex() const {
  return ASNParser::RuleSubtypeConstraint;
}

antlrcpp::Any ASNParser::SubtypeConstraintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSubtypeConstraint(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SubtypeConstraintContext* ASNParser::subtypeConstraint() {
  SubtypeConstraintContext *_localctx = _tracker.createInstance<SubtypeConstraintContext>(_ctx, getState());
  enterRule(_localctx, 196, ASNParser::RuleSubtypeConstraint);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(957);
    elementSetSpecs();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueContext ------------------------------------------------------------------

ASNParser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::BuiltinValueContext* ASNParser::ValueContext::builtinValue() {
  return getRuleContext<ASNParser::BuiltinValueContext>(0);
}


size_t ASNParser::ValueContext::getRuleIndex() const {
  return ASNParser::RuleValue;
}

antlrcpp::Any ASNParser::ValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitValue(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ValueContext* ASNParser::value() {
  ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, getState());
  enterRule(_localctx, 198, ASNParser::RuleValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(959);
    builtinValue();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BuiltinValueContext ------------------------------------------------------------------

ASNParser::BuiltinValueContext::BuiltinValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::EnumeratedValueContext* ASNParser::BuiltinValueContext::enumeratedValue() {
  return getRuleContext<ASNParser::EnumeratedValueContext>(0);
}

ASNParser::IntegerValueContext* ASNParser::BuiltinValueContext::integerValue() {
  return getRuleContext<ASNParser::IntegerValueContext>(0);
}

ASNParser::ChoiceValueContext* ASNParser::BuiltinValueContext::choiceValue() {
  return getRuleContext<ASNParser::ChoiceValueContext>(0);
}

ASNParser::ObjectIdentifierValueContext* ASNParser::BuiltinValueContext::objectIdentifierValue() {
  return getRuleContext<ASNParser::ObjectIdentifierValueContext>(0);
}

ASNParser::BooleanValueContext* ASNParser::BuiltinValueContext::booleanValue() {
  return getRuleContext<ASNParser::BooleanValueContext>(0);
}

ASNParser::RealValueContext* ASNParser::BuiltinValueContext::realValue() {
  return getRuleContext<ASNParser::RealValueContext>(0);
}

tree::TerminalNode* ASNParser::BuiltinValueContext::CSTRING() {
  return getToken(ASNParser::CSTRING, 0);
}


size_t ASNParser::BuiltinValueContext::getRuleIndex() const {
  return ASNParser::RuleBuiltinValue;
}

antlrcpp::Any ASNParser::BuiltinValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitBuiltinValue(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::BuiltinValueContext* ASNParser::builtinValue() {
  BuiltinValueContext *_localctx = _tracker.createInstance<BuiltinValueContext>(_ctx, getState());
  enterRule(_localctx, 200, ASNParser::RuleBuiltinValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(968);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 103, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(961);
      enumeratedValue();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(962);
      integerValue();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(963);
      choiceValue();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(964);
      objectIdentifierValue();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(965);
      booleanValue();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(966);
      realValue();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(967);
      match(ASNParser::CSTRING);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectIdentifierValueContext ------------------------------------------------------------------

ASNParser::ObjectIdentifierValueContext::ObjectIdentifierValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ObjectIdentifierValueContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

ASNParser::ObjIdComponentsListContext* ASNParser::ObjectIdentifierValueContext::objIdComponentsList() {
  return getRuleContext<ASNParser::ObjIdComponentsListContext>(0);
}

tree::TerminalNode* ASNParser::ObjectIdentifierValueContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}


size_t ASNParser::ObjectIdentifierValueContext::getRuleIndex() const {
  return ASNParser::RuleObjectIdentifierValue;
}

antlrcpp::Any ASNParser::ObjectIdentifierValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjectIdentifierValue(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectIdentifierValueContext* ASNParser::objectIdentifierValue() {
  ObjectIdentifierValueContext *_localctx = _tracker.createInstance<ObjectIdentifierValueContext>(_ctx, getState());
  enterRule(_localctx, 202, ASNParser::RuleObjectIdentifierValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(970);
    match(ASNParser::L_BRACE);
    setState(971);
    objIdComponentsList();
    setState(972);
    match(ASNParser::R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjIdComponentsListContext ------------------------------------------------------------------

ASNParser::ObjIdComponentsListContext::ObjIdComponentsListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::ObjIdComponentsContext *> ASNParser::ObjIdComponentsListContext::objIdComponents() {
  return getRuleContexts<ASNParser::ObjIdComponentsContext>();
}

ASNParser::ObjIdComponentsContext* ASNParser::ObjIdComponentsListContext::objIdComponents(size_t i) {
  return getRuleContext<ASNParser::ObjIdComponentsContext>(i);
}


size_t ASNParser::ObjIdComponentsListContext::getRuleIndex() const {
  return ASNParser::RuleObjIdComponentsList;
}

antlrcpp::Any ASNParser::ObjIdComponentsListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjIdComponentsList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjIdComponentsListContext* ASNParser::objIdComponentsList() {
  ObjIdComponentsListContext *_localctx = _tracker.createInstance<ObjIdComponentsListContext>(_ctx, getState());
  enterRule(_localctx, 204, ASNParser::RuleObjIdComponentsList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(974);
    objIdComponents();
    setState(978);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::NUMBER

    || _la == ASNParser::IDENTIFIER) {
      setState(975);
      objIdComponents();
      setState(980);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjIdComponentsContext ------------------------------------------------------------------

ASNParser::ObjIdComponentsContext::ObjIdComponentsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ObjIdComponentsContext::NUMBER() {
  return getToken(ASNParser::NUMBER, 0);
}

tree::TerminalNode* ASNParser::ObjIdComponentsContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

tree::TerminalNode* ASNParser::ObjIdComponentsContext::L_PARAN() {
  return getToken(ASNParser::L_PARAN, 0);
}

tree::TerminalNode* ASNParser::ObjIdComponentsContext::R_PARAN() {
  return getToken(ASNParser::R_PARAN, 0);
}

ASNParser::DefinedValueContext* ASNParser::ObjIdComponentsContext::definedValue() {
  return getRuleContext<ASNParser::DefinedValueContext>(0);
}


size_t ASNParser::ObjIdComponentsContext::getRuleIndex() const {
  return ASNParser::RuleObjIdComponents;
}

antlrcpp::Any ASNParser::ObjIdComponentsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjIdComponents(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjIdComponentsContext* ASNParser::objIdComponents() {
  ObjIdComponentsContext *_localctx = _tracker.createInstance<ObjIdComponentsContext>(_ctx, getState());
  enterRule(_localctx, 206, ASNParser::RuleObjIdComponents);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(992);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 107, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(981);
      match(ASNParser::NUMBER);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(982);
      match(ASNParser::IDENTIFIER);
      setState(989);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ASNParser::L_PARAN) {
        setState(983);
        match(ASNParser::L_PARAN);
        setState(986);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case ASNParser::NUMBER: {
            setState(984);
            match(ASNParser::NUMBER);
            break;
          }

          case ASNParser::IDENTIFIER: {
            setState(985);
            definedValue();
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(988);
        match(ASNParser::R_PARAN);
      }
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(991);
      definedValue();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntegerValueContext ------------------------------------------------------------------

ASNParser::IntegerValueContext::IntegerValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::SignedNumberContext* ASNParser::IntegerValueContext::signedNumber() {
  return getRuleContext<ASNParser::SignedNumberContext>(0);
}

tree::TerminalNode* ASNParser::IntegerValueContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}


size_t ASNParser::IntegerValueContext::getRuleIndex() const {
  return ASNParser::RuleIntegerValue;
}

antlrcpp::Any ASNParser::IntegerValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitIntegerValue(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::IntegerValueContext* ASNParser::integerValue() {
  IntegerValueContext *_localctx = _tracker.createInstance<IntegerValueContext>(_ctx, getState());
  enterRule(_localctx, 208, ASNParser::RuleIntegerValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(996);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::MINUS:
      case ASNParser::NUMBER: {
        enterOuterAlt(_localctx, 1);
        setState(994);
        signedNumber();
        break;
      }

      case ASNParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(995);
        match(ASNParser::IDENTIFIER);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RealValueContext ------------------------------------------------------------------

ASNParser::RealValueContext::RealValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::SignedNumberContext* ASNParser::RealValueContext::signedNumber() {
  return getRuleContext<ASNParser::SignedNumberContext>(0);
}

tree::TerminalNode* ASNParser::RealValueContext::DOT() {
  return getToken(ASNParser::DOT, 0);
}

tree::TerminalNode* ASNParser::RealValueContext::NUMBER() {
  return getToken(ASNParser::NUMBER, 0);
}

tree::TerminalNode* ASNParser::RealValueContext::Exponent() {
  return getToken(ASNParser::Exponent, 0);
}


size_t ASNParser::RealValueContext::getRuleIndex() const {
  return ASNParser::RuleRealValue;
}

antlrcpp::Any ASNParser::RealValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitRealValue(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::RealValueContext* ASNParser::realValue() {
  RealValueContext *_localctx = _tracker.createInstance<RealValueContext>(_ctx, getState());
  enterRule(_localctx, 210, ASNParser::RuleRealValue);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(998);
    signedNumber();
    setState(999);
    match(ASNParser::DOT);
    setState(1001);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::NUMBER) {
      setState(1000);
      match(ASNParser::NUMBER);
    }
    setState(1004);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::Exponent) {
      setState(1003);
      match(ASNParser::Exponent);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChoiceValueContext ------------------------------------------------------------------

ASNParser::ChoiceValueContext::ChoiceValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ChoiceValueContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

tree::TerminalNode* ASNParser::ChoiceValueContext::COLON() {
  return getToken(ASNParser::COLON, 0);
}

ASNParser::ValueContext* ASNParser::ChoiceValueContext::value() {
  return getRuleContext<ASNParser::ValueContext>(0);
}


size_t ASNParser::ChoiceValueContext::getRuleIndex() const {
  return ASNParser::RuleChoiceValue;
}

antlrcpp::Any ASNParser::ChoiceValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitChoiceValue(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ChoiceValueContext* ASNParser::choiceValue() {
  ChoiceValueContext *_localctx = _tracker.createInstance<ChoiceValueContext>(_ctx, getState());
  enterRule(_localctx, 212, ASNParser::RuleChoiceValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1006);
    match(ASNParser::IDENTIFIER);
    setState(1007);
    match(ASNParser::COLON);
    setState(1008);
    value();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EnumeratedValueContext ------------------------------------------------------------------

ASNParser::EnumeratedValueContext::EnumeratedValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::EnumeratedValueContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}


size_t ASNParser::EnumeratedValueContext::getRuleIndex() const {
  return ASNParser::RuleEnumeratedValue;
}

antlrcpp::Any ASNParser::EnumeratedValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitEnumeratedValue(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::EnumeratedValueContext* ASNParser::enumeratedValue() {
  EnumeratedValueContext *_localctx = _tracker.createInstance<EnumeratedValueContext>(_ctx, getState());
  enterRule(_localctx, 214, ASNParser::RuleEnumeratedValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1010);
    match(ASNParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SignedNumberContext ------------------------------------------------------------------

ASNParser::SignedNumberContext::SignedNumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::SignedNumberContext::NUMBER() {
  return getToken(ASNParser::NUMBER, 0);
}

tree::TerminalNode* ASNParser::SignedNumberContext::MINUS() {
  return getToken(ASNParser::MINUS, 0);
}


size_t ASNParser::SignedNumberContext::getRuleIndex() const {
  return ASNParser::RuleSignedNumber;
}

antlrcpp::Any ASNParser::SignedNumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSignedNumber(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SignedNumberContext* ASNParser::signedNumber() {
  SignedNumberContext *_localctx = _tracker.createInstance<SignedNumberContext>(_ctx, getState());
  enterRule(_localctx, 216, ASNParser::RuleSignedNumber);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1013);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::MINUS) {
      setState(1012);
      match(ASNParser::MINUS);
    }
    setState(1015);
    match(ASNParser::NUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ChoiceTypeContext ------------------------------------------------------------------

ASNParser::ChoiceTypeContext::ChoiceTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ChoiceTypeContext::CHOICE_LITERAL() {
  return getToken(ASNParser::CHOICE_LITERAL, 0);
}

tree::TerminalNode* ASNParser::ChoiceTypeContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

ASNParser::AlternativeTypeListsContext* ASNParser::ChoiceTypeContext::alternativeTypeLists() {
  return getRuleContext<ASNParser::AlternativeTypeListsContext>(0);
}

tree::TerminalNode* ASNParser::ChoiceTypeContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}


size_t ASNParser::ChoiceTypeContext::getRuleIndex() const {
  return ASNParser::RuleChoiceType;
}

antlrcpp::Any ASNParser::ChoiceTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitChoiceType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ChoiceTypeContext* ASNParser::choiceType() {
  ChoiceTypeContext *_localctx = _tracker.createInstance<ChoiceTypeContext>(_ctx, getState());
  enterRule(_localctx, 218, ASNParser::RuleChoiceType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1017);
    match(ASNParser::CHOICE_LITERAL);
    setState(1018);
    match(ASNParser::L_BRACE);
    setState(1019);
    alternativeTypeLists();
    setState(1020);
    match(ASNParser::R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AlternativeTypeListsContext ------------------------------------------------------------------

ASNParser::AlternativeTypeListsContext::AlternativeTypeListsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::RootAlternativeTypeListContext* ASNParser::AlternativeTypeListsContext::rootAlternativeTypeList() {
  return getRuleContext<ASNParser::RootAlternativeTypeListContext>(0);
}

tree::TerminalNode* ASNParser::AlternativeTypeListsContext::COMMA() {
  return getToken(ASNParser::COMMA, 0);
}

ASNParser::ExtensionAndExceptionContext* ASNParser::AlternativeTypeListsContext::extensionAndException() {
  return getRuleContext<ASNParser::ExtensionAndExceptionContext>(0);
}

ASNParser::ExtensionAdditionAlternativesContext* ASNParser::AlternativeTypeListsContext::extensionAdditionAlternatives() {
  return getRuleContext<ASNParser::ExtensionAdditionAlternativesContext>(0);
}

ASNParser::OptionalExtensionMarkerContext* ASNParser::AlternativeTypeListsContext::optionalExtensionMarker() {
  return getRuleContext<ASNParser::OptionalExtensionMarkerContext>(0);
}


size_t ASNParser::AlternativeTypeListsContext::getRuleIndex() const {
  return ASNParser::RuleAlternativeTypeLists;
}

antlrcpp::Any ASNParser::AlternativeTypeListsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitAlternativeTypeLists(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::AlternativeTypeListsContext* ASNParser::alternativeTypeLists() {
  AlternativeTypeListsContext *_localctx = _tracker.createInstance<AlternativeTypeListsContext>(_ctx, getState());
  enterRule(_localctx, 220, ASNParser::RuleAlternativeTypeLists);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1022);
    rootAlternativeTypeList();
    setState(1028);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::COMMA) {
      setState(1023);
      match(ASNParser::COMMA);
      setState(1024);
      extensionAndException();
      setState(1025);
      extensionAdditionAlternatives();
      setState(1026);
      optionalExtensionMarker();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExtensionAdditionAlternativesContext ------------------------------------------------------------------

ASNParser::ExtensionAdditionAlternativesContext::ExtensionAdditionAlternativesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ExtensionAdditionAlternativesContext::COMMA() {
  return getToken(ASNParser::COMMA, 0);
}

ASNParser::ExtensionAdditionAlternativesListContext* ASNParser::ExtensionAdditionAlternativesContext::extensionAdditionAlternativesList() {
  return getRuleContext<ASNParser::ExtensionAdditionAlternativesListContext>(0);
}


size_t ASNParser::ExtensionAdditionAlternativesContext::getRuleIndex() const {
  return ASNParser::RuleExtensionAdditionAlternatives;
}

antlrcpp::Any ASNParser::ExtensionAdditionAlternativesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExtensionAdditionAlternatives(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExtensionAdditionAlternativesContext* ASNParser::extensionAdditionAlternatives() {
  ExtensionAdditionAlternativesContext *_localctx = _tracker.createInstance<ExtensionAdditionAlternativesContext>(_ctx, getState());
  enterRule(_localctx, 222, ASNParser::RuleExtensionAdditionAlternatives);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1032);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 113, _ctx)) {
    case 1: {
      setState(1030);
      match(ASNParser::COMMA);
      setState(1031);
      extensionAdditionAlternativesList();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExtensionAdditionAlternativesListContext ------------------------------------------------------------------

ASNParser::ExtensionAdditionAlternativesListContext::ExtensionAdditionAlternativesListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::ExtensionAdditionAlternativeContext *> ASNParser::ExtensionAdditionAlternativesListContext::extensionAdditionAlternative() {
  return getRuleContexts<ASNParser::ExtensionAdditionAlternativeContext>();
}

ASNParser::ExtensionAdditionAlternativeContext* ASNParser::ExtensionAdditionAlternativesListContext::extensionAdditionAlternative(size_t i) {
  return getRuleContext<ASNParser::ExtensionAdditionAlternativeContext>(i);
}

std::vector<tree::TerminalNode *> ASNParser::ExtensionAdditionAlternativesListContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::ExtensionAdditionAlternativesListContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}


size_t ASNParser::ExtensionAdditionAlternativesListContext::getRuleIndex() const {
  return ASNParser::RuleExtensionAdditionAlternativesList;
}

antlrcpp::Any ASNParser::ExtensionAdditionAlternativesListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExtensionAdditionAlternativesList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExtensionAdditionAlternativesListContext* ASNParser::extensionAdditionAlternativesList() {
  ExtensionAdditionAlternativesListContext *_localctx = _tracker.createInstance<ExtensionAdditionAlternativesListContext>(_ctx, getState());
  enterRule(_localctx, 224, ASNParser::RuleExtensionAdditionAlternativesList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1034);
    extensionAdditionAlternative();
    setState(1039);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 114, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1035);
        match(ASNParser::COMMA);
        setState(1036);
        extensionAdditionAlternative(); 
      }
      setState(1041);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 114, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExtensionAdditionAlternativeContext ------------------------------------------------------------------

ASNParser::ExtensionAdditionAlternativeContext::ExtensionAdditionAlternativeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ExtensionAdditionAlternativesGroupContext* ASNParser::ExtensionAdditionAlternativeContext::extensionAdditionAlternativesGroup() {
  return getRuleContext<ASNParser::ExtensionAdditionAlternativesGroupContext>(0);
}

ASNParser::NamedTypeContext* ASNParser::ExtensionAdditionAlternativeContext::namedType() {
  return getRuleContext<ASNParser::NamedTypeContext>(0);
}


size_t ASNParser::ExtensionAdditionAlternativeContext::getRuleIndex() const {
  return ASNParser::RuleExtensionAdditionAlternative;
}

antlrcpp::Any ASNParser::ExtensionAdditionAlternativeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExtensionAdditionAlternative(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExtensionAdditionAlternativeContext* ASNParser::extensionAdditionAlternative() {
  ExtensionAdditionAlternativeContext *_localctx = _tracker.createInstance<ExtensionAdditionAlternativeContext>(_ctx, getState());
  enterRule(_localctx, 226, ASNParser::RuleExtensionAdditionAlternative);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1044);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::DOUBLE_L_BRACKET: {
        enterOuterAlt(_localctx, 1);
        setState(1042);
        extensionAdditionAlternativesGroup();
        break;
      }

      case ASNParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 2);
        setState(1043);
        namedType();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExtensionAdditionAlternativesGroupContext ------------------------------------------------------------------

ASNParser::ExtensionAdditionAlternativesGroupContext::ExtensionAdditionAlternativesGroupContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ExtensionAdditionAlternativesGroupContext::DOUBLE_L_BRACKET() {
  return getToken(ASNParser::DOUBLE_L_BRACKET, 0);
}

ASNParser::VersionNumberContext* ASNParser::ExtensionAdditionAlternativesGroupContext::versionNumber() {
  return getRuleContext<ASNParser::VersionNumberContext>(0);
}

ASNParser::AlternativeTypeListContext* ASNParser::ExtensionAdditionAlternativesGroupContext::alternativeTypeList() {
  return getRuleContext<ASNParser::AlternativeTypeListContext>(0);
}

tree::TerminalNode* ASNParser::ExtensionAdditionAlternativesGroupContext::DOUBLE_R_BRACKET() {
  return getToken(ASNParser::DOUBLE_R_BRACKET, 0);
}


size_t ASNParser::ExtensionAdditionAlternativesGroupContext::getRuleIndex() const {
  return ASNParser::RuleExtensionAdditionAlternativesGroup;
}

antlrcpp::Any ASNParser::ExtensionAdditionAlternativesGroupContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExtensionAdditionAlternativesGroup(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExtensionAdditionAlternativesGroupContext* ASNParser::extensionAdditionAlternativesGroup() {
  ExtensionAdditionAlternativesGroupContext *_localctx = _tracker.createInstance<ExtensionAdditionAlternativesGroupContext>(_ctx, getState());
  enterRule(_localctx, 228, ASNParser::RuleExtensionAdditionAlternativesGroup);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1046);
    match(ASNParser::DOUBLE_L_BRACKET);
    setState(1047);
    versionNumber();
    setState(1048);
    alternativeTypeList();
    setState(1049);
    match(ASNParser::DOUBLE_R_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RootAlternativeTypeListContext ------------------------------------------------------------------

ASNParser::RootAlternativeTypeListContext::RootAlternativeTypeListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::AlternativeTypeListContext* ASNParser::RootAlternativeTypeListContext::alternativeTypeList() {
  return getRuleContext<ASNParser::AlternativeTypeListContext>(0);
}


size_t ASNParser::RootAlternativeTypeListContext::getRuleIndex() const {
  return ASNParser::RuleRootAlternativeTypeList;
}

antlrcpp::Any ASNParser::RootAlternativeTypeListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitRootAlternativeTypeList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::RootAlternativeTypeListContext* ASNParser::rootAlternativeTypeList() {
  RootAlternativeTypeListContext *_localctx = _tracker.createInstance<RootAlternativeTypeListContext>(_ctx, getState());
  enterRule(_localctx, 230, ASNParser::RuleRootAlternativeTypeList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1051);
    alternativeTypeList();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AlternativeTypeListContext ------------------------------------------------------------------

ASNParser::AlternativeTypeListContext::AlternativeTypeListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::NamedTypeContext *> ASNParser::AlternativeTypeListContext::namedType() {
  return getRuleContexts<ASNParser::NamedTypeContext>();
}

ASNParser::NamedTypeContext* ASNParser::AlternativeTypeListContext::namedType(size_t i) {
  return getRuleContext<ASNParser::NamedTypeContext>(i);
}

std::vector<tree::TerminalNode *> ASNParser::AlternativeTypeListContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::AlternativeTypeListContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}


size_t ASNParser::AlternativeTypeListContext::getRuleIndex() const {
  return ASNParser::RuleAlternativeTypeList;
}

antlrcpp::Any ASNParser::AlternativeTypeListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitAlternativeTypeList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::AlternativeTypeListContext* ASNParser::alternativeTypeList() {
  AlternativeTypeListContext *_localctx = _tracker.createInstance<AlternativeTypeListContext>(_ctx, getState());
  enterRule(_localctx, 232, ASNParser::RuleAlternativeTypeList);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1053);
    namedType();
    setState(1058);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 116, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1054);
        match(ASNParser::COMMA);
        setState(1055);
        namedType(); 
      }
      setState(1060);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 116, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NamedTypeContext ------------------------------------------------------------------

ASNParser::NamedTypeContext::NamedTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::NamedTypeContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

ASNParser::AsnTypeContext* ASNParser::NamedTypeContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}


size_t ASNParser::NamedTypeContext::getRuleIndex() const {
  return ASNParser::RuleNamedType;
}

antlrcpp::Any ASNParser::NamedTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitNamedType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::NamedTypeContext* ASNParser::namedType() {
  NamedTypeContext *_localctx = _tracker.createInstance<NamedTypeContext>(_ctx, getState());
  enterRule(_localctx, 234, ASNParser::RuleNamedType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1061);
    match(ASNParser::IDENTIFIER);
    setState(1062);
    asnType();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EnumeratedTypeContext ------------------------------------------------------------------

ASNParser::EnumeratedTypeContext::EnumeratedTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::EnumeratedTypeContext::ENUMERATED_LITERAL() {
  return getToken(ASNParser::ENUMERATED_LITERAL, 0);
}

tree::TerminalNode* ASNParser::EnumeratedTypeContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

ASNParser::EnumerationsContext* ASNParser::EnumeratedTypeContext::enumerations() {
  return getRuleContext<ASNParser::EnumerationsContext>(0);
}

tree::TerminalNode* ASNParser::EnumeratedTypeContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}


size_t ASNParser::EnumeratedTypeContext::getRuleIndex() const {
  return ASNParser::RuleEnumeratedType;
}

antlrcpp::Any ASNParser::EnumeratedTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitEnumeratedType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::EnumeratedTypeContext* ASNParser::enumeratedType() {
  EnumeratedTypeContext *_localctx = _tracker.createInstance<EnumeratedTypeContext>(_ctx, getState());
  enterRule(_localctx, 236, ASNParser::RuleEnumeratedType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1064);
    match(ASNParser::ENUMERATED_LITERAL);
    setState(1065);
    match(ASNParser::L_BRACE);
    setState(1066);
    enumerations();
    setState(1067);
    match(ASNParser::R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EnumerationsContext ------------------------------------------------------------------

ASNParser::EnumerationsContext::EnumerationsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::RootEnumerationContext* ASNParser::EnumerationsContext::rootEnumeration() {
  return getRuleContext<ASNParser::RootEnumerationContext>(0);
}

std::vector<tree::TerminalNode *> ASNParser::EnumerationsContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::EnumerationsContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}

tree::TerminalNode* ASNParser::EnumerationsContext::ELLIPSIS() {
  return getToken(ASNParser::ELLIPSIS, 0);
}

ASNParser::ExceptionSpecContext* ASNParser::EnumerationsContext::exceptionSpec() {
  return getRuleContext<ASNParser::ExceptionSpecContext>(0);
}

ASNParser::AdditionalEnumerationContext* ASNParser::EnumerationsContext::additionalEnumeration() {
  return getRuleContext<ASNParser::AdditionalEnumerationContext>(0);
}


size_t ASNParser::EnumerationsContext::getRuleIndex() const {
  return ASNParser::RuleEnumerations;
}

antlrcpp::Any ASNParser::EnumerationsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitEnumerations(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::EnumerationsContext* ASNParser::enumerations() {
  EnumerationsContext *_localctx = _tracker.createInstance<EnumerationsContext>(_ctx, getState());
  enterRule(_localctx, 238, ASNParser::RuleEnumerations);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1069);
    rootEnumeration();
    setState(1079);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::COMMA) {
      setState(1070);
      match(ASNParser::COMMA);
      setState(1071);
      match(ASNParser::ELLIPSIS);
      setState(1073);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ASNParser::EXCLAM) {
        setState(1072);
        exceptionSpec();
      }
      setState(1077);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ASNParser::COMMA) {
        setState(1075);
        match(ASNParser::COMMA);
        setState(1076);
        additionalEnumeration();
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RootEnumerationContext ------------------------------------------------------------------

ASNParser::RootEnumerationContext::RootEnumerationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::EnumerationContext* ASNParser::RootEnumerationContext::enumeration() {
  return getRuleContext<ASNParser::EnumerationContext>(0);
}


size_t ASNParser::RootEnumerationContext::getRuleIndex() const {
  return ASNParser::RuleRootEnumeration;
}

antlrcpp::Any ASNParser::RootEnumerationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitRootEnumeration(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::RootEnumerationContext* ASNParser::rootEnumeration() {
  RootEnumerationContext *_localctx = _tracker.createInstance<RootEnumerationContext>(_ctx, getState());
  enterRule(_localctx, 240, ASNParser::RuleRootEnumeration);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1081);
    enumeration();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EnumerationContext ------------------------------------------------------------------

ASNParser::EnumerationContext::EnumerationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::EnumerationItemContext *> ASNParser::EnumerationContext::enumerationItem() {
  return getRuleContexts<ASNParser::EnumerationItemContext>();
}

ASNParser::EnumerationItemContext* ASNParser::EnumerationContext::enumerationItem(size_t i) {
  return getRuleContext<ASNParser::EnumerationItemContext>(i);
}

std::vector<tree::TerminalNode *> ASNParser::EnumerationContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::EnumerationContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}


size_t ASNParser::EnumerationContext::getRuleIndex() const {
  return ASNParser::RuleEnumeration;
}

antlrcpp::Any ASNParser::EnumerationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitEnumeration(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::EnumerationContext* ASNParser::enumeration() {
  EnumerationContext *_localctx = _tracker.createInstance<EnumerationContext>(_ctx, getState());
  enterRule(_localctx, 242, ASNParser::RuleEnumeration);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(1083);
    enumerationItem();
    setState(1088);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 120, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(1084);
        match(ASNParser::COMMA);
        setState(1085);
        enumerationItem(); 
      }
      setState(1090);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 120, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EnumerationItemContext ------------------------------------------------------------------

ASNParser::EnumerationItemContext::EnumerationItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::EnumerationItemContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

ASNParser::NamedNumberContext* ASNParser::EnumerationItemContext::namedNumber() {
  return getRuleContext<ASNParser::NamedNumberContext>(0);
}

ASNParser::ValueContext* ASNParser::EnumerationItemContext::value() {
  return getRuleContext<ASNParser::ValueContext>(0);
}


size_t ASNParser::EnumerationItemContext::getRuleIndex() const {
  return ASNParser::RuleEnumerationItem;
}

antlrcpp::Any ASNParser::EnumerationItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitEnumerationItem(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::EnumerationItemContext* ASNParser::enumerationItem() {
  EnumerationItemContext *_localctx = _tracker.createInstance<EnumerationItemContext>(_ctx, getState());
  enterRule(_localctx, 244, ASNParser::RuleEnumerationItem);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1094);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 121, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1091);
      match(ASNParser::IDENTIFIER);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1092);
      namedNumber();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1093);
      value();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NamedNumberContext ------------------------------------------------------------------

ASNParser::NamedNumberContext::NamedNumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::NamedNumberContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

tree::TerminalNode* ASNParser::NamedNumberContext::L_PARAN() {
  return getToken(ASNParser::L_PARAN, 0);
}

tree::TerminalNode* ASNParser::NamedNumberContext::R_PARAN() {
  return getToken(ASNParser::R_PARAN, 0);
}

ASNParser::SignedNumberContext* ASNParser::NamedNumberContext::signedNumber() {
  return getRuleContext<ASNParser::SignedNumberContext>(0);
}

ASNParser::DefinedValueContext* ASNParser::NamedNumberContext::definedValue() {
  return getRuleContext<ASNParser::DefinedValueContext>(0);
}


size_t ASNParser::NamedNumberContext::getRuleIndex() const {
  return ASNParser::RuleNamedNumber;
}

antlrcpp::Any ASNParser::NamedNumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitNamedNumber(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::NamedNumberContext* ASNParser::namedNumber() {
  NamedNumberContext *_localctx = _tracker.createInstance<NamedNumberContext>(_ctx, getState());
  enterRule(_localctx, 246, ASNParser::RuleNamedNumber);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1096);
    match(ASNParser::IDENTIFIER);
    setState(1097);
    match(ASNParser::L_PARAN);
    setState(1100);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::MINUS:
      case ASNParser::NUMBER: {
        setState(1098);
        signedNumber();
        break;
      }

      case ASNParser::IDENTIFIER: {
        setState(1099);
        definedValue();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(1102);
    match(ASNParser::R_PARAN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NamedRealNumberContext ------------------------------------------------------------------

ASNParser::NamedRealNumberContext::NamedRealNumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::NamedRealNumberContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

tree::TerminalNode* ASNParser::NamedRealNumberContext::L_PARAN() {
  return getToken(ASNParser::L_PARAN, 0);
}

tree::TerminalNode* ASNParser::NamedRealNumberContext::R_PARAN() {
  return getToken(ASNParser::R_PARAN, 0);
}

ASNParser::RealValueContext* ASNParser::NamedRealNumberContext::realValue() {
  return getRuleContext<ASNParser::RealValueContext>(0);
}

ASNParser::DefinedValueContext* ASNParser::NamedRealNumberContext::definedValue() {
  return getRuleContext<ASNParser::DefinedValueContext>(0);
}


size_t ASNParser::NamedRealNumberContext::getRuleIndex() const {
  return ASNParser::RuleNamedRealNumber;
}

antlrcpp::Any ASNParser::NamedRealNumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitNamedRealNumber(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::NamedRealNumberContext* ASNParser::namedRealNumber() {
  NamedRealNumberContext *_localctx = _tracker.createInstance<NamedRealNumberContext>(_ctx, getState());
  enterRule(_localctx, 248, ASNParser::RuleNamedRealNumber);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1104);
    match(ASNParser::IDENTIFIER);
    setState(1105);
    match(ASNParser::L_PARAN);
    setState(1108);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::MINUS:
      case ASNParser::NUMBER: {
        setState(1106);
        realValue();
        break;
      }

      case ASNParser::IDENTIFIER: {
        setState(1107);
        definedValue();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(1110);
    match(ASNParser::R_PARAN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DefinedValueContext ------------------------------------------------------------------

ASNParser::DefinedValueContext::DefinedValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ParameterizedValueContext* ASNParser::DefinedValueContext::parameterizedValue() {
  return getRuleContext<ASNParser::ParameterizedValueContext>(0);
}


size_t ASNParser::DefinedValueContext::getRuleIndex() const {
  return ASNParser::RuleDefinedValue;
}

antlrcpp::Any ASNParser::DefinedValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitDefinedValue(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::DefinedValueContext* ASNParser::definedValue() {
  DefinedValueContext *_localctx = _tracker.createInstance<DefinedValueContext>(_ctx, getState());
  enterRule(_localctx, 250, ASNParser::RuleDefinedValue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1112);
    parameterizedValue();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterizedValueContext ------------------------------------------------------------------

ASNParser::ParameterizedValueContext::ParameterizedValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::SimpleDefinedValueContext* ASNParser::ParameterizedValueContext::simpleDefinedValue() {
  return getRuleContext<ASNParser::SimpleDefinedValueContext>(0);
}

ASNParser::ActualParameterListContext* ASNParser::ParameterizedValueContext::actualParameterList() {
  return getRuleContext<ASNParser::ActualParameterListContext>(0);
}


size_t ASNParser::ParameterizedValueContext::getRuleIndex() const {
  return ASNParser::RuleParameterizedValue;
}

antlrcpp::Any ASNParser::ParameterizedValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitParameterizedValue(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ParameterizedValueContext* ASNParser::parameterizedValue() {
  ParameterizedValueContext *_localctx = _tracker.createInstance<ParameterizedValueContext>(_ctx, getState());
  enterRule(_localctx, 252, ASNParser::RuleParameterizedValue);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1114);
    simpleDefinedValue();
    setState(1116);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::L_BRACE) {
      setState(1115);
      actualParameterList();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleDefinedValueContext ------------------------------------------------------------------

ASNParser::SimpleDefinedValueContext::SimpleDefinedValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ASNParser::SimpleDefinedValueContext::IDENTIFIER() {
  return getTokens(ASNParser::IDENTIFIER);
}

tree::TerminalNode* ASNParser::SimpleDefinedValueContext::IDENTIFIER(size_t i) {
  return getToken(ASNParser::IDENTIFIER, i);
}

tree::TerminalNode* ASNParser::SimpleDefinedValueContext::DOT() {
  return getToken(ASNParser::DOT, 0);
}


size_t ASNParser::SimpleDefinedValueContext::getRuleIndex() const {
  return ASNParser::RuleSimpleDefinedValue;
}

antlrcpp::Any ASNParser::SimpleDefinedValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitSimpleDefinedValue(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::SimpleDefinedValueContext* ASNParser::simpleDefinedValue() {
  SimpleDefinedValueContext *_localctx = _tracker.createInstance<SimpleDefinedValueContext>(_ctx, getState());
  enterRule(_localctx, 254, ASNParser::RuleSimpleDefinedValue);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1118);
    match(ASNParser::IDENTIFIER);
    setState(1121);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::DOT) {
      setState(1119);
      match(ASNParser::DOT);
      setState(1120);
      match(ASNParser::IDENTIFIER);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ActualParameterListContext ------------------------------------------------------------------

ASNParser::ActualParameterListContext::ActualParameterListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ActualParameterListContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

std::vector<ASNParser::ActualParameterContext *> ASNParser::ActualParameterListContext::actualParameter() {
  return getRuleContexts<ASNParser::ActualParameterContext>();
}

ASNParser::ActualParameterContext* ASNParser::ActualParameterListContext::actualParameter(size_t i) {
  return getRuleContext<ASNParser::ActualParameterContext>(i);
}

tree::TerminalNode* ASNParser::ActualParameterListContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}

std::vector<tree::TerminalNode *> ASNParser::ActualParameterListContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::ActualParameterListContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}


size_t ASNParser::ActualParameterListContext::getRuleIndex() const {
  return ASNParser::RuleActualParameterList;
}

antlrcpp::Any ASNParser::ActualParameterListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitActualParameterList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ActualParameterListContext* ASNParser::actualParameterList() {
  ActualParameterListContext *_localctx = _tracker.createInstance<ActualParameterListContext>(_ctx, getState());
  enterRule(_localctx, 256, ASNParser::RuleActualParameterList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1123);
    match(ASNParser::L_BRACE);
    setState(1124);
    actualParameter();
    setState(1129);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::COMMA) {
      setState(1125);
      match(ASNParser::COMMA);
      setState(1126);
      actualParameter();
      setState(1131);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(1132);
    match(ASNParser::R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ActualParameterContext ------------------------------------------------------------------

ASNParser::ActualParameterContext::ActualParameterContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::AsnTypeContext* ASNParser::ActualParameterContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}

ASNParser::ValueContext* ASNParser::ActualParameterContext::value() {
  return getRuleContext<ASNParser::ValueContext>(0);
}


size_t ASNParser::ActualParameterContext::getRuleIndex() const {
  return ASNParser::RuleActualParameter;
}

antlrcpp::Any ASNParser::ActualParameterContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitActualParameter(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ActualParameterContext* ASNParser::actualParameter() {
  ActualParameterContext *_localctx = _tracker.createInstance<ActualParameterContext>(_ctx, getState());
  enterRule(_localctx, 258, ASNParser::RuleActualParameter);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1136);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 127, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1134);
      asnType();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1135);
      value();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExceptionSpecContext ------------------------------------------------------------------

ASNParser::ExceptionSpecContext::ExceptionSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ExceptionSpecContext::EXCLAM() {
  return getToken(ASNParser::EXCLAM, 0);
}

ASNParser::ExceptionIdentificationContext* ASNParser::ExceptionSpecContext::exceptionIdentification() {
  return getRuleContext<ASNParser::ExceptionIdentificationContext>(0);
}


size_t ASNParser::ExceptionSpecContext::getRuleIndex() const {
  return ASNParser::RuleExceptionSpec;
}

antlrcpp::Any ASNParser::ExceptionSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExceptionSpec(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExceptionSpecContext* ASNParser::exceptionSpec() {
  ExceptionSpecContext *_localctx = _tracker.createInstance<ExceptionSpecContext>(_ctx, getState());
  enterRule(_localctx, 260, ASNParser::RuleExceptionSpec);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1138);
    match(ASNParser::EXCLAM);
    setState(1139);
    exceptionIdentification();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExceptionIdentificationContext ------------------------------------------------------------------

ASNParser::ExceptionIdentificationContext::ExceptionIdentificationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::SignedNumberContext* ASNParser::ExceptionIdentificationContext::signedNumber() {
  return getRuleContext<ASNParser::SignedNumberContext>(0);
}

ASNParser::DefinedValueContext* ASNParser::ExceptionIdentificationContext::definedValue() {
  return getRuleContext<ASNParser::DefinedValueContext>(0);
}

ASNParser::AsnTypeContext* ASNParser::ExceptionIdentificationContext::asnType() {
  return getRuleContext<ASNParser::AsnTypeContext>(0);
}

tree::TerminalNode* ASNParser::ExceptionIdentificationContext::COLON() {
  return getToken(ASNParser::COLON, 0);
}

ASNParser::ValueContext* ASNParser::ExceptionIdentificationContext::value() {
  return getRuleContext<ASNParser::ValueContext>(0);
}


size_t ASNParser::ExceptionIdentificationContext::getRuleIndex() const {
  return ASNParser::RuleExceptionIdentification;
}

antlrcpp::Any ASNParser::ExceptionIdentificationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitExceptionIdentification(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ExceptionIdentificationContext* ASNParser::exceptionIdentification() {
  ExceptionIdentificationContext *_localctx = _tracker.createInstance<ExceptionIdentificationContext>(_ctx, getState());
  enterRule(_localctx, 262, ASNParser::RuleExceptionIdentification);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(1147);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 128, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(1141);
      signedNumber();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(1142);
      definedValue();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(1143);
      asnType();
      setState(1144);
      match(ASNParser::COLON);
      setState(1145);
      value();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AdditionalEnumerationContext ------------------------------------------------------------------

ASNParser::AdditionalEnumerationContext::AdditionalEnumerationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::EnumerationContext* ASNParser::AdditionalEnumerationContext::enumeration() {
  return getRuleContext<ASNParser::EnumerationContext>(0);
}


size_t ASNParser::AdditionalEnumerationContext::getRuleIndex() const {
  return ASNParser::RuleAdditionalEnumeration;
}

antlrcpp::Any ASNParser::AdditionalEnumerationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitAdditionalEnumeration(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::AdditionalEnumerationContext* ASNParser::additionalEnumeration() {
  AdditionalEnumerationContext *_localctx = _tracker.createInstance<AdditionalEnumerationContext>(_ctx, getState());
  enterRule(_localctx, 264, ASNParser::RuleAdditionalEnumeration);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1149);
    enumeration();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntegerTypeContext ------------------------------------------------------------------

ASNParser::IntegerTypeContext::IntegerTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::IntegerTypeContext::INTEGER_LITERAL() {
  return getToken(ASNParser::INTEGER_LITERAL, 0);
}

tree::TerminalNode* ASNParser::IntegerTypeContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

ASNParser::NamedNumberListContext* ASNParser::IntegerTypeContext::namedNumberList() {
  return getRuleContext<ASNParser::NamedNumberListContext>(0);
}

tree::TerminalNode* ASNParser::IntegerTypeContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}


size_t ASNParser::IntegerTypeContext::getRuleIndex() const {
  return ASNParser::RuleIntegerType;
}

antlrcpp::Any ASNParser::IntegerTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitIntegerType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::IntegerTypeContext* ASNParser::integerType() {
  IntegerTypeContext *_localctx = _tracker.createInstance<IntegerTypeContext>(_ctx, getState());
  enterRule(_localctx, 266, ASNParser::RuleIntegerType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1151);
    match(ASNParser::INTEGER_LITERAL);
    setState(1156);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 129, _ctx)) {
    case 1: {
      setState(1152);
      match(ASNParser::L_BRACE);
      setState(1153);
      namedNumberList();
      setState(1154);
      match(ASNParser::R_BRACE);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RealTypeContext ------------------------------------------------------------------

ASNParser::RealTypeContext::RealTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::RealTypeContext::REAL_LITERAL() {
  return getToken(ASNParser::REAL_LITERAL, 0);
}

tree::TerminalNode* ASNParser::RealTypeContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

ASNParser::NamedRealNumberListContext* ASNParser::RealTypeContext::namedRealNumberList() {
  return getRuleContext<ASNParser::NamedRealNumberListContext>(0);
}

tree::TerminalNode* ASNParser::RealTypeContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}


size_t ASNParser::RealTypeContext::getRuleIndex() const {
  return ASNParser::RuleRealType;
}

antlrcpp::Any ASNParser::RealTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitRealType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::RealTypeContext* ASNParser::realType() {
  RealTypeContext *_localctx = _tracker.createInstance<RealTypeContext>(_ctx, getState());
  enterRule(_localctx, 268, ASNParser::RuleRealType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1158);
    match(ASNParser::REAL_LITERAL);
    setState(1163);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 130, _ctx)) {
    case 1: {
      setState(1159);
      match(ASNParser::L_BRACE);
      setState(1160);
      namedRealNumberList();
      setState(1161);
      match(ASNParser::R_BRACE);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BooleanTypeContext ------------------------------------------------------------------

ASNParser::BooleanTypeContext::BooleanTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::BooleanTypeContext::BOOLEAN_LITERAL() {
  return getToken(ASNParser::BOOLEAN_LITERAL, 0);
}


size_t ASNParser::BooleanTypeContext::getRuleIndex() const {
  return ASNParser::RuleBooleanType;
}

antlrcpp::Any ASNParser::BooleanTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitBooleanType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::BooleanTypeContext* ASNParser::booleanType() {
  BooleanTypeContext *_localctx = _tracker.createInstance<BooleanTypeContext>(_ctx, getState());
  enterRule(_localctx, 270, ASNParser::RuleBooleanType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1165);
    match(ASNParser::BOOLEAN_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NamedNumberListContext ------------------------------------------------------------------

ASNParser::NamedNumberListContext::NamedNumberListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::NamedNumberContext *> ASNParser::NamedNumberListContext::namedNumber() {
  return getRuleContexts<ASNParser::NamedNumberContext>();
}

ASNParser::NamedNumberContext* ASNParser::NamedNumberListContext::namedNumber(size_t i) {
  return getRuleContext<ASNParser::NamedNumberContext>(i);
}

std::vector<tree::TerminalNode *> ASNParser::NamedNumberListContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::NamedNumberListContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}


size_t ASNParser::NamedNumberListContext::getRuleIndex() const {
  return ASNParser::RuleNamedNumberList;
}

antlrcpp::Any ASNParser::NamedNumberListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitNamedNumberList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::NamedNumberListContext* ASNParser::namedNumberList() {
  NamedNumberListContext *_localctx = _tracker.createInstance<NamedNumberListContext>(_ctx, getState());
  enterRule(_localctx, 272, ASNParser::RuleNamedNumberList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1167);
    namedNumber();
    setState(1172);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::COMMA) {
      setState(1168);
      match(ASNParser::COMMA);
      setState(1169);
      namedNumber();
      setState(1174);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NamedRealNumberListContext ------------------------------------------------------------------

ASNParser::NamedRealNumberListContext::NamedRealNumberListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::NamedRealNumberContext *> ASNParser::NamedRealNumberListContext::namedRealNumber() {
  return getRuleContexts<ASNParser::NamedRealNumberContext>();
}

ASNParser::NamedRealNumberContext* ASNParser::NamedRealNumberListContext::namedRealNumber(size_t i) {
  return getRuleContext<ASNParser::NamedRealNumberContext>(i);
}

std::vector<tree::TerminalNode *> ASNParser::NamedRealNumberListContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::NamedRealNumberListContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}


size_t ASNParser::NamedRealNumberListContext::getRuleIndex() const {
  return ASNParser::RuleNamedRealNumberList;
}

antlrcpp::Any ASNParser::NamedRealNumberListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitNamedRealNumberList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::NamedRealNumberListContext* ASNParser::namedRealNumberList() {
  NamedRealNumberListContext *_localctx = _tracker.createInstance<NamedRealNumberListContext>(_ctx, getState());
  enterRule(_localctx, 274, ASNParser::RuleNamedRealNumberList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1175);
    namedRealNumber();
    setState(1180);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::COMMA) {
      setState(1176);
      match(ASNParser::COMMA);
      setState(1177);
      namedRealNumber();
      setState(1182);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ObjectidentifiertypeContext ------------------------------------------------------------------

ASNParser::ObjectidentifiertypeContext::ObjectidentifiertypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::ObjectidentifiertypeContext::OBJECT_LITERAL() {
  return getToken(ASNParser::OBJECT_LITERAL, 0);
}

tree::TerminalNode* ASNParser::ObjectidentifiertypeContext::IDENTIFIER_LITERAL() {
  return getToken(ASNParser::IDENTIFIER_LITERAL, 0);
}


size_t ASNParser::ObjectidentifiertypeContext::getRuleIndex() const {
  return ASNParser::RuleObjectidentifiertype;
}

antlrcpp::Any ASNParser::ObjectidentifiertypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitObjectidentifiertype(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ObjectidentifiertypeContext* ASNParser::objectidentifiertype() {
  ObjectidentifiertypeContext *_localctx = _tracker.createInstance<ObjectidentifiertypeContext>(_ctx, getState());
  enterRule(_localctx, 276, ASNParser::RuleObjectidentifiertype);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1183);
    match(ASNParser::OBJECT_LITERAL);
    setState(1184);
    match(ASNParser::IDENTIFIER_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComponentRelationConstraintContext ------------------------------------------------------------------

ASNParser::ComponentRelationConstraintContext::ComponentRelationConstraintContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ASNParser::ComponentRelationConstraintContext::L_BRACE() {
  return getTokens(ASNParser::L_BRACE);
}

tree::TerminalNode* ASNParser::ComponentRelationConstraintContext::L_BRACE(size_t i) {
  return getToken(ASNParser::L_BRACE, i);
}

std::vector<tree::TerminalNode *> ASNParser::ComponentRelationConstraintContext::R_BRACE() {
  return getTokens(ASNParser::R_BRACE);
}

tree::TerminalNode* ASNParser::ComponentRelationConstraintContext::R_BRACE(size_t i) {
  return getToken(ASNParser::R_BRACE, i);
}

std::vector<tree::TerminalNode *> ASNParser::ComponentRelationConstraintContext::IDENTIFIER() {
  return getTokens(ASNParser::IDENTIFIER);
}

tree::TerminalNode* ASNParser::ComponentRelationConstraintContext::IDENTIFIER(size_t i) {
  return getToken(ASNParser::IDENTIFIER, i);
}

std::vector<ASNParser::AtNotationContext *> ASNParser::ComponentRelationConstraintContext::atNotation() {
  return getRuleContexts<ASNParser::AtNotationContext>();
}

ASNParser::AtNotationContext* ASNParser::ComponentRelationConstraintContext::atNotation(size_t i) {
  return getRuleContext<ASNParser::AtNotationContext>(i);
}

tree::TerminalNode* ASNParser::ComponentRelationConstraintContext::DOT() {
  return getToken(ASNParser::DOT, 0);
}

std::vector<tree::TerminalNode *> ASNParser::ComponentRelationConstraintContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::ComponentRelationConstraintContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}


size_t ASNParser::ComponentRelationConstraintContext::getRuleIndex() const {
  return ASNParser::RuleComponentRelationConstraint;
}

antlrcpp::Any ASNParser::ComponentRelationConstraintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitComponentRelationConstraint(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ComponentRelationConstraintContext* ASNParser::componentRelationConstraint() {
  ComponentRelationConstraintContext *_localctx = _tracker.createInstance<ComponentRelationConstraintContext>(_ctx, getState());
  enterRule(_localctx, 278, ASNParser::RuleComponentRelationConstraint);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1186);
    match(ASNParser::L_BRACE);

    setState(1187);
    match(ASNParser::IDENTIFIER);
    setState(1190);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::DOT) {
      setState(1188);
      match(ASNParser::DOT);
      setState(1189);
      match(ASNParser::IDENTIFIER);
    }
    setState(1192);
    match(ASNParser::R_BRACE);
    setState(1204);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::L_BRACE) {
      setState(1193);
      match(ASNParser::L_BRACE);
      setState(1194);
      atNotation();
      setState(1199);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ASNParser::COMMA) {
        setState(1195);
        match(ASNParser::COMMA);
        setState(1196);
        atNotation();
        setState(1201);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(1202);
      match(ASNParser::R_BRACE);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtNotationContext ------------------------------------------------------------------

ASNParser::AtNotationContext::AtNotationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ASNParser::ComponentIdListContext* ASNParser::AtNotationContext::componentIdList() {
  return getRuleContext<ASNParser::ComponentIdListContext>(0);
}

tree::TerminalNode* ASNParser::AtNotationContext::A_ROND() {
  return getToken(ASNParser::A_ROND, 0);
}

tree::TerminalNode* ASNParser::AtNotationContext::A_ROND_DOT() {
  return getToken(ASNParser::A_ROND_DOT, 0);
}

ASNParser::LevelContext* ASNParser::AtNotationContext::level() {
  return getRuleContext<ASNParser::LevelContext>(0);
}


size_t ASNParser::AtNotationContext::getRuleIndex() const {
  return ASNParser::RuleAtNotation;
}

antlrcpp::Any ASNParser::AtNotationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitAtNotation(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::AtNotationContext* ASNParser::atNotation() {
  AtNotationContext *_localctx = _tracker.createInstance<AtNotationContext>(_ctx, getState());
  enterRule(_localctx, 280, ASNParser::RuleAtNotation);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1209);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::A_ROND: {
        setState(1206);
        match(ASNParser::A_ROND);
        break;
      }

      case ASNParser::A_ROND_DOT: {
        setState(1207);
        match(ASNParser::A_ROND_DOT);
        setState(1208);
        level();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(1211);
    componentIdList();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LevelContext ------------------------------------------------------------------

ASNParser::LevelContext::LevelContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::LevelContext::DOT() {
  return getToken(ASNParser::DOT, 0);
}

ASNParser::LevelContext* ASNParser::LevelContext::level() {
  return getRuleContext<ASNParser::LevelContext>(0);
}


size_t ASNParser::LevelContext::getRuleIndex() const {
  return ASNParser::RuleLevel;
}

antlrcpp::Any ASNParser::LevelContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitLevel(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::LevelContext* ASNParser::level() {
  LevelContext *_localctx = _tracker.createInstance<LevelContext>(_ctx, getState());
  enterRule(_localctx, 282, ASNParser::RuleLevel);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1215);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ASNParser::DOT) {
      setState(1213);
      match(ASNParser::DOT);
      setState(1214);
      level();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComponentIdListContext ------------------------------------------------------------------

ASNParser::ComponentIdListContext::ComponentIdListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ASNParser::ComponentIdListContext::IDENTIFIER() {
  return getTokens(ASNParser::IDENTIFIER);
}

tree::TerminalNode* ASNParser::ComponentIdListContext::IDENTIFIER(size_t i) {
  return getToken(ASNParser::IDENTIFIER, i);
}

std::vector<tree::TerminalNode *> ASNParser::ComponentIdListContext::DOT() {
  return getTokens(ASNParser::DOT);
}

tree::TerminalNode* ASNParser::ComponentIdListContext::DOT(size_t i) {
  return getToken(ASNParser::DOT, i);
}


size_t ASNParser::ComponentIdListContext::getRuleIndex() const {
  return ASNParser::RuleComponentIdList;
}

antlrcpp::Any ASNParser::ComponentIdListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitComponentIdList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::ComponentIdListContext* ASNParser::componentIdList() {
  ComponentIdListContext *_localctx = _tracker.createInstance<ComponentIdListContext>(_ctx, getState());
  enterRule(_localctx, 284, ASNParser::RuleComponentIdList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1217);
    match(ASNParser::IDENTIFIER);
    setState(1222);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::DOT) {
      setState(1218);
      match(ASNParser::DOT);
      setState(1219);
      match(ASNParser::IDENTIFIER);
      setState(1224);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OctetStringTypeContext ------------------------------------------------------------------

ASNParser::OctetStringTypeContext::OctetStringTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::OctetStringTypeContext::OCTET_LITERAL() {
  return getToken(ASNParser::OCTET_LITERAL, 0);
}

tree::TerminalNode* ASNParser::OctetStringTypeContext::STRING_LITERAL() {
  return getToken(ASNParser::STRING_LITERAL, 0);
}


size_t ASNParser::OctetStringTypeContext::getRuleIndex() const {
  return ASNParser::RuleOctetStringType;
}

antlrcpp::Any ASNParser::OctetStringTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitOctetStringType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::OctetStringTypeContext* ASNParser::octetStringType() {
  OctetStringTypeContext *_localctx = _tracker.createInstance<OctetStringTypeContext>(_ctx, getState());
  enterRule(_localctx, 286, ASNParser::RuleOctetStringType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1225);
    match(ASNParser::OCTET_LITERAL);
    setState(1226);
    match(ASNParser::STRING_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BitStringTypeContext ------------------------------------------------------------------

ASNParser::BitStringTypeContext::BitStringTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::BitStringTypeContext::BIT_LITERAL() {
  return getToken(ASNParser::BIT_LITERAL, 0);
}

tree::TerminalNode* ASNParser::BitStringTypeContext::STRING_LITERAL() {
  return getToken(ASNParser::STRING_LITERAL, 0);
}

tree::TerminalNode* ASNParser::BitStringTypeContext::L_BRACE() {
  return getToken(ASNParser::L_BRACE, 0);
}

ASNParser::NamedBitListContext* ASNParser::BitStringTypeContext::namedBitList() {
  return getRuleContext<ASNParser::NamedBitListContext>(0);
}

tree::TerminalNode* ASNParser::BitStringTypeContext::R_BRACE() {
  return getToken(ASNParser::R_BRACE, 0);
}


size_t ASNParser::BitStringTypeContext::getRuleIndex() const {
  return ASNParser::RuleBitStringType;
}

antlrcpp::Any ASNParser::BitStringTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitBitStringType(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::BitStringTypeContext* ASNParser::bitStringType() {
  BitStringTypeContext *_localctx = _tracker.createInstance<BitStringTypeContext>(_ctx, getState());
  enterRule(_localctx, 288, ASNParser::RuleBitStringType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1228);
    match(ASNParser::BIT_LITERAL);
    setState(1229);
    match(ASNParser::STRING_LITERAL);
    setState(1235);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 139, _ctx)) {
    case 1: {
      setState(1231);
      match(ASNParser::L_BRACE);
      setState(1232);
      namedBitList();
      setState(1233);
      match(ASNParser::R_BRACE);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NamedBitListContext ------------------------------------------------------------------

ASNParser::NamedBitListContext::NamedBitListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ASNParser::NamedBitContext *> ASNParser::NamedBitListContext::namedBit() {
  return getRuleContexts<ASNParser::NamedBitContext>();
}

ASNParser::NamedBitContext* ASNParser::NamedBitListContext::namedBit(size_t i) {
  return getRuleContext<ASNParser::NamedBitContext>(i);
}

std::vector<tree::TerminalNode *> ASNParser::NamedBitListContext::COMMA() {
  return getTokens(ASNParser::COMMA);
}

tree::TerminalNode* ASNParser::NamedBitListContext::COMMA(size_t i) {
  return getToken(ASNParser::COMMA, i);
}


size_t ASNParser::NamedBitListContext::getRuleIndex() const {
  return ASNParser::RuleNamedBitList;
}

antlrcpp::Any ASNParser::NamedBitListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitNamedBitList(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::NamedBitListContext* ASNParser::namedBitList() {
  NamedBitListContext *_localctx = _tracker.createInstance<NamedBitListContext>(_ctx, getState());
  enterRule(_localctx, 290, ASNParser::RuleNamedBitList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1237);
    namedBit();
    setState(1242);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ASNParser::COMMA) {
      setState(1238);
      match(ASNParser::COMMA);
      setState(1239);
      namedBit();
      setState(1244);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NamedBitContext ------------------------------------------------------------------

ASNParser::NamedBitContext::NamedBitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::NamedBitContext::IDENTIFIER() {
  return getToken(ASNParser::IDENTIFIER, 0);
}

tree::TerminalNode* ASNParser::NamedBitContext::L_PARAN() {
  return getToken(ASNParser::L_PARAN, 0);
}

tree::TerminalNode* ASNParser::NamedBitContext::R_PARAN() {
  return getToken(ASNParser::R_PARAN, 0);
}

tree::TerminalNode* ASNParser::NamedBitContext::NUMBER() {
  return getToken(ASNParser::NUMBER, 0);
}

ASNParser::DefinedValueContext* ASNParser::NamedBitContext::definedValue() {
  return getRuleContext<ASNParser::DefinedValueContext>(0);
}


size_t ASNParser::NamedBitContext::getRuleIndex() const {
  return ASNParser::RuleNamedBit;
}

antlrcpp::Any ASNParser::NamedBitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitNamedBit(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::NamedBitContext* ASNParser::namedBit() {
  NamedBitContext *_localctx = _tracker.createInstance<NamedBitContext>(_ctx, getState());
  enterRule(_localctx, 292, ASNParser::RuleNamedBit);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1245);
    match(ASNParser::IDENTIFIER);
    setState(1246);
    match(ASNParser::L_PARAN);
    setState(1249);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ASNParser::NUMBER: {
        setState(1247);
        match(ASNParser::NUMBER);
        break;
      }

      case ASNParser::IDENTIFIER: {
        setState(1248);
        definedValue();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(1251);
    match(ASNParser::R_PARAN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BooleanValueContext ------------------------------------------------------------------

ASNParser::BooleanValueContext::BooleanValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ASNParser::BooleanValueContext::TRUE_LITERAL() {
  return getToken(ASNParser::TRUE_LITERAL, 0);
}

tree::TerminalNode* ASNParser::BooleanValueContext::FALSE_LITERAL() {
  return getToken(ASNParser::FALSE_LITERAL, 0);
}

tree::TerminalNode* ASNParser::BooleanValueContext::TRUE_SMALL_LITERAL() {
  return getToken(ASNParser::TRUE_SMALL_LITERAL, 0);
}

tree::TerminalNode* ASNParser::BooleanValueContext::FALSE_SMALL_LITERAL() {
  return getToken(ASNParser::FALSE_SMALL_LITERAL, 0);
}


size_t ASNParser::BooleanValueContext::getRuleIndex() const {
  return ASNParser::RuleBooleanValue;
}

antlrcpp::Any ASNParser::BooleanValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ASNVisitor*>(visitor))
    return parserVisitor->visitBooleanValue(this);
  else
    return visitor->visitChildren(this);
}

ASNParser::BooleanValueContext* ASNParser::booleanValue() {
  BooleanValueContext *_localctx = _tracker.createInstance<BooleanValueContext>(_ctx, getState());
  enterRule(_localctx, 294, ASNParser::RuleBooleanValue);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(1253);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ASNParser::TRUE_LITERAL)
      | (1ULL << ASNParser::FALSE_LITERAL)
      | (1ULL << ASNParser::TRUE_SMALL_LITERAL)
      | (1ULL << ASNParser::FALSE_SMALL_LITERAL))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> ASNParser::_decisionToDFA;
atn::PredictionContextCache ASNParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN ASNParser::_atn;
std::vector<uint16_t> ASNParser::_serializedATN;

std::vector<std::string> ASNParser::_ruleNames = {
  "moduleDefinition", "tagDefault", "extensionDefault", "moduleBody", "exports", 
  "symbolsExported", "imports", "symbolsImported", "symbolsFromModuleList", 
  "symbolsFromModule", "globalModuleReference", "assignedIdentifier", "symbolList", 
  "symbol", "assignmentList", "assignment", "sequenceType", "extensionAndException", 
  "optionalExtensionMarker", "componentTypeLists", "rootComponentTypeList", 
  "componentTypeList", "componentType", "extensionAdditions", "extensionAdditionList", 
  "extensionAddition", "extensionAdditionGroup", "versionNumber", "sequenceOfType", 
  "sizeConstraint", "parameterizedAssignment", "parameterList", "parameter", 
  "paramGovernor", "governor", "objectClassAssignment", "objectClass", "definedObjectClass", 
  "usefulObjectClassReference", "externalObjectClassReference", "objectClassDefn", 
  "withSyntaxSpec", "syntaxList", "tokenOrGroupSpec", "optionalGroup", "requiredToken", 
  "literal", "primitiveFieldName", "fieldSpec", "typeFieldSpec", "typeOptionalitySpec", 
  "fixedTypeValueFieldSpec", "valueOptionalitySpec", "variableTypeValueFieldSpec", 
  "fixedTypeValueSetFieldSpec", "valueSetOptionalitySpec", "object", "parameterizedObject", 
  "definedObject", "objectSet", "objectSetSpec", "fieldName", "valueSet", 
  "elementSetSpecs", "rootElementSetSpec", "additionalElementSetSpec", "elementSetSpec", 
  "unions", "exclusions", "intersections", "unionMark", "intersectionMark", 
  "elements", "objectSetElements", "intersectionElements", "subtypeElements", 
  "variableTypeValueSetFieldSpec", "objectFieldSpec", "objectOptionalitySpec", 
  "objectSetFieldSpec", "objectSetOptionalitySpec", "typeAssignment", "valueAssignment", 
  "asnType", "builtinType", "objectClassFieldType", "setType", "setOfType", 
  "referencedType", "definedType", "constraint", "constraintSpec", "userDefinedConstraint", 
  "generalConstraint", "userDefinedConstraintParameter", "tableConstraint", 
  "simpleTableConstraint", "contentsConstraint", "subtypeConstraint", "value", 
  "builtinValue", "objectIdentifierValue", "objIdComponentsList", "objIdComponents", 
  "integerValue", "realValue", "choiceValue", "enumeratedValue", "signedNumber", 
  "choiceType", "alternativeTypeLists", "extensionAdditionAlternatives", 
  "extensionAdditionAlternativesList", "extensionAdditionAlternative", "extensionAdditionAlternativesGroup", 
  "rootAlternativeTypeList", "alternativeTypeList", "namedType", "enumeratedType", 
  "enumerations", "rootEnumeration", "enumeration", "enumerationItem", "namedNumber", 
  "namedRealNumber", "definedValue", "parameterizedValue", "simpleDefinedValue", 
  "actualParameterList", "actualParameter", "exceptionSpec", "exceptionIdentification", 
  "additionalEnumeration", "integerType", "realType", "booleanType", "namedNumberList", 
  "namedRealNumberList", "objectidentifiertype", "componentRelationConstraint", 
  "atNotation", "level", "componentIdList", "octetStringType", "bitStringType", 
  "namedBitList", "namedBit", "booleanValue"
};

std::vector<std::string> ASNParser::_literalNames = {
  "", "'@'", "'*'", "'::='", "'BOOLEAN'", "'TRUE'", "'FALSE'", "'.'", "'..'", 
  "'...'", "'''", "'&'", "'<'", "'>'", "'</'", "'/>'", "'true'", "'false'", 
  "'INTEGER'", "'{'", "'}'", "','", "'('", "')'", "'-'", "'ENUMERATED'", 
  "'REAL'", "'PLUS-INFINITY'", "'MINUS-INFINITY'", "'BIT'", "'STRING'", 
  "'CONTAINING'", "'OCTET'", "'NULL'", "'SEQUENCE'", "'OPTIONAL'", "'DEFAULT'", 
  "'COMPONENTS'", "'OF'", "'SET'", "'!'", "'ALL'", "'EXCEPT'", "'^'", "'|'", 
  "'UNION'", "'INTERSECTION'", "'INCLUDES'", "'MIN'", "'MAX'", "'SIZE'", 
  "'FROM'", "'WITH'", "'COMPONENT'", "'PRESENT'", "'ABSENT'", "'PATTERN'", 
  "'TYPE-Identifier'", "'ABSTRACT-SYNTAX'", "'CLASS'", "'UNIQUE'", "'SYNTAX'", 
  "'['", "']'", "'INSTANCE'", "';'", "'IMPORTS'", "'EXPORTS'", "'EXTENSIBILITY'", 
  "'IMPLIED'", "'EXPLICIT'", "'TAGS'", "'IMPLICIT'", "'AUTOMATIC'", "'DEFINITIONS'", 
  "'BEGIN'", "'END'", "'[['", "']]'", "':'", "'CHOICE'", "'UNIVERSAL'", 
  "'APPLICATION'", "'PRIVATE'", "'EMBEDDED'", "'PDV'", "'EXTERNAL'", "'OBJECT'", 
  "'IDENTIFIER'", "'RELATIVE-OID'", "'CHARACTER'", "'CONSTRAINED'", "'BY'", 
  "'@.'", "'ENCODED'", "'--'"
};

std::vector<std::string> ASNParser::_symbolicNames = {
  "", "A_ROND", "STAR", "ASSIGN_OP", "BOOLEAN_LITERAL", "TRUE_LITERAL", 
  "FALSE_LITERAL", "DOT", "DOUBLE_DOT", "ELLIPSIS", "APOSTROPHE", "AMPERSAND", 
  "LESS_THAN", "GREATER_THAN", "LESS_THAN_SLASH", "SLASH_GREATER_THAN", 
  "TRUE_SMALL_LITERAL", "FALSE_SMALL_LITERAL", "INTEGER_LITERAL", "L_BRACE", 
  "R_BRACE", "COMMA", "L_PARAN", "R_PARAN", "MINUS", "ENUMERATED_LITERAL", 
  "REAL_LITERAL", "PLUS_INFINITY_LITERAL", "MINUS_INFINITY_LITERAL", "BIT_LITERAL", 
  "STRING_LITERAL", "CONTAINING_LITERAL", "OCTET_LITERAL", "NULL_LITERAL", 
  "SEQUENCE_LITERAL", "OPTIONAL_LITERAL", "DEFAULT_LITERAL", "COMPONENTS_LITERAL", 
  "OF_LITERAL", "SET_LITERAL", "EXCLAM", "ALL_LITERAL", "EXCEPT_LITERAL", 
  "POWER", "PIPE", "UNION_LITERAL", "INTERSECTION_LITERAL", "INCLUDES_LITERAL", 
  "MIN_LITERAL", "MAX_LITERAL", "SIZE_LITERAL", "FROM_LITERAL", "WITH_LITERAL", 
  "COMPONENT_LITERAL", "PRESENT_LITERAL", "ABSENT_LITERAL", "PATTERN_LITERAL", 
  "TYPE_IDENTIFIER_LITERAL", "ABSTRACT_SYNTAX_LITERAL", "CLASS_LITERAL", 
  "UNIQUE_LITERAL", "SYNTAX_LITERAL", "L_BRACKET", "R_BRACKET", "INSTANCE_LITERAL", 
  "SEMI_COLON", "IMPORTS_LITERAL", "EXPORTS_LITERAL", "EXTENSIBILITY_LITERAL", 
  "IMPLIED_LITERAL", "EXPLICIT_LITERAL", "TAGS_LITERAL", "IMPLICIT_LITERAL", 
  "AUTOMATIC_LITERAL", "DEFINITIONS_LITERAL", "BEGIN_LITERAL", "END_LITERAL", 
  "DOUBLE_L_BRACKET", "DOUBLE_R_BRACKET", "COLON", "CHOICE_LITERAL", "UNIVERSAL_LITERAL", 
  "APPLICATION_LITERAL", "PRIVATE_LITERAL", "EMBEDDED_LITERAL", "PDV_LITERAL", 
  "EXTERNAL_LITERAL", "OBJECT_LITERAL", "IDENTIFIER_LITERAL", "RELATIVE_OID_LITERAL", 
  "CHARACTER_LITERAL", "CONSTRAINED_LITERAL", "BY_LITERAL", "A_ROND_DOT", 
  "ENCODED_LITERAL", "COMMENT", "UNRESTRICTEDCHARACTERSTRINGTYPE", "EXTENSTIONENDMARKER", 
  "NUMBER", "WS", "LINE_COMMENT", "BSTRING", "HSTRING", "CSTRING", "IDENTIFIER", 
  "Exponent"
};

dfa::Vocabulary ASNParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> ASNParser::_tokenNames;

ASNParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x6b, 0x4ea, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 
    0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15, 
    0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 0x18, 0x9, 
    0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 
    0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4, 
    0x1f, 0x9, 0x1f, 0x4, 0x20, 0x9, 0x20, 0x4, 0x21, 0x9, 0x21, 0x4, 0x22, 
    0x9, 0x22, 0x4, 0x23, 0x9, 0x23, 0x4, 0x24, 0x9, 0x24, 0x4, 0x25, 0x9, 
    0x25, 0x4, 0x26, 0x9, 0x26, 0x4, 0x27, 0x9, 0x27, 0x4, 0x28, 0x9, 0x28, 
    0x4, 0x29, 0x9, 0x29, 0x4, 0x2a, 0x9, 0x2a, 0x4, 0x2b, 0x9, 0x2b, 0x4, 
    0x2c, 0x9, 0x2c, 0x4, 0x2d, 0x9, 0x2d, 0x4, 0x2e, 0x9, 0x2e, 0x4, 0x2f, 
    0x9, 0x2f, 0x4, 0x30, 0x9, 0x30, 0x4, 0x31, 0x9, 0x31, 0x4, 0x32, 0x9, 
    0x32, 0x4, 0x33, 0x9, 0x33, 0x4, 0x34, 0x9, 0x34, 0x4, 0x35, 0x9, 0x35, 
    0x4, 0x36, 0x9, 0x36, 0x4, 0x37, 0x9, 0x37, 0x4, 0x38, 0x9, 0x38, 0x4, 
    0x39, 0x9, 0x39, 0x4, 0x3a, 0x9, 0x3a, 0x4, 0x3b, 0x9, 0x3b, 0x4, 0x3c, 
    0x9, 0x3c, 0x4, 0x3d, 0x9, 0x3d, 0x4, 0x3e, 0x9, 0x3e, 0x4, 0x3f, 0x9, 
    0x3f, 0x4, 0x40, 0x9, 0x40, 0x4, 0x41, 0x9, 0x41, 0x4, 0x42, 0x9, 0x42, 
    0x4, 0x43, 0x9, 0x43, 0x4, 0x44, 0x9, 0x44, 0x4, 0x45, 0x9, 0x45, 0x4, 
    0x46, 0x9, 0x46, 0x4, 0x47, 0x9, 0x47, 0x4, 0x48, 0x9, 0x48, 0x4, 0x49, 
    0x9, 0x49, 0x4, 0x4a, 0x9, 0x4a, 0x4, 0x4b, 0x9, 0x4b, 0x4, 0x4c, 0x9, 
    0x4c, 0x4, 0x4d, 0x9, 0x4d, 0x4, 0x4e, 0x9, 0x4e, 0x4, 0x4f, 0x9, 0x4f, 
    0x4, 0x50, 0x9, 0x50, 0x4, 0x51, 0x9, 0x51, 0x4, 0x52, 0x9, 0x52, 0x4, 
    0x53, 0x9, 0x53, 0x4, 0x54, 0x9, 0x54, 0x4, 0x55, 0x9, 0x55, 0x4, 0x56, 
    0x9, 0x56, 0x4, 0x57, 0x9, 0x57, 0x4, 0x58, 0x9, 0x58, 0x4, 0x59, 0x9, 
    0x59, 0x4, 0x5a, 0x9, 0x5a, 0x4, 0x5b, 0x9, 0x5b, 0x4, 0x5c, 0x9, 0x5c, 
    0x4, 0x5d, 0x9, 0x5d, 0x4, 0x5e, 0x9, 0x5e, 0x4, 0x5f, 0x9, 0x5f, 0x4, 
    0x60, 0x9, 0x60, 0x4, 0x61, 0x9, 0x61, 0x4, 0x62, 0x9, 0x62, 0x4, 0x63, 
    0x9, 0x63, 0x4, 0x64, 0x9, 0x64, 0x4, 0x65, 0x9, 0x65, 0x4, 0x66, 0x9, 
    0x66, 0x4, 0x67, 0x9, 0x67, 0x4, 0x68, 0x9, 0x68, 0x4, 0x69, 0x9, 0x69, 
    0x4, 0x6a, 0x9, 0x6a, 0x4, 0x6b, 0x9, 0x6b, 0x4, 0x6c, 0x9, 0x6c, 0x4, 
    0x6d, 0x9, 0x6d, 0x4, 0x6e, 0x9, 0x6e, 0x4, 0x6f, 0x9, 0x6f, 0x4, 0x70, 
    0x9, 0x70, 0x4, 0x71, 0x9, 0x71, 0x4, 0x72, 0x9, 0x72, 0x4, 0x73, 0x9, 
    0x73, 0x4, 0x74, 0x9, 0x74, 0x4, 0x75, 0x9, 0x75, 0x4, 0x76, 0x9, 0x76, 
    0x4, 0x77, 0x9, 0x77, 0x4, 0x78, 0x9, 0x78, 0x4, 0x79, 0x9, 0x79, 0x4, 
    0x7a, 0x9, 0x7a, 0x4, 0x7b, 0x9, 0x7b, 0x4, 0x7c, 0x9, 0x7c, 0x4, 0x7d, 
    0x9, 0x7d, 0x4, 0x7e, 0x9, 0x7e, 0x4, 0x7f, 0x9, 0x7f, 0x4, 0x80, 0x9, 
    0x80, 0x4, 0x81, 0x9, 0x81, 0x4, 0x82, 0x9, 0x82, 0x4, 0x83, 0x9, 0x83, 
    0x4, 0x84, 0x9, 0x84, 0x4, 0x85, 0x9, 0x85, 0x4, 0x86, 0x9, 0x86, 0x4, 
    0x87, 0x9, 0x87, 0x4, 0x88, 0x9, 0x88, 0x4, 0x89, 0x9, 0x89, 0x4, 0x8a, 
    0x9, 0x8a, 0x4, 0x8b, 0x9, 0x8b, 0x4, 0x8c, 0x9, 0x8c, 0x4, 0x8d, 0x9, 
    0x8d, 0x4, 0x8e, 0x9, 0x8e, 0x4, 0x8f, 0x9, 0x8f, 0x4, 0x90, 0x9, 0x90, 
    0x4, 0x91, 0x9, 0x91, 0x4, 0x92, 0x9, 0x92, 0x4, 0x93, 0x9, 0x93, 0x4, 
    0x94, 0x9, 0x94, 0x4, 0x95, 0x9, 0x95, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 
    0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0x131, 0xa, 0x2, 0xc, 0x2, 0xe, 
    0x2, 0x134, 0xb, 0x2, 0x3, 0x2, 0x5, 0x2, 0x137, 0xa, 0x2, 0x3, 0x2, 
    0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 
    0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 0x143, 0xa, 0x3, 0x3, 0x4, 0x3, 0x4, 0x5, 
    0x4, 0x147, 0xa, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 
    0x14d, 0xa, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x5, 0x6, 0x156, 0xa, 0x6, 0x3, 0x7, 0x5, 0x7, 0x159, 
    0xa, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 0x15f, 0xa, 
    0x8, 0x3, 0x9, 0x5, 0x9, 0x162, 0xa, 0x9, 0x3, 0xa, 0x3, 0xa, 0x7, 0xa, 
    0x166, 0xa, 0xa, 0xc, 0xa, 0xe, 0xa, 0x169, 0xb, 0xa, 0x3, 0xb, 0x3, 
    0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x7, 0xe, 0x177, 0xa, 0xe, 0xc, 0xe, 
    0xe, 0xe, 0x17a, 0xb, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0x17f, 
    0xa, 0xf, 0x3, 0x10, 0x3, 0x10, 0x7, 0x10, 0x183, 0xa, 0x10, 0xc, 0x10, 
    0xe, 0x10, 0x186, 0xb, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 
    0x3, 0x11, 0x5, 0x11, 0x18d, 0xa, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0x195, 0xa, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0x19b, 0xa, 0x13, 0x3, 0x14, 
    0x3, 0x14, 0x5, 0x14, 0x19f, 0xa, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1a9, 
    0xa, 0x15, 0x5, 0x15, 0x1ab, 0xa, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x1b3, 0xa, 0x15, 0x5, 0x15, 
    0x1b5, 0xa, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 
    0x7, 0x17, 0x1bc, 0xa, 0x17, 0xc, 0x17, 0xe, 0x17, 0x1bf, 0xb, 0x17, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0x1c5, 0xa, 0x18, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0x1ca, 0xa, 0x18, 0x3, 0x19, 
    0x3, 0x19, 0x5, 0x19, 0x1ce, 0xa, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1a, 
    0x7, 0x1a, 0x1d3, 0xa, 0x1a, 0xc, 0x1a, 0xe, 0x1a, 0x1d6, 0xb, 0x1a, 
    0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0x1da, 0xa, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 
    0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x5, 0x1d, 0x1e3, 
    0xa, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x1e9, 
    0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x1ed, 0xa, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 0x1f2, 0xa, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 
    0x3, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x5, 
    0x20, 0x1fc, 0xa, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 
    0x3, 0x20, 0x5, 0x20, 0x203, 0xa, 0x20, 0x5, 0x20, 0x205, 0xa, 0x20, 
    0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x7, 0x21, 0x20b, 0xa, 0x21, 
    0xc, 0x21, 0xe, 0x21, 0x20e, 0xb, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x22, 
    0x3, 0x22, 0x3, 0x22, 0x5, 0x22, 0x215, 0xa, 0x22, 0x3, 0x22, 0x3, 0x22, 
    0x3, 0x23, 0x3, 0x23, 0x5, 0x23, 0x21b, 0xa, 0x23, 0x3, 0x24, 0x3, 0x24, 
    0x5, 0x24, 0x21f, 0xa, 0x24, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x26, 
    0x3, 0x26, 0x5, 0x26, 0x226, 0xa, 0x26, 0x3, 0x27, 0x3, 0x27, 0x5, 0x27, 
    0x22a, 0xa, 0x27, 0x3, 0x27, 0x3, 0x27, 0x3, 0x27, 0x5, 0x27, 0x22f, 
    0xa, 0x27, 0x3, 0x28, 0x3, 0x28, 0x3, 0x29, 0x3, 0x29, 0x3, 0x29, 0x3, 
    0x29, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x7, 0x2a, 
    0x23c, 0xa, 0x2a, 0xc, 0x2a, 0xe, 0x2a, 0x23f, 0xb, 0x2a, 0x3, 0x2a, 
    0x3, 0x2a, 0x5, 0x2a, 0x243, 0xa, 0x2a, 0x3, 0x2b, 0x3, 0x2b, 0x3, 0x2b, 
    0x3, 0x2b, 0x3, 0x2c, 0x3, 0x2c, 0x6, 0x2c, 0x24b, 0xa, 0x2c, 0xd, 0x2c, 
    0xe, 0x2c, 0x24c, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 0x5, 0x2d, 
    0x253, 0xa, 0x2d, 0x3, 0x2e, 0x3, 0x2e, 0x6, 0x2e, 0x257, 0xa, 0x2e, 
    0xd, 0x2e, 0xe, 0x2e, 0x258, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 
    0x5, 0x2f, 0x25f, 0xa, 0x2f, 0x3, 0x30, 0x3, 0x30, 0x3, 0x31, 0x3, 0x31, 
    0x3, 0x31, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x5, 0x32, 0x269, 0xa, 0x32, 
    0x3, 0x32, 0x3, 0x32, 0x5, 0x32, 0x26d, 0xa, 0x32, 0x3, 0x32, 0x5, 0x32, 
    0x270, 0xa, 0x32, 0x3, 0x32, 0x5, 0x32, 0x273, 0xa, 0x32, 0x5, 0x32, 
    0x275, 0xa, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 
    0x5, 0x32, 0x27c, 0xa, 0x32, 0x5, 0x32, 0x27e, 0xa, 0x32, 0x3, 0x32, 
    0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x5, 0x32, 0x285, 0xa, 0x32, 
    0x5, 0x32, 0x287, 0xa, 0x32, 0x5, 0x32, 0x289, 0xa, 0x32, 0x3, 0x33, 
    0x3, 0x33, 0x3, 0x33, 0x5, 0x33, 0x28e, 0xa, 0x33, 0x3, 0x34, 0x3, 0x34, 
    0x3, 0x34, 0x5, 0x34, 0x293, 0xa, 0x34, 0x3, 0x35, 0x3, 0x35, 0x3, 0x35, 
    0x3, 0x35, 0x5, 0x35, 0x299, 0xa, 0x35, 0x3, 0x35, 0x5, 0x35, 0x29c, 
    0xa, 0x35, 0x3, 0x36, 0x3, 0x36, 0x3, 0x36, 0x5, 0x36, 0x2a1, 0xa, 0x36, 
    0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 0x5, 0x37, 0x2a7, 0xa, 0x37, 
    0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x5, 0x38, 0x2ad, 0xa, 0x38, 
    0x3, 0x39, 0x3, 0x39, 0x3, 0x39, 0x5, 0x39, 0x2b2, 0xa, 0x39, 0x3, 0x3a, 
    0x3, 0x3a, 0x5, 0x3a, 0x2b6, 0xa, 0x3a, 0x3, 0x3b, 0x3, 0x3b, 0x3, 0x3b, 
    0x3, 0x3c, 0x3, 0x3c, 0x5, 0x3c, 0x2bd, 0xa, 0x3c, 0x3, 0x3d, 0x3, 0x3d, 
    0x3, 0x3d, 0x3, 0x3d, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x3, 
    0x3e, 0x5, 0x3e, 0x2c8, 0xa, 0x3e, 0x5, 0x3e, 0x2ca, 0xa, 0x3e, 0x3, 
    0x3e, 0x3, 0x3e, 0x3, 0x3e, 0x5, 0x3e, 0x2cf, 0xa, 0x3e, 0x5, 0x3e, 
    0x2d1, 0xa, 0x3e, 0x3, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 0x3, 0x3f, 
    0x3, 0x3f, 0x7, 0x3f, 0x2d9, 0xa, 0x3f, 0xc, 0x3f, 0xe, 0x3f, 0x2dc, 
    0xb, 0x3f, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x40, 0x3, 0x41, 0x3, 
    0x41, 0x3, 0x41, 0x3, 0x41, 0x3, 0x41, 0x5, 0x41, 0x2e7, 0xa, 0x41, 
    0x5, 0x41, 0x2e9, 0xa, 0x41, 0x3, 0x42, 0x3, 0x42, 0x3, 0x43, 0x3, 0x43, 
    0x3, 0x44, 0x3, 0x44, 0x3, 0x44, 0x5, 0x44, 0x2f2, 0xa, 0x44, 0x3, 0x45, 
    0x3, 0x45, 0x3, 0x45, 0x3, 0x45, 0x7, 0x45, 0x2f8, 0xa, 0x45, 0xc, 0x45, 
    0xe, 0x45, 0x2fb, 0xb, 0x45, 0x3, 0x46, 0x3, 0x46, 0x3, 0x46, 0x3, 0x47, 
    0x3, 0x47, 0x3, 0x47, 0x3, 0x47, 0x7, 0x47, 0x304, 0xa, 0x47, 0xc, 0x47, 
    0xe, 0x47, 0x307, 0xb, 0x47, 0x3, 0x48, 0x3, 0x48, 0x3, 0x49, 0x3, 0x49, 
    0x3, 0x4a, 0x3, 0x4a, 0x3, 0x4b, 0x3, 0x4b, 0x5, 0x4b, 0x311, 0xa, 0x4b, 
    0x3, 0x4c, 0x3, 0x4c, 0x5, 0x4c, 0x315, 0xa, 0x4c, 0x3, 0x4d, 0x3, 0x4d, 
    0x5, 0x4d, 0x319, 0xa, 0x4d, 0x3, 0x4d, 0x5, 0x4d, 0x31c, 0xa, 0x4d, 
    0x3, 0x4d, 0x3, 0x4d, 0x5, 0x4d, 0x320, 0xa, 0x4d, 0x3, 0x4d, 0x3, 0x4d, 
    0x5, 0x4d, 0x324, 0xa, 0x4d, 0x3, 0x4d, 0x3, 0x4d, 0x3, 0x4d, 0x3, 0x4d, 
    0x5, 0x4d, 0x32a, 0xa, 0x4d, 0x3, 0x4e, 0x3, 0x4e, 0x3, 0x4e, 0x3, 0x4e, 
    0x5, 0x4e, 0x330, 0xa, 0x4e, 0x3, 0x4f, 0x3, 0x4f, 0x3, 0x4f, 0x3, 0x4f, 
    0x5, 0x4f, 0x336, 0xa, 0x4f, 0x3, 0x50, 0x3, 0x50, 0x3, 0x50, 0x5, 0x50, 
    0x33b, 0xa, 0x50, 0x3, 0x51, 0x3, 0x51, 0x3, 0x51, 0x3, 0x51, 0x5, 0x51, 
    0x341, 0xa, 0x51, 0x3, 0x52, 0x3, 0x52, 0x3, 0x52, 0x5, 0x52, 0x346, 
    0xa, 0x52, 0x3, 0x53, 0x3, 0x53, 0x3, 0x53, 0x3, 0x54, 0x3, 0x54, 0x3, 
    0x54, 0x3, 0x54, 0x3, 0x55, 0x3, 0x55, 0x5, 0x55, 0x351, 0xa, 0x55, 
    0x3, 0x55, 0x7, 0x55, 0x354, 0xa, 0x55, 0xc, 0x55, 0xe, 0x55, 0x357, 
    0xb, 0x55, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 
    0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 0x3, 0x56, 
    0x3, 0x56, 0x5, 0x56, 0x366, 0xa, 0x56, 0x3, 0x57, 0x3, 0x57, 0x3, 0x57, 
    0x3, 0x57, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 
    0x58, 0x5, 0x58, 0x372, 0xa, 0x58, 0x3, 0x58, 0x3, 0x58, 0x3, 0x59, 
    0x3, 0x59, 0x3, 0x59, 0x5, 0x59, 0x379, 0xa, 0x59, 0x3, 0x59, 0x3, 0x59, 
    0x3, 0x59, 0x5, 0x59, 0x37e, 0xa, 0x59, 0x3, 0x5a, 0x3, 0x5a, 0x3, 0x5b, 
    0x3, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 0x385, 0xa, 0x5b, 0x3, 0x5b, 0x5, 0x5b, 
    0x388, 0xa, 0x5b, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x5, 0x5c, 0x38d, 
    0xa, 0x5c, 0x3, 0x5c, 0x3, 0x5c, 0x3, 0x5d, 0x3, 0x5d, 0x5, 0x5d, 0x393, 
    0xa, 0x5d, 0x3, 0x5e, 0x3, 0x5e, 0x3, 0x5e, 0x3, 0x5e, 0x3, 0x5e, 0x3, 
    0x5e, 0x7, 0x5e, 0x39b, 0xa, 0x5e, 0xc, 0x5e, 0xe, 0x5e, 0x39e, 0xb, 
    0x5e, 0x3, 0x5e, 0x3, 0x5e, 0x3, 0x5f, 0x3, 0x5f, 0x3, 0x5f, 0x5, 0x5f, 
    0x3a5, 0xa, 0x5f, 0x3, 0x60, 0x3, 0x60, 0x3, 0x60, 0x3, 0x60, 0x3, 0x60, 
    0x3, 0x60, 0x5, 0x60, 0x3ad, 0xa, 0x60, 0x3, 0x61, 0x3, 0x61, 0x3, 0x62, 
    0x3, 0x62, 0x3, 0x63, 0x3, 0x63, 0x3, 0x63, 0x3, 0x63, 0x3, 0x63, 0x3, 
    0x63, 0x3, 0x63, 0x3, 0x63, 0x3, 0x63, 0x3, 0x63, 0x3, 0x63, 0x5, 0x63, 
    0x3be, 0xa, 0x63, 0x3, 0x64, 0x3, 0x64, 0x3, 0x65, 0x3, 0x65, 0x3, 0x66, 
    0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x3, 0x66, 0x5, 
    0x66, 0x3cb, 0xa, 0x66, 0x3, 0x67, 0x3, 0x67, 0x3, 0x67, 0x3, 0x67, 
    0x3, 0x68, 0x3, 0x68, 0x7, 0x68, 0x3d3, 0xa, 0x68, 0xc, 0x68, 0xe, 0x68, 
    0x3d6, 0xb, 0x68, 0x3, 0x69, 0x3, 0x69, 0x3, 0x69, 0x3, 0x69, 0x3, 0x69, 
    0x5, 0x69, 0x3dd, 0xa, 0x69, 0x3, 0x69, 0x5, 0x69, 0x3e0, 0xa, 0x69, 
    0x3, 0x69, 0x5, 0x69, 0x3e3, 0xa, 0x69, 0x3, 0x6a, 0x3, 0x6a, 0x5, 0x6a, 
    0x3e7, 0xa, 0x6a, 0x3, 0x6b, 0x3, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x3ec, 
    0xa, 0x6b, 0x3, 0x6b, 0x5, 0x6b, 0x3ef, 0xa, 0x6b, 0x3, 0x6c, 0x3, 0x6c, 
    0x3, 0x6c, 0x3, 0x6c, 0x3, 0x6d, 0x3, 0x6d, 0x3, 0x6e, 0x5, 0x6e, 0x3f8, 
    0xa, 0x6e, 0x3, 0x6e, 0x3, 0x6e, 0x3, 0x6f, 0x3, 0x6f, 0x3, 0x6f, 0x3, 
    0x6f, 0x3, 0x6f, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 0x3, 0x70, 
    0x3, 0x70, 0x5, 0x70, 0x407, 0xa, 0x70, 0x3, 0x71, 0x3, 0x71, 0x5, 0x71, 
    0x40b, 0xa, 0x71, 0x3, 0x72, 0x3, 0x72, 0x3, 0x72, 0x7, 0x72, 0x410, 
    0xa, 0x72, 0xc, 0x72, 0xe, 0x72, 0x413, 0xb, 0x72, 0x3, 0x73, 0x3, 0x73, 
    0x5, 0x73, 0x417, 0xa, 0x73, 0x3, 0x74, 0x3, 0x74, 0x3, 0x74, 0x3, 0x74, 
    0x3, 0x74, 0x3, 0x75, 0x3, 0x75, 0x3, 0x76, 0x3, 0x76, 0x3, 0x76, 0x7, 
    0x76, 0x423, 0xa, 0x76, 0xc, 0x76, 0xe, 0x76, 0x426, 0xb, 0x76, 0x3, 
    0x77, 0x3, 0x77, 0x3, 0x77, 0x3, 0x78, 0x3, 0x78, 0x3, 0x78, 0x3, 0x78, 
    0x3, 0x78, 0x3, 0x79, 0x3, 0x79, 0x3, 0x79, 0x3, 0x79, 0x5, 0x79, 0x434, 
    0xa, 0x79, 0x3, 0x79, 0x3, 0x79, 0x5, 0x79, 0x438, 0xa, 0x79, 0x5, 0x79, 
    0x43a, 0xa, 0x79, 0x3, 0x7a, 0x3, 0x7a, 0x3, 0x7b, 0x3, 0x7b, 0x3, 0x7b, 
    0x7, 0x7b, 0x441, 0xa, 0x7b, 0xc, 0x7b, 0xe, 0x7b, 0x444, 0xb, 0x7b, 
    0x3, 0x7c, 0x3, 0x7c, 0x3, 0x7c, 0x5, 0x7c, 0x449, 0xa, 0x7c, 0x3, 0x7d, 
    0x3, 0x7d, 0x3, 0x7d, 0x3, 0x7d, 0x5, 0x7d, 0x44f, 0xa, 0x7d, 0x3, 0x7d, 
    0x3, 0x7d, 0x3, 0x7e, 0x3, 0x7e, 0x3, 0x7e, 0x3, 0x7e, 0x5, 0x7e, 0x457, 
    0xa, 0x7e, 0x3, 0x7e, 0x3, 0x7e, 0x3, 0x7f, 0x3, 0x7f, 0x3, 0x80, 0x3, 
    0x80, 0x5, 0x80, 0x45f, 0xa, 0x80, 0x3, 0x81, 0x3, 0x81, 0x3, 0x81, 
    0x5, 0x81, 0x464, 0xa, 0x81, 0x3, 0x82, 0x3, 0x82, 0x3, 0x82, 0x3, 0x82, 
    0x7, 0x82, 0x46a, 0xa, 0x82, 0xc, 0x82, 0xe, 0x82, 0x46d, 0xb, 0x82, 
    0x3, 0x82, 0x3, 0x82, 0x3, 0x83, 0x3, 0x83, 0x5, 0x83, 0x473, 0xa, 0x83, 
    0x3, 0x84, 0x3, 0x84, 0x3, 0x84, 0x3, 0x85, 0x3, 0x85, 0x3, 0x85, 0x3, 
    0x85, 0x3, 0x85, 0x3, 0x85, 0x5, 0x85, 0x47e, 0xa, 0x85, 0x3, 0x86, 
    0x3, 0x86, 0x3, 0x87, 0x3, 0x87, 0x3, 0x87, 0x3, 0x87, 0x3, 0x87, 0x5, 
    0x87, 0x487, 0xa, 0x87, 0x3, 0x88, 0x3, 0x88, 0x3, 0x88, 0x3, 0x88, 
    0x3, 0x88, 0x5, 0x88, 0x48e, 0xa, 0x88, 0x3, 0x89, 0x3, 0x89, 0x3, 0x8a, 
    0x3, 0x8a, 0x3, 0x8a, 0x7, 0x8a, 0x495, 0xa, 0x8a, 0xc, 0x8a, 0xe, 0x8a, 
    0x498, 0xb, 0x8a, 0x3, 0x8b, 0x3, 0x8b, 0x3, 0x8b, 0x7, 0x8b, 0x49d, 
    0xa, 0x8b, 0xc, 0x8b, 0xe, 0x8b, 0x4a0, 0xb, 0x8b, 0x3, 0x8c, 0x3, 0x8c, 
    0x3, 0x8c, 0x3, 0x8d, 0x3, 0x8d, 0x3, 0x8d, 0x3, 0x8d, 0x5, 0x8d, 0x4a9, 
    0xa, 0x8d, 0x3, 0x8d, 0x3, 0x8d, 0x3, 0x8d, 0x3, 0x8d, 0x3, 0x8d, 0x7, 
    0x8d, 0x4b0, 0xa, 0x8d, 0xc, 0x8d, 0xe, 0x8d, 0x4b3, 0xb, 0x8d, 0x3, 
    0x8d, 0x3, 0x8d, 0x5, 0x8d, 0x4b7, 0xa, 0x8d, 0x3, 0x8e, 0x3, 0x8e, 
    0x3, 0x8e, 0x5, 0x8e, 0x4bc, 0xa, 0x8e, 0x3, 0x8e, 0x3, 0x8e, 0x3, 0x8f, 
    0x3, 0x8f, 0x5, 0x8f, 0x4c2, 0xa, 0x8f, 0x3, 0x90, 0x3, 0x90, 0x3, 0x90, 
    0x7, 0x90, 0x4c7, 0xa, 0x90, 0xc, 0x90, 0xe, 0x90, 0x4ca, 0xb, 0x90, 
    0x3, 0x91, 0x3, 0x91, 0x3, 0x91, 0x3, 0x92, 0x3, 0x92, 0x3, 0x92, 0x3, 
    0x92, 0x3, 0x92, 0x3, 0x92, 0x3, 0x92, 0x5, 0x92, 0x4d6, 0xa, 0x92, 
    0x3, 0x93, 0x3, 0x93, 0x3, 0x93, 0x7, 0x93, 0x4db, 0xa, 0x93, 0xc, 0x93, 
    0xe, 0x93, 0x4de, 0xb, 0x93, 0x3, 0x94, 0x3, 0x94, 0x3, 0x94, 0x3, 0x94, 
    0x5, 0x94, 0x4e4, 0xa, 0x94, 0x3, 0x94, 0x3, 0x94, 0x3, 0x95, 0x3, 0x95, 
    0x3, 0x95, 0x2, 0x2, 0x96, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 
    0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 
    0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 
    0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 
    0x5a, 0x5c, 0x5e, 0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 
    0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c, 0x7e, 0x80, 0x82, 0x84, 0x86, 0x88, 
    0x8a, 0x8c, 0x8e, 0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, 0xa0, 
    0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 
    0xba, 0xbc, 0xbe, 0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce, 0xd0, 
    0xd2, 0xd4, 0xd6, 0xd8, 0xda, 0xdc, 0xde, 0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 
    0xea, 0xec, 0xee, 0xf0, 0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe, 0x100, 
    0x102, 0x104, 0x106, 0x108, 0x10a, 0x10c, 0x10e, 0x110, 0x112, 0x114, 
    0x116, 0x118, 0x11a, 0x11c, 0x11e, 0x120, 0x122, 0x124, 0x126, 0x128, 
    0x2, 0x8, 0x4, 0x2, 0x48, 0x48, 0x4a, 0x4b, 0x3, 0x2, 0x3b, 0x3c, 0x4, 
    0x2, 0x17, 0x17, 0x6a, 0x6a, 0x3, 0x2, 0x2e, 0x2f, 0x4, 0x2, 0x2d, 0x2d, 
    0x30, 0x30, 0x4, 0x2, 0x7, 0x8, 0x12, 0x13, 0x2, 0x50b, 0x2, 0x12a, 
    0x3, 0x2, 0x2, 0x2, 0x4, 0x142, 0x3, 0x2, 0x2, 0x2, 0x6, 0x146, 0x3, 
    0x2, 0x2, 0x2, 0x8, 0x14c, 0x3, 0x2, 0x2, 0x2, 0xa, 0x155, 0x3, 0x2, 
    0x2, 0x2, 0xc, 0x158, 0x3, 0x2, 0x2, 0x2, 0xe, 0x15e, 0x3, 0x2, 0x2, 
    0x2, 0x10, 0x161, 0x3, 0x2, 0x2, 0x2, 0x12, 0x163, 0x3, 0x2, 0x2, 0x2, 
    0x14, 0x16a, 0x3, 0x2, 0x2, 0x2, 0x16, 0x16e, 0x3, 0x2, 0x2, 0x2, 0x18, 
    0x171, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x173, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x17b, 
    0x3, 0x2, 0x2, 0x2, 0x1e, 0x180, 0x3, 0x2, 0x2, 0x2, 0x20, 0x187, 0x3, 
    0x2, 0x2, 0x2, 0x22, 0x18e, 0x3, 0x2, 0x2, 0x2, 0x24, 0x198, 0x3, 0x2, 
    0x2, 0x2, 0x26, 0x19e, 0x3, 0x2, 0x2, 0x2, 0x28, 0x1b4, 0x3, 0x2, 0x2, 
    0x2, 0x2a, 0x1b6, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x1b8, 0x3, 0x2, 0x2, 0x2, 
    0x2e, 0x1c9, 0x3, 0x2, 0x2, 0x2, 0x30, 0x1cd, 0x3, 0x2, 0x2, 0x2, 0x32, 
    0x1cf, 0x3, 0x2, 0x2, 0x2, 0x34, 0x1d9, 0x3, 0x2, 0x2, 0x2, 0x36, 0x1db, 
    0x3, 0x2, 0x2, 0x2, 0x38, 0x1e2, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x1e4, 0x3, 
    0x2, 0x2, 0x2, 0x3c, 0x1f3, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x204, 0x3, 0x2, 
    0x2, 0x2, 0x40, 0x206, 0x3, 0x2, 0x2, 0x2, 0x42, 0x214, 0x3, 0x2, 0x2, 
    0x2, 0x44, 0x21a, 0x3, 0x2, 0x2, 0x2, 0x46, 0x21e, 0x3, 0x2, 0x2, 0x2, 
    0x48, 0x220, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x225, 0x3, 0x2, 0x2, 0x2, 0x4c, 
    0x22e, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x230, 0x3, 0x2, 0x2, 0x2, 0x50, 0x232, 
    0x3, 0x2, 0x2, 0x2, 0x52, 0x236, 0x3, 0x2, 0x2, 0x2, 0x54, 0x244, 0x3, 
    0x2, 0x2, 0x2, 0x56, 0x248, 0x3, 0x2, 0x2, 0x2, 0x58, 0x252, 0x3, 0x2, 
    0x2, 0x2, 0x5a, 0x254, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x25e, 0x3, 0x2, 0x2, 
    0x2, 0x5e, 0x260, 0x3, 0x2, 0x2, 0x2, 0x60, 0x262, 0x3, 0x2, 0x2, 0x2, 
    0x62, 0x265, 0x3, 0x2, 0x2, 0x2, 0x64, 0x28a, 0x3, 0x2, 0x2, 0x2, 0x66, 
    0x292, 0x3, 0x2, 0x2, 0x2, 0x68, 0x294, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x2a0, 
    0x3, 0x2, 0x2, 0x2, 0x6c, 0x2a2, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x2a8, 0x3, 
    0x2, 0x2, 0x2, 0x70, 0x2b1, 0x3, 0x2, 0x2, 0x2, 0x72, 0x2b5, 0x3, 0x2, 
    0x2, 0x2, 0x74, 0x2b7, 0x3, 0x2, 0x2, 0x2, 0x76, 0x2ba, 0x3, 0x2, 0x2, 
    0x2, 0x78, 0x2be, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x2d0, 0x3, 0x2, 0x2, 0x2, 
    0x7c, 0x2d2, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x2dd, 0x3, 0x2, 0x2, 0x2, 0x80, 
    0x2e1, 0x3, 0x2, 0x2, 0x2, 0x82, 0x2ea, 0x3, 0x2, 0x2, 0x2, 0x84, 0x2ec, 
    0x3, 0x2, 0x2, 0x2, 0x86, 0x2f1, 0x3, 0x2, 0x2, 0x2, 0x88, 0x2f3, 0x3, 
    0x2, 0x2, 0x2, 0x8a, 0x2fc, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x2ff, 0x3, 0x2, 
    0x2, 0x2, 0x8e, 0x308, 0x3, 0x2, 0x2, 0x2, 0x90, 0x30a, 0x3, 0x2, 0x2, 
    0x2, 0x92, 0x30c, 0x3, 0x2, 0x2, 0x2, 0x94, 0x310, 0x3, 0x2, 0x2, 0x2, 
    0x96, 0x312, 0x3, 0x2, 0x2, 0x2, 0x98, 0x329, 0x3, 0x2, 0x2, 0x2, 0x9a, 
    0x32b, 0x3, 0x2, 0x2, 0x2, 0x9c, 0x331, 0x3, 0x2, 0x2, 0x2, 0x9e, 0x33a, 
    0x3, 0x2, 0x2, 0x2, 0xa0, 0x33c, 0x3, 0x2, 0x2, 0x2, 0xa2, 0x345, 0x3, 
    0x2, 0x2, 0x2, 0xa4, 0x347, 0x3, 0x2, 0x2, 0x2, 0xa6, 0x34a, 0x3, 0x2, 
    0x2, 0x2, 0xa8, 0x350, 0x3, 0x2, 0x2, 0x2, 0xaa, 0x365, 0x3, 0x2, 0x2, 
    0x2, 0xac, 0x367, 0x3, 0x2, 0x2, 0x2, 0xae, 0x36b, 0x3, 0x2, 0x2, 0x2, 
    0xb0, 0x375, 0x3, 0x2, 0x2, 0x2, 0xb2, 0x37f, 0x3, 0x2, 0x2, 0x2, 0xb4, 
    0x381, 0x3, 0x2, 0x2, 0x2, 0xb6, 0x389, 0x3, 0x2, 0x2, 0x2, 0xb8, 0x392, 
    0x3, 0x2, 0x2, 0x2, 0xba, 0x394, 0x3, 0x2, 0x2, 0x2, 0xbc, 0x3a4, 0x3, 
    0x2, 0x2, 0x2, 0xbe, 0x3a6, 0x3, 0x2, 0x2, 0x2, 0xc0, 0x3ae, 0x3, 0x2, 
    0x2, 0x2, 0xc2, 0x3b0, 0x3, 0x2, 0x2, 0x2, 0xc4, 0x3bd, 0x3, 0x2, 0x2, 
    0x2, 0xc6, 0x3bf, 0x3, 0x2, 0x2, 0x2, 0xc8, 0x3c1, 0x3, 0x2, 0x2, 0x2, 
    0xca, 0x3ca, 0x3, 0x2, 0x2, 0x2, 0xcc, 0x3cc, 0x3, 0x2, 0x2, 0x2, 0xce, 
    0x3d0, 0x3, 0x2, 0x2, 0x2, 0xd0, 0x3e2, 0x3, 0x2, 0x2, 0x2, 0xd2, 0x3e6, 
    0x3, 0x2, 0x2, 0x2, 0xd4, 0x3e8, 0x3, 0x2, 0x2, 0x2, 0xd6, 0x3f0, 0x3, 
    0x2, 0x2, 0x2, 0xd8, 0x3f4, 0x3, 0x2, 0x2, 0x2, 0xda, 0x3f7, 0x3, 0x2, 
    0x2, 0x2, 0xdc, 0x3fb, 0x3, 0x2, 0x2, 0x2, 0xde, 0x400, 0x3, 0x2, 0x2, 
    0x2, 0xe0, 0x40a, 0x3, 0x2, 0x2, 0x2, 0xe2, 0x40c, 0x3, 0x2, 0x2, 0x2, 
    0xe4, 0x416, 0x3, 0x2, 0x2, 0x2, 0xe6, 0x418, 0x3, 0x2, 0x2, 0x2, 0xe8, 
    0x41d, 0x3, 0x2, 0x2, 0x2, 0xea, 0x41f, 0x3, 0x2, 0x2, 0x2, 0xec, 0x427, 
    0x3, 0x2, 0x2, 0x2, 0xee, 0x42a, 0x3, 0x2, 0x2, 0x2, 0xf0, 0x42f, 0x3, 
    0x2, 0x2, 0x2, 0xf2, 0x43b, 0x3, 0x2, 0x2, 0x2, 0xf4, 0x43d, 0x3, 0x2, 
    0x2, 0x2, 0xf6, 0x448, 0x3, 0x2, 0x2, 0x2, 0xf8, 0x44a, 0x3, 0x2, 0x2, 
    0x2, 0xfa, 0x452, 0x3, 0x2, 0x2, 0x2, 0xfc, 0x45a, 0x3, 0x2, 0x2, 0x2, 
    0xfe, 0x45c, 0x3, 0x2, 0x2, 0x2, 0x100, 0x460, 0x3, 0x2, 0x2, 0x2, 0x102, 
    0x465, 0x3, 0x2, 0x2, 0x2, 0x104, 0x472, 0x3, 0x2, 0x2, 0x2, 0x106, 
    0x474, 0x3, 0x2, 0x2, 0x2, 0x108, 0x47d, 0x3, 0x2, 0x2, 0x2, 0x10a, 
    0x47f, 0x3, 0x2, 0x2, 0x2, 0x10c, 0x481, 0x3, 0x2, 0x2, 0x2, 0x10e, 
    0x488, 0x3, 0x2, 0x2, 0x2, 0x110, 0x48f, 0x3, 0x2, 0x2, 0x2, 0x112, 
    0x491, 0x3, 0x2, 0x2, 0x2, 0x114, 0x499, 0x3, 0x2, 0x2, 0x2, 0x116, 
    0x4a1, 0x3, 0x2, 0x2, 0x2, 0x118, 0x4a4, 0x3, 0x2, 0x2, 0x2, 0x11a, 
    0x4bb, 0x3, 0x2, 0x2, 0x2, 0x11c, 0x4c1, 0x3, 0x2, 0x2, 0x2, 0x11e, 
    0x4c3, 0x3, 0x2, 0x2, 0x2, 0x120, 0x4cb, 0x3, 0x2, 0x2, 0x2, 0x122, 
    0x4ce, 0x3, 0x2, 0x2, 0x2, 0x124, 0x4d7, 0x3, 0x2, 0x2, 0x2, 0x126, 
    0x4df, 0x3, 0x2, 0x2, 0x2, 0x128, 0x4e7, 0x3, 0x2, 0x2, 0x2, 0x12a, 
    0x136, 0x7, 0x6a, 0x2, 0x2, 0x12b, 0x132, 0x7, 0x15, 0x2, 0x2, 0x12c, 
    0x12d, 0x7, 0x6a, 0x2, 0x2, 0x12d, 0x12e, 0x7, 0x18, 0x2, 0x2, 0x12e, 
    0x12f, 0x7, 0x64, 0x2, 0x2, 0x12f, 0x131, 0x7, 0x19, 0x2, 0x2, 0x130, 
    0x12c, 0x3, 0x2, 0x2, 0x2, 0x131, 0x134, 0x3, 0x2, 0x2, 0x2, 0x132, 
    0x130, 0x3, 0x2, 0x2, 0x2, 0x132, 0x133, 0x3, 0x2, 0x2, 0x2, 0x133, 
    0x135, 0x3, 0x2, 0x2, 0x2, 0x134, 0x132, 0x3, 0x2, 0x2, 0x2, 0x135, 
    0x137, 0x7, 0x16, 0x2, 0x2, 0x136, 0x12b, 0x3, 0x2, 0x2, 0x2, 0x136, 
    0x137, 0x3, 0x2, 0x2, 0x2, 0x137, 0x138, 0x3, 0x2, 0x2, 0x2, 0x138, 
    0x139, 0x7, 0x4c, 0x2, 0x2, 0x139, 0x13a, 0x5, 0x4, 0x3, 0x2, 0x13a, 
    0x13b, 0x5, 0x6, 0x4, 0x2, 0x13b, 0x13c, 0x7, 0x5, 0x2, 0x2, 0x13c, 
    0x13d, 0x7, 0x4d, 0x2, 0x2, 0x13d, 0x13e, 0x5, 0x8, 0x5, 0x2, 0x13e, 
    0x13f, 0x7, 0x4e, 0x2, 0x2, 0x13f, 0x3, 0x3, 0x2, 0x2, 0x2, 0x140, 0x141, 
    0x9, 0x2, 0x2, 0x2, 0x141, 0x143, 0x7, 0x49, 0x2, 0x2, 0x142, 0x140, 
    0x3, 0x2, 0x2, 0x2, 0x142, 0x143, 0x3, 0x2, 0x2, 0x2, 0x143, 0x5, 0x3, 
    0x2, 0x2, 0x2, 0x144, 0x145, 0x7, 0x46, 0x2, 0x2, 0x145, 0x147, 0x7, 
    0x47, 0x2, 0x2, 0x146, 0x144, 0x3, 0x2, 0x2, 0x2, 0x146, 0x147, 0x3, 
    0x2, 0x2, 0x2, 0x147, 0x7, 0x3, 0x2, 0x2, 0x2, 0x148, 0x149, 0x5, 0xa, 
    0x6, 0x2, 0x149, 0x14a, 0x5, 0xe, 0x8, 0x2, 0x14a, 0x14b, 0x5, 0x1e, 
    0x10, 0x2, 0x14b, 0x14d, 0x3, 0x2, 0x2, 0x2, 0x14c, 0x148, 0x3, 0x2, 
    0x2, 0x2, 0x14c, 0x14d, 0x3, 0x2, 0x2, 0x2, 0x14d, 0x9, 0x3, 0x2, 0x2, 
    0x2, 0x14e, 0x14f, 0x7, 0x45, 0x2, 0x2, 0x14f, 0x150, 0x5, 0xc, 0x7, 
    0x2, 0x150, 0x151, 0x7, 0x43, 0x2, 0x2, 0x151, 0x156, 0x3, 0x2, 0x2, 
    0x2, 0x152, 0x153, 0x7, 0x45, 0x2, 0x2, 0x153, 0x154, 0x7, 0x2b, 0x2, 
    0x2, 0x154, 0x156, 0x7, 0x43, 0x2, 0x2, 0x155, 0x14e, 0x3, 0x2, 0x2, 
    0x2, 0x155, 0x152, 0x3, 0x2, 0x2, 0x2, 0x155, 0x156, 0x3, 0x2, 0x2, 
    0x2, 0x156, 0xb, 0x3, 0x2, 0x2, 0x2, 0x157, 0x159, 0x5, 0x1a, 0xe, 0x2, 
    0x158, 0x157, 0x3, 0x2, 0x2, 0x2, 0x158, 0x159, 0x3, 0x2, 0x2, 0x2, 
    0x159, 0xd, 0x3, 0x2, 0x2, 0x2, 0x15a, 0x15b, 0x7, 0x44, 0x2, 0x2, 0x15b, 
    0x15c, 0x5, 0x10, 0x9, 0x2, 0x15c, 0x15d, 0x7, 0x43, 0x2, 0x2, 0x15d, 
    0x15f, 0x3, 0x2, 0x2, 0x2, 0x15e, 0x15a, 0x3, 0x2, 0x2, 0x2, 0x15e, 
    0x15f, 0x3, 0x2, 0x2, 0x2, 0x15f, 0xf, 0x3, 0x2, 0x2, 0x2, 0x160, 0x162, 
    0x5, 0x12, 0xa, 0x2, 0x161, 0x160, 0x3, 0x2, 0x2, 0x2, 0x161, 0x162, 
    0x3, 0x2, 0x2, 0x2, 0x162, 0x11, 0x3, 0x2, 0x2, 0x2, 0x163, 0x167, 0x5, 
    0x14, 0xb, 0x2, 0x164, 0x166, 0x5, 0x14, 0xb, 0x2, 0x165, 0x164, 0x3, 
    0x2, 0x2, 0x2, 0x166, 0x169, 0x3, 0x2, 0x2, 0x2, 0x167, 0x165, 0x3, 
    0x2, 0x2, 0x2, 0x167, 0x168, 0x3, 0x2, 0x2, 0x2, 0x168, 0x13, 0x3, 0x2, 
    0x2, 0x2, 0x169, 0x167, 0x3, 0x2, 0x2, 0x2, 0x16a, 0x16b, 0x5, 0x1a, 
    0xe, 0x2, 0x16b, 0x16c, 0x7, 0x35, 0x2, 0x2, 0x16c, 0x16d, 0x5, 0x16, 
    0xc, 0x2, 0x16d, 0x15, 0x3, 0x2, 0x2, 0x2, 0x16e, 0x16f, 0x7, 0x6a, 
    0x2, 0x2, 0x16f, 0x170, 0x5, 0x18, 0xd, 0x2, 0x170, 0x17, 0x3, 0x2, 
    0x2, 0x2, 0x171, 0x172, 0x3, 0x2, 0x2, 0x2, 0x172, 0x19, 0x3, 0x2, 0x2, 
    0x2, 0x173, 0x178, 0x5, 0x1c, 0xf, 0x2, 0x174, 0x175, 0x7, 0x17, 0x2, 
    0x2, 0x175, 0x177, 0x5, 0x1c, 0xf, 0x2, 0x176, 0x174, 0x3, 0x2, 0x2, 
    0x2, 0x177, 0x17a, 0x3, 0x2, 0x2, 0x2, 0x178, 0x176, 0x3, 0x2, 0x2, 
    0x2, 0x178, 0x179, 0x3, 0x2, 0x2, 0x2, 0x179, 0x1b, 0x3, 0x2, 0x2, 0x2, 
    0x17a, 0x178, 0x3, 0x2, 0x2, 0x2, 0x17b, 0x17e, 0x7, 0x6a, 0x2, 0x2, 
    0x17c, 0x17d, 0x7, 0x15, 0x2, 0x2, 0x17d, 0x17f, 0x7, 0x16, 0x2, 0x2, 
    0x17e, 0x17c, 0x3, 0x2, 0x2, 0x2, 0x17e, 0x17f, 0x3, 0x2, 0x2, 0x2, 
    0x17f, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x180, 0x184, 0x5, 0x20, 0x11, 0x2, 
    0x181, 0x183, 0x5, 0x20, 0x11, 0x2, 0x182, 0x181, 0x3, 0x2, 0x2, 0x2, 
    0x183, 0x186, 0x3, 0x2, 0x2, 0x2, 0x184, 0x182, 0x3, 0x2, 0x2, 0x2, 
    0x184, 0x185, 0x3, 0x2, 0x2, 0x2, 0x185, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x186, 
    0x184, 0x3, 0x2, 0x2, 0x2, 0x187, 0x18c, 0x7, 0x6a, 0x2, 0x2, 0x188, 
    0x18d, 0x5, 0xa6, 0x54, 0x2, 0x189, 0x18d, 0x5, 0xa4, 0x53, 0x2, 0x18a, 
    0x18d, 0x5, 0x3e, 0x20, 0x2, 0x18b, 0x18d, 0x5, 0x48, 0x25, 0x2, 0x18c, 
    0x188, 0x3, 0x2, 0x2, 0x2, 0x18c, 0x189, 0x3, 0x2, 0x2, 0x2, 0x18c, 
    0x18a, 0x3, 0x2, 0x2, 0x2, 0x18c, 0x18b, 0x3, 0x2, 0x2, 0x2, 0x18d, 
    0x21, 0x3, 0x2, 0x2, 0x2, 0x18e, 0x18f, 0x7, 0x24, 0x2, 0x2, 0x18f, 
    0x194, 0x7, 0x15, 0x2, 0x2, 0x190, 0x191, 0x5, 0x24, 0x13, 0x2, 0x191, 
    0x192, 0x5, 0x26, 0x14, 0x2, 0x192, 0x195, 0x3, 0x2, 0x2, 0x2, 0x193, 
    0x195, 0x5, 0x28, 0x15, 0x2, 0x194, 0x190, 0x3, 0x2, 0x2, 0x2, 0x194, 
    0x193, 0x3, 0x2, 0x2, 0x2, 0x194, 0x195, 0x3, 0x2, 0x2, 0x2, 0x195, 
    0x196, 0x3, 0x2, 0x2, 0x2, 0x196, 0x197, 0x7, 0x16, 0x2, 0x2, 0x197, 
    0x23, 0x3, 0x2, 0x2, 0x2, 0x198, 0x19a, 0x7, 0xb, 0x2, 0x2, 0x199, 0x19b, 
    0x5, 0x106, 0x84, 0x2, 0x19a, 0x199, 0x3, 0x2, 0x2, 0x2, 0x19a, 0x19b, 
    0x3, 0x2, 0x2, 0x2, 0x19b, 0x25, 0x3, 0x2, 0x2, 0x2, 0x19c, 0x19d, 0x7, 
    0x17, 0x2, 0x2, 0x19d, 0x19f, 0x7, 0xb, 0x2, 0x2, 0x19e, 0x19c, 0x3, 
    0x2, 0x2, 0x2, 0x19e, 0x19f, 0x3, 0x2, 0x2, 0x2, 0x19f, 0x27, 0x3, 0x2, 
    0x2, 0x2, 0x1a0, 0x1aa, 0x5, 0x2a, 0x16, 0x2, 0x1a1, 0x1a2, 0x7, 0x17, 
    0x2, 0x2, 0x1a2, 0x1a3, 0x5, 0x24, 0x13, 0x2, 0x1a3, 0x1a8, 0x5, 0x30, 
    0x19, 0x2, 0x1a4, 0x1a9, 0x5, 0x26, 0x14, 0x2, 0x1a5, 0x1a6, 0x7, 0x63, 
    0x2, 0x2, 0x1a6, 0x1a7, 0x7, 0x17, 0x2, 0x2, 0x1a7, 0x1a9, 0x5, 0x2a, 
    0x16, 0x2, 0x1a8, 0x1a4, 0x3, 0x2, 0x2, 0x2, 0x1a8, 0x1a5, 0x3, 0x2, 
    0x2, 0x2, 0x1a9, 0x1ab, 0x3, 0x2, 0x2, 0x2, 0x1aa, 0x1a1, 0x3, 0x2, 
    0x2, 0x2, 0x1aa, 0x1ab, 0x3, 0x2, 0x2, 0x2, 0x1ab, 0x1b5, 0x3, 0x2, 
    0x2, 0x2, 0x1ac, 0x1ad, 0x5, 0x24, 0x13, 0x2, 0x1ad, 0x1b2, 0x5, 0x30, 
    0x19, 0x2, 0x1ae, 0x1b3, 0x5, 0x26, 0x14, 0x2, 0x1af, 0x1b0, 0x7, 0x63, 
    0x2, 0x2, 0x1b0, 0x1b1, 0x7, 0x17, 0x2, 0x2, 0x1b1, 0x1b3, 0x5, 0x2a, 
    0x16, 0x2, 0x1b2, 0x1ae, 0x3, 0x2, 0x2, 0x2, 0x1b2, 0x1af, 0x3, 0x2, 
    0x2, 0x2, 0x1b3, 0x1b5, 0x3, 0x2, 0x2, 0x2, 0x1b4, 0x1a0, 0x3, 0x2, 
    0x2, 0x2, 0x1b4, 0x1ac, 0x3, 0x2, 0x2, 0x2, 0x1b5, 0x29, 0x3, 0x2, 0x2, 
    0x2, 0x1b6, 0x1b7, 0x5, 0x2c, 0x17, 0x2, 0x1b7, 0x2b, 0x3, 0x2, 0x2, 
    0x2, 0x1b8, 0x1bd, 0x5, 0x2e, 0x18, 0x2, 0x1b9, 0x1ba, 0x7, 0x17, 0x2, 
    0x2, 0x1ba, 0x1bc, 0x5, 0x2e, 0x18, 0x2, 0x1bb, 0x1b9, 0x3, 0x2, 0x2, 
    0x2, 0x1bc, 0x1bf, 0x3, 0x2, 0x2, 0x2, 0x1bd, 0x1bb, 0x3, 0x2, 0x2, 
    0x2, 0x1bd, 0x1be, 0x3, 0x2, 0x2, 0x2, 0x1be, 0x2d, 0x3, 0x2, 0x2, 0x2, 
    0x1bf, 0x1bd, 0x3, 0x2, 0x2, 0x2, 0x1c0, 0x1c4, 0x5, 0xec, 0x77, 0x2, 
    0x1c1, 0x1c5, 0x7, 0x25, 0x2, 0x2, 0x1c2, 0x1c3, 0x7, 0x26, 0x2, 0x2, 
    0x1c3, 0x1c5, 0x5, 0xc8, 0x65, 0x2, 0x1c4, 0x1c1, 0x3, 0x2, 0x2, 0x2, 
    0x1c4, 0x1c2, 0x3, 0x2, 0x2, 0x2, 0x1c4, 0x1c5, 0x3, 0x2, 0x2, 0x2, 
    0x1c5, 0x1ca, 0x3, 0x2, 0x2, 0x2, 0x1c6, 0x1c7, 0x7, 0x27, 0x2, 0x2, 
    0x1c7, 0x1c8, 0x7, 0x28, 0x2, 0x2, 0x1c8, 0x1ca, 0x5, 0xa8, 0x55, 0x2, 
    0x1c9, 0x1c0, 0x3, 0x2, 0x2, 0x2, 0x1c9, 0x1c6, 0x3, 0x2, 0x2, 0x2, 
    0x1ca, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x1cb, 0x1cc, 0x7, 0x17, 0x2, 0x2, 
    0x1cc, 0x1ce, 0x5, 0x32, 0x1a, 0x2, 0x1cd, 0x1cb, 0x3, 0x2, 0x2, 0x2, 
    0x1cd, 0x1ce, 0x3, 0x2, 0x2, 0x2, 0x1ce, 0x31, 0x3, 0x2, 0x2, 0x2, 0x1cf, 
    0x1d4, 0x5, 0x34, 0x1b, 0x2, 0x1d0, 0x1d1, 0x7, 0x17, 0x2, 0x2, 0x1d1, 
    0x1d3, 0x5, 0x34, 0x1b, 0x2, 0x1d2, 0x1d0, 0x3, 0x2, 0x2, 0x2, 0x1d3, 
    0x1d6, 0x3, 0x2, 0x2, 0x2, 0x1d4, 0x1d2, 0x3, 0x2, 0x2, 0x2, 0x1d4, 
    0x1d5, 0x3, 0x2, 0x2, 0x2, 0x1d5, 0x33, 0x3, 0x2, 0x2, 0x2, 0x1d6, 0x1d4, 
    0x3, 0x2, 0x2, 0x2, 0x1d7, 0x1da, 0x5, 0x2e, 0x18, 0x2, 0x1d8, 0x1da, 
    0x5, 0x36, 0x1c, 0x2, 0x1d9, 0x1d7, 0x3, 0x2, 0x2, 0x2, 0x1d9, 0x1d8, 
    0x3, 0x2, 0x2, 0x2, 0x1da, 0x35, 0x3, 0x2, 0x2, 0x2, 0x1db, 0x1dc, 0x7, 
    0x4f, 0x2, 0x2, 0x1dc, 0x1dd, 0x5, 0x38, 0x1d, 0x2, 0x1dd, 0x1de, 0x5, 
    0x2c, 0x17, 0x2, 0x1de, 0x1df, 0x7, 0x50, 0x2, 0x2, 0x1df, 0x37, 0x3, 
    0x2, 0x2, 0x2, 0x1e0, 0x1e1, 0x7, 0x64, 0x2, 0x2, 0x1e1, 0x1e3, 0x7, 
    0x51, 0x2, 0x2, 0x1e2, 0x1e0, 0x3, 0x2, 0x2, 0x2, 0x1e2, 0x1e3, 0x3, 
    0x2, 0x2, 0x2, 0x1e3, 0x39, 0x3, 0x2, 0x2, 0x2, 0x1e4, 0x1ec, 0x7, 0x24, 
    0x2, 0x2, 0x1e5, 0x1e8, 0x7, 0x18, 0x2, 0x2, 0x1e6, 0x1e9, 0x5, 0xb6, 
    0x5c, 0x2, 0x1e7, 0x1e9, 0x5, 0x3c, 0x1f, 0x2, 0x1e8, 0x1e6, 0x3, 0x2, 
    0x2, 0x2, 0x1e8, 0x1e7, 0x3, 0x2, 0x2, 0x2, 0x1e9, 0x1ea, 0x3, 0x2, 
    0x2, 0x2, 0x1ea, 0x1eb, 0x7, 0x19, 0x2, 0x2, 0x1eb, 0x1ed, 0x3, 0x2, 
    0x2, 0x2, 0x1ec, 0x1e5, 0x3, 0x2, 0x2, 0x2, 0x1ec, 0x1ed, 0x3, 0x2, 
    0x2, 0x2, 0x1ed, 0x1ee, 0x3, 0x2, 0x2, 0x2, 0x1ee, 0x1f1, 0x7, 0x28, 
    0x2, 0x2, 0x1ef, 0x1f2, 0x5, 0xa8, 0x55, 0x2, 0x1f0, 0x1f2, 0x5, 0xec, 
    0x77, 0x2, 0x1f1, 0x1ef, 0x3, 0x2, 0x2, 0x2, 0x1f1, 0x1f0, 0x3, 0x2, 
    0x2, 0x2, 0x1f2, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x1f3, 0x1f4, 0x7, 0x34, 
    0x2, 0x2, 0x1f4, 0x1f5, 0x5, 0xb6, 0x5c, 0x2, 0x1f5, 0x3d, 0x3, 0x2, 
    0x2, 0x2, 0x1f6, 0x1f7, 0x5, 0x40, 0x21, 0x2, 0x1f7, 0x1fb, 0x7, 0x5, 
    0x2, 0x2, 0x1f8, 0x1fc, 0x5, 0xa8, 0x55, 0x2, 0x1f9, 0x1fc, 0x5, 0xc8, 
    0x65, 0x2, 0x1fa, 0x1fc, 0x5, 0x7e, 0x40, 0x2, 0x1fb, 0x1f8, 0x3, 0x2, 
    0x2, 0x2, 0x1fb, 0x1f9, 0x3, 0x2, 0x2, 0x2, 0x1fb, 0x1fa, 0x3, 0x2, 
    0x2, 0x2, 0x1fc, 0x205, 0x3, 0x2, 0x2, 0x2, 0x1fd, 0x1fe, 0x5, 0x4c, 
    0x27, 0x2, 0x1fe, 0x202, 0x7, 0x5, 0x2, 0x2, 0x1ff, 0x203, 0x5, 0x72, 
    0x3a, 0x2, 0x200, 0x203, 0x5, 0x4a, 0x26, 0x2, 0x201, 0x203, 0x5, 0x78, 
    0x3d, 0x2, 0x202, 0x1ff, 0x3, 0x2, 0x2, 0x2, 0x202, 0x200, 0x3, 0x2, 
    0x2, 0x2, 0x202, 0x201, 0x3, 0x2, 0x2, 0x2, 0x203, 0x205, 0x3, 0x2, 
    0x2, 0x2, 0x204, 0x1f6, 0x3, 0x2, 0x2, 0x2, 0x204, 0x1fd, 0x3, 0x2, 
    0x2, 0x2, 0x205, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x206, 0x207, 0x7, 0x15, 
    0x2, 0x2, 0x207, 0x20c, 0x5, 0x42, 0x22, 0x2, 0x208, 0x209, 0x7, 0x17, 
    0x2, 0x2, 0x209, 0x20b, 0x5, 0x42, 0x22, 0x2, 0x20a, 0x208, 0x3, 0x2, 
    0x2, 0x2, 0x20b, 0x20e, 0x3, 0x2, 0x2, 0x2, 0x20c, 0x20a, 0x3, 0x2, 
    0x2, 0x2, 0x20c, 0x20d, 0x3, 0x2, 0x2, 0x2, 0x20d, 0x20f, 0x3, 0x2, 
    0x2, 0x2, 0x20e, 0x20c, 0x3, 0x2, 0x2, 0x2, 0x20f, 0x210, 0x7, 0x16, 
    0x2, 0x2, 0x210, 0x41, 0x3, 0x2, 0x2, 0x2, 0x211, 0x212, 0x5, 0x44, 
    0x23, 0x2, 0x212, 0x213, 0x7, 0x51, 0x2, 0x2, 0x213, 0x215, 0x3, 0x2, 
    0x2, 0x2, 0x214, 0x211, 0x3, 0x2, 0x2, 0x2, 0x214, 0x215, 0x3, 0x2, 
    0x2, 0x2, 0x215, 0x216, 0x3, 0x2, 0x2, 0x2, 0x216, 0x217, 0x7, 0x6a, 
    0x2, 0x2, 0x217, 0x43, 0x3, 0x2, 0x2, 0x2, 0x218, 0x21b, 0x5, 0x46, 
    0x24, 0x2, 0x219, 0x21b, 0x7, 0x6a, 0x2, 0x2, 0x21a, 0x218, 0x3, 0x2, 
    0x2, 0x2, 0x21a, 0x219, 0x3, 0x2, 0x2, 0x2, 0x21b, 0x45, 0x3, 0x2, 0x2, 
    0x2, 0x21c, 0x21f, 0x5, 0xa8, 0x55, 0x2, 0x21d, 0x21f, 0x5, 0x4c, 0x27, 
    0x2, 0x21e, 0x21c, 0x3, 0x2, 0x2, 0x2, 0x21e, 0x21d, 0x3, 0x2, 0x2, 
    0x2, 0x21f, 0x47, 0x3, 0x2, 0x2, 0x2, 0x220, 0x221, 0x7, 0x5, 0x2, 0x2, 
    0x221, 0x222, 0x5, 0x4a, 0x26, 0x2, 0x222, 0x49, 0x3, 0x2, 0x2, 0x2, 
    0x223, 0x226, 0x5, 0x4c, 0x27, 0x2, 0x224, 0x226, 0x5, 0x52, 0x2a, 0x2, 
    0x225, 0x223, 0x3, 0x2, 0x2, 0x2, 0x225, 0x224, 0x3, 0x2, 0x2, 0x2, 
    0x226, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x227, 0x228, 0x7, 0x6a, 0x2, 0x2, 
    0x228, 0x22a, 0x7, 0x9, 0x2, 0x2, 0x229, 0x227, 0x3, 0x2, 0x2, 0x2, 
    0x229, 0x22a, 0x3, 0x2, 0x2, 0x2, 0x22a, 0x22b, 0x3, 0x2, 0x2, 0x2, 
    0x22b, 0x22f, 0x7, 0x6a, 0x2, 0x2, 0x22c, 0x22f, 0x7, 0x3b, 0x2, 0x2, 
    0x22d, 0x22f, 0x7, 0x3c, 0x2, 0x2, 0x22e, 0x229, 0x3, 0x2, 0x2, 0x2, 
    0x22e, 0x22c, 0x3, 0x2, 0x2, 0x2, 0x22e, 0x22d, 0x3, 0x2, 0x2, 0x2, 
    0x22f, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x230, 0x231, 0x9, 0x3, 0x2, 0x2, 0x231, 
    0x4f, 0x3, 0x2, 0x2, 0x2, 0x232, 0x233, 0x7, 0x6a, 0x2, 0x2, 0x233, 
    0x234, 0x7, 0x9, 0x2, 0x2, 0x234, 0x235, 0x7, 0x6a, 0x2, 0x2, 0x235, 
    0x51, 0x3, 0x2, 0x2, 0x2, 0x236, 0x237, 0x7, 0x3d, 0x2, 0x2, 0x237, 
    0x238, 0x7, 0x15, 0x2, 0x2, 0x238, 0x23d, 0x5, 0x62, 0x32, 0x2, 0x239, 
    0x23a, 0x7, 0x17, 0x2, 0x2, 0x23a, 0x23c, 0x5, 0x62, 0x32, 0x2, 0x23b, 
    0x239, 0x3, 0x2, 0x2, 0x2, 0x23c, 0x23f, 0x3, 0x2, 0x2, 0x2, 0x23d, 
    0x23b, 0x3, 0x2, 0x2, 0x2, 0x23d, 0x23e, 0x3, 0x2, 0x2, 0x2, 0x23e, 
    0x240, 0x3, 0x2, 0x2, 0x2, 0x23f, 0x23d, 0x3, 0x2, 0x2, 0x2, 0x240, 
    0x242, 0x7, 0x16, 0x2, 0x2, 0x241, 0x243, 0x5, 0x54, 0x2b, 0x2, 0x242, 
    0x241, 0x3, 0x2, 0x2, 0x2, 0x242, 0x243, 0x3, 0x2, 0x2, 0x2, 0x243, 
    0x53, 0x3, 0x2, 0x2, 0x2, 0x244, 0x245, 0x7, 0x36, 0x2, 0x2, 0x245, 
    0x246, 0x7, 0x3f, 0x2, 0x2, 0x246, 0x247, 0x5, 0x56, 0x2c, 0x2, 0x247, 
    0x55, 0x3, 0x2, 0x2, 0x2, 0x248, 0x24a, 0x7, 0x15, 0x2, 0x2, 0x249, 
    0x24b, 0x5, 0x58, 0x2d, 0x2, 0x24a, 0x249, 0x3, 0x2, 0x2, 0x2, 0x24b, 
    0x24c, 0x3, 0x2, 0x2, 0x2, 0x24c, 0x24a, 0x3, 0x2, 0x2, 0x2, 0x24c, 
    0x24d, 0x3, 0x2, 0x2, 0x2, 0x24d, 0x24e, 0x3, 0x2, 0x2, 0x2, 0x24e, 
    0x24f, 0x7, 0x16, 0x2, 0x2, 0x24f, 0x57, 0x3, 0x2, 0x2, 0x2, 0x250, 
    0x253, 0x5, 0x5c, 0x2f, 0x2, 0x251, 0x253, 0x5, 0x5a, 0x2e, 0x2, 0x252, 
    0x250, 0x3, 0x2, 0x2, 0x2, 0x252, 0x251, 0x3, 0x2, 0x2, 0x2, 0x253, 
    0x59, 0x3, 0x2, 0x2, 0x2, 0x254, 0x256, 0x7, 0x40, 0x2, 0x2, 0x255, 
    0x257, 0x5, 0x58, 0x2d, 0x2, 0x256, 0x255, 0x3, 0x2, 0x2, 0x2, 0x257, 
    0x258, 0x3, 0x2, 0x2, 0x2, 0x258, 0x256, 0x3, 0x2, 0x2, 0x2, 0x258, 
    0x259, 0x3, 0x2, 0x2, 0x2, 0x259, 0x25a, 0x3, 0x2, 0x2, 0x2, 0x25a, 
    0x25b, 0x7, 0x41, 0x2, 0x2, 0x25b, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x25c, 
    0x25f, 0x5, 0x5e, 0x30, 0x2, 0x25d, 0x25f, 0x5, 0x60, 0x31, 0x2, 0x25e, 
    0x25c, 0x3, 0x2, 0x2, 0x2, 0x25e, 0x25d, 0x3, 0x2, 0x2, 0x2, 0x25f, 
    0x5d, 0x3, 0x2, 0x2, 0x2, 0x260, 0x261, 0x9, 0x4, 0x2, 0x2, 0x261, 0x5f, 
    0x3, 0x2, 0x2, 0x2, 0x262, 0x263, 0x7, 0xd, 0x2, 0x2, 0x263, 0x264, 
    0x7, 0x6a, 0x2, 0x2, 0x264, 0x61, 0x3, 0x2, 0x2, 0x2, 0x265, 0x266, 
    0x7, 0xd, 0x2, 0x2, 0x266, 0x288, 0x7, 0x6a, 0x2, 0x2, 0x267, 0x269, 
    0x5, 0x66, 0x34, 0x2, 0x268, 0x267, 0x3, 0x2, 0x2, 0x2, 0x268, 0x269, 
    0x3, 0x2, 0x2, 0x2, 0x269, 0x289, 0x3, 0x2, 0x2, 0x2, 0x26a, 0x274, 
    0x5, 0xa8, 0x55, 0x2, 0x26b, 0x26d, 0x5, 0x70, 0x39, 0x2, 0x26c, 0x26b, 
    0x3, 0x2, 0x2, 0x2, 0x26c, 0x26d, 0x3, 0x2, 0x2, 0x2, 0x26d, 0x275, 
    0x3, 0x2, 0x2, 0x2, 0x26e, 0x270, 0x7, 0x3e, 0x2, 0x2, 0x26f, 0x26e, 
    0x3, 0x2, 0x2, 0x2, 0x26f, 0x270, 0x3, 0x2, 0x2, 0x2, 0x270, 0x272, 
    0x3, 0x2, 0x2, 0x2, 0x271, 0x273, 0x5, 0x6a, 0x36, 0x2, 0x272, 0x271, 
    0x3, 0x2, 0x2, 0x2, 0x272, 0x273, 0x3, 0x2, 0x2, 0x2, 0x273, 0x275, 
    0x3, 0x2, 0x2, 0x2, 0x274, 0x26c, 0x3, 0x2, 0x2, 0x2, 0x274, 0x26f, 
    0x3, 0x2, 0x2, 0x2, 0x275, 0x289, 0x3, 0x2, 0x2, 0x2, 0x276, 0x27d, 
    0x5, 0x7c, 0x3f, 0x2, 0x277, 0x27e, 0x7, 0x25, 0x2, 0x2, 0x278, 0x27b, 
    0x7, 0x26, 0x2, 0x2, 0x279, 0x27c, 0x5, 0x7e, 0x40, 0x2, 0x27a, 0x27c, 
    0x5, 0xc8, 0x65, 0x2, 0x27b, 0x279, 0x3, 0x2, 0x2, 0x2, 0x27b, 0x27a, 
    0x3, 0x2, 0x2, 0x2, 0x27c, 0x27e, 0x3, 0x2, 0x2, 0x2, 0x27d, 0x277, 
    0x3, 0x2, 0x2, 0x2, 0x27d, 0x278, 0x3, 0x2, 0x2, 0x2, 0x27d, 0x27e, 
    0x3, 0x2, 0x2, 0x2, 0x27e, 0x289, 0x3, 0x2, 0x2, 0x2, 0x27f, 0x286, 
    0x5, 0x4c, 0x27, 0x2, 0x280, 0x287, 0x7, 0x25, 0x2, 0x2, 0x281, 0x284, 
    0x7, 0x26, 0x2, 0x2, 0x282, 0x285, 0x5, 0x78, 0x3d, 0x2, 0x283, 0x285, 
    0x5, 0x72, 0x3a, 0x2, 0x284, 0x282, 0x3, 0x2, 0x2, 0x2, 0x284, 0x283, 
    0x3, 0x2, 0x2, 0x2, 0x285, 0x287, 0x3, 0x2, 0x2, 0x2, 0x286, 0x280, 
    0x3, 0x2, 0x2, 0x2, 0x286, 0x281, 0x3, 0x2, 0x2, 0x2, 0x286, 0x287, 
    0x3, 0x2, 0x2, 0x2, 0x287, 0x289, 0x3, 0x2, 0x2, 0x2, 0x288, 0x268, 
    0x3, 0x2, 0x2, 0x2, 0x288, 0x26a, 0x3, 0x2, 0x2, 0x2, 0x288, 0x276, 
    0x3, 0x2, 0x2, 0x2, 0x288, 0x27f, 0x3, 0x2, 0x2, 0x2, 0x289, 0x63, 0x3, 
    0x2, 0x2, 0x2, 0x28a, 0x28b, 0x7, 0xd, 0x2, 0x2, 0x28b, 0x28d, 0x7, 
    0x6a, 0x2, 0x2, 0x28c, 0x28e, 0x5, 0x66, 0x34, 0x2, 0x28d, 0x28c, 0x3, 
    0x2, 0x2, 0x2, 0x28d, 0x28e, 0x3, 0x2, 0x2, 0x2, 0x28e, 0x65, 0x3, 0x2, 
    0x2, 0x2, 0x28f, 0x293, 0x7, 0x25, 0x2, 0x2, 0x290, 0x291, 0x7, 0x26, 
    0x2, 0x2, 0x291, 0x293, 0x5, 0xa8, 0x55, 0x2, 0x292, 0x28f, 0x3, 0x2, 
    0x2, 0x2, 0x292, 0x290, 0x3, 0x2, 0x2, 0x2, 0x293, 0x67, 0x3, 0x2, 0x2, 
    0x2, 0x294, 0x295, 0x7, 0xd, 0x2, 0x2, 0x295, 0x296, 0x7, 0x6a, 0x2, 
    0x2, 0x296, 0x298, 0x5, 0xa8, 0x55, 0x2, 0x297, 0x299, 0x7, 0x3e, 0x2, 
    0x2, 0x298, 0x297, 0x3, 0x2, 0x2, 0x2, 0x298, 0x299, 0x3, 0x2, 0x2, 
    0x2, 0x299, 0x29b, 0x3, 0x2, 0x2, 0x2, 0x29a, 0x29c, 0x5, 0x6a, 0x36, 
    0x2, 0x29b, 0x29a, 0x3, 0x2, 0x2, 0x2, 0x29b, 0x29c, 0x3, 0x2, 0x2, 
    0x2, 0x29c, 0x69, 0x3, 0x2, 0x2, 0x2, 0x29d, 0x2a1, 0x7, 0x25, 0x2, 
    0x2, 0x29e, 0x29f, 0x7, 0x26, 0x2, 0x2, 0x29f, 0x2a1, 0x5, 0xc8, 0x65, 
    0x2, 0x2a0, 0x29d, 0x3, 0x2, 0x2, 0x2, 0x2a0, 0x29e, 0x3, 0x2, 0x2, 
    0x2, 0x2a1, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x2a2, 0x2a3, 0x7, 0xd, 0x2, 0x2, 
    0x2a3, 0x2a4, 0x7, 0x6a, 0x2, 0x2, 0x2a4, 0x2a6, 0x5, 0x7c, 0x3f, 0x2, 
    0x2a5, 0x2a7, 0x5, 0x6a, 0x36, 0x2, 0x2a6, 0x2a5, 0x3, 0x2, 0x2, 0x2, 
    0x2a6, 0x2a7, 0x3, 0x2, 0x2, 0x2, 0x2a7, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x2a8, 
    0x2a9, 0x7, 0xd, 0x2, 0x2, 0x2a9, 0x2aa, 0x7, 0x6a, 0x2, 0x2, 0x2aa, 
    0x2ac, 0x5, 0xa8, 0x55, 0x2, 0x2ab, 0x2ad, 0x5, 0x70, 0x39, 0x2, 0x2ac, 
    0x2ab, 0x3, 0x2, 0x2, 0x2, 0x2ac, 0x2ad, 0x3, 0x2, 0x2, 0x2, 0x2ad, 
    0x6f, 0x3, 0x2, 0x2, 0x2, 0x2ae, 0x2b2, 0x7, 0x25, 0x2, 0x2, 0x2af, 
    0x2b0, 0x7, 0x26, 0x2, 0x2, 0x2b0, 0x2b2, 0x5, 0x7e, 0x40, 0x2, 0x2b1, 
    0x2ae, 0x3, 0x2, 0x2, 0x2, 0x2b1, 0x2af, 0x3, 0x2, 0x2, 0x2, 0x2b2, 
    0x71, 0x3, 0x2, 0x2, 0x2, 0x2b3, 0x2b6, 0x5, 0x76, 0x3c, 0x2, 0x2b4, 
    0x2b6, 0x5, 0x74, 0x3b, 0x2, 0x2b5, 0x2b3, 0x3, 0x2, 0x2, 0x2, 0x2b5, 
    0x2b4, 0x3, 0x2, 0x2, 0x2, 0x2b6, 0x73, 0x3, 0x2, 0x2, 0x2, 0x2b7, 0x2b8, 
    0x5, 0x76, 0x3c, 0x2, 0x2b8, 0x2b9, 0x5, 0x102, 0x82, 0x2, 0x2b9, 0x75, 
    0x3, 0x2, 0x2, 0x2, 0x2ba, 0x2bc, 0x7, 0x6a, 0x2, 0x2, 0x2bb, 0x2bd, 
    0x7, 0x9, 0x2, 0x2, 0x2bc, 0x2bb, 0x3, 0x2, 0x2, 0x2, 0x2bc, 0x2bd, 
    0x3, 0x2, 0x2, 0x2, 0x2bd, 0x77, 0x3, 0x2, 0x2, 0x2, 0x2be, 0x2bf, 0x7, 
    0x15, 0x2, 0x2, 0x2bf, 0x2c0, 0x5, 0x7a, 0x3e, 0x2, 0x2c0, 0x2c1, 0x7, 
    0x16, 0x2, 0x2, 0x2c1, 0x79, 0x3, 0x2, 0x2, 0x2, 0x2c2, 0x2c9, 0x5, 
    0x82, 0x42, 0x2, 0x2c3, 0x2c4, 0x7, 0x17, 0x2, 0x2, 0x2c4, 0x2c7, 0x7, 
    0xb, 0x2, 0x2, 0x2c5, 0x2c6, 0x7, 0x17, 0x2, 0x2, 0x2c6, 0x2c8, 0x5, 
    0x84, 0x43, 0x2, 0x2c7, 0x2c5, 0x3, 0x2, 0x2, 0x2, 0x2c7, 0x2c8, 0x3, 
    0x2, 0x2, 0x2, 0x2c8, 0x2ca, 0x3, 0x2, 0x2, 0x2, 0x2c9, 0x2c3, 0x3, 
    0x2, 0x2, 0x2, 0x2c9, 0x2ca, 0x3, 0x2, 0x2, 0x2, 0x2ca, 0x2d1, 0x3, 
    0x2, 0x2, 0x2, 0x2cb, 0x2ce, 0x7, 0xb, 0x2, 0x2, 0x2cc, 0x2cd, 0x7, 
    0x17, 0x2, 0x2, 0x2cd, 0x2cf, 0x5, 0x84, 0x43, 0x2, 0x2ce, 0x2cc, 0x3, 
    0x2, 0x2, 0x2, 0x2ce, 0x2cf, 0x3, 0x2, 0x2, 0x2, 0x2cf, 0x2d1, 0x3, 
    0x2, 0x2, 0x2, 0x2d0, 0x2c2, 0x3, 0x2, 0x2, 0x2, 0x2d0, 0x2cb, 0x3, 
    0x2, 0x2, 0x2, 0x2d1, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x2d2, 0x2d3, 0x7, 0xd, 
    0x2, 0x2, 0x2d3, 0x2d4, 0x7, 0x6a, 0x2, 0x2, 0x2d4, 0x2da, 0x3, 0x2, 
    0x2, 0x2, 0x2d5, 0x2d6, 0x7, 0xd, 0x2, 0x2, 0x2d6, 0x2d7, 0x7, 0x6a, 
    0x2, 0x2, 0x2d7, 0x2d9, 0x7, 0x9, 0x2, 0x2, 0x2d8, 0x2d5, 0x3, 0x2, 
    0x2, 0x2, 0x2d9, 0x2dc, 0x3, 0x2, 0x2, 0x2, 0x2da, 0x2d8, 0x3, 0x2, 
    0x2, 0x2, 0x2da, 0x2db, 0x3, 0x2, 0x2, 0x2, 0x2db, 0x7d, 0x3, 0x2, 0x2, 
    0x2, 0x2dc, 0x2da, 0x3, 0x2, 0x2, 0x2, 0x2dd, 0x2de, 0x7, 0x15, 0x2, 
    0x2, 0x2de, 0x2df, 0x5, 0x80, 0x41, 0x2, 0x2df, 0x2e0, 0x7, 0x16, 0x2, 
    0x2, 0x2e0, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x2e1, 0x2e8, 0x5, 0x82, 0x42, 
    0x2, 0x2e2, 0x2e3, 0x7, 0x17, 0x2, 0x2, 0x2e3, 0x2e6, 0x7, 0xb, 0x2, 
    0x2, 0x2e4, 0x2e5, 0x7, 0x17, 0x2, 0x2, 0x2e5, 0x2e7, 0x5, 0x84, 0x43, 
    0x2, 0x2e6, 0x2e4, 0x3, 0x2, 0x2, 0x2, 0x2e6, 0x2e7, 0x3, 0x2, 0x2, 
    0x2, 0x2e7, 0x2e9, 0x3, 0x2, 0x2, 0x2, 0x2e8, 0x2e2, 0x3, 0x2, 0x2, 
    0x2, 0x2e8, 0x2e9, 0x3, 0x2, 0x2, 0x2, 0x2e9, 0x81, 0x3, 0x2, 0x2, 0x2, 
    0x2ea, 0x2eb, 0x5, 0x86, 0x44, 0x2, 0x2eb, 0x83, 0x3, 0x2, 0x2, 0x2, 
    0x2ec, 0x2ed, 0x5, 0x86, 0x44, 0x2, 0x2ed, 0x85, 0x3, 0x2, 0x2, 0x2, 
    0x2ee, 0x2f2, 0x5, 0x88, 0x45, 0x2, 0x2ef, 0x2f0, 0x7, 0x2b, 0x2, 0x2, 
    0x2f0, 0x2f2, 0x5, 0x8a, 0x46, 0x2, 0x2f1, 0x2ee, 0x3, 0x2, 0x2, 0x2, 
    0x2f1, 0x2ef, 0x3, 0x2, 0x2, 0x2, 0x2f2, 0x87, 0x3, 0x2, 0x2, 0x2, 0x2f3, 
    0x2f9, 0x5, 0x8c, 0x47, 0x2, 0x2f4, 0x2f5, 0x5, 0x8e, 0x48, 0x2, 0x2f5, 
    0x2f6, 0x5, 0x8c, 0x47, 0x2, 0x2f6, 0x2f8, 0x3, 0x2, 0x2, 0x2, 0x2f7, 
    0x2f4, 0x3, 0x2, 0x2, 0x2, 0x2f8, 0x2fb, 0x3, 0x2, 0x2, 0x2, 0x2f9, 
    0x2f7, 0x3, 0x2, 0x2, 0x2, 0x2f9, 0x2fa, 0x3, 0x2, 0x2, 0x2, 0x2fa, 
    0x89, 0x3, 0x2, 0x2, 0x2, 0x2fb, 0x2f9, 0x3, 0x2, 0x2, 0x2, 0x2fc, 0x2fd, 
    0x7, 0x2c, 0x2, 0x2, 0x2fd, 0x2fe, 0x5, 0x92, 0x4a, 0x2, 0x2fe, 0x8b, 
    0x3, 0x2, 0x2, 0x2, 0x2ff, 0x305, 0x5, 0x96, 0x4c, 0x2, 0x300, 0x301, 
    0x5, 0x90, 0x49, 0x2, 0x301, 0x302, 0x5, 0x96, 0x4c, 0x2, 0x302, 0x304, 
    0x3, 0x2, 0x2, 0x2, 0x303, 0x300, 0x3, 0x2, 0x2, 0x2, 0x304, 0x307, 
    0x3, 0x2, 0x2, 0x2, 0x305, 0x303, 0x3, 0x2, 0x2, 0x2, 0x305, 0x306, 
    0x3, 0x2, 0x2, 0x2, 0x306, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x307, 0x305, 0x3, 
    0x2, 0x2, 0x2, 0x308, 0x309, 0x9, 0x5, 0x2, 0x2, 0x309, 0x8f, 0x3, 0x2, 
    0x2, 0x2, 0x30a, 0x30b, 0x9, 0x6, 0x2, 0x2, 0x30b, 0x91, 0x3, 0x2, 0x2, 
    0x2, 0x30c, 0x30d, 0x5, 0x98, 0x4d, 0x2, 0x30d, 0x93, 0x3, 0x2, 0x2, 
    0x2, 0x30e, 0x311, 0x5, 0x72, 0x3a, 0x2, 0x30f, 0x311, 0x5, 0x76, 0x3c, 
    0x2, 0x310, 0x30e, 0x3, 0x2, 0x2, 0x2, 0x310, 0x30f, 0x3, 0x2, 0x2, 
    0x2, 0x311, 0x95, 0x3, 0x2, 0x2, 0x2, 0x312, 0x314, 0x5, 0x92, 0x4a, 
    0x2, 0x313, 0x315, 0x5, 0x8a, 0x46, 0x2, 0x314, 0x313, 0x3, 0x2, 0x2, 
    0x2, 0x314, 0x315, 0x3, 0x2, 0x2, 0x2, 0x315, 0x97, 0x3, 0x2, 0x2, 0x2, 
    0x316, 0x319, 0x5, 0xc8, 0x65, 0x2, 0x317, 0x319, 0x7, 0x32, 0x2, 0x2, 
    0x318, 0x316, 0x3, 0x2, 0x2, 0x2, 0x318, 0x317, 0x3, 0x2, 0x2, 0x2, 
    0x319, 0x31b, 0x3, 0x2, 0x2, 0x2, 0x31a, 0x31c, 0x7, 0xe, 0x2, 0x2, 
    0x31b, 0x31a, 0x3, 0x2, 0x2, 0x2, 0x31b, 0x31c, 0x3, 0x2, 0x2, 0x2, 
    0x31c, 0x31d, 0x3, 0x2, 0x2, 0x2, 0x31d, 0x31f, 0x7, 0xa, 0x2, 0x2, 
    0x31e, 0x320, 0x7, 0xe, 0x2, 0x2, 0x31f, 0x31e, 0x3, 0x2, 0x2, 0x2, 
    0x31f, 0x320, 0x3, 0x2, 0x2, 0x2, 0x320, 0x323, 0x3, 0x2, 0x2, 0x2, 
    0x321, 0x324, 0x5, 0xc8, 0x65, 0x2, 0x322, 0x324, 0x7, 0x33, 0x2, 0x2, 
    0x323, 0x321, 0x3, 0x2, 0x2, 0x2, 0x323, 0x322, 0x3, 0x2, 0x2, 0x2, 
    0x324, 0x32a, 0x3, 0x2, 0x2, 0x2, 0x325, 0x32a, 0x5, 0x3c, 0x1f, 0x2, 
    0x326, 0x327, 0x7, 0x3a, 0x2, 0x2, 0x327, 0x32a, 0x5, 0xc8, 0x65, 0x2, 
    0x328, 0x32a, 0x5, 0xc8, 0x65, 0x2, 0x329, 0x318, 0x3, 0x2, 0x2, 0x2, 
    0x329, 0x325, 0x3, 0x2, 0x2, 0x2, 0x329, 0x326, 0x3, 0x2, 0x2, 0x2, 
    0x329, 0x328, 0x3, 0x2, 0x2, 0x2, 0x32a, 0x99, 0x3, 0x2, 0x2, 0x2, 0x32b, 
    0x32c, 0x7, 0xd, 0x2, 0x2, 0x32c, 0x32d, 0x7, 0x6a, 0x2, 0x2, 0x32d, 
    0x32f, 0x5, 0x7c, 0x3f, 0x2, 0x32e, 0x330, 0x5, 0x70, 0x39, 0x2, 0x32f, 
    0x32e, 0x3, 0x2, 0x2, 0x2, 0x32f, 0x330, 0x3, 0x2, 0x2, 0x2, 0x330, 
    0x9b, 0x3, 0x2, 0x2, 0x2, 0x331, 0x332, 0x7, 0xd, 0x2, 0x2, 0x332, 0x333, 
    0x7, 0x6a, 0x2, 0x2, 0x333, 0x335, 0x5, 0x4c, 0x27, 0x2, 0x334, 0x336, 
    0x5, 0x9e, 0x50, 0x2, 0x335, 0x334, 0x3, 0x2, 0x2, 0x2, 0x335, 0x336, 
    0x3, 0x2, 0x2, 0x2, 0x336, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x337, 0x33b, 0x7, 
    0x25, 0x2, 0x2, 0x338, 0x339, 0x7, 0x26, 0x2, 0x2, 0x339, 0x33b, 0x5, 
    0x72, 0x3a, 0x2, 0x33a, 0x337, 0x3, 0x2, 0x2, 0x2, 0x33a, 0x338, 0x3, 
    0x2, 0x2, 0x2, 0x33b, 0x9f, 0x3, 0x2, 0x2, 0x2, 0x33c, 0x33d, 0x7, 0xd, 
    0x2, 0x2, 0x33d, 0x33e, 0x7, 0x6a, 0x2, 0x2, 0x33e, 0x340, 0x5, 0x4c, 
    0x27, 0x2, 0x33f, 0x341, 0x5, 0xa2, 0x52, 0x2, 0x340, 0x33f, 0x3, 0x2, 
    0x2, 0x2, 0x340, 0x341, 0x3, 0x2, 0x2, 0x2, 0x341, 0xa1, 0x3, 0x2, 0x2, 
    0x2, 0x342, 0x346, 0x7, 0x25, 0x2, 0x2, 0x343, 0x344, 0x7, 0x26, 0x2, 
    0x2, 0x344, 0x346, 0x5, 0x78, 0x3d, 0x2, 0x345, 0x342, 0x3, 0x2, 0x2, 
    0x2, 0x345, 0x343, 0x3, 0x2, 0x2, 0x2, 0x346, 0xa3, 0x3, 0x2, 0x2, 0x2, 
    0x347, 0x348, 0x7, 0x5, 0x2, 0x2, 0x348, 0x349, 0x5, 0xa8, 0x55, 0x2, 
    0x349, 0xa5, 0x3, 0x2, 0x2, 0x2, 0x34a, 0x34b, 0x5, 0xa8, 0x55, 0x2, 
    0x34b, 0x34c, 0x7, 0x5, 0x2, 0x2, 0x34c, 0x34d, 0x5, 0xc8, 0x65, 0x2, 
    0x34d, 0xa7, 0x3, 0x2, 0x2, 0x2, 0x34e, 0x351, 0x5, 0xaa, 0x56, 0x2, 
    0x34f, 0x351, 0x5, 0xb2, 0x5a, 0x2, 0x350, 0x34e, 0x3, 0x2, 0x2, 0x2, 
    0x350, 0x34f, 0x3, 0x2, 0x2, 0x2, 0x351, 0x355, 0x3, 0x2, 0x2, 0x2, 
    0x352, 0x354, 0x5, 0xb6, 0x5c, 0x2, 0x353, 0x352, 0x3, 0x2, 0x2, 0x2, 
    0x354, 0x357, 0x3, 0x2, 0x2, 0x2, 0x355, 0x353, 0x3, 0x2, 0x2, 0x2, 
    0x355, 0x356, 0x3, 0x2, 0x2, 0x2, 0x356, 0xa9, 0x3, 0x2, 0x2, 0x2, 0x357, 
    0x355, 0x3, 0x2, 0x2, 0x2, 0x358, 0x366, 0x5, 0x120, 0x91, 0x2, 0x359, 
    0x366, 0x5, 0x122, 0x92, 0x2, 0x35a, 0x366, 0x5, 0xdc, 0x6f, 0x2, 0x35b, 
    0x366, 0x5, 0xee, 0x78, 0x2, 0x35c, 0x366, 0x5, 0x10c, 0x87, 0x2, 0x35d, 
    0x366, 0x5, 0x10e, 0x88, 0x2, 0x35e, 0x366, 0x5, 0x110, 0x89, 0x2, 0x35f, 
    0x366, 0x5, 0x22, 0x12, 0x2, 0x360, 0x366, 0x5, 0x3a, 0x1e, 0x2, 0x361, 
    0x366, 0x5, 0xae, 0x58, 0x2, 0x362, 0x366, 0x5, 0xb0, 0x59, 0x2, 0x363, 
    0x366, 0x5, 0x116, 0x8c, 0x2, 0x364, 0x366, 0x5, 0xac, 0x57, 0x2, 0x365, 
    0x358, 0x3, 0x2, 0x2, 0x2, 0x365, 0x359, 0x3, 0x2, 0x2, 0x2, 0x365, 
    0x35a, 0x3, 0x2, 0x2, 0x2, 0x365, 0x35b, 0x3, 0x2, 0x2, 0x2, 0x365, 
    0x35c, 0x3, 0x2, 0x2, 0x2, 0x365, 0x35d, 0x3, 0x2, 0x2, 0x2, 0x365, 
    0x35e, 0x3, 0x2, 0x2, 0x2, 0x365, 0x35f, 0x3, 0x2, 0x2, 0x2, 0x365, 
    0x360, 0x3, 0x2, 0x2, 0x2, 0x365, 0x361, 0x3, 0x2, 0x2, 0x2, 0x365, 
    0x362, 0x3, 0x2, 0x2, 0x2, 0x365, 0x363, 0x3, 0x2, 0x2, 0x2, 0x365, 
    0x364, 0x3, 0x2, 0x2, 0x2, 0x366, 0xab, 0x3, 0x2, 0x2, 0x2, 0x367, 0x368, 
    0x5, 0x4c, 0x27, 0x2, 0x368, 0x369, 0x7, 0x9, 0x2, 0x2, 0x369, 0x36a, 
    0x5, 0x7c, 0x3f, 0x2, 0x36a, 0xad, 0x3, 0x2, 0x2, 0x2, 0x36b, 0x36c, 
    0x7, 0x29, 0x2, 0x2, 0x36c, 0x371, 0x7, 0x15, 0x2, 0x2, 0x36d, 0x36e, 
    0x5, 0x24, 0x13, 0x2, 0x36e, 0x36f, 0x5, 0x26, 0x14, 0x2, 0x36f, 0x372, 
    0x3, 0x2, 0x2, 0x2, 0x370, 0x372, 0x5, 0x28, 0x15, 0x2, 0x371, 0x36d, 
    0x3, 0x2, 0x2, 0x2, 0x371, 0x370, 0x3, 0x2, 0x2, 0x2, 0x371, 0x372, 
    0x3, 0x2, 0x2, 0x2, 0x372, 0x373, 0x3, 0x2, 0x2, 0x2, 0x373, 0x374, 
    0x7, 0x16, 0x2, 0x2, 0x374, 0xaf, 0x3, 0x2, 0x2, 0x2, 0x375, 0x378, 
    0x7, 0x29, 0x2, 0x2, 0x376, 0x379, 0x5, 0xb6, 0x5c, 0x2, 0x377, 0x379, 
    0x5, 0x3c, 0x1f, 0x2, 0x378, 0x376, 0x3, 0x2, 0x2, 0x2, 0x378, 0x377, 
    0x3, 0x2, 0x2, 0x2, 0x378, 0x379, 0x3, 0x2, 0x2, 0x2, 0x379, 0x37a, 
    0x3, 0x2, 0x2, 0x2, 0x37a, 0x37d, 0x7, 0x28, 0x2, 0x2, 0x37b, 0x37e, 
    0x5, 0xa8, 0x55, 0x2, 0x37c, 0x37e, 0x5, 0xec, 0x77, 0x2, 0x37d, 0x37b, 
    0x3, 0x2, 0x2, 0x2, 0x37d, 0x37c, 0x3, 0x2, 0x2, 0x2, 0x37e, 0xb1, 0x3, 
    0x2, 0x2, 0x2, 0x37f, 0x380, 0x5, 0xb4, 0x5b, 0x2, 0x380, 0xb3, 0x3, 
    0x2, 0x2, 0x2, 0x381, 0x384, 0x7, 0x6a, 0x2, 0x2, 0x382, 0x383, 0x7, 
    0x9, 0x2, 0x2, 0x383, 0x385, 0x7, 0x6a, 0x2, 0x2, 0x384, 0x382, 0x3, 
    0x2, 0x2, 0x2, 0x384, 0x385, 0x3, 0x2, 0x2, 0x2, 0x385, 0x387, 0x3, 
    0x2, 0x2, 0x2, 0x386, 0x388, 0x5, 0x102, 0x82, 0x2, 0x387, 0x386, 0x3, 
    0x2, 0x2, 0x2, 0x387, 0x388, 0x3, 0x2, 0x2, 0x2, 0x388, 0xb5, 0x3, 0x2, 
    0x2, 0x2, 0x389, 0x38a, 0x7, 0x18, 0x2, 0x2, 0x38a, 0x38c, 0x5, 0xb8, 
    0x5d, 0x2, 0x38b, 0x38d, 0x5, 0x106, 0x84, 0x2, 0x38c, 0x38b, 0x3, 0x2, 
    0x2, 0x2, 0x38c, 0x38d, 0x3, 0x2, 0x2, 0x2, 0x38d, 0x38e, 0x3, 0x2, 
    0x2, 0x2, 0x38e, 0x38f, 0x7, 0x19, 0x2, 0x2, 0x38f, 0xb7, 0x3, 0x2, 
    0x2, 0x2, 0x390, 0x393, 0x5, 0xbc, 0x5f, 0x2, 0x391, 0x393, 0x5, 0xc6, 
    0x64, 0x2, 0x392, 0x390, 0x3, 0x2, 0x2, 0x2, 0x392, 0x391, 0x3, 0x2, 
    0x2, 0x2, 0x393, 0xb9, 0x3, 0x2, 0x2, 0x2, 0x394, 0x395, 0x7, 0x5d, 
    0x2, 0x2, 0x395, 0x396, 0x7, 0x5e, 0x2, 0x2, 0x396, 0x397, 0x7, 0x15, 
    0x2, 0x2, 0x397, 0x39c, 0x5, 0xbe, 0x60, 0x2, 0x398, 0x399, 0x7, 0x17, 
    0x2, 0x2, 0x399, 0x39b, 0x5, 0xbe, 0x60, 0x2, 0x39a, 0x398, 0x3, 0x2, 
    0x2, 0x2, 0x39b, 0x39e, 0x3, 0x2, 0x2, 0x2, 0x39c, 0x39a, 0x3, 0x2, 
    0x2, 0x2, 0x39c, 0x39d, 0x3, 0x2, 0x2, 0x2, 0x39d, 0x39f, 0x3, 0x2, 
    0x2, 0x2, 0x39e, 0x39c, 0x3, 0x2, 0x2, 0x2, 0x39f, 0x3a0, 0x7, 0x16, 
    0x2, 0x2, 0x3a0, 0xbb, 0x3, 0x2, 0x2, 0x2, 0x3a1, 0x3a5, 0x5, 0xba, 
    0x5e, 0x2, 0x3a2, 0x3a5, 0x5, 0xc0, 0x61, 0x2, 0x3a3, 0x3a5, 0x5, 0xc4, 
    0x63, 0x2, 0x3a4, 0x3a1, 0x3, 0x2, 0x2, 0x2, 0x3a4, 0x3a2, 0x3, 0x2, 
    0x2, 0x2, 0x3a4, 0x3a3, 0x3, 0x2, 0x2, 0x2, 0x3a5, 0xbd, 0x3, 0x2, 0x2, 
    0x2, 0x3a6, 0x3ac, 0x5, 0x46, 0x24, 0x2, 0x3a7, 0x3a8, 0x7, 0x51, 0x2, 
    0x2, 0x3a8, 0x3ad, 0x5, 0xc8, 0x65, 0x2, 0x3a9, 0x3ad, 0x5, 0x7e, 0x40, 
    0x2, 0x3aa, 0x3ad, 0x5, 0x72, 0x3a, 0x2, 0x3ab, 0x3ad, 0x5, 0x78, 0x3d, 
    0x2, 0x3ac, 0x3a7, 0x3, 0x2, 0x2, 0x2, 0x3ac, 0x3a9, 0x3, 0x2, 0x2, 
    0x2, 0x3ac, 0x3aa, 0x3, 0x2, 0x2, 0x2, 0x3ac, 0x3ab, 0x3, 0x2, 0x2, 
    0x2, 0x3ac, 0x3ad, 0x3, 0x2, 0x2, 0x2, 0x3ad, 0xbf, 0x3, 0x2, 0x2, 0x2, 
    0x3ae, 0x3af, 0x5, 0x118, 0x8d, 0x2, 0x3af, 0xc1, 0x3, 0x2, 0x2, 0x2, 
    0x3b0, 0x3b1, 0x5, 0x78, 0x3d, 0x2, 0x3b1, 0xc3, 0x3, 0x2, 0x2, 0x2, 
    0x3b2, 0x3b3, 0x7, 0x21, 0x2, 0x2, 0x3b3, 0x3be, 0x5, 0xa8, 0x55, 0x2, 
    0x3b4, 0x3b5, 0x7, 0x60, 0x2, 0x2, 0x3b5, 0x3b6, 0x7, 0x5e, 0x2, 0x2, 
    0x3b6, 0x3be, 0x5, 0xc8, 0x65, 0x2, 0x3b7, 0x3b8, 0x7, 0x21, 0x2, 0x2, 
    0x3b8, 0x3b9, 0x5, 0xa8, 0x55, 0x2, 0x3b9, 0x3ba, 0x7, 0x60, 0x2, 0x2, 
    0x3ba, 0x3bb, 0x7, 0x5e, 0x2, 0x2, 0x3bb, 0x3bc, 0x5, 0xc8, 0x65, 0x2, 
    0x3bc, 0x3be, 0x3, 0x2, 0x2, 0x2, 0x3bd, 0x3b2, 0x3, 0x2, 0x2, 0x2, 
    0x3bd, 0x3b4, 0x3, 0x2, 0x2, 0x2, 0x3bd, 0x3b7, 0x3, 0x2, 0x2, 0x2, 
    0x3be, 0xc5, 0x3, 0x2, 0x2, 0x2, 0x3bf, 0x3c0, 0x5, 0x80, 0x41, 0x2, 
    0x3c0, 0xc7, 0x3, 0x2, 0x2, 0x2, 0x3c1, 0x3c2, 0x5, 0xca, 0x66, 0x2, 
    0x3c2, 0xc9, 0x3, 0x2, 0x2, 0x2, 0x3c3, 0x3cb, 0x5, 0xd8, 0x6d, 0x2, 
    0x3c4, 0x3cb, 0x5, 0xd2, 0x6a, 0x2, 0x3c5, 0x3cb, 0x5, 0xd6, 0x6c, 0x2, 
    0x3c6, 0x3cb, 0x5, 0xcc, 0x67, 0x2, 0x3c7, 0x3cb, 0x5, 0x128, 0x95, 
    0x2, 0x3c8, 0x3cb, 0x5, 0xd4, 0x6b, 0x2, 0x3c9, 0x3cb, 0x7, 0x69, 0x2, 
    0x2, 0x3ca, 0x3c3, 0x3, 0x2, 0x2, 0x2, 0x3ca, 0x3c4, 0x3, 0x2, 0x2, 
    0x2, 0x3ca, 0x3c5, 0x3, 0x2, 0x2, 0x2, 0x3ca, 0x3c6, 0x3, 0x2, 0x2, 
    0x2, 0x3ca, 0x3c7, 0x3, 0x2, 0x2, 0x2, 0x3ca, 0x3c8, 0x3, 0x2, 0x2, 
    0x2, 0x3ca, 0x3c9, 0x3, 0x2, 0x2, 0x2, 0x3cb, 0xcb, 0x3, 0x2, 0x2, 0x2, 
    0x3cc, 0x3cd, 0x7, 0x15, 0x2, 0x2, 0x3cd, 0x3ce, 0x5, 0xce, 0x68, 0x2, 
    0x3ce, 0x3cf, 0x7, 0x16, 0x2, 0x2, 0x3cf, 0xcd, 0x3, 0x2, 0x2, 0x2, 
    0x3d0, 0x3d4, 0x5, 0xd0, 0x69, 0x2, 0x3d1, 0x3d3, 0x5, 0xd0, 0x69, 0x2, 
    0x3d2, 0x3d1, 0x3, 0x2, 0x2, 0x2, 0x3d3, 0x3d6, 0x3, 0x2, 0x2, 0x2, 
    0x3d4, 0x3d2, 0x3, 0x2, 0x2, 0x2, 0x3d4, 0x3d5, 0x3, 0x2, 0x2, 0x2, 
    0x3d5, 0xcf, 0x3, 0x2, 0x2, 0x2, 0x3d6, 0x3d4, 0x3, 0x2, 0x2, 0x2, 0x3d7, 
    0x3e3, 0x7, 0x64, 0x2, 0x2, 0x3d8, 0x3df, 0x7, 0x6a, 0x2, 0x2, 0x3d9, 
    0x3dc, 0x7, 0x18, 0x2, 0x2, 0x3da, 0x3dd, 0x7, 0x64, 0x2, 0x2, 0x3db, 
    0x3dd, 0x5, 0xfc, 0x7f, 0x2, 0x3dc, 0x3da, 0x3, 0x2, 0x2, 0x2, 0x3dc, 
    0x3db, 0x3, 0x2, 0x2, 0x2, 0x3dd, 0x3de, 0x3, 0x2, 0x2, 0x2, 0x3de, 
    0x3e0, 0x7, 0x19, 0x2, 0x2, 0x3df, 0x3d9, 0x3, 0x2, 0x2, 0x2, 0x3df, 
    0x3e0, 0x3, 0x2, 0x2, 0x2, 0x3e0, 0x3e3, 0x3, 0x2, 0x2, 0x2, 0x3e1, 
    0x3e3, 0x5, 0xfc, 0x7f, 0x2, 0x3e2, 0x3d7, 0x3, 0x2, 0x2, 0x2, 0x3e2, 
    0x3d8, 0x3, 0x2, 0x2, 0x2, 0x3e2, 0x3e1, 0x3, 0x2, 0x2, 0x2, 0x3e3, 
    0xd1, 0x3, 0x2, 0x2, 0x2, 0x3e4, 0x3e7, 0x5, 0xda, 0x6e, 0x2, 0x3e5, 
    0x3e7, 0x7, 0x6a, 0x2, 0x2, 0x3e6, 0x3e4, 0x3, 0x2, 0x2, 0x2, 0x3e6, 
    0x3e5, 0x3, 0x2, 0x2, 0x2, 0x3e7, 0xd3, 0x3, 0x2, 0x2, 0x2, 0x3e8, 0x3e9, 
    0x5, 0xda, 0x6e, 0x2, 0x3e9, 0x3eb, 0x7, 0x9, 0x2, 0x2, 0x3ea, 0x3ec, 
    0x7, 0x64, 0x2, 0x2, 0x3eb, 0x3ea, 0x3, 0x2, 0x2, 0x2, 0x3eb, 0x3ec, 
    0x3, 0x2, 0x2, 0x2, 0x3ec, 0x3ee, 0x3, 0x2, 0x2, 0x2, 0x3ed, 0x3ef, 
    0x7, 0x6b, 0x2, 0x2, 0x3ee, 0x3ed, 0x3, 0x2, 0x2, 0x2, 0x3ee, 0x3ef, 
    0x3, 0x2, 0x2, 0x2, 0x3ef, 0xd5, 0x3, 0x2, 0x2, 0x2, 0x3f0, 0x3f1, 0x7, 
    0x6a, 0x2, 0x2, 0x3f1, 0x3f2, 0x7, 0x51, 0x2, 0x2, 0x3f2, 0x3f3, 0x5, 
    0xc8, 0x65, 0x2, 0x3f3, 0xd7, 0x3, 0x2, 0x2, 0x2, 0x3f4, 0x3f5, 0x7, 
    0x6a, 0x2, 0x2, 0x3f5, 0xd9, 0x3, 0x2, 0x2, 0x2, 0x3f6, 0x3f8, 0x7, 
    0x1a, 0x2, 0x2, 0x3f7, 0x3f6, 0x3, 0x2, 0x2, 0x2, 0x3f7, 0x3f8, 0x3, 
    0x2, 0x2, 0x2, 0x3f8, 0x3f9, 0x3, 0x2, 0x2, 0x2, 0x3f9, 0x3fa, 0x7, 
    0x64, 0x2, 0x2, 0x3fa, 0xdb, 0x3, 0x2, 0x2, 0x2, 0x3fb, 0x3fc, 0x7, 
    0x52, 0x2, 0x2, 0x3fc, 0x3fd, 0x7, 0x15, 0x2, 0x2, 0x3fd, 0x3fe, 0x5, 
    0xde, 0x70, 0x2, 0x3fe, 0x3ff, 0x7, 0x16, 0x2, 0x2, 0x3ff, 0xdd, 0x3, 
    0x2, 0x2, 0x2, 0x400, 0x406, 0x5, 0xe8, 0x75, 0x2, 0x401, 0x402, 0x7, 
    0x17, 0x2, 0x2, 0x402, 0x403, 0x5, 0x24, 0x13, 0x2, 0x403, 0x404, 0x5, 
    0xe0, 0x71, 0x2, 0x404, 0x405, 0x5, 0x26, 0x14, 0x2, 0x405, 0x407, 0x3, 
    0x2, 0x2, 0x2, 0x406, 0x401, 0x3, 0x2, 0x2, 0x2, 0x406, 0x407, 0x3, 
    0x2, 0x2, 0x2, 0x407, 0xdf, 0x3, 0x2, 0x2, 0x2, 0x408, 0x409, 0x7, 0x17, 
    0x2, 0x2, 0x409, 0x40b, 0x5, 0xe2, 0x72, 0x2, 0x40a, 0x408, 0x3, 0x2, 
    0x2, 0x2, 0x40a, 0x40b, 0x3, 0x2, 0x2, 0x2, 0x40b, 0xe1, 0x3, 0x2, 0x2, 
    0x2, 0x40c, 0x411, 0x5, 0xe4, 0x73, 0x2, 0x40d, 0x40e, 0x7, 0x17, 0x2, 
    0x2, 0x40e, 0x410, 0x5, 0xe4, 0x73, 0x2, 0x40f, 0x40d, 0x3, 0x2, 0x2, 
    0x2, 0x410, 0x413, 0x3, 0x2, 0x2, 0x2, 0x411, 0x40f, 0x3, 0x2, 0x2, 
    0x2, 0x411, 0x412, 0x3, 0x2, 0x2, 0x2, 0x412, 0xe3, 0x3, 0x2, 0x2, 0x2, 
    0x413, 0x411, 0x3, 0x2, 0x2, 0x2, 0x414, 0x417, 0x5, 0xe6, 0x74, 0x2, 
    0x415, 0x417, 0x5, 0xec, 0x77, 0x2, 0x416, 0x414, 0x3, 0x2, 0x2, 0x2, 
    0x416, 0x415, 0x3, 0x2, 0x2, 0x2, 0x417, 0xe5, 0x3, 0x2, 0x2, 0x2, 0x418, 
    0x419, 0x7, 0x4f, 0x2, 0x2, 0x419, 0x41a, 0x5, 0x38, 0x1d, 0x2, 0x41a, 
    0x41b, 0x5, 0xea, 0x76, 0x2, 0x41b, 0x41c, 0x7, 0x50, 0x2, 0x2, 0x41c, 
    0xe7, 0x3, 0x2, 0x2, 0x2, 0x41d, 0x41e, 0x5, 0xea, 0x76, 0x2, 0x41e, 
    0xe9, 0x3, 0x2, 0x2, 0x2, 0x41f, 0x424, 0x5, 0xec, 0x77, 0x2, 0x420, 
    0x421, 0x7, 0x17, 0x2, 0x2, 0x421, 0x423, 0x5, 0xec, 0x77, 0x2, 0x422, 
    0x420, 0x3, 0x2, 0x2, 0x2, 0x423, 0x426, 0x3, 0x2, 0x2, 0x2, 0x424, 
    0x422, 0x3, 0x2, 0x2, 0x2, 0x424, 0x425, 0x3, 0x2, 0x2, 0x2, 0x425, 
    0xeb, 0x3, 0x2, 0x2, 0x2, 0x426, 0x424, 0x3, 0x2, 0x2, 0x2, 0x427, 0x428, 
    0x7, 0x6a, 0x2, 0x2, 0x428, 0x429, 0x5, 0xa8, 0x55, 0x2, 0x429, 0xed, 
    0x3, 0x2, 0x2, 0x2, 0x42a, 0x42b, 0x7, 0x1b, 0x2, 0x2, 0x42b, 0x42c, 
    0x7, 0x15, 0x2, 0x2, 0x42c, 0x42d, 0x5, 0xf0, 0x79, 0x2, 0x42d, 0x42e, 
    0x7, 0x16, 0x2, 0x2, 0x42e, 0xef, 0x3, 0x2, 0x2, 0x2, 0x42f, 0x439, 
    0x5, 0xf2, 0x7a, 0x2, 0x430, 0x431, 0x7, 0x17, 0x2, 0x2, 0x431, 0x433, 
    0x7, 0xb, 0x2, 0x2, 0x432, 0x434, 0x5, 0x106, 0x84, 0x2, 0x433, 0x432, 
    0x3, 0x2, 0x2, 0x2, 0x433, 0x434, 0x3, 0x2, 0x2, 0x2, 0x434, 0x437, 
    0x3, 0x2, 0x2, 0x2, 0x435, 0x436, 0x7, 0x17, 0x2, 0x2, 0x436, 0x438, 
    0x5, 0x10a, 0x86, 0x2, 0x437, 0x435, 0x3, 0x2, 0x2, 0x2, 0x437, 0x438, 
    0x3, 0x2, 0x2, 0x2, 0x438, 0x43a, 0x3, 0x2, 0x2, 0x2, 0x439, 0x430, 
    0x3, 0x2, 0x2, 0x2, 0x439, 0x43a, 0x3, 0x2, 0x2, 0x2, 0x43a, 0xf1, 0x3, 
    0x2, 0x2, 0x2, 0x43b, 0x43c, 0x5, 0xf4, 0x7b, 0x2, 0x43c, 0xf3, 0x3, 
    0x2, 0x2, 0x2, 0x43d, 0x442, 0x5, 0xf6, 0x7c, 0x2, 0x43e, 0x43f, 0x7, 
    0x17, 0x2, 0x2, 0x43f, 0x441, 0x5, 0xf6, 0x7c, 0x2, 0x440, 0x43e, 0x3, 
    0x2, 0x2, 0x2, 0x441, 0x444, 0x3, 0x2, 0x2, 0x2, 0x442, 0x440, 0x3, 
    0x2, 0x2, 0x2, 0x442, 0x443, 0x3, 0x2, 0x2, 0x2, 0x443, 0xf5, 0x3, 0x2, 
    0x2, 0x2, 0x444, 0x442, 0x3, 0x2, 0x2, 0x2, 0x445, 0x449, 0x7, 0x6a, 
    0x2, 0x2, 0x446, 0x449, 0x5, 0xf8, 0x7d, 0x2, 0x447, 0x449, 0x5, 0xc8, 
    0x65, 0x2, 0x448, 0x445, 0x3, 0x2, 0x2, 0x2, 0x448, 0x446, 0x3, 0x2, 
    0x2, 0x2, 0x448, 0x447, 0x3, 0x2, 0x2, 0x2, 0x449, 0xf7, 0x3, 0x2, 0x2, 
    0x2, 0x44a, 0x44b, 0x7, 0x6a, 0x2, 0x2, 0x44b, 0x44e, 0x7, 0x18, 0x2, 
    0x2, 0x44c, 0x44f, 0x5, 0xda, 0x6e, 0x2, 0x44d, 0x44f, 0x5, 0xfc, 0x7f, 
    0x2, 0x44e, 0x44c, 0x3, 0x2, 0x2, 0x2, 0x44e, 0x44d, 0x3, 0x2, 0x2, 
    0x2, 0x44f, 0x450, 0x3, 0x2, 0x2, 0x2, 0x450, 0x451, 0x7, 0x19, 0x2, 
    0x2, 0x451, 0xf9, 0x3, 0x2, 0x2, 0x2, 0x452, 0x453, 0x7, 0x6a, 0x2, 
    0x2, 0x453, 0x456, 0x7, 0x18, 0x2, 0x2, 0x454, 0x457, 0x5, 0xd4, 0x6b, 
    0x2, 0x455, 0x457, 0x5, 0xfc, 0x7f, 0x2, 0x456, 0x454, 0x3, 0x2, 0x2, 
    0x2, 0x456, 0x455, 0x3, 0x2, 0x2, 0x2, 0x457, 0x458, 0x3, 0x2, 0x2, 
    0x2, 0x458, 0x459, 0x7, 0x19, 0x2, 0x2, 0x459, 0xfb, 0x3, 0x2, 0x2, 
    0x2, 0x45a, 0x45b, 0x5, 0xfe, 0x80, 0x2, 0x45b, 0xfd, 0x3, 0x2, 0x2, 
    0x2, 0x45c, 0x45e, 0x5, 0x100, 0x81, 0x2, 0x45d, 0x45f, 0x5, 0x102, 
    0x82, 0x2, 0x45e, 0x45d, 0x3, 0x2, 0x2, 0x2, 0x45e, 0x45f, 0x3, 0x2, 
    0x2, 0x2, 0x45f, 0xff, 0x3, 0x2, 0x2, 0x2, 0x460, 0x463, 0x7, 0x6a, 
    0x2, 0x2, 0x461, 0x462, 0x7, 0x9, 0x2, 0x2, 0x462, 0x464, 0x7, 0x6a, 
    0x2, 0x2, 0x463, 0x461, 0x3, 0x2, 0x2, 0x2, 0x463, 0x464, 0x3, 0x2, 
    0x2, 0x2, 0x464, 0x101, 0x3, 0x2, 0x2, 0x2, 0x465, 0x466, 0x7, 0x15, 
    0x2, 0x2, 0x466, 0x46b, 0x5, 0x104, 0x83, 0x2, 0x467, 0x468, 0x7, 0x17, 
    0x2, 0x2, 0x468, 0x46a, 0x5, 0x104, 0x83, 0x2, 0x469, 0x467, 0x3, 0x2, 
    0x2, 0x2, 0x46a, 0x46d, 0x3, 0x2, 0x2, 0x2, 0x46b, 0x469, 0x3, 0x2, 
    0x2, 0x2, 0x46b, 0x46c, 0x3, 0x2, 0x2, 0x2, 0x46c, 0x46e, 0x3, 0x2, 
    0x2, 0x2, 0x46d, 0x46b, 0x3, 0x2, 0x2, 0x2, 0x46e, 0x46f, 0x7, 0x16, 
    0x2, 0x2, 0x46f, 0x103, 0x3, 0x2, 0x2, 0x2, 0x470, 0x473, 0x5, 0xa8, 
    0x55, 0x2, 0x471, 0x473, 0x5, 0xc8, 0x65, 0x2, 0x472, 0x470, 0x3, 0x2, 
    0x2, 0x2, 0x472, 0x471, 0x3, 0x2, 0x2, 0x2, 0x473, 0x105, 0x3, 0x2, 
    0x2, 0x2, 0x474, 0x475, 0x7, 0x2a, 0x2, 0x2, 0x475, 0x476, 0x5, 0x108, 
    0x85, 0x2, 0x476, 0x107, 0x3, 0x2, 0x2, 0x2, 0x477, 0x47e, 0x5, 0xda, 
    0x6e, 0x2, 0x478, 0x47e, 0x5, 0xfc, 0x7f, 0x2, 0x479, 0x47a, 0x5, 0xa8, 
    0x55, 0x2, 0x47a, 0x47b, 0x7, 0x51, 0x2, 0x2, 0x47b, 0x47c, 0x5, 0xc8, 
    0x65, 0x2, 0x47c, 0x47e, 0x3, 0x2, 0x2, 0x2, 0x47d, 0x477, 0x3, 0x2, 
    0x2, 0x2, 0x47d, 0x478, 0x3, 0x2, 0x2, 0x2, 0x47d, 0x479, 0x3, 0x2, 
    0x2, 0x2, 0x47e, 0x109, 0x3, 0x2, 0x2, 0x2, 0x47f, 0x480, 0x5, 0xf4, 
    0x7b, 0x2, 0x480, 0x10b, 0x3, 0x2, 0x2, 0x2, 0x481, 0x486, 0x7, 0x14, 
    0x2, 0x2, 0x482, 0x483, 0x7, 0x15, 0x2, 0x2, 0x483, 0x484, 0x5, 0x112, 
    0x8a, 0x2, 0x484, 0x485, 0x7, 0x16, 0x2, 0x2, 0x485, 0x487, 0x3, 0x2, 
    0x2, 0x2, 0x486, 0x482, 0x3, 0x2, 0x2, 0x2, 0x486, 0x487, 0x3, 0x2, 
    0x2, 0x2, 0x487, 0x10d, 0x3, 0x2, 0x2, 0x2, 0x488, 0x48d, 0x7, 0x1c, 
    0x2, 0x2, 0x489, 0x48a, 0x7, 0x15, 0x2, 0x2, 0x48a, 0x48b, 0x5, 0x114, 
    0x8b, 0x2, 0x48b, 0x48c, 0x7, 0x16, 0x2, 0x2, 0x48c, 0x48e, 0x3, 0x2, 
    0x2, 0x2, 0x48d, 0x489, 0x3, 0x2, 0x2, 0x2, 0x48d, 0x48e, 0x3, 0x2, 
    0x2, 0x2, 0x48e, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x48f, 0x490, 0x7, 0x6, 
    0x2, 0x2, 0x490, 0x111, 0x3, 0x2, 0x2, 0x2, 0x491, 0x496, 0x5, 0xf8, 
    0x7d, 0x2, 0x492, 0x493, 0x7, 0x17, 0x2, 0x2, 0x493, 0x495, 0x5, 0xf8, 
    0x7d, 0x2, 0x494, 0x492, 0x3, 0x2, 0x2, 0x2, 0x495, 0x498, 0x3, 0x2, 
    0x2, 0x2, 0x496, 0x494, 0x3, 0x2, 0x2, 0x2, 0x496, 0x497, 0x3, 0x2, 
    0x2, 0x2, 0x497, 0x113, 0x3, 0x2, 0x2, 0x2, 0x498, 0x496, 0x3, 0x2, 
    0x2, 0x2, 0x499, 0x49e, 0x5, 0xfa, 0x7e, 0x2, 0x49a, 0x49b, 0x7, 0x17, 
    0x2, 0x2, 0x49b, 0x49d, 0x5, 0xfa, 0x7e, 0x2, 0x49c, 0x49a, 0x3, 0x2, 
    0x2, 0x2, 0x49d, 0x4a0, 0x3, 0x2, 0x2, 0x2, 0x49e, 0x49c, 0x3, 0x2, 
    0x2, 0x2, 0x49e, 0x49f, 0x3, 0x2, 0x2, 0x2, 0x49f, 0x115, 0x3, 0x2, 
    0x2, 0x2, 0x4a0, 0x49e, 0x3, 0x2, 0x2, 0x2, 0x4a1, 0x4a2, 0x7, 0x59, 
    0x2, 0x2, 0x4a2, 0x4a3, 0x7, 0x5a, 0x2, 0x2, 0x4a3, 0x117, 0x3, 0x2, 
    0x2, 0x2, 0x4a4, 0x4a5, 0x7, 0x15, 0x2, 0x2, 0x4a5, 0x4a8, 0x7, 0x6a, 
    0x2, 0x2, 0x4a6, 0x4a7, 0x7, 0x9, 0x2, 0x2, 0x4a7, 0x4a9, 0x7, 0x6a, 
    0x2, 0x2, 0x4a8, 0x4a6, 0x3, 0x2, 0x2, 0x2, 0x4a8, 0x4a9, 0x3, 0x2, 
    0x2, 0x2, 0x4a9, 0x4aa, 0x3, 0x2, 0x2, 0x2, 0x4aa, 0x4b6, 0x7, 0x16, 
    0x2, 0x2, 0x4ab, 0x4ac, 0x7, 0x15, 0x2, 0x2, 0x4ac, 0x4b1, 0x5, 0x11a, 
    0x8e, 0x2, 0x4ad, 0x4ae, 0x7, 0x17, 0x2, 0x2, 0x4ae, 0x4b0, 0x5, 0x11a, 
    0x8e, 0x2, 0x4af, 0x4ad, 0x3, 0x2, 0x2, 0x2, 0x4b0, 0x4b3, 0x3, 0x2, 
    0x2, 0x2, 0x4b1, 0x4af, 0x3, 0x2, 0x2, 0x2, 0x4b1, 0x4b2, 0x3, 0x2, 
    0x2, 0x2, 0x4b2, 0x4b4, 0x3, 0x2, 0x2, 0x2, 0x4b3, 0x4b1, 0x3, 0x2, 
    0x2, 0x2, 0x4b4, 0x4b5, 0x7, 0x16, 0x2, 0x2, 0x4b5, 0x4b7, 0x3, 0x2, 
    0x2, 0x2, 0x4b6, 0x4ab, 0x3, 0x2, 0x2, 0x2, 0x4b6, 0x4b7, 0x3, 0x2, 
    0x2, 0x2, 0x4b7, 0x119, 0x3, 0x2, 0x2, 0x2, 0x4b8, 0x4bc, 0x7, 0x3, 
    0x2, 0x2, 0x4b9, 0x4ba, 0x7, 0x5f, 0x2, 0x2, 0x4ba, 0x4bc, 0x5, 0x11c, 
    0x8f, 0x2, 0x4bb, 0x4b8, 0x3, 0x2, 0x2, 0x2, 0x4bb, 0x4b9, 0x3, 0x2, 
    0x2, 0x2, 0x4bc, 0x4bd, 0x3, 0x2, 0x2, 0x2, 0x4bd, 0x4be, 0x5, 0x11e, 
    0x90, 0x2, 0x4be, 0x11b, 0x3, 0x2, 0x2, 0x2, 0x4bf, 0x4c0, 0x7, 0x9, 
    0x2, 0x2, 0x4c0, 0x4c2, 0x5, 0x11c, 0x8f, 0x2, 0x4c1, 0x4bf, 0x3, 0x2, 
    0x2, 0x2, 0x4c1, 0x4c2, 0x3, 0x2, 0x2, 0x2, 0x4c2, 0x11d, 0x3, 0x2, 
    0x2, 0x2, 0x4c3, 0x4c8, 0x7, 0x6a, 0x2, 0x2, 0x4c4, 0x4c5, 0x7, 0x9, 
    0x2, 0x2, 0x4c5, 0x4c7, 0x7, 0x6a, 0x2, 0x2, 0x4c6, 0x4c4, 0x3, 0x2, 
    0x2, 0x2, 0x4c7, 0x4ca, 0x3, 0x2, 0x2, 0x2, 0x4c8, 0x4c6, 0x3, 0x2, 
    0x2, 0x2, 0x4c8, 0x4c9, 0x3, 0x2, 0x2, 0x2, 0x4c9, 0x11f, 0x3, 0x2, 
    0x2, 0x2, 0x4ca, 0x4c8, 0x3, 0x2, 0x2, 0x2, 0x4cb, 0x4cc, 0x7, 0x22, 
    0x2, 0x2, 0x4cc, 0x4cd, 0x7, 0x20, 0x2, 0x2, 0x4cd, 0x121, 0x3, 0x2, 
    0x2, 0x2, 0x4ce, 0x4cf, 0x7, 0x1f, 0x2, 0x2, 0x4cf, 0x4d0, 0x7, 0x20, 
    0x2, 0x2, 0x4d0, 0x4d5, 0x3, 0x2, 0x2, 0x2, 0x4d1, 0x4d2, 0x7, 0x15, 
    0x2, 0x2, 0x4d2, 0x4d3, 0x5, 0x124, 0x93, 0x2, 0x4d3, 0x4d4, 0x7, 0x16, 
    0x2, 0x2, 0x4d4, 0x4d6, 0x3, 0x2, 0x2, 0x2, 0x4d5, 0x4d1, 0x3, 0x2, 
    0x2, 0x2, 0x4d5, 0x4d6, 0x3, 0x2, 0x2, 0x2, 0x4d6, 0x123, 0x3, 0x2, 
    0x2, 0x2, 0x4d7, 0x4dc, 0x5, 0x126, 0x94, 0x2, 0x4d8, 0x4d9, 0x7, 0x17, 
    0x2, 0x2, 0x4d9, 0x4db, 0x5, 0x126, 0x94, 0x2, 0x4da, 0x4d8, 0x3, 0x2, 
    0x2, 0x2, 0x4db, 0x4de, 0x3, 0x2, 0x2, 0x2, 0x4dc, 0x4da, 0x3, 0x2, 
    0x2, 0x2, 0x4dc, 0x4dd, 0x3, 0x2, 0x2, 0x2, 0x4dd, 0x125, 0x3, 0x2, 
    0x2, 0x2, 0x4de, 0x4dc, 0x3, 0x2, 0x2, 0x2, 0x4df, 0x4e0, 0x7, 0x6a, 
    0x2, 0x2, 0x4e0, 0x4e3, 0x7, 0x18, 0x2, 0x2, 0x4e1, 0x4e4, 0x7, 0x64, 
    0x2, 0x2, 0x4e2, 0x4e4, 0x5, 0xfc, 0x7f, 0x2, 0x4e3, 0x4e1, 0x3, 0x2, 
    0x2, 0x2, 0x4e3, 0x4e2, 0x3, 0x2, 0x2, 0x2, 0x4e4, 0x4e5, 0x3, 0x2, 
    0x2, 0x2, 0x4e5, 0x4e6, 0x7, 0x19, 0x2, 0x2, 0x4e6, 0x127, 0x3, 0x2, 
    0x2, 0x2, 0x4e7, 0x4e8, 0x9, 0x7, 0x2, 0x2, 0x4e8, 0x129, 0x3, 0x2, 
    0x2, 0x2, 0x90, 0x132, 0x136, 0x142, 0x146, 0x14c, 0x155, 0x158, 0x15e, 
    0x161, 0x167, 0x178, 0x17e, 0x184, 0x18c, 0x194, 0x19a, 0x19e, 0x1a8, 
    0x1aa, 0x1b2, 0x1b4, 0x1bd, 0x1c4, 0x1c9, 0x1cd, 0x1d4, 0x1d9, 0x1e2, 
    0x1e8, 0x1ec, 0x1f1, 0x1fb, 0x202, 0x204, 0x20c, 0x214, 0x21a, 0x21e, 
    0x225, 0x229, 0x22e, 0x23d, 0x242, 0x24c, 0x252, 0x258, 0x25e, 0x268, 
    0x26c, 0x26f, 0x272, 0x274, 0x27b, 0x27d, 0x284, 0x286, 0x288, 0x28d, 
    0x292, 0x298, 0x29b, 0x2a0, 0x2a6, 0x2ac, 0x2b1, 0x2b5, 0x2bc, 0x2c7, 
    0x2c9, 0x2ce, 0x2d0, 0x2da, 0x2e6, 0x2e8, 0x2f1, 0x2f9, 0x305, 0x310, 
    0x314, 0x318, 0x31b, 0x31f, 0x323, 0x329, 0x32f, 0x335, 0x33a, 0x340, 
    0x345, 0x350, 0x355, 0x365, 0x371, 0x378, 0x37d, 0x384, 0x387, 0x38c, 
    0x392, 0x39c, 0x3a4, 0x3ac, 0x3bd, 0x3ca, 0x3d4, 0x3dc, 0x3df, 0x3e2, 
    0x3e6, 0x3eb, 0x3ee, 0x3f7, 0x406, 0x40a, 0x411, 0x416, 0x424, 0x433, 
    0x437, 0x439, 0x442, 0x448, 0x44e, 0x456, 0x45e, 0x463, 0x46b, 0x472, 
    0x47d, 0x486, 0x48d, 0x496, 0x49e, 0x4a8, 0x4b1, 0x4b6, 0x4bb, 0x4c1, 
    0x4c8, 0x4d5, 0x4dc, 0x4e3, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

ASNParser::Initializer ASNParser::_init;
