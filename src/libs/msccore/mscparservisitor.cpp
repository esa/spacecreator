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

#include "mscparservisitor.h"

#include "datastatement.h"
#include "exceptions.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccomment.h"
#include "msccondition.h"
#include "msccoregion.h"
#include "msccreate.h"
#include "mscdocument.h"
#include "mscgate.h"
#include "mscinstance.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "msctimer.h"

#include <QDebug>
#include <QRegularExpression>
#include <QScopedPointer>
#include <string>

namespace {

static QString sourceTextForContext(antlr4::ParserRuleContext *context)
{
    if (context != nullptr) {
        antlr4::CharStream *cs = context->start->getInputStream();
        size_t startIndex = context->start->getStartIndex();
        size_t stopIndex = context->stop != nullptr ? context->stop->getStopIndex() : -1;
        antlr4::misc::Interval interval(startIndex, stopIndex);
        std::string str = cs->getText(interval);
        return QString::fromStdString(str);
    }
    return QString();
}

template<typename T>
static QString treeNodeToString(T *node)
{
    if (node != nullptr) {
        QString text = QString::fromStdString(node->getText());

        // filtering hyperlinks: `oid`1`351`enterAmount`
        static const QRegularExpression filter("^`oid`\\d{1}`\\d{1,}`\\w+`$");
        if (text.indexOf(filter) == 0) {
            text.chop(1);
            text = text.mid(text.lastIndexOf("`") + 1);
        }

        return text;
    }
    return QString();
};

static QString nameToString(MscParser::NameContext *nameNode)
{
    QString name;
    if (nameNode) {
        if (nameNode->NAME().empty()) {
            name = ::treeNodeToString(nameNode);
        } else {
            for (auto nameToken : nameNode->NAME()) {
                if (!name.isEmpty()) {
                    name += " ";
                }
                name += ::treeNodeToString(nameToken);
            }
        }
    }
    return name;
}

/*!
   Helper function to store a MSC CHARACTERSTRING in a QString
 */
static QString charactersToString(antlr4::tree::TerminalNode *characterString)
{
    QString string = treeNodeToString(characterString);

    // remove the single quotes
    if (string.startsWith("'")) {
        string.remove(0, 1);
    }
    if (string.endsWith("'")) {
        string.remove(string.size() - 1, 1);
    }
    return string;
}

static msc::MscComment *parseComment(msc::MscEntity *entity, MscParser::EndContext *end)
{
    if (!entity || !end)
        return nullptr;

    if (end->comment())
        return entity->setCommentString(charactersToString(end->comment()->CHARACTERSTRING()));

    return nullptr;
}

}

using namespace msc;

MscParserVisitor::MscParserVisitor(antlr4::CommonTokenStream *tokens)
    : m_model(new MscModel)
    , m_tokens(tokens)
    , m_cifParser(new cif::CifParser())
{
}

