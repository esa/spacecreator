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
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"

#include <QDebug>

#include <string>

using namespace msc;

MscParserVisitor::MscParserVisitor()
    : m_model(new MscModel)
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
    const std::string docName = context->NAME()->getText();
    doc->setName(QString::fromStdString(docName));

    m_currentDocument = doc;
    auto ret = visitChildren(context);
    m_currentDocument = parent;
    return ret;
}

antlrcpp::Any MscParserVisitor::visitMessageSequenceChart(MscParser::MessageSequenceChartContext *context)
{
    auto chart = new MscChart();
    if (m_currentDocument == nullptr) {
        m_model->addChart(chart);
    } else {
        m_currentDocument->addChart(chart);
    }
    if (context->NAME()) {
        const std::string mscName = context->NAME()->getText();
        chart->setName(QString::fromStdString(mscName));
    }

    m_currentChart = chart;
    // add all instances first, so messages can reference them
    for (auto instanceDeclCtx : context->mscBody()->instanceDeclStatement()) {
        addInstance(instanceDeclCtx->instance());
    }

    auto result = visitChildren(context);

    orderMessages();

    m_currentChart = nullptr;
    return result;
}

antlrcpp::Any MscParserVisitor::visitInstance(MscParser::InstanceContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    const QString name = QString::fromStdString(context->NAME(0)->getText());
    m_currentInstance = m_currentChart->instanceByName(name);
    auto result = visitChildren(context);

    resetMessages();

    return result;
}

antlrcpp::Any MscParserVisitor::visitMscEvent(MscParser::MscEventContext *context)
{
    if (!m_currentChart) {
        return visitChildren(context);
    }

    QString name;
    if (context->NAME()) {
        name = QString::fromStdString(context->NAME()->getText());
    }
    if (context->messageIdentification()) {
        name = QString::fromStdString(context->messageIdentification()->NAME(0)->getText());
    }

    if (m_currentChart->messageByName(name) == nullptr) {
        auto message = new MscMessage(name);
        if (context->IN()) {
            // is an input event
            if (context->outputAddress()) {
                MscParser::OutputAddressContext *outputAddress = context->outputAddress();
                if (outputAddress->instanceName) {
                    const QString source = QString::fromStdString(outputAddress->instanceName->getText());
                    message->setSourceInstance(m_currentChart->instanceByName(source));
                }
            }
            message->setTargetInstance(m_currentInstance);
        }
        if (context->OUT()) {
            // is an output event
            if (context->inputAddress()) {
                MscParser::InputAddressContext *inputAddress = context->inputAddress();
                if (inputAddress->instanceName) {
                    const QString target = QString::fromStdString(inputAddress->instanceName->getText());
                    message->setTargetInstance(m_currentChart->instanceByName(target));
                }
                message->setSourceInstance(m_currentInstance);
            }
        }
        m_messages.append(message);

        m_currentInstance->addEvent(MscInstance::MscMessagePtr(message->clone()));
    }
    return visitChildren(context);
}

void MscParserVisitor::addInstance(MscParser::InstanceContext *context)
{
    Q_ASSERT(m_currentChart != nullptr);
    if (context->GATE()) {
        return;
    }

    const QString name = QString::fromStdString(context->NAME(0)->getText());

    MscInstance *instance = m_currentChart->instanceByName(name);
    if (!instance) {
        instance = new MscInstance(name);
    }
    if (context->instanceKind() != nullptr) {
        QString kind = QString::fromStdString(context->instanceKind()->NAME(0)->getText());
        instance->setKind(kind);

        if (context->instanceKind()->NAME(1) != nullptr) {
            instance->setInheritance(QString::fromStdString(context->instanceKind()->NAME(1)->getText()));
        }
    }
    m_currentChart->addInstance(instance);
}

void MscParserVisitor::resetMessages()
{
    if (!m_messages.isEmpty()) {
        m_messagesList.append(m_messages);
        m_messages.clear();
    }
}

void MscParserVisitor::orderMessages()
{
    bool found;

    while (m_messagesList.size()) {
        found = false;

        for (int i = 0; i < m_messagesList.size(); ++i) {
            bool inOther = false;

            // annotate the first element of the list
            auto firstMessage = m_messagesList[i][0];

            // look first elements of others list
            for (int j = i + 1; j < m_messagesList.size(); ++j) {
                if (std::count_if(m_messagesList[j].begin(), m_messagesList[j].end(), [&](MscMessage *m) { return m->name() == firstMessage->name(); })) {
                    if (m_messagesList[j][0]->name() == firstMessage->name()) {
                        delete m_messagesList[j].takeFirst();

                        if (m_messagesList[j].size() == 0) {
                            m_messagesList.remove(j);
                        }

                        found = true;
                        break;
                    } else {
                        inOther = true;
                    }
                }
            }

            if (found || !inOther) {
                m_messagesList[i].removeFirst();
                m_currentChart->addMessage(firstMessage);

                if (m_messagesList[i].size() == 0) {
                    m_messagesList.remove(i);
                }

                break;
            }

            if (found && inOther) {
                delete m_messagesList[i].takeFirst();
            }
        }
    }

    m_messagesList.clear();
}
