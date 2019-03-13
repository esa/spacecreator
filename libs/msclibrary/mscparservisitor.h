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

#pragma once

#include "cif/cifparser.h"
#include "mscmessage.h"
#include "parser/MscBaseVisitor.h"

#include <QVector>

namespace msc {

class MscChart;
class MscDocument;
class MscEntity;
class MscInstance;
class MscInstanceEvent;
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
    antlrcpp::Any visitEnd(MscParser::EndContext *ctx) override;

    antlrcpp::Any visitChildren(antlr4::tree::ParseTree *node) override;

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

    typedef QVector<msc::MscInstanceEvent *> InstanceEvents;

    InstanceEvents m_instanceEvents;
    QVector<InstanceEvents> m_instanceEventsList;

    antlr4::CommonTokenStream *m_tokens = nullptr;
    msc::cif::CifParser *m_cifParser = nullptr;

    // keep it synced with Msc.g4 -
    // COMMENTLOST : '/*' .*? '*/' -> channel(2);
    static const int m_commentsStreamNum = 2;

    QStringList m_cifBlockKeys;
    QVector<msc::cif::CifBlockShared> m_cifBlocks;

    msc::MscMessage *lookupMessageIn(const QString &name, msc::MscInstance *to);
    msc::MscMessage *lookupMessageOut(const QString &name, msc::MscInstance *from);

    void checkMessagesDoubleNotation() const;

    void storePrecedingCif(antlr4::ParserRuleContext *ctx);
    static QString readCommentLine(const antlr4::Token *const token);
    static QString dropCommentBraces(const QString &line);
    QStringList readComments(const QVector<antlr4::Token *> &tokens) const;
    msc::MscEntity *cifTarget() const;
    QVector<msc::MscMessage::Parameter> readParameterList(MscParser::ParameterListContext *parameterList);
};