MscParserVisitor::~MscParserVisitor()
{
    delete m_cifParser;
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
    if (context->REFERENCED()) {
        // ignore referenced documents (spec extension) for now
        qWarning() << "Referenced documents are not supported";
        return visitChildren(context);
    }

    if (!context->documentHead()) {
        throw ParserException(QObject::tr("No document head node in the MscDocument"));
    }

    MscDocument *parent = m_currentDocument;

    auto doc = new MscDocument();
    doc->setHierarchyType(MscDocument::HierarchyUnkown);
    const auto docName = ::nameToString(context->documentHead()->name());
    doc->setName(docName);

    if (parent == nullptr) {
        m_model->addDocument(doc);
    } else {
        bool ok = parent->addDocument(doc, false);
        if (!ok) {
            throw ParserException(QString("Unable to add document %1 to %2").arg(docName, parent->name()));
        }
    }

    auto handleComment = [=](antlr4::Token *token) {
        const QString &line = dropCommentBraces(readCommentLine(token)).toLower();
        if (!line.isEmpty()) {
            if (line.startsWith("msc ")) {
                // Handle MSC here
                // This is really simple first version of an MSC hierarchy parser
                // For something more complete, we might need to really parse this
                if (line.contains("goal") || line.contains("time")) {
                    // Not supported
                } else if (line.contains("leaf")) {
                    doc->setHierarchyType(MscDocument::HierarchyLeaf);
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

antlrcpp::Any MscParserVisitor::visitDocumentHead(MscParser::DocumentHeadContext *context)
{
    if (m_currentDocument) {
        if (msc::MscComment *comment = parseComment(m_currentDocument, context->end())) {
            m_comments.append(comment);
        }
    }

    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitInstanceItem(MscParser::InstanceItemContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    const QString &name = ::treeNodeToString(context->instanceName);
    m_currentInstance = m_currentChart->instanceByName(name);
    auto result = visitChildren(context);
    m_currentInstance = nullptr;
    return result;
}

antlrcpp::Any MscParserVisitor::visitMessageSequenceChart(MscParser::MessageSequenceChartContext *context)
{
    QString mscName;
    if (context->mscHead()) {
        mscName = ::nameToString(context->mscHead()->name());
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
            } else {
                addInstance(::nameToString(headCtx->name()));
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
    // Add document comments
    for (MscInstanceEvent *comment : qAsConst(m_comments)) {
        m_currentChart->addInstanceEvent(comment, {});
    }
    m_comments.clear();

    auto result = visitChildren(context);

    insertInstanceEvents();

    m_currentChart = nullptr;
    return result;
}

antlrcpp::Any MscParserVisitor::visitMscHead(MscParser::MscHeadContext *context)
{
    if (auto comment = parseComment(m_currentChart, context->end())) {
        m_currentChart->addInstanceEvent(comment, {});
    }

    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitInstanceKind(MscParser::InstanceKindContext *context)
{
    if (m_currentInstance) {
        QString kind = ::treeNodeToString(context->NAME(0));
        const QString denominator = denominatorString(kind);
        if (denominator.isEmpty()) {
            m_currentInstance->setKind(kind);
        } else {
            m_currentInstance->setDenominator(denominator);
            kind.clear();
            for (size_t i = 1; i < context->NAME().size(); ++i) {
                if (kind.isEmpty())
                    kind = ::treeNodeToString(context->NAME(i));
                else
                    kind += QStringLiteral(" ") + ::treeNodeToString(context->NAME(i));
            }
            m_currentInstance->setKind(kind);
        }
    }

    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitEventDefinition(MscParser::EventDefinitionContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    const QString &name = ::treeNodeToString(context->instanceName);
    if (name.isEmpty()) {
        return visitChildren(context);
    }

    m_currentInstance = m_currentChart->instanceByName(name);
    auto ret = visitChildren(context);
    m_currentInstance = nullptr;
    return ret;
}

antlrcpp::Any MscParserVisitor::visitInstanceEvent(MscParser::InstanceEventContext *context)
{
    auto ret = visitChildren(context);
    m_currentEvent = nullptr;
    return ret;
}

antlrcpp::Any MscParserVisitor::visitOrderableEvent(MscParser::OrderableEventContext *context)
{
    auto ret = visitChildren(context);

    if (!context->end().empty()) {
        if (auto comment = parseComment(m_currentEvent, context->end().back())) {
            m_currentChart->addInstanceEvent(comment, {});
        }
    }

    m_currentMessage = nullptr;
    m_currentEvent = nullptr;
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
            } else {
                if (context->name())
                    name = ::nameToString(context->name());
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

    if (auto comment = parseComment(m_currentInstance, context->end())) {
        m_currentChart->addInstanceEvent(comment, {});
    }

    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitInstanceEndStatement(MscParser::InstanceEndStatementContext *context)
{
    resetInstanceEvents();
    m_currentInstance = nullptr;
    return visitChildren(context);
}

MscMessage *MscParserVisitor::lookupMessageIn(const MscMessage *msg)
{
    for (int i = 0; i < m_instanceEventsList.size(); ++i)
        for (int j = 0; j < m_instanceEventsList.at(i).size(); ++j) {
            MscInstanceEvent *event = m_instanceEventsList.at(i).at(j);
            if (event->entityType() != MscEntity::EntityType::Message)
                continue;

            MscMessage *message = static_cast<MscMessage *>(event);
            if (msg->isSame(message) && !message->isConnected())
                return message;
        }
    return nullptr;
}

MscMessage *MscParserVisitor::lookupMessageOut(const MscMessage *msg)
{
    for (int i = 0; i < m_instanceEventsList.size(); ++i)
        for (int j = 0; j < m_instanceEventsList.at(i).size(); ++j) {
            MscInstanceEvent *event = m_instanceEventsList.at(i).at(j);
            if (event->entityType() != MscEntity::EntityType::Message)
                continue;

            MscMessage *message = static_cast<MscMessage *>(event);
            if (msg->isSame(message) && !message->isConnected())
                return message;
        }
    return nullptr;
}

antlrcpp::Any MscParserVisitor::visitMessageOutput(MscParser::MessageOutputContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    const QString name = ::nameToString(context->msgIdentification()->messageName);
    MscMessage *message = new MscMessage(name);
    m_currentMessage = message;
    auto result = visitChildren(context); // get instance name and parameters

    m_currentMessage->setSourceInstance(m_currentInstance);
    MscInstance *targetInstance = nullptr;
    MscParser::InputAddressContext *inputAddress = context->inputAddress();
    if (inputAddress && inputAddress->instanceName) {
        const QString target = ::treeNodeToString(inputAddress->instanceName);
        targetInstance = m_currentChart->instanceByName(target);
        m_currentMessage->setTargetInstance(targetInstance);
    }

    m_currentMessage = lookupMessageIn(message);
    if (m_currentMessage == nullptr) {
        m_currentMessage = message; // this is a new message
    } else {
        delete message; // the message already existed - delete this
    }

    m_currentEvent = m_currentMessage;
    m_instanceEvents.append(m_currentEvent);

    m_currentMessage->m_descrOut.to = targetInstance;
    m_currentMessage->m_descrOut.from = m_currentInstance;
    m_currentMessage->m_descrOut.m_sourceLineInfo.m_line = context->start->getLine();
    m_currentMessage->m_descrOut.m_sourceLineInfo.m_pos = context->start->getCharPositionInLine();

    return result;
}

antlrcpp::Any MscParserVisitor::visitMessageInput(MscParser::MessageInputContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    const QString name = ::treeNodeToString(context->msgIdentification()->messageName);
    MscMessage *message = new MscMessage(name);
    m_currentMessage = message;
    auto result = visitChildren(context); // get instance name and parameters

    m_currentMessage->setTargetInstance(m_currentInstance);
    MscInstance *sourceInstance = nullptr;
    MscParser::OutputAddressContext *outputAddress = context->outputAddress();
    if (outputAddress && outputAddress->instanceName) {
        const QString source = ::treeNodeToString(outputAddress->instanceName);
        sourceInstance = m_currentChart->instanceByName(source);
        m_currentMessage->setSourceInstance(sourceInstance);
    }

    m_currentMessage = lookupMessageOut(message);
    if (m_currentMessage == nullptr) {
        m_currentMessage = message; // this is a new message
    } else {
        delete message; // the message already existed - delete this
    }

    m_currentEvent = m_currentMessage;
    m_instanceEvents.append(m_currentEvent);

    m_currentMessage->m_descrIn.from = sourceInstance;
    m_currentMessage->m_descrIn.to = m_currentInstance;
    m_currentMessage->m_descrIn.m_sourceLineInfo.m_line = context->start->getLine();
    m_currentMessage->m_descrIn.m_sourceLineInfo.m_pos = context->start->getCharPositionInLine();

    return result;
}

antlrcpp::Any MscParserVisitor::visitMsgIdentification(MscParser::MsgIdentificationContext *context)
{
    if (!m_currentMessage) {
        return visitChildren(context);
    }

    if (context->messageInstanceName) {
        m_currentMessage->setMessageInstanceName(::treeNodeToString(context->messageInstanceName));
    }

    m_currentMessage->setParameters(readParameterList(context->parameterList()));
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
        name = nameToString(conditionText->conditionNameList()->name());
    }
    auto *condition = new MscCondition(name);
    m_currentEvent = condition;

    if (!context->shared().empty()) {
        MscParser::SharedContext *shared = context->shared().at(0);
        condition->setShared(shared->ALL());
    }
    condition->setInstance(m_currentInstance);

    m_instanceEvents.append(condition);

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

    auto statement = new msc::DataStatement(action);
    if (statementCtx->defineStatement()) {
        statement->setType(msc::DataStatement::StatementType::Define);
        statement->setVariableString(::treeNodeToString(statementCtx->defineStatement()->variableString()->STRING()));
        action->addDataStatement(statement);
    }
    if (statementCtx->undefineStatement()) {
        statement->setType(msc::DataStatement::StatementType::UnDefine);
        statement->setVariableString(treeNodeToString(statementCtx->undefineStatement()->variableString()->STRING()));
        action->addDataStatement(statement);
    }
    if (statementCtx->binding()) {
        qWarning() << "Formal binding action types is not supported";
        delete statement;
    }

    appendDataStatement(action, statementList->dataStatementList());
}

antlrcpp::Any MscParserVisitor::visitActionStatement(MscParser::ActionStatementContext *context)
{
    if (!m_currentChart || !m_currentInstance) {
        return visitChildren(context);
    }

    auto action = new MscAction;
    m_currentEvent = action;
    action->setInstance(m_currentInstance);
    if (MscParser::InformalActionContext *iaCtx = context->informalAction()) {
        action->setActionType(MscAction::ActionType::Informal);
        if (iaCtx->CHARACTERSTRING()) {
            action->setInformalAction(charactersToString(iaCtx->CHARACTERSTRING()));
        } else {
            if (iaCtx->functionText().empty()) {
                const QString &informalAction = nameToString(iaCtx->name());
                action->setInformalAction(informalAction);
            } else {
                action->setInformalAction(::sourceTextForContext(iaCtx));
            }
        }
    } else {
        action->setActionType(MscAction::ActionType::Formal);
        appendDataStatement(action, context->dataStatementList());
    }

    m_instanceEvents.append(action);

    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitCreate(MscParser::CreateContext *context)
{
    if (context->CREATE()) {
        QString name = ::treeNodeToString(context->NAME());

        // find duplicate create name
        auto isDuplicate = [&](const MscInstanceEvent *event) {
            if (event->entityType() == MscEntity::EntityType::Create) {
                const MscCreate *message = static_cast<const MscCreate *>(event);
                return message->messageType() == MscMessage::MessageType::Create
                        && message->targetInstance()->name() == name;
            }
            return false;
        };

        auto checkForDuplicates = [&name, &isDuplicate](const InstanceEvents &instanceEvents) {
            auto find = std::find_if(instanceEvents.cbegin(), instanceEvents.cend(), isDuplicate);
            if (find != instanceEvents.cend()) {
                throw ParserException(QObject::tr("Incorrect (duplicate) create name '%1'").arg(name));
            }
        };

        for (int i = 0; i < m_instanceEventsList.size(); ++i)
            checkForDuplicates(m_instanceEventsList.at(i));
        checkForDuplicates(m_instanceEvents);

        auto *createInstance = m_currentChart->instanceByName(name);
        if (!createInstance) {
            throw ParserException(QObject::tr("Incorrect (unknown) create name '%1'").arg(name));
        }

        createInstance->setExplicitCreator(m_currentInstance);

        m_currentMessage = new MscCreate();
        m_currentEvent = m_currentMessage;
        m_currentMessage->setSourceInstance(m_currentInstance);
        m_currentMessage->setTargetInstance(createInstance);
        m_currentMessage->m_descrOut.to = createInstance;
        m_currentMessage->m_descrOut.from = m_currentInstance;
        m_currentMessage->m_descrIn.to = m_currentInstance;
        m_currentMessage->m_descrIn.from = createInstance;

        m_currentMessage->setMessageInstanceName(name); // TODO: use the createInstance's name instead?

        m_currentMessage->setParameters(readParameterList(context->parameterList()));

        m_instanceEvents.append(m_currentMessage);
    }

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

antlrcpp::Any MscParserVisitor::visitMessageDecl(MscParser::MessageDeclContext *context)
{
    if (m_currentDocument) {
        MscMessageDeclarationList *mdl = m_currentDocument->messageDeclarations();
        auto md = new MscMessageDeclaration(mdl);
        QStringList names;
        MscParser::MessageNameListContext *nameListContext = context->messageNameList();
        while (nameListContext) {
            names.append(::nameToString(nameListContext->name()));
            nameListContext = nameListContext->messageNameList();
        }
        md->setNames(names);
        md->setTypeRefList(readTypeRefList(context->typeRefList()));
        mdl->append(md);
    }
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitStartCoregion(MscParser::StartCoregionContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    MscCoregion *coregion = new MscCoregion(MscCoregion::Type::Begin);
    coregion->setInstance(m_currentInstance);
    m_instanceEvents.append(coregion);
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitEndCoregion(MscParser::EndCoregionContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    MscCoregion *coregion = new MscCoregion(MscCoregion::Type::End);
    coregion->setInstance(m_currentInstance);
    m_instanceEvents.append(coregion);
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitTimerStatement(MscParser::TimerStatementContext *context)
{
    if (!m_currentChart || !m_currentInstance) {
        return visitChildren(context);
    }

    MscTimer *timer = new MscTimer();
    m_currentEvent = timer;
    timer->setInstance(m_currentInstance);
    if (MscParser::StartTimerContext *startTimer = context->startTimer()) {
        timer->setTimerType(MscTimer::TimerType::Start);
        timer->setName(::nameToString(startTimer->name()));
        timer->setTimerInstanceName(::treeNodeToString(context->startTimer()->timerInstanceName));
    } else if (MscParser::StopTimerContext *stopTimer = context->stopTimer()) {
        timer->setTimerType(MscTimer::TimerType::Stop);
        timer->setName(::nameToString(stopTimer->name()));
        timer->setTimerInstanceName(::treeNodeToString(context->stopTimer()->timerInstanceName));
    } else if (MscParser::TimeoutContext *timeout = context->timeout()) {
        timer->setTimerType(MscTimer::TimerType::Timeout);
        timer->setName(::nameToString(timeout->name()));
        timer->setTimerInstanceName(::treeNodeToString(context->timeout()->timerInstanceName));
    } else {
        qWarning() << Q_FUNC_INFO << "Bad timer declaration";
        throw ParserException(QObject::tr("Bad timer declaration '%1'").arg(::treeNodeToString(context)));
    }

    m_instanceEvents.append(timer);

    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitCoregion(MscParser::CoregionContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    if (context->CONCURRENT()) {
        MscCoregion *coregion = new MscCoregion(MscCoregion::Type::Begin);
        m_currentEvent = coregion;
        coregion->setInstance(m_currentInstance);
        m_instanceEvents.append(coregion);
    }
    if (context->ENDCONCURRENT()) {
        MscCoregion *coregion = new MscCoregion(MscCoregion::Type::End);
        m_currentEvent = coregion;
        coregion->setInstance(m_currentInstance);
        m_instanceEvents.append(coregion);
    }

    return visitChildren(context);
}

void MscParserVisitor::addInstance(const QString &name)
{
    m_currentChart->makeInstance(name);
}

void MscParserVisitor::resetInstanceEvents()
{
    if (m_currentInstance != nullptr) {
        m_instanceEventsList.append(m_instanceEvents);
    }
    m_instanceEvents.clear();
}

/*!
   Fixes the ordering of events
Algorithm:
- First fix create-messages, to be on the created instance-list as the first event
- Loop through all instance event lists and check the first event, if it can be added
  Adding it is possible if
    + It has no dependencies to any other instance (action, single condition, timer, ...)
    + The event is on top of the corresponding instance-list as well
*/
void MscParserVisitor::insertInstanceEvents()
{
    m_cifBlocks.clear();

    // Copy create messages on top of the created instance (event list) - so they are handled the same as messages
    for (int i = 0; i < m_instanceEventsList.size(); ++i) {
        const InstanceEvents &events = m_instanceEventsList[i];
        for (MscInstanceEvent *event : events) {
            if (event->entityType() == MscEntity::EntityType::Create) {
                auto create = static_cast<MscCreate *>(event);
                const int targetInstanceIdx = m_currentChart->indexOfInstance(create->targetInstance());
                if (targetInstanceIdx != i && targetInstanceIdx >= 0) { // do not insert in current list
                    m_instanceEventsList[targetInstanceIdx].prepend(create);
                    if (create->sourceInstance()->explicitCreator() == create->targetInstance()) {
                        throw ParserException("Deadlok in create the events - error ins the MSC file");
                    }
                }
            }
        }
    }

    // Fix shared conditions
    auto replaceDuplicateCondition = [&](MscCondition *condition, InstanceEvents &events) {
        for (int k = 0; k < events.size(); ++k) {
            if (events[k]->entityType() == msc::MscEntity::EntityType::Condition) { }
            auto condition2 = static_cast<MscCondition *>(events[k]);
            if (condition2->shared() && condition != condition2 && condition->name() == condition2->name()) {
                // replace the duplicate
                delete condition2;
                events[k] = condition;
            }
        }
    };

    for (int i = 0; i < m_instanceEventsList.size(); ++i) {
        for (MscInstanceEvent *event : m_instanceEventsList[i]) {
            if (event->entityType() == msc::MscEntity::EntityType::Condition) {
                auto condition = static_cast<MscCondition *>(event);
                if (condition->shared()) {
                    // Search for same condition in other (instance) lists
                    for (int j = i + 1; j < m_instanceEventsList.size(); ++j) {
                        replaceDuplicateCondition(condition, m_instanceEventsList[j]);
                    }
                }
            }
        }
    }

    // Set all the events in the chart
    QHash<MscInstance *, QVector<MscInstanceEvent *>> events;
    for (int i = 0; i < m_instanceEventsList.size(); ++i) {
        MscInstance *instance = m_currentChart->instances().at(i);
        events[instance] = {};
        for (MscInstanceEvent *event : m_instanceEventsList[i]) {
            events[instance].append(event);
        }
    }
    QVector<MscInstanceEvent *> orphans = m_currentChart->orphanEvents();
    m_currentChart->setInstanceEvents(events, orphans);

    m_instanceEventsList.clear();

    checkMessagesDoubleNotation();
}

void MscParserVisitor::checkMessagesDoubleNotation() const
{
    auto instanceName = [](const MscInstance *instance) {
        return instance ? instance->name() : QObject::tr("[Unknown]");
    };

    auto prepareErrorMessage = [&instanceName](const MscMessage *message, const MscMessage::Flow &foundFlow) {
        const bool missedIn = foundFlow == message->m_descrOut;

        const QString missedLine =
                QString("%1 %2 %3 %4;")
                        .arg(QLatin1String(missedIn ? "in" : "out"), message->fullName(),
                                QLatin1String(missedIn ? "from" : "to"),
                                instanceName(missedIn ? message->sourceInstance() : message->targetInstance()));

        return QObject::tr("@%1:%2 A missed reference to this message is required ('%3' in %4).")
                .arg(QString::number(foundFlow.m_sourceLineInfo.m_line),
                        QString::number(foundFlow.m_sourceLineInfo.m_pos), missedLine,
                        instanceName(missedIn ? message->targetInstance() : message->sourceInstance()));
    };

    for (const MscMessage *message : m_currentChart->messages()) {
        if (!message->isGlobal()) { // Ignore messages to/from Env
            QString errorMessage;
            if (!message->m_descrIn.isComplete()) {
                errorMessage = prepareErrorMessage(message, message->m_descrOut);
            }
            if (!message->m_descrOut.isComplete()) {
                errorMessage = prepareErrorMessage(message, message->m_descrIn);
            }

            if (!errorMessage.isEmpty())
                throw ParserException(errorMessage);
        }
    }
}

/*!
   Returns the string, if it is a denominator keyword match
 */
QString MscParserVisitor::denominatorString(const QString &name) const
{
    if (name.compare("system", Qt::CaseInsensitive) == 0 || name.compare("block", Qt::CaseInsensitive) == 0
            || name.compare("process", Qt::CaseInsensitive) == 0 || name.compare("service", Qt::CaseInsensitive) == 0) {
        return name;
    }

    return {};
}

QString MscParserVisitor::readCommentLine(const antlr4::Token *const token)
{
    if (!token || token->getChannel() != m_commentsStreamNum)
        return QString();

    return QString::fromStdString(token->getText()).trimmed();
}

QString MscParserVisitor::dropCommentBraces(const QString &srcLine)
{
    QString line(srcLine);
    if (!line.isEmpty()) {
        if (line.startsWith("/*")) {
            line = line.mid(2);
        }
        if (line.endsWith("*/")) {
            line.chop(2);
        }
        line = line.trimmed();
    }
    return line;
}

QStringList MscParserVisitor::readComments(const std::vector<antlr4::Token *> &tokens) const
{
    QStringList lines;
    for (const antlr4::Token *const token : tokens)
        lines << dropCommentBraces(readCommentLine(token));
    return lines;
}

void MscParserVisitor::storePrecedingCif(antlr4::ParserRuleContext *ctx)
{
    if (!ctx)
        return;

    const auto &precedingTokens = getHiddenCommentTokensToLeft(static_cast<int>(ctx->start->getTokenIndex()));
    if (precedingTokens.empty())
        return;

    const QStringList &left = readComments(precedingTokens);
    if (left.isEmpty())
        return;

    const QVector<cif::CifBlockShared> &cifBlocks = m_cifParser->readCifBlocks(left);
    for (const cif::CifBlockShared &cifBlock : cifBlocks) {
        const QString &hashKey = cifBlock->hashKey();
        if (!m_cifBlockKeys.contains(hashKey)) {
            if (cifBlock->blockType() == cif::CifLine::CifType::Text) {
                if (m_currentChart) { /// TODO: check support for entities other than MscChart
                    MscComment *comment = new MscComment(m_currentChart);
                    comment->attachTo(m_currentChart);
                    comment->addCif(cifBlock);
                    m_currentChart->addInstanceEvent(comment, {});
                }
            } else {
                m_cifBlocks.append(cifBlock);
            }
            m_cifBlockKeys.append(hashKey);
        }
    }
}

antlrcpp::Any MscParserVisitor::visitEnd(MscParser::EndContext *ctx)
{
    storePrecedingCif(ctx);

    // While currently the order of rules parsing seems to be correct,
    // dbg output below may be useful for upcoming CIF improvements/tuning.
    // Please keep it as a dead code for a while
    // TODO: Remove after completing (more or less) CIF support
    // if (antlr4::ParserRuleContext *pParentRule = dynamic_cast<antlr4::ParserRuleContext *>(ctx->parent))
    // {
    //     qDebug() << "visitEnd of:" << msc_dbg::ruleNameFromIndex(pParentRule->getRuleIndex());
    // } else {
    //     qDebug() << "visitEnd of:" << msc_dbg::ruleNameFromIndex(ctx->getRuleIndex());
    // }

    if (!m_cifBlocks.isEmpty()) {
        int ctr(0);
        for (const cif::CifBlockShared &cifBlock : m_cifBlocks) {
            const QString marker = QString("CB %1/%2").arg(++ctr).arg(m_cifBlocks.size());
            if (cifBlock->isPeculiar()) {
                if (m_currentChart) {
                    MscComment *comment = new MscComment(m_currentChart);
                    comment->attachTo(m_currentChart);
                    comment->addCif(cifBlock);
                    m_currentChart->addInstanceEvent(comment, {});
                }
            } else if (MscEntity *targetEntity = cifTarget()) {
                if (cifBlock->hasPayloadFor(cif::CifLine::CifType::Comment)) {
                    MscComment *comment = targetEntity->comment();
                    if (!comment) {
                        comment = new MscComment(targetEntity);
                        comment->attachTo(targetEntity);
                        m_currentChart->addInstanceEvent(comment, {});
                    }
                    comment->addCif(cifBlock);
                } else {
                    targetEntity->addCif(cifBlock);
                }
            } else {
                static const QString wrn = QObject::tr("CIF target unknown! Line: %1; pos: %2");
                qWarning() << marker
                           << wrn.arg(QString::number(ctx->start->getLine()),
                                      QString::number(ctx->start->getCharPositionInLine()));
            }
        }

        m_cifBlocks.clear();
    }

    return MscParserVisitor::visitChildren(ctx);
}

antlrcpp::Any MscParserVisitor::visitChildren(antlr4::tree::ParseTree *node)
{
    storePrecedingCif(dynamic_cast<antlr4::ParserRuleContext *>(node));
    return MscBaseVisitor::visitChildren(node);
}

msc::MscEntity *MscParserVisitor::cifTarget() const
{
    const QVector<MscEntity *> possibleTargets = { m_currentMessage, m_currentEvent, m_currentInstance, m_currentChart,
        m_currentDocument };
    for (MscEntity *target : possibleTargets) {
        if (target) {
            return target;
        }
    }
    return nullptr;
}

/*!
   Replaces antlr4::BufferedTokenStream::getHiddenTokensToLeft as that one has a bug if the first toke in a
   file is a hidden one
 */
std::vector<antlr4::Token *> MscParserVisitor::getHiddenCommentTokensToLeft(int tokenIndex)
{
    std::vector<antlr4::Token *> commentTokens;
    --tokenIndex;
    while (tokenIndex >= 0) {
        antlr4::Token *token = m_tokens->get(tokenIndex);
        if (token->getText().substr(0, 2) == "/*") {
            commentTokens.insert(commentTokens.begin(), token);
        } else {
            break;
        }
        --tokenIndex;
    }

    return commentTokens;
}

msc::MscParameterList MscParserVisitor::readParameterList(MscParser::ParameterListContext *parameterList)
{
    msc::MscParameterList parameters;
    while (parameterList && parameterList->paramaterDefn()) {

        auto *paramaterDefn = parameterList->paramaterDefn();
        const QString &expression = ::sourceTextForContext(paramaterDefn->expression());
        QString pattern = ::sourceTextForContext(paramaterDefn->pattern());
        if (pattern.isEmpty()) {
            pattern = ::sourceTextForContext(paramaterDefn->binding());
        }
        if (pattern.isEmpty()) {
            pattern = ::sourceTextForContext(paramaterDefn->sdlText());
        }
        if (pattern.isEmpty()) {
            pattern = ::sourceTextForContext(paramaterDefn->asnSequence());
        }
        if (pattern.isEmpty()) {
            pattern = ::sourceTextForContext(paramaterDefn->asnSequenceOf());
        }
        if (pattern.isEmpty()) {
            pattern = ::treeNodeToString(paramaterDefn->choiceOfChoice());
        }
        if (!expression.isEmpty() || !pattern.isEmpty()) {
            msc::MscParameter parameter(expression, pattern);
            parameters << parameter;
        }

        parameterList = parameterList->parameterList();
    }

    return parameters;
}

QStringList MscParserVisitor::readTypeRefList(MscParser::TypeRefListContext *context)
{
    QStringList result;
    while (context) {
        result.append(::nameToString(context->name()));
        context = context->typeRefList();
    }

    return result;
}
