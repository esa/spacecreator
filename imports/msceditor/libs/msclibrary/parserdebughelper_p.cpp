/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "parserdebughelper_p.h"

#include "MscParser.h"

#include <QDebug>

namespace msc_dbg {

QString ruleNameFromIndex(size_t ruleId)
{
    switch (ruleId) {
    case MscParser::RuleFile:
        return QString("RuleFile");
    case MscParser::RuleName:
        return QString("RuleName");
    case MscParser::RuleEnd:
        return QString("RuleEnd");
    case MscParser::RuleComment:
        return QString("RuleComment");
    case MscParser::RuleTextDefinition:
        return QString("RuleTextDefinition");
    case MscParser::RuleMscDocument:
        return QString("RuleMscDocument");
    case MscParser::RuleDocumentHead:
        return QString("RuleDocumentHead");
    case MscParser::RuleDefiningMscReference:
        return QString("RuleDefiningMscReference");
    case MscParser::RuleVirtuality:
        return QString("RuleVirtuality");
    case MscParser::RuleContainingClause:
        return QString("RuleContainingClause");
    case MscParser::RuleInstanceItem:
        return QString("RuleInstanceItem");
    case MscParser::RuleInheritance:
        return QString("RuleInheritance");
    case MscParser::RuleMessageDeclClause:
        return QString("RuleMessageDeclClause");
    case MscParser::RuleMscDefinition:
        return QString("RuleMscDefinition");
    case MscParser::RuleIdentifier:
        return QString("RuleIdentifier");
    case MscParser::RuleMessageSequenceChart:
        return QString("RuleMessageSequenceChart");
    case MscParser::RuleMscHead:
        return QString("RuleMscHead");
    case MscParser::RuleMscParameterDecl:
        return QString("RuleMscParameterDecl");
    case MscParser::RuleMscParmDeclList:
        return QString("RuleMscParmDeclList");
    case MscParser::RuleMscParmDeclBlock:
        return QString("RuleMscParmDeclBlock");
    case MscParser::RuleInstanceParameterDecl:
        return QString("RuleInstanceParameterDecl");
    case MscParser::RuleInstanceParmDeclList:
        return QString("RuleInstanceParmDeclList");
    case MscParser::RuleMessageParameterDecl:
        return QString("RuleMessageParameterDecl");
    case MscParser::RuleMessageParmDeclList:
        return QString("RuleMessageParmDeclList");
    case MscParser::RuleTimerParameterDecl:
        return QString("RuleTimerParameterDecl");
    case MscParser::RuleTimerParmDeclList:
        return QString("RuleTimerParmDeclList");
    case MscParser::RuleMscInstInterface:
        return QString("RuleMscInstInterface");
    case MscParser::RuleInstanceKind:
        return QString("RuleInstanceKind");
    case MscParser::RuleMscGateInterface:
        return QString("RuleMscGateInterface");
    case MscParser::RuleMscGateDef:
        return QString("RuleMscGateDef");
    case MscParser::RuleMsgGate:
        return QString("RuleMsgGate");
    case MscParser::RuleMscBody:
        return QString("RuleMscBody");
    case MscParser::RuleMscStatement:
        return QString("RuleMscStatement");
    case MscParser::RuleEventDefinition:
        return QString("RuleEventDefinition");
    case MscParser::RuleInstanceEventList:
        return QString("RuleInstanceEventList");
    case MscParser::RuleInstanceDeclStatement:
        return QString("RuleInstanceDeclStatement");
    case MscParser::RuleInstanceEvent:
        return QString("RuleInstanceEvent");
    case MscParser::RuleOrderableEvent:
        return QString("RuleOrderableEvent");
    case MscParser::RuleNonOrderableEvent:
        return QString("RuleNonOrderableEvent");
    case MscParser::RuleInstanceNameList:
        return QString("RuleInstanceNameList");
    case MscParser::RuleMultiInstanceEventList:
        return QString("RuleMultiInstanceEventList");
    case MscParser::RuleMultiInstanceEvent:
        return QString("RuleMultiInstanceEvent");
    case MscParser::RuleInstanceHeadStatement:
        return QString("RuleInstanceHeadStatement");
    case MscParser::RuleInstanceEndStatement:
        return QString("RuleInstanceEndStatement");
    case MscParser::RuleMessageEvent:
        return QString("RuleMessageEvent");
    case MscParser::RuleMessageOutput:
        return QString("RuleMessageOutput");
    case MscParser::RuleMessageInput:
        return QString("RuleMessageInput");
    case MscParser::RuleIncompleteMessageEvent:
        return QString("RuleIncompleteMessageEvent");
    case MscParser::RuleIncompleteMessageOutput:
        return QString("RuleIncompleteMessageOutput");
    case MscParser::RuleIncompleteMessageInput:
        return QString("RuleIncompleteMessageInput");
    case MscParser::RuleMsgIdentification:
        return QString("RuleMsgIdentification");
    case MscParser::RuleOutputAddress:
        return QString("RuleOutputAddress");
    case MscParser::RuleInputAddress:
        return QString("RuleInputAddress");
    case MscParser::RuleMethodCallEvent:
        return QString("RuleMethodCallEvent");
    case MscParser::RuleCallOut:
        return QString("RuleCallOut");
    case MscParser::RuleCallIn:
        return QString("RuleCallIn");
    case MscParser::RuleReplyOut:
        return QString("RuleReplyOut");
    case MscParser::RuleReplyIn:
        return QString("RuleReplyIn");
    case MscParser::RuleIncompleteMethodCallEvent:
        return QString("RuleIncompleteMethodCallEvent");
    case MscParser::RuleIncompleteCallOut:
        return QString("RuleIncompleteCallOut");
    case MscParser::RuleIncompleteCallIn:
        return QString("RuleIncompleteCallIn");
    case MscParser::RuleIncompleteReplyOut:
        return QString("RuleIncompleteReplyOut");
    case MscParser::RuleIncompleteReplyIn:
        return QString("RuleIncompleteReplyIn");
    case MscParser::RuleStartMethod:
        return QString("RuleStartMethod");
    case MscParser::RuleEndMethod:
        return QString("RuleEndMethod");
    case MscParser::RuleStartSuspension:
        return QString("RuleStartSuspension");
    case MscParser::RuleEndSuspension:
        return QString("RuleEndSuspension");
    case MscParser::RuleInputDest:
        return QString("RuleInputDest");
    case MscParser::RuleOutputDest:
        return QString("RuleOutputDest");
    case MscParser::RuleDefInGate:
        return QString("RuleDefInGate");
    case MscParser::RuleDefOutGate:
        return QString("RuleDefOutGate");
    case MscParser::RuleSharedCondition:
        return QString("RuleSharedCondition");
    case MscParser::RuleConditionIdentification:
        return QString("RuleConditionIdentification");
    case MscParser::RuleConditionText:
        return QString("RuleConditionText");
    case MscParser::RuleConditionNameList:
        return QString("RuleConditionNameList");
    case MscParser::RuleShared:
        return QString("RuleShared");
    case MscParser::RuleSharedInstanceList:
        return QString("RuleSharedInstanceList");
    case MscParser::RuleCondition:
        return QString("RuleCondition");
    case MscParser::RuleTimerStatement:
        return QString("RuleTimerStatement");
    case MscParser::RuleStartTimer:
        return QString("RuleStartTimer");
    case MscParser::RuleDuration:
        return QString("RuleDuration");
    case MscParser::RuleDurationLimit:
        return QString("RuleDurationLimit");
    case MscParser::RuleStopTimer:
        return QString("RuleStopTimer");
    case MscParser::RuleTimeout:
        return QString("RuleTimeout");
    case MscParser::RuleAction:
        return QString("RuleAction");
    case MscParser::RuleActionStatement:
        return QString("RuleActionStatement");
    case MscParser::RuleInformalAction:
        return QString("RuleInformalAction");
    case MscParser::RuleCreate:
        return QString("RuleCreate");
    case MscParser::RuleStop:
        return QString("RuleStop");
    case MscParser::RuleVariableString:
        return QString("RuleVariableString");
    case MscParser::RuleTypeRefString:
        return QString("RuleTypeRefString");
    case MscParser::RuleDataDefinitionString:
        return QString("RuleDataDefinitionString");
    case MscParser::RuleMessageDeclList:
        return QString("RuleMessageDeclList");
    case MscParser::RuleMessageDecl:
        return QString("RuleMessageDecl");
    case MscParser::RuleMessageNameList:
        return QString("RuleMessageNameList");
    case MscParser::RuleTimerDeclList:
        return QString("RuleTimerDeclList");
    case MscParser::RuleTimerDecl:
        return QString("RuleTimerDecl");
    case MscParser::RuleTimerNameList:
        return QString("RuleTimerNameList");
    case MscParser::RuleTypeRefList:
        return QString("RuleTypeRefList");
    case MscParser::RuleDynamicDeclList:
        return QString("RuleDynamicDeclList");
    case MscParser::RuleVariableDeclList:
        return QString("RuleVariableDeclList");
    case MscParser::RuleVariableDeclItem:
        return QString("RuleVariableDeclItem");
    case MscParser::RuleVariableList:
        return QString("RuleVariableList");
    case MscParser::RuleDataDefinition:
        return QString("RuleDataDefinition");
    case MscParser::RuleWildcardDecl:
        return QString("RuleWildcardDecl");
    case MscParser::RuleDataParameterDecl:
        return QString("RuleDataParameterDecl");
    case MscParser::RuleActualDataParameters:
        return QString("RuleActualDataParameters");
    case MscParser::RuleActualDataParameterList:
        return QString("RuleActualDataParameterList");
    case MscParser::RuleBinding:
        return QString("RuleBinding");
    case MscParser::RuleLeftBinding:
        return QString("RuleLeftBinding");
    case MscParser::RuleRightBinding:
        return QString("RuleRightBinding");
    case MscParser::RuleExpression:
        return QString("RuleExpression");
    case MscParser::RulePattern:
        return QString("RulePattern");
    case MscParser::RuleWildcard:
        return QString("RuleWildcard");
    case MscParser::RuleParameterList:
        return QString("RuleParameterList");
    case MscParser::RuleParamaterDefn:
        return QString("RuleParamaterDefn");
    case MscParser::RuleExpressionString:
        return QString("RuleExpressionString");
    case MscParser::RuleVariableValue:
        return QString("RuleVariableValue");
    case MscParser::RuleDataStatementList:
        return QString("RuleDataStatementList");
    case MscParser::RuleDataStatement:
        return QString("RuleDataStatement");
    case MscParser::RuleDefineStatement:
        return QString("RuleDefineStatement");
    case MscParser::RuleUndefineStatement:
        return QString("RuleUndefineStatement");
    case MscParser::RuleStartCoregion:
        return QString("RuleStartCoregion");
    case MscParser::RuleEndCoregion:
        return QString("RuleEndCoregion");
    case MscParser::RuleCoregion:
        return QString("RuleCoregion");
    case MscParser::RuleDecomposition:
        return QString("RuleDecomposition");
    case MscParser::RuleSubstructureReference:
        return QString("RuleSubstructureReference");
    case MscParser::RuleFunctionText:
        return QString("RuleFunctionText");
    case MscParser::RuleSdlText:
        return QString("RuleSdlText");
    default:
        qWarning() << "msc_dbg::ruleNameFromIndex: Unknown rule #" << ruleId;
        return QString();
    }
}

} // ns msc
