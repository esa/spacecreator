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
class MscMessage;
class MscModel;
}

class MscParserVisitor : public MscBaseVisitor
{
public:
    MscParserVisitor();
    ~MscParserVisitor();

    // Returns the model that is built up during parsing
    // The caller has to take over ownership of the model object
    msc::MscModel *detachModel();

    antlrcpp::Any visitFile(MscParser::FileContext *context) override;
    antlrcpp::Any visitMscDocument(MscParser::MscDocumentContext *context) override;
    antlrcpp::Any visitMessageSequenceChart(MscParser::MessageSequenceChartContext *context) override;
    antlrcpp::Any visitInstance(MscParser::InstanceContext *context) override;
    antlrcpp::Any visitMscEvent(MscParser::MscEventContext *context) override;

private:
    using Messages = QVector<msc::MscMessage *>;
    using MessagesList = QVector<Messages>;

    void addInstance(MscParser::InstanceContext *context);

    void resetMessages();
    void orderMessages();

    msc::MscModel *m_model = nullptr;

    msc::MscDocument *m_currentDocument = nullptr;
    msc::MscChart *m_currentChart = nullptr;
    msc::MscInstance *m_currentInstance = nullptr;

    Messages m_messages;
    MessagesList m_messagesList;
};

#endif // MSCPARSERVISITOR_H
