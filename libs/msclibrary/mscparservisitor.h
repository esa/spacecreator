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

#ifndef MSCPARSERVISITOR_H
#define MSCPARSERVISITOR_H

#include <QVector>

#include "parser/MscBaseVisitor.h"

namespace msc {
class MscChart;
class MscDocument;
class MscInstance;
class MscInstanceEvent;
class MscMessage;
class MscModel;
}

class MscParserVisitor : public MscBaseVisitor
{
public:
    explicit MscParserVisitor(antlr4::CommonTokenStream *tokens = nullptr);
    ~MscParserVisitor() override;

    // Returns the model that is built up during parsing
    // The caller has to take over ownership of the model object
    msc::MscModel *detachModel();

    antlrcpp::Any visitFile(MscParser::FileContext *context) override;
    antlrcpp::Any visitMscDocument(MscParser::MscDocumentContext *context) override;
    antlrcpp::Any visitDocumentHead(MscParser::DocumentHeadContext *context) override;
    antlrcpp::Any visitInstanceItem(MscParser::InstanceItemContext *context) override;
    antlrcpp::Any visitMessageSequenceChart(MscParser::MessageSequenceChartContext *context) override;
    antlrcpp::Any visitMscHead(MscParser::MscHeadContext *context) override;
    antlrcpp::Any visitInstanceKind(MscParser::InstanceKindContext *context) override;
    antlrcpp::Any visitEventDefinition(MscParser::EventDefinitionContext *context) override;
    antlrcpp::Any visitInstanceEvent(MscParser::InstanceEventContext *context) override;
    antlrcpp::Any visitOrderableEvent(MscParser::OrderableEventContext *context) override;
    antlrcpp::Any visitInstanceHeadStatement(MscParser::InstanceHeadStatementContext *context) override;
    antlrcpp::Any visitInstanceEndStatement(MscParser::InstanceEndStatementContext *context) override;
    antlrcpp::Any visitMessageOutput(MscParser::MessageOutputContext *context) override;
    antlrcpp::Any visitMessageInput(MscParser::MessageInputContext *context) override;
    antlrcpp::Any visitMsgIdentification(MscParser::MsgIdentificationContext *context) override;
    antlrcpp::Any visitTimerStatement(MscParser::TimerStatementContext *context) override;
    antlrcpp::Any visitDefInGate(MscParser::DefInGateContext *context) override;
    antlrcpp::Any visitDefOutGate(MscParser::DefOutGateContext *context) override;
    antlrcpp::Any visitSharedCondition(MscParser::SharedConditionContext *context) override;
    antlrcpp::Any visitActionStatement(MscParser::ActionStatementContext *context) override;
    antlrcpp::Any visitCreate(MscParser::CreateContext *context) override;
    antlrcpp::Any visitStop(MscParser::StopContext *context) override;
    antlrcpp::Any visitDataDefinition(MscParser::DataDefinitionContext *context) override;
    antlrcpp::Any visitStartCoregion(MscParser::StartCoregionContext *ctx) override;
    antlrcpp::Any visitEndCoregion(MscParser::EndCoregionContext *ctx) override;
    antlrcpp::Any visitCoregion(MscParser::CoregionContext *context) override;

private:
    void addInstance(const QString &name);

    void resetInstanceEvents();
    void orderInstanceEvents();

    QString denominatorString(const QString &name) const;

    msc::MscModel *m_model = nullptr;

    msc::MscDocument *m_currentDocument = nullptr;
    msc::MscChart *m_currentChart = nullptr;
    msc::MscInstance *m_currentInstance = nullptr;
    msc::MscMessage *m_currentMessage = nullptr;
    msc::MscInstanceEvent *m_currentEvent = nullptr;

    QVector<msc::MscInstanceEvent *> m_instanceEvents;
    QVector<QVector<msc::MscInstanceEvent *>> m_instanceEventsList;

    antlr4::CommonTokenStream *m_tokens = nullptr;
};

#endif // MSCPARSERVISITOR_H
