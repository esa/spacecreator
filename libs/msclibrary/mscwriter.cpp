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

#include "mscwriter.h"
#include "mscchart.h"
#include "msccondition.h"
#include "msccreate.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "msctimer.h"
#include "mscaction.h"
#include "msccoregion.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

namespace msc {

MscWriter::MscWriter(QObject *parent)
    : QObject(parent)
{
}

void MscWriter::saveModel(const MscModel *model, const QString &fileName)
{
    if (model == nullptr || fileName.isEmpty())
        return;

    QFile mscFile(fileName);
    if (!mscFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&mscFile);

    setModel(model);

    int tabCount = 0;
    for (const auto *doc : model->documents())
        out << serialize(doc, tabCount++);

    for (const auto *chart : model->charts())
        out << serialize(chart);

    mscFile.close();

    setModel(nullptr);
}

void MscWriter::saveChart(const MscChart *chart, const QString &fileName)
{
    if (chart == nullptr || fileName.isEmpty())
        return;

    QFile mscFile(fileName);
    if (!mscFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&mscFile);

    out << serialize(chart);

    mscFile.close();
}

QString MscWriter::serialize(const MscInstance *instance, const QVector<MscInstanceEvent *> &instanceEvents,
                             int tabsSize)
{
    if (instance == nullptr)
        return "";

    QString events;

    QString tabString = tabs(tabsSize);
    QString header = QString("%1instance %2").arg(tabString, instance->name());
    QString footer = tabString + "endinstance;\n";

    // The rest is the internal part of this which should be indented
    ++tabsSize;

    if (!instance->kind().isEmpty()) {
        header += QString(": %1 %2;\n").arg(instance->kind(), instance->inheritance());
    } else
        header += ";\n";

    auto addCondition = [&](const QString &messageName) {
        std::for_each(instanceEvents.begin(), instanceEvents.end(), [&](MscInstanceEvent *event) {
            auto *condition = dynamic_cast<MscCondition *>(event);
            if (condition && condition->instance() == instance && condition->messageName() == messageName)
                events += serialize(condition, tabsSize);
        });
    };

    // serialize conditions with empty messageName
    addCondition("");

    for (const auto &instanceEvent : instanceEvents) {
        switch (instanceEvent->entityType()) {
        case MscEntity::EntityType::Message:
            events += serialize(static_cast<MscMessage *>(instanceEvent), instance, tabsSize);
            addCondition(instanceEvent->name());
            break;
        case MscEntity::EntityType::Timer:
            events += serialize(static_cast<MscTimer *>(instanceEvent), tabsSize);
            break;
        case MscEntity::EntityType::Coregion:
            events += serialize(static_cast<MscCoregion *>(instanceEvent), tabsSize);
            break;
        case MscEntity::EntityType::Action:
            events += serialize(static_cast<MscAction *>(instanceEvent), tabsSize);
            break;
        case MscEntity::EntityType::Create:
            events += serialize(static_cast<MscCreate *>(instanceEvent), tabsSize);
            break;
        default:
            break;
        }
    }

    return header + events + footer;
}

QString MscWriter::serialize(const MscMessage *message, const MscInstance *instance, int tabsSize)
{
    if (message == nullptr || !(message->sourceInstance() == instance || message->targetInstance() == instance))
        return QString();

    QString direction = tabs(tabsSize);
    QString name = message->name();
    QString instanceName;

    if (message->sourceInstance() == instance) {
        direction += "out %1 to %2;\n";
        instanceName = message->targetInstance() != nullptr ? message->targetInstance()->name() : "env";
    } else {
        direction += "in %1 from %2;\n";
        instanceName = message->sourceInstance() != nullptr ? message->sourceInstance()->name() : "env";
    }

    if (!message->parameters().name.isEmpty())
        name += QString(",%1").arg(message->parameters().name);

    if (!message->parameters().expression.isEmpty() || !message->parameters().pattern.isEmpty())
        name += QString("(%1)").arg(!message->parameters().expression.isEmpty() ? message->parameters().expression
                                                                                : message->parameters().pattern);

    return QString(direction).arg(name, instanceName);
}

QString MscWriter::serialize(const MscCondition *condition, int tabsSize)
{
    if (condition == nullptr)
        return "";

    return QString("%1condition %2%3;\n")
            .arg(tabs(tabsSize), condition->name(), condition->shared() ? " shared all" : "");
}

QString MscWriter::serialize(const MscCreate *create, int tabsSize)
{
    if (create == nullptr)
        return "";

    const QString parameters =
            create->parameters().isEmpty() ? "" : QString("(%1)").arg(create->parameters().join(", "));

    return QString("%1create %2%3;\n").arg(tabs(tabsSize), create->name(), parameters);
}

QString MscWriter::serialize(const MscTimer *timer, int tabsSize)
{
    if (timer == nullptr) {
        return QString();
    }

    QString timerType;
    switch (timer->timerType()) {
    case MscTimer::TimerType::Start:
        timerType = "starttimer";
        break;
    case MscTimer::TimerType::Stop:
        timerType = "stoptimer";
        break;
    case MscTimer::TimerType::Timeout:
        timerType = "timeout";
        break;
    default:
        // Not good
        return QString();
    }

    return tabs(tabsSize) + timerType + " " + timer->name() + ";\n";
}

QString MscWriter::serialize(const MscAction *action, int tabsSize)
{
    if (action == nullptr) {
        return QString();
    }

    if (action->actionType() == MscAction::ActionType::Informal) {
        return tabs(tabsSize) + "action '" + action->informalAction() + "';\n";
    } else {
        QString actionText = tabs(tabsSize) + "action ";
        bool first = true;
        for (const auto &statement : action->dataStatements()) {
            if (!first) {
                actionText += ", ";
            }
            switch (statement.m_type) {
            case MscAction::DataStatement::StatementType::Define:
                actionText += "define " + statement.m_variableString;
                break;
            case MscAction::DataStatement::StatementType::UnDefine:
                actionText += "undefine " + statement.m_variableString;
                break;
            case MscAction::DataStatement::StatementType::Binding:
                qWarning() << "Writing of formal binding actions is not yet supported";
                continue;
            }
            first = false;
        }
        actionText += ";\n";
        return actionText;
    }
}

QString MscWriter::serialize(const MscCoregion *region, int tabsSize)
{
    const char *type = region->type() == MscCoregion::Type::Begin ? "concurrent" : "endconcurrent";
    return QString("%1%2;\n").arg(tabs(tabsSize), type);
}

QString MscWriter::serialize(const MscChart *chart, int tabsSize)
{
    if (chart == nullptr)
        return QString();

    QString instances;

    for (const auto *instance : chart->instances())
        instances += serialize(instance, chart->instanceEvents(), tabsSize + 1);

    QString tabString = tabs(tabsSize);
    return QString("%1msc %2;\n%3%1endmsc;\n").arg(tabString, chart->name(), instances);
}

QString MscWriter::serialize(const MscDocument *document, int tabsSize)
{
    if (document == nullptr)
        return QString();

    const QString dataDef = tabsSize == 0 ? dataDefinition() : "";

    QString documentBody;

    int tabCount = tabsSize + 1;
    for (const auto *doc : document->documents())
        documentBody += serialize(doc, tabCount++);

    for (const auto *chart : document->charts())
        documentBody += serialize(chart, tabsSize + 1);

    QString relation;
    switch (document->hierarchyType()) {
    case MscDocument::HierarchyLeaf:
        // Nothing to add
        break;
    case MscDocument::HierarchyIs:
        relation = " /* MSC IS */";
        break;
    case MscDocument::HierarchyAnd:
        relation = " /* MSC AND */";
        break;
    case MscDocument::HierarchyOr:
        relation = " /* MSC OR */";
        break;
    case MscDocument::HierarchyParallel:
        relation = " /* MSC PARALLEL */";
        break;
    case MscDocument::HierarchyRepeat:
        relation = " /* MSC REPEAT */";
        break;
    case MscDocument::HierarchyException:
        relation = " /* MSC EXCEPTION */";
        break;
    }

    QString tabString = tabs(tabsSize);
    return QString("%1mscdocument %2%4;%5\n%3%1endmscdocument;\n")
            .arg(tabString, document->name(), documentBody, relation, dataDef);
}

void MscWriter::setModel(const MscModel *model)
{
    m_model = model;
}

QString MscWriter::tabs(int tabsSize) const
{
    const static QString TABS = "   ";

    QString tabsString;

    for (int x = 0; x < tabsSize; ++x)
        tabsString += TABS;

    return tabsString;
}

QString MscWriter::dataDefinition() const
{
    if (!m_model) {
        return {};
    }

    QString data;
    if (!m_model->dataLanguage().isEmpty()) {
        data = QString("\n%1language %2;").arg(tabs(1), m_model->dataLanguage());
    }
    if (!m_model->dataDefinitionString().isEmpty()) {
        data += QString("\n%1data %2;").arg(tabs(1), m_model->dataDefinitionString());
    }

    return data;
}

} // namespace msc
