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

MscParserVisitor::MscParserVisitor(antlr4::CommonTokenStream *tokens)
    : m_model(new MscModel), m_tokens(tokens)
{
}

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
    auto docName = ::treeNodeToString(context->NAME());
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
                //qDebug() << "CIF comment on" << docName << ":" << line;
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

antlrcpp::Any MscParserVisitor::visitMessageSequenceChart(MscParser::MessageSequenceChartContext *context)
{
    auto chart = new MscChart(::treeNodeToString(context->NAME()));
    if (m_currentDocument == nullptr) {
        m_model->addChart(chart);
    } else {
        m_currentDocument->addChart(chart);
    }

    m_currentChart = chart;
    // add all instances first, so messages can reference them
    if (context->mscBody()) {
        for (auto instanceDeclCtx : context->mscBody()->instanceDeclStatement()) {
            addInstance(instanceDeclCtx->instance());
        }
    }

    auto result = visitChildren(context);

    orderInstanceEvents();

    m_currentChart = nullptr;
    return result;
}

antlrcpp::Any MscParserVisitor::visitInstance(MscParser::InstanceContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    const QString name = ::treeNodeToString(context->NAME());
    if (name.isEmpty()) {
        return visitChildren(context);
    }

    m_currentInstance = m_currentChart->instanceByName(name);
    m_currentMessage = nullptr;

    auto result = visitChildren(context);

    resetInstanceEvents();

    return result;
}

antlrcpp::Any MscParserVisitor::visitCoregion(MscParser::CoregionContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    // TODO: Add these to the stack
    //new MscCoregion(MscCoregion::Type::Begin);
    auto rc = visitChildren(context);
    //new MscCoregion(MscCoregion::Type::End);

    return rc;
}

antlrcpp::Any MscParserVisitor::visitMscEvent(MscParser::MscEventContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    MscMessage::Parameters parameters;
    QString name = ::treeNodeToString(context->NAME());

    if (context->messageIdentification()) {
        name = ::treeNodeToString(context->messageIdentification()->NAME(0));
        parameters.name = ::treeNodeToString(context->messageIdentification()->NAME(1));

        auto *parameterList = context->messageIdentification()->parameterList();
        if (parameterList && parameterList->paramaterDefn()) {
            auto *paramaterDefn = parameterList->paramaterDefn();
            parameters.expression = ::treeNodeToString(paramaterDefn->expression());
            parameters.pattern = ::treeNodeToString(paramaterDefn->pattern());
        }
    }

    if (context->CONDITION()) {
        auto *condition = new MscCondition(name);
        condition->setShared(context->SHARED() && context->ALL());
        condition->setInstance(m_currentInstance);

        if (m_currentMessage)
            condition->setMessageName(m_currentMessage->name());

        m_currentChart->addInstanceEvent(condition);
    } else if (m_currentChart->messageByName(name) == nullptr) {
        m_currentMessage = new MscMessage(name);
        m_currentMessage->setParameters(parameters);

        if (context->IN()) {
            // is an input event
            if (context->outputAddress()) {
                MscParser::OutputAddressContext *outputAddress = context->outputAddress();
                if (outputAddress->instanceName) {
                    const QString source = QString::fromStdString(outputAddress->instanceName->getText());
                    m_currentMessage->setSourceInstance(m_currentChart->instanceByName(source));
                }
            }
            m_currentMessage->setTargetInstance(m_currentInstance);
        }

        if (context->OUT()) {
            // is an output event
            if (context->inputAddress()) {
                MscParser::InputAddressContext *inputAddress = context->inputAddress();
                if (inputAddress->instanceName) {
                    const QString target = QString::fromStdString(inputAddress->instanceName->getText());
                    m_currentMessage->setTargetInstance(m_currentChart->instanceByName(target));
                }
                m_currentMessage->setSourceInstance(m_currentInstance);
            }
        }

        m_instanceEvents.append(m_currentMessage);
    }

    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitGateDeclaration(MscParser::GateDeclarationContext *context)
{
    QScopedPointer<MscGate> gate(new MscGate());
    if (!m_currentChart) {
        return visitChildren(context);
    }

    const bool isIN(context->IN() || context->FROM());
    const bool isOUT(context->OUT() || context->TO());
    if (!isIN && !isOUT) {
        qWarning() << Q_FUNC_INFO << "A gate with no IN/FROM nor OUT/TO direction, ignored.";
        return visitChildren(context);
    }

    static constexpr size_t messageNameId = 0;
    const size_t paramNameId = context->COMMA() && context->parameterList() ? messageNameId + 1 : 0;
    const size_t instanceNameId = paramNameId + 1;

    auto readNAME = [&context](size_t id) {
        if (antlr4::tree::TerminalNode *node = context->NAME(id))
            return QString::fromStdString(node->getText());
        return QString();
    };

    const QString messageName(readNAME(messageNameId));
    const QString paramName(paramNameId ? readNAME(paramNameId) : QString());
    const QString instanceName(readNAME(instanceNameId));

    const MscGate::Direction direction = isIN ? MscGate::Direction::In : MscGate::Direction::Out;

    QVariantList params;
    if (MscParser::ParameterListContext *paramList = context->parameterList()) {
        while (paramList) {
            params << QString::fromStdString(paramList->getText());
            paramList = paramList->parameterList();
        }
    }

    gate->setName(messageName);
    gate->setInstanceName(instanceName);
    gate->setParamName(paramName);
    gate->setParams(params);
    gate->setDirection(direction);

    m_currentChart->addGate(gate.take());
    return visitChildren(context);
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

    m_instanceEvents << timer.take();

    return visitChildren(context);
}

void MscParserVisitor::addInstance(MscParser::InstanceContext *context)
{
    Q_ASSERT(m_currentChart != nullptr);
    if (context->gateDeclaration()) {
        return;
    }

    const QString name = ::treeNodeToString(context->NAME());

    MscInstance *instance = m_currentChart->instanceByName(name);
    if (!instance) {
        instance = new MscInstance(name);
    }
    if (context->instanceKind() != nullptr) {
        QString kind = ::treeNodeToString(context->instanceKind()->NAME(0));
        instance->setKind(kind);

        if (context->instanceKind()->NAME(1) != nullptr) {
            instance->setInheritance(QString::fromStdString(context->instanceKind()->NAME(1)->getText()));
        }
    }
    m_currentChart->addInstance(instance);
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
