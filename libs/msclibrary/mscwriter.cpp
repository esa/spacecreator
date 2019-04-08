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

#include "mscwriter.h"

#include "mscaction.h"
#include "mscchart.h"
#include "msccondition.h"
#include "msccoregion.h"
#include "msccreate.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "msctimer.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

namespace msc {

MscWriter::MscWriter(QObject *parent)
    : QObject(parent)
{
}

void MscWriter::saveModel(MscModel *model, const QString &fileName)
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

QString MscWriter::modelText(MscModel *model)
{
    setModel(model);

    QString text;
    QTextStream out(&text);

    int tabCount = 0;
    for (const auto *doc : model->documents())
        out << serialize(doc, tabCount++);

    for (const auto *chart : model->charts())
        out << serialize(chart);

    setModel(nullptr);

    return text;
}

QString MscWriter::serialize(const MscInstance *instance, const QVector<MscInstanceEvent *> &instanceEvents,
                             int tabsSize)
{
    if (instance == nullptr)
        return "";

    QString events;

    const QString end = QString("%1;\n").arg(instance->explicitStop() ? "stop" : "endinstance");
    const QString tabString = tabs(tabsSize);
    QString header = QString("%1instance %2").arg(tabString, instance->name());
    const QString footer = tabString + end;

    // The rest is the internal part of this which should be indented
    ++tabsSize;

    QString comment = serializeComment(instance);
    if (!instance->denominatorAndKind().isEmpty()) {
        header += QString(": %1 %2%3;\n").arg(instance->denominatorAndKind(), instance->inheritance(), comment);
    } else
        header += comment + ";\n";

    for (const auto &instanceEvent : instanceEvents) {
        switch (instanceEvent->entityType()) {
        case MscEntity::EntityType::Message:
            events += serialize(static_cast<MscMessage *>(instanceEvent), instance, tabsSize);
            break;
        case MscEntity::EntityType::Timer:
            events += serialize(static_cast<MscTimer *>(instanceEvent), instance, tabsSize);
            break;
        case MscEntity::EntityType::Coregion:
            events += serialize(static_cast<MscCoregion *>(instanceEvent), tabsSize);
            break;
        case MscEntity::EntityType::Action:
            events += serialize(static_cast<MscAction *>(instanceEvent), instance, tabsSize);
            break;
        case MscEntity::EntityType::Create:
            events += serialize(static_cast<MscCreate *>(instanceEvent), instance, tabsSize);
            break;
        case MscEntity::EntityType::Condition: {
            auto condition = static_cast<MscCondition *>(instanceEvent);

            if (condition->instance() == instance) {
                events += serialize(condition, tabsSize);
            }

            break;
        }
        default:
            break;
        }
    }

    const QString instanceSerialized(header + events + footer);
    return serializeCif(instance, instanceSerialized, tabsSize);
}

QString MscWriter::serialize(const MscMessage *message, const MscInstance *instance, int tabsSize)
{
    if (message == nullptr || !(message->relatesTo(instance)))
        return QString();

    const QString comment = serializeComment(message);

    QString direction = tabs(tabsSize);
    QString name = message->fullName();
    QString instanceName;

    if (message->sourceInstance() == instance) {
        direction += "out %1 to %2%3;\n";
        instanceName = message->targetInstance() != nullptr ? message->targetInstance()->name() : "env";
    } else {
        direction += "in %1 from %2%3;\n";
        instanceName = message->sourceInstance() != nullptr ? message->sourceInstance()->name() : "env";
    }

    const QString parameters = serializeParameters(message);
    if (!parameters.isEmpty()) {
        name = name.append("(%1)").arg(parameters);
    }

    const QString messageSerialized(QString(direction).arg(name, instanceName, comment));
    return serializeCif(message, messageSerialized, tabsSize);
}

QString MscWriter::serialize(const MscCondition *condition, int tabsSize)
{
    if (condition == nullptr)
        return "";

    const QString conditionSerialized(
            QString("%1condition %2%3;\n")
                    .arg(tabs(tabsSize), condition->name(), condition->shared() ? " shared all" : ""));
    return serializeCif(condition, conditionSerialized, tabsSize);
}

QString MscWriter::serialize(const MscCreate *create, const MscInstance *instance, int tabsSize)
{
    Q_ASSERT(create != nullptr);
    Q_ASSERT(create->targetInstance() != nullptr);

    if (create && instance == create->sourceInstance()) {
        QString res = tabs(tabsSize) + QString("create %1").arg(create->targetInstance()->name());
        const QString parameters = serializeParameters(create);
        if (!parameters.isEmpty()) {
            res = res.append("(%1)").arg(parameters);
        }

        const QString createSerialized(res.append(";\n"));
        return serializeCif(create, createSerialized, tabsSize);
    }
    return {};
}

QString MscWriter::serialize(const MscTimer *timer, const MscInstance *instance, int tabsSize)
{
    if (timer == nullptr || timer->instance() != instance) {
        return QString();
    }

    QString timerType;
    switch (timer->timerType()) {
    case MscTimer::TimerType::Start:
        timerType = QStringLiteral("starttimer");
        break;
    case MscTimer::TimerType::Stop:
        timerType = QStringLiteral("stoptimer");
        break;
    case MscTimer::TimerType::Timeout:
        timerType = QStringLiteral("timeout");
        break;
    default:
        // Not good
        return QString();
    }

    const QString timerSerialized(QString("%1%2 %3;\n").arg(tabs(tabsSize), timerType, timer->fullName()));
    return serializeCif(timer, timerSerialized, tabsSize);
}

QString MscWriter::serialize(const MscAction *action, const MscInstance *instance, int tabsSize)
{
    if (action == nullptr) {
        return {};
    }

    if (action->instance() != instance) {
        return {};
    }

    const QString tabString = tabs(tabsSize);
    const QString comment = serializeComment(action);

    if (action->actionType() == MscAction::ActionType::Informal) {
        if (action->informalAction().contains('='))
            return serializeCif(action, QString("%1action %2%3;\n").arg(tabString, action->informalAction(), comment),
                                tabsSize);
        else
            return serializeCif(action, QString("%1action '%2'%3;\n").arg(tabString, action->informalAction(), comment),
                                tabsSize);
    } else {
        QString actionText = tabString + "action ";
        bool first = true;
        for (const auto &statement : action->dataStatements()) {
            if (!first) {
                actionText += ", ";
            }
            switch (statement.m_type) {
            case MscAction::DataStatement::StatementType::Define:
                actionText += "def " + statement.m_variableString;
                break;
            case MscAction::DataStatement::StatementType::UnDefine:
                actionText += "undef " + statement.m_variableString;
                break;
            case MscAction::DataStatement::StatementType::Binding:
                qWarning() << "Writing of formal binding actions is not yet supported";
                continue;
            }
            first = false;
        }
        actionText += comment + ";\n";
        return serializeCif(action, actionText, tabsSize);
    }
}

QString MscWriter::serialize(const MscCoregion *region, int tabsSize)
{
    const char *type = region->type() == MscCoregion::Type::Begin ? "concurrent" : "endconcurrent";
    const QString regionSerialized(QString("%1%2;\n").arg(tabs(tabsSize), type));
    return serializeCif(region, regionSerialized, tabsSize);
}

QString MscWriter::serialize(const MscMessageDeclarationList *declarationList, int tabsSize)
{
    Q_ASSERT(declarationList);
    QString declarations;
    for (const MscMessageDeclaration *declaration : *declarationList) {
        declarations += serialize(declaration, tabsSize);
        declarations += "\n";
    }
    return declarations;
}

QString MscWriter::serialize(const MscMessageDeclaration *declaration, int tabsSize)
{
    Q_ASSERT(declaration);
    QString text = tabs(tabsSize) + "msg " + declaration->names().join(", ");
    if (!declaration->typeRefList().isEmpty()) {
        text += QString(" : (%1)").arg(declaration->typeRefList().join(", "));
    }
    text += ";";
    return text;
}

QString MscWriter::serialize(const MscChart *chart, int tabsSize)
{
    if (chart == nullptr)
        return QString();

    QString instances;

    for (const auto *instance : chart->instances())
        instances += serialize(instance, chart->instanceEvents(), tabsSize + 1);

    const QString &tabString = tabs(tabsSize);
    const QString chartSerialized(
            QString("%1msc %2%4;\n%3%1endmsc;\n").arg(tabString, chart->name(), instances, serializeComment(chart)));
    return serializeCif(chart, chartSerialized, tabsSize);
}

QString MscWriter::serialize(const MscDocument *document, int tabsSize)
{
    if (document == nullptr)
        return QString();

    const QString dataDef = tabsSize == 0 ? dataDefinition() : "";

    QString documentBody;

    const int tabCount = tabsSize + 1;

    documentBody += serialize(document->messageDeclarations(), tabCount);

    for (const auto *doc : document->documents())
        documentBody += serialize(doc, tabCount);

    for (const auto *chart : document->charts())
        documentBody += serialize(chart, tabCount);

    QString relation;
    switch (document->hierarchyType()) {
    case MscDocument::HierarchyLeaf:
        relation = " /* MSC LEAF */";
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
    default:
        Q_ASSERT(true);
        qWarning() << "Invalid document type of document " << document->name();
        relation = "";
        break;
    }

    const QString &tabString = tabs(tabsSize);
    const QString documentSerialized(
            QString("%1mscdocument %2%6%4;%5\n%3%1endmscdocument;\n")
                    .arg(tabString, document->name(), documentBody, relation, dataDef, serializeComment(document)));
    return serializeCif(document, documentSerialized, tabsSize);
}

void MscWriter::setModel(MscModel *model)
{
    m_model = model;
    if (m_model)
        m_model->checkInstanceNames();
}

QString MscWriter::tabs(int tabsSize) const
{
    static constexpr int spaces(4);
    return QString().fill(' ', spaces * tabsSize);
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

QString MscWriter::serializeComment(const msc::MscEntity *entity) const
{
    if (!entity || entity->comment().isEmpty()) {
        return {};
    }

    return QString(" comment '%1'").arg(entity->comment());
}

QString MscWriter::serializeParameters(const MscMessage *message) const
{
    QString parameters;
    for (const MscParameter &param : message->parameters()) {
        if (!parameters.isEmpty())
            parameters += ", ";
        parameters += param.pattern().isEmpty() ? param.expression() : param.pattern();
    }
    return parameters;
}

QString MscWriter::serializeCif(const msc::MscEntity *entity, const QString &entitySerialized, int tabsSize) const
{
    if (!entity || entitySerialized.isEmpty() || entity->cifs().isEmpty())
        return entitySerialized;

    const QVector<cif::CifBlockShared> &cifs = entity->cifs();
    QStringList cifTexts;
    cifTexts.reserve(cifs.size());
    for (const cif::CifBlockShared &cifBlock : cifs) {
        cifTexts << cifBlock->toString(tabsSize);
    }

    return cifTexts.join("\n") + entitySerialized;
}

} // namespace msc
