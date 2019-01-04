/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscparservisitor.h"
#include "mscaction.h"
#include "mscmodel.h"
#include "mscchart.h"
#include "msccondition.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscgate.h"
#include "msctimer.h"
#include "msccoregion.h"

#include <QDebug>

#include <string>

template<typename T>
static QString treeNodeToString(T *node)
{
    if (node != nullptr) {
        return QString::fromStdString(node->getText());
    }
    return QString();
};

using namespace msc;

MscParserVisitor::MscParserVisitor(antlr4::CommonTokenStream *tokens) : m_model(new MscModel), m_tokens(tokens) {}

MscParserVisitor::~MscParserVisitor()
{
    delete m_model;
}

msc::MscModel *MscParserVisitor::detachModel()
{
    msc::MscModel *model = m_model;
    m_model = nullptr;
    return model;
}

antlrcpp::Any MscParserVisitor::visitFile(MscParser::FileContext *context)
{
    Q_ASSERT(m_model != nullptr);
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMscDocument(MscParser::MscDocumentContext *context)
{
    MscDocument *parent = m_currentDocument;

    auto doc = new MscDocument();
    if (m_currentDocument == nullptr) {
        m_model->addDocument(doc);
    } else {
        m_currentDocument->addDocument(doc);
    }
    const auto docName = ::treeNodeToString(context->documentHead()->NAME());
    doc->setName(docName);

    auto handleComment = [=](antlr4::Token *token) {
        if (token->getChannel() == 2) {
            // Handle this token
            auto line = QString::fromStdString(token->getText()).trimmed();
            if (line.startsWith("/*")) {
                line = line.mid(2);
            }
            if (line.endsWith("*/")) {
                line.chop(2);
            }
            line = line.trimmed();

            if (line.startsWith("CIF")) {
                // Handle CIF here
                // qDebug() << "CIF comment on" << docName << ":" << line;
            } else if (line.startsWith("MSC")) {
                // Handle MSC here
                // This is really simple first version of an MSC hierarchy parser
                // For something more complete, we might need to really parse this
                line = line.toLower();
                if (line.contains("goal") || line.contains("time")) {
                    // Not supported
                } else if (line.contains("exception")) {
                    doc->setHierarchyType(MscDocument::HierarchyException);
                } else if (line.contains("parallel")) {
                    doc->setHierarchyType(MscDocument::HierarchyParallel);
                } else if (line.contains("repeat")) {
                    doc->setHierarchyType(MscDocument::HierarchyRepeat);
                } else if (line.contains("is")) {
                    doc->setHierarchyType(MscDocument::HierarchyIs);
                } else if (line.contains("and")) {
                    doc->setHierarchyType(MscDocument::HierarchyAnd);
                } else if (line.contains("or")) {
                    doc->setHierarchyType(MscDocument::HierarchyOr);
                }
            }
        }
    };

    // Get the comments on the document and handle them
    auto cifComments = m_tokens->getHiddenTokensToLeft(context->start->getTokenIndex());
    for (auto token : cifComments) {
        handleComment(token);
    }
    auto mscComments = m_tokens->getHiddenTokensToRight(context->start->getTokenIndex() + 1);
    for (auto token : mscComments) {
        handleComment(token);
    }

    m_currentDocument = doc;
    auto ret = visitChildren(context);
    m_currentDocument = parent;
    return ret;
}

antlrcpp::Any MscParserVisitor::visitInstanceItem(MscParser::InstanceItemContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    const QString name = ::treeNodeToString(context->instanceName);
    m_currentInstance = m_currentChart->instanceByName(name);
    auto result = visitChildren(context);
    m_currentInstance = nullptr;
    return result;
}

antlrcpp::Any MscParserVisitor::visitMessageSequenceChart(MscParser::MessageSequenceChartContext *context)
{
    QString mscName;
    if (context->mscHead()) {
        mscName = ::treeNodeToString(context->mscHead()->NAME());
    }
    auto chart = new MscChart(mscName);
    if (m_currentDocument == nullptr) {
        m_model->addChart(chart);
    } else {
        m_currentDocument->addChart(chart);
    }

    m_currentChart = chart;
    // add all instances first, so messages can reference them
    if (context->mscHead() && context->mscHead()->mscInstInterface()) {
        auto clause = context->mscHead()->mscInstInterface()->containingClause();
        for (auto instanceItem : clause->instanceItem()) {
            addInstance(::treeNodeToString(instanceItem->instanceName));
        }
    }
    if (context->mscBody()) {
        for (auto instanceDeclCtx : context->mscBody()->instanceDeclStatement()) {
            auto headCtx = instanceDeclCtx->instanceHeadStatement();
            if (headCtx->instanceName) {
                addInstance(::treeNodeToString(headCtx->instanceName));
            }
        }
        for (auto mscStatementCtx : context->mscBody()->mscStatement()) {
            auto eventDefCtx = mscStatementCtx->eventDefinition();
            if (eventDefCtx) {
                auto eventListCtx = eventDefCtx->instanceEventList();
                if (eventListCtx) {
                    addInstance(::treeNodeToString(eventDefCtx->NAME()));
                }
            }
        }
    }

    auto result = visitChildren(context);

    orderInstanceEvents();

    m_currentChart = nullptr;
    return result;
}

antlrcpp::Any MscParserVisitor::visitInstanceKind(MscParser::InstanceKindContext *context)
{
    if (m_currentInstance) {
        const QString kind = ::treeNodeToString(context->NAME(0));
        const QString denominator = denominatorString(kind);
        if (denominator.isEmpty()) {
            m_currentInstance->setKind(kind);
        } else {
            m_currentInstance->setDenominator(denominator);
            if (context->NAME().size() > 1) {
                m_currentInstance->setKind(::treeNodeToString(context->NAME(1)));
            }
        }
    }

    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitEventDefinition(MscParser::EventDefinitionContext *context)
{
    const QString name = ::treeNodeToString(context->instanceName);
    if (name.isEmpty()) {
        return visitChildren(context);
    }

    m_currentInstance = m_currentChart->instanceByName(name);
    auto ret = visitChildren(context);
    m_currentInstance = nullptr;
    return ret;
}

antlrcpp::Any MscParserVisitor::visitInstanceHeadStatement(MscParser::InstanceHeadStatementContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    if (!m_currentInstance) {
        QString name;
        if (context->instanceName) {
            name = ::treeNodeToString(context->instanceName);
        } else {
            auto kindCtx = context->instanceKind();
            if (kindCtx) {
                name = ::treeNodeToString(kindCtx->NAME(0));
            }
        }

        if (name.isEmpty()) {
            qWarning() << "No name for the instance";
            return visitChildren(context);
        }

        m_currentInstance = m_currentChart->instanceByName(name);
        if (!m_currentInstance) {
            qWarning() << "Instance" << name << "has not been created before as expected";
        }
        m_currentMessage = nullptr;
    } else {
        if (context->instanceName) {
            m_currentInstance->setDenominator(denominatorString(treeNodeToString(context->instanceName)));
        }
    }

    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitInstanceEndStatement(MscParser::InstanceEndStatementContext *context)
{
    resetInstanceEvents();
    m_currentInstance = nullptr;
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMessageOutput(MscParser::MessageOutputContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    const QString name = ::treeNodeToString(context->msgIdentification()->messageName);
    m_currentMessage = m_currentChart->messageByName(name);
    if (m_currentMessage == nullptr) {
        m_currentMessage = new MscMessage(name);
        m_instanceEvents.append(m_currentMessage);
    }

    MscParser::InputAddressContext *inputAddress = context->inputAddress();
    if (inputAddress && inputAddress->instanceName) {
        const QString target = QString::fromStdString(inputAddress->instanceName->getText());
        m_currentMessage->setTargetInstance(m_currentChart->instanceByName(target));
    }
    m_currentMessage->setSourceInstance(m_currentInstance);

    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMessageInput(MscParser::MessageInputContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    const QString name = ::treeNodeToString(context->msgIdentification()->messageName);
    m_currentMessage = m_currentChart->messageByName(name);
    if (m_currentMessage == nullptr) {
        m_currentMessage = new MscMessage(name);
        m_instanceEvents.append(m_currentMessage);
    }

    MscParser::OutputAddressContext *outputAddress = context->outputAddress();
    if (outputAddress && outputAddress->instanceName) {
        const QString source = ::treeNodeToString(outputAddress->instanceName);
        m_currentMessage->setSourceInstance(m_currentChart->instanceByName(source));
    }
    m_currentMessage->setTargetInstance(m_currentInstance);
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMsgIdentification(MscParser::MsgIdentificationContext *context)
{
    if (!m_currentMessage) {
        return visitChildren(context);
    }

    MscMessage::Parameters parameters;
    if (context->NAME().size() > 1) {
        parameters.name = ::treeNodeToString(context->NAME(1));
    }

    auto *parameterList = context->parameterList();
    if (parameterList && parameterList->paramaterDefn()) {
        auto *paramaterDefn = parameterList->paramaterDefn();
        parameters.expression = ::treeNodeToString(paramaterDefn->expression());
        parameters.pattern = ::treeNodeToString(paramaterDefn->pattern());
    }
    m_currentMessage->setParameters(parameters);
    return visitChildren(context);
}

void applyGateDetails(MscGate *gate, MscParser::MsgIdentificationContext *msgCtx)
{
    if (msgCtx->messageInstanceName && msgCtx->parameterList()) {
        gate->setParamName(::treeNodeToString(msgCtx->messageInstanceName));
    } else {
        gate->setParamName(::treeNodeToString(msgCtx->messageName));
    }
    QVariantList params;
    if (MscParser::ParameterListContext *paramList = msgCtx->parameterList()) {
        while (paramList) {
            params << QString::fromStdString(paramList->getText());
            paramList = paramList->parameterList();
        }
    }
    gate->setParams(params);
}

antlrcpp::Any MscParserVisitor::visitDefInGate(MscParser::DefInGateContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    MscGate *gate = new MscGate;
    gate->setName(::treeNodeToString(context->gateName));
    gate->setDirection(MscGate::Direction::In);

    if (context->inputDest()->inputAddress()) {
        gate->setInstanceName(::treeNodeToString(context->inputDest()->inputAddress()->instanceName));
    }

    applyGateDetails(gate, context->msgIdentification());

    m_currentChart->addGate(gate);
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitDefOutGate(MscParser::DefOutGateContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    MscGate *gate = new MscGate;
    gate->setName(::treeNodeToString(context->gateName));
    gate->setDirection(MscGate::Direction::Out);
    if (context->outputDest()->outputAddress()) {
        gate->setInstanceName(::treeNodeToString(context->outputDest()->outputAddress()->instanceName));
    }

    applyGateDetails(gate, context->msgIdentification());

    m_currentChart->addGate(gate);
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitSharedCondition(MscParser::SharedConditionContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    MscParser::ConditionTextContext *conditionText = context->conditionIdentification()->conditionText();

    QString name;
    if (conditionText->conditionNameList()) {
        name = ::treeNodeToString(conditionText->conditionNameList()->NAME(0));
    }
    auto *condition = new MscCondition(name);

    if (!context->shared().empty()) {
        MscParser::SharedContext *shared = context->shared().at(0);
        condition->setShared(shared->ALL());
    }
    condition->setInstance(m_currentInstance);

    if (m_currentMessage)
        condition->setMessageName(m_currentMessage->name());

    m_currentChart->addInstanceEvent(condition);

    return visitChildren(context);
}

void appendDataStatement(MscAction *action, MscParser::DataStatementListContext *statementList)
{
    if (statementList == nullptr) {
        return;
    }
    MscParser::DataStatementContext *statementCtx = statementList->dataStatement();
    if (statementCtx == nullptr) {
        return;
    }

    MscAction::DataStatement statement;
    if (statementCtx->defineStatement()) {
        statement.m_type = MscAction::DataStatement::StatementType::Define;
        statement.m_variableString = ::treeNodeToString(statementCtx->defineStatement()->variableString()->STRING());
        action->addDataStatement(statement);
    }
    if (statementCtx->undefineStatement()) {
        statement.m_type = MscAction::DataStatement::StatementType::UnDefine;
        statement.m_variableString = ::treeNodeToString(statementCtx->undefineStatement()->variableString()->STRING());
        action->addDataStatement(statement);
    }
    if (statementCtx->binding()) {
        qWarning() << "Formal binding action types is not suported";
    }

    appendDataStatement(action, statementList->dataStatementList());
}

antlrcpp::Any MscParserVisitor::visitActionStatement(MscParser::ActionStatementContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    auto action = new MscAction;
    if (context->informalAction()) {
        action->setActionType(MscAction::ActionType::Informal);
        QString informalAction = ::treeNodeToString(context->informalAction()->CHARACTERSTRING());
        // remove the single quotes
        informalAction = informalAction.mid(1, informalAction.size() - 2);
        action->setInformalAction(informalAction);
    } else {
        action->setActionType(MscAction::ActionType::Formal);
        appendDataStatement(action, context->dataStatementList());
    }

    m_instanceEvents.append(action);

    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitStop(MscParser::StopContext *context)
{
    if (m_currentInstance)
        m_currentInstance->setExplicitStop(true);

    resetInstanceEvents();
    m_currentInstance = nullptr;
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitDataDefinition(MscParser::DataDefinitionContext *context)
{
    if (context->LANGUAGE()) {
        m_model->setDataLanguage(::treeNodeToString(context->dataLanguageName));
    }
    if (context->DATA()) {
        m_model->setDataDefinitionString(::treeNodeToString(context->dataDefString));
    }
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitStartCoregion(MscParser::StartCoregionContext *ctx)
{
    if (!m_currentChart) {
        return visitChildren(ctx);
    }

    m_instanceEvents.append(new MscCoregion(MscCoregion::Type::Begin));
    return visitChildren(ctx);
}

antlrcpp::Any MscParserVisitor::visitEndCoregion(MscParser::EndCoregionContext *ctx)
{
    if (!m_currentChart) {
        return visitChildren(ctx);
    }

    m_instanceEvents.append(new MscCoregion(MscCoregion::Type::End));
    return visitChildren(ctx);
}

antlrcpp::Any MscParserVisitor::visitTimerStatement(MscParser::TimerStatementContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    QScopedPointer<MscTimer> timer(new MscTimer());
    if (MscParser::StartTimerContext *startTimer = context->startTimer()) {
        timer->setTimerType(MscTimer::TimerType::Start);
        timer->setName(::treeNodeToString(startTimer->NAME(0)));
        timer->setInstanceName(::treeNodeToString(startTimer->NAME(1)));
    } else if (MscParser::StopTimerContext *stopTimer = context->stopTimer()) {
        timer->setTimerType(MscTimer::TimerType::Stop);
        timer->setName(::treeNodeToString(stopTimer->NAME(0)));
        timer->setInstanceName(::treeNodeToString(stopTimer->NAME(1)));
    } else if (MscParser::TimeoutContext *timeout = context->timeout()) {
        timer->setTimerType(MscTimer::TimerType::Timeout);
        timer->setName(::treeNodeToString(timeout->NAME(0)));
        timer->setInstanceName(::treeNodeToString(timeout->NAME(1)));
    } else {
        qWarning() << Q_FUNC_INFO << "Bad timer declaration";
    }

    m_instanceEvents.append(timer.take());

    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitCoregion(MscParser::CoregionContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    if (context->CONCURRENT()) {
        m_instanceEvents.append(new MscCoregion(MscCoregion::Type::Begin));
    }
    if (context->ENDCONCURRENT()) {
        m_instanceEvents.append(new MscCoregion(MscCoregion::Type::End));
    }

    return visitChildren(context);
}

void MscParserVisitor::addInstance(const QString &name)
{
    MscInstance *instance = m_currentChart->instanceByName(name);
    if (!instance) {
        instance = new MscInstance(name, m_currentChart);
        m_currentChart->addInstance(instance);
    }
}

void MscParserVisitor::resetInstanceEvents()
{
    if (!m_instanceEvents.isEmpty()) {
        m_instanceEventsList.append(m_instanceEvents);
        m_instanceEvents.clear();
    }
}

void MscParserVisitor::orderInstanceEvents()
{
    bool found;

    while (!m_instanceEventsList.isEmpty()) {
        found = false;

        for (int i = 0; i < m_instanceEventsList.size(); ++i) {
            // First, go through all the stacks and take away non-messages. This has to be done for every loop
            for (int j = 0; j < m_instanceEventsList.size(); ++j) {
                while (!m_instanceEventsList.at(j).isEmpty()
                       && m_instanceEventsList.at(j).first()->entityType() != MscEntity::EntityType::Message) {
                    // This is not a message, move it to the chart
                    m_currentChart->addInstanceEvent(m_instanceEventsList[j].takeFirst());
                }
            }

            if (m_instanceEventsList.at(i).isEmpty()) {
                continue;
            }

            bool inOther = false;

            // annotate the first element of the list
            auto firstMessage = m_instanceEventsList[i][0];

            auto checkMessage = [&](MscInstanceEvent *event) {
                if (event->entityType() != MscEntity::EntityType::Message) {
                    return false;
                }
                return static_cast<MscMessage *>(event)->name() == firstMessage->name();
            };

            // look first elements of others list
            for (int j = i + 1; j < m_instanceEventsList.size(); ++j) {
                if (std::count_if(m_instanceEventsList[j].begin(), m_instanceEventsList[j].end(), checkMessage)) {
                    if (m_instanceEventsList[j][0]->name() == firstMessage->name()) {
                        delete m_instanceEventsList[j].takeFirst();

                        found = true;
                        break;
                    } else {
                        inOther = true;
                    }
                }
            }

            if (found || !inOther) {
                m_instanceEventsList[i].removeFirst();
                m_currentChart->addInstanceEvent(firstMessage);

                break;
            }

            if (found && inOther) {
                delete m_instanceEventsList[i].takeFirst();
            }
        }

        // Remove all empty stacks
        for (int i = m_instanceEventsList.size() - 1; i >= 0; --i) {
            if (m_instanceEventsList[i].isEmpty()) {
                m_instanceEventsList.remove(i);
            }
        }
    }

    m_instanceEventsList.clear();
}

/*!
   Returns thestring, if it is a denominator keyword match
 */
QString MscParserVisitor::denominatorString(const QString &name) const
{
    if (name.compare("system", Qt::CaseInsensitive) == 0 || name.compare("block", Qt::CaseInsensitive) == 0
        || name.compare("process", Qt::CaseInsensitive) == 0 || name.compare("service", Qt::CaseInsensitive) == 0) {
        return name;
    }

    return {};
}
