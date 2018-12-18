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
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "msctimer.h"
#include "msccoregion.h"

#include <QFile>
#include <QTextStream>

namespace msc {

MscWriter::MscWriter(QObject *parent)
    : QObject(parent)
{
}

void MscWriter::saveModel(const MscModel *model, const QString &fileName)
{
    QString mscText;

    if (model == nullptr || fileName.isEmpty())
        return;

    QFile mscFile(fileName);
    if (!mscFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&mscFile);

    int tabCount = 0;
    for (const auto *doc : model->documents())
        out << serialize(doc, tabCount++);

    for (const auto *chart : model->charts())
        out << serialize(chart);

    mscFile.close();
}

void MscWriter::saveChart(const MscChart *chart, const QString &fileName)
{
    QString mscText;

    if (chart == nullptr || fileName.isEmpty())
        return;

    QFile mscFile(fileName);
    if (!mscFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&mscFile);

    out << serialize(chart);

    mscFile.close();
}

QString MscWriter::serialize(const MscInstance *instance, const QVector<MscInstanceEvent *> &instanceEvents, int tabsSize)
{
    if (instance == nullptr)
        return "";

    QString events;

    QString tabString = tabs(tabsSize);
    QString header = QString("%1instance %2").arg(tabString, instance->name());
    QString footer = tabString + "endinstance;\n";

    if (!instance->kind().isEmpty()) {
        header += QString(": %1 %2;\n").arg(instance->kind(), instance->inheritance());
    } else
        header += ";\n";

    auto addCondition = [&](const QString &messageName, int tabsSize) {
        std::for_each(instanceEvents.begin(),
                      instanceEvents.end(),
                      [&](MscInstanceEvent *event) {
                          auto *condition = static_cast<MscCondition *>(event);
                          if (condition->instance() == instance && condition->messageName() == messageName)
                              events += serialize(condition, tabsSize);
                      });
    };

    // serialize conditions with empty messageName
    addCondition("", tabsSize + 1);

    for (const auto &instanceEvent : instanceEvents) {
        switch (instanceEvent->entityType()) {
        case MscEntity::EntityType::Message:
            events += serialize(static_cast<MscMessage *>(instanceEvent), instance, tabsSize + 1);
            addCondition(instanceEvent->name(), tabsSize + 1);
            break;
        case MscEntity::EntityType::Timer:
            events += serialize(static_cast<MscTimer *>(instanceEvent), tabsSize + 1);
            break;
        case MscEntity::EntityType::Coregion:
            events += serialize(static_cast<MscCoregion *>(instanceEvent), tabsSize + 1);
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
        name += QString("(%1)").arg(!message->parameters().expression.isEmpty() ? message->parameters().expression : message->parameters().pattern);

    return QString(direction).arg(name, instanceName);
}

QString MscWriter::serialize(const MscCondition *condition, int tabsSize)
{
    if (condition == nullptr)
        return "";

    return QString("%1condition %2%3;\n").arg(tabs(tabsSize), condition->name(), condition->shared() ? " shared all" : "");
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

    QString instances;

    int tabCount = tabsSize + 1;
    for (const auto *doc : document->documents())
        instances += serialize(doc, tabCount++);

    for (const auto *chart : document->charts())
        instances += serialize(chart, tabsSize + 1);

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
    return QString("%1mscdocument %2%4;\n%3%1endmscdocument;\n").arg(tabString, document->name(), instances, relation);
}

QString MscWriter::tabs(int tabsSize)
{
    const static QString TABS = "   ";

    QString tabsString;

    for (int x = 0; x < tabsSize; ++x)
        tabsString += TABS;

    return tabsString;
}

} // namespace msc
