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

#include "datastatement.h"
#include "mscaction.h"
#include "mscchart.h"
#include "msccomment.h"
#include "msccondition.h"
#include "msccoregion.h"
#include "msccreate.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "mscreader.h"
#include "msctimer.h"
#include "stringtemplate.h"

#include <QBuffer>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QScopedPointer>
#include <QTextStream>
#include <QVariant>

namespace msc {

/*!
 * \class MscWriter
 *
 * This class writes an MSC model to a file.
 */
MscWriter::MscWriter(QObject *parent)
    : QObject(parent)
{
    setSaveMode(m_saveMode);
}

/**
   Defines which engine to use for saving msc data
 */
void MscWriter::setSaveMode(MscWriter::SaveMode mode)
{
    m_saveMode = mode;

    if (m_saveMode == SaveMode::GRANTLEE && m_template == nullptr) {
        m_template = templating::StringTemplate::create(this);
        m_template->setNeedValidateXMLDocument(false);
        m_template->setEscapeCharacters(false);
    }
    if (m_saveMode == SaveMode::CUSTOM && m_template != nullptr) {
        delete m_template;
        m_template = nullptr;
    }
}

/*!
 * \brief MscWriter::saveModel Save a model text to file.
 * \param model
 * \param fileName
 * \return True if success
 */
bool MscWriter::saveModel(MscModel *model, const QString &fileName)
{
    if (model == nullptr || fileName.isEmpty()) {
        return false;
    }

    QFile mscFile(fileName);
    if (!mscFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QString mscText = modelText(model);
    mscFile.write(mscText.toUtf8());

    mscFile.close();

    setModel(nullptr);
    return true;
}

/*!
 * \brief MscWriter::saveChart Save a chart to file.
 * \param chart
 * \param fileName
 * \return
 */
bool MscWriter::saveChart(const MscChart *chart, const QString &fileName)
{
    if (chart == nullptr || fileName.isEmpty())
        return false;

    QFile mscFile(fileName);
    if (!mscFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&mscFile);

    out << serialize(chart);

    mscFile.close();
    return true;
}

/*!
 * \brief MscWriter::modelText Get a string representation of a model.
 * \param model
 * \return
 */
QString MscWriter::modelText(MscModel *model)
{
    if (m_saveMode == SaveMode::GRANTLEE) {
        return exportGrantlee(model, QString());
    }

    setModel(model);

    QString text;
    QTextStream out(&text);

    if (!model->documents().isEmpty()) {
        for (MscDocument *doc : model->documents()) {
            out << serialize(doc);
        }
    } else {
        for (const auto *chart : model->charts()) {
            out << serialize(chart);
        }
    }

    setModel(nullptr);
    return text;
}

/*!
 * \brief MscWriter::serialize Get a string representation of an instance and events.
 * \param instance
 * \param instanceEvents
 * \param tabsSize
 * \return
 */
QString MscWriter::serialize(
        const MscInstance *instance, const QVector<MscInstanceEvent *> &instanceEvents, int tabsSize)
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

    QString comment = serializeComment(instance, tabsSize);
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
            events += serialize(static_cast<MscCoregion *>(instanceEvent), instance, tabsSize);
            break;
        case MscEntity::EntityType::Action:
            events += serialize(static_cast<MscAction *>(instanceEvent), instance, tabsSize);
            break;
        case MscEntity::EntityType::Create:
            events += serialize(static_cast<MscCreate *>(instanceEvent), instance, tabsSize);
            break;
        case MscEntity::EntityType::Condition: {
            auto condition = static_cast<MscCondition *>(instanceEvent);
            if (condition->relatesTo(instance)) {
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

/*!
 * \brief MscWriter::serialize Get a string representation of a message
 * \param message
 * \param instance
 * \param tabsSize
 * \return
 */
QString MscWriter::serialize(const MscMessage *message, const MscInstance *instance, int tabsSize)
{
    if (message == nullptr || !(message->relatesTo(instance)))
        return QString();

    const QString comment = serializeComment(message, tabsSize);

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

/*!
 * \brief MscWriter::serialize Get a string representation of a condition
 * \param condition
 * \param tabsSize
 * \return
 */
QString MscWriter::serialize(const MscCondition *condition, int tabsSize)
{
    if (condition == nullptr)
        return "";

    const QString comment = serializeComment(condition, tabsSize);
    const QString conditionSerialized(
            QString("%1condition %2%3%4;\n")
                    .arg(tabs(tabsSize), condition->name(), condition->shared() ? " shared all" : "", comment));
    return serializeCif(condition, conditionSerialized, tabsSize);
}

/*!
 * \brief MscWriter::serialize Get a string representation
 * \param create
 * \param instance
 * \param tabsSize
 * \return
 */
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
        const QString comment = serializeComment(create, tabsSize);
        res += comment;

        const QString createSerialized(res.append(";\n"));
        return serializeCif(create, createSerialized, tabsSize);
    }
    return {};
}

/*!
 * \brief MscWriter::serialize Get a string representation
 * \param timer
 * \param instance
 * \param tabsSize
 * \return
 */
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

    const QString comment = serializeComment(timer, tabsSize);
    const QString timerSerialized(QString("%1%2 %3%4;\n").arg(tabs(tabsSize), timerType, timer->fullName(), comment));
    return serializeCif(timer, timerSerialized, tabsSize);
}

/*!
 * \brief MscWriter::serialize Get a string representation
 * \param action
 * \param instance
 * \param tabsSize
 * \return
 */
QString MscWriter::serialize(const MscAction *action, const MscInstance *instance, int tabsSize)
{
    if (action == nullptr) {
        return {};
    }

    if (action->instance() != instance) {
        return {};
    }

    const QString tabString = tabs(tabsSize);
    const QString comment = serializeComment(action, tabsSize);

    if (action->actionType() == MscAction::ActionType::Informal) {
        if (action->informalAction().contains('='))
            return serializeCif(
                    action, QString("%1action %2%3;\n").arg(tabString, action->informalAction(), comment), tabsSize);
        else
            return serializeCif(
                    action, QString("%1action '%2'%3;\n").arg(tabString, action->informalAction(), comment), tabsSize);
    } else {
        QString actionText = tabString + "action ";
        bool first = true;
        for (const auto statement : action->dataStatements()) {
            if (!first) {
                actionText += ", ";
            }
            switch (statement->type()) {
            case msc::DataStatement::StatementType::Define:
                actionText += "def " + statement->variableString();
                break;
            case msc::DataStatement::StatementType::UnDefine:
                actionText += "undef " + statement->variableString();
                break;
            case msc::DataStatement::StatementType::Binding:
                qWarning() << "Writing of formal binding actions is not yet supported";
                continue;
            }
            first = false;
        }
        actionText += comment + ";\n";
        return serializeCif(action, actionText, tabsSize);
    }
}

/*!
 * \brief MscWriter::serialize Get a string representation
 * \param region
 * \param instance
 * \param tabsSize
 * \return
 */
QString MscWriter::serialize(const MscCoregion *region, const MscInstance *instance, int tabsSize)
{
    if (region == nullptr || region->instance() == nullptr || region->instance() != instance)
        return {};

    const char *type = region->type() == MscCoregion::Type::Begin ? "concurrent" : "endconcurrent";
    const QString comment = serializeComment(region, tabsSize);
    const QString regionSerialized(QString("%1%2%4;\n").arg(tabs(tabsSize), type, comment));
    return serializeCif(region, regionSerialized, tabsSize);
}

/*!
 * \brief MscWriter::serialize Get a string representation
 * \param declarationList
 * \param tabsSize
 * \return
 */
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

/*!
 * \brief MscWriter::serialize Get a string representation
 * \param declaration
 * \param tabsSize
 * \return
 */
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

/*!
 * \brief MscWriter::serialize Get a string representation
 * \param chart
 * \param tabsSize
 * \return
 */
QString MscWriter::serialize(const MscChart *chart, int tabsSize)
{
    if (chart == nullptr)
        return QString();

    QString instances;

    for (auto instance : chart->instances()) {
        instances += serialize(instance, chart->eventsForInstance(instance), tabsSize + 1);
    }

    const QString &tabString = tabs(tabsSize);
    const QString chartSerialized(QString("%1msc %2%4;%5\n%3%1endmsc;\n")
                                          .arg(tabString, chart->name(), instances, serializeComment(chart, tabsSize),
                                                  serializeGlobalComments(chart, tabsSize)));
    return serializeCif(chart, chartSerialized, tabsSize);
}

/*!
 * \brief MscWriter::serialize Get a string representation
 * \param document
 * \param tabsSize
 * \return
 */
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
    const QString documentSerialized(QString("%1mscdocument %2%6%4;%5\n%3%1endmscdocument;\n")
                                             .arg(tabString, document->name(), documentBody, relation, dataDef,
                                                     serializeComment(document, tabsSize)));
    return serializeCif(document, documentSerialized, tabsSize);
}

/*!
   \brief MscWriter::exportGrantlee exports the given msc model using the given template file. If the template file
   can't be loaded, the default template in the resource is used.
   \param model The model to be exported
   \param templateFile the grantlee template file to use
   \return the string representation.
 */
QString MscWriter::exportGrantlee(MscModel *model, QString templateFile)
{
    QFileInfo fi(templateFile);
    if (!fi.exists() || !fi.isReadable()) {
        templateFile = ":/mscresources/mscmodel.tmplt";
    }

    QBuffer buffer;
    QByteArray bufferArray;
    buffer.setBuffer(&bufferArray);
    buffer.open(QIODevice::WriteOnly);
    QHash<QString, QVariant> grouppedObjects;
    grouppedObjects.insert("model", QVariant::fromValue(model));
    m_template->parseFile(grouppedObjects, templateFile, &buffer);

    buffer.close();
    buffer.open(QIODevice::ReadOnly);
    QString result = buffer.readAll();

    // Fix too many empty lines
    result.replace("\n\n\n", "\n");
    result.replace("\n\n", "\n");

    return result;
}

/*!
   \brief MscWriter::convertMscFile convert the msc file, based on the given grantlee template
   \param inputFile the .msc file to read
   \param templateFile the grantlee template file to be used
   \param outputFile the file to store the result to
   \return
 */
bool MscWriter::convertMscFile(const QString &inputFile, const QString &templateFile, const QString &outputFile) const
{
    QFileInfo fi(templateFile);
    if (!fi.exists() || !fi.isReadable()) {
        qWarning() << tr("Unable to use template file '%1'").arg(templateFile);
        return false;
    }

    fi.setFile(inputFile);
    if (!fi.exists() || !fi.isReadable()) {
        qWarning() << tr("Unable to use input file file '%1'").arg(inputFile);
        return false;
    }

    msc::MscReader reader(msc::MscReader::NOTIFY::NO_HUB);
    QStringList errors;
    QScopedPointer<msc::MscModel> mscModel(reader.parseFile(inputFile, &errors));
    if (!errors.isEmpty() || mscModel.isNull()) {
        qWarning() << errors.join('\n');
        return false;
    }

    msc::MscWriter writer;
    writer.setSaveMode(msc::MscWriter::SaveMode::GRANTLEE);
    const QString fileContent = writer.exportGrantlee(mscModel.data(), templateFile);
    QFile out(outputFile);
    if (!out.open(QIODevice::WriteOnly)) {
        qWarning() << tr("Can't write file '%1'").arg(outputFile);
        return false;
    }
    out.write(fileContent.toUtf8());
    out.close();

    return true;
}

/*!
 * \brief MscWriter::setModel Set the model to write
 * \param model
 */
void MscWriter::setModel(MscModel *model)
{
    m_model = model;
    if (m_model)
        m_model->checkInstanceNames();
}

/*!
 * \brief MscWriter::tabs Set the tab size. Default is 4.
 * \param tabsSize
 * \return
 */
QString MscWriter::tabs(int tabsSize) const
{
    static constexpr int spaces(4);
    return QString().fill(' ', spaces * tabsSize);
}

/*!
 * \brief MscWriter::dataDefinition Serialize the language and data of the model.
 * \return
 */
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

/*!
 * \brief MscWriter::serializeComment Get a string representation
 * \param entity
 * \param tabsSize
 * \return
 */
QString MscWriter::serializeComment(const msc::MscEntity *entity, int tabsSize) const
{
    if (!entity)
        return {};

    MscComment *commentEntity = entity->comment();
    if (!commentEntity || commentEntity->isGlobal())
        return {};

    QString cifInfo;
    const QVector<cif::CifBlockShared> &cifs = commentEntity->cifs();
    QStringList cifTexts;
    cifTexts.reserve(cifs.size());
    for (const cif::CifBlockShared &cifBlock : cifs)
        cifTexts << cifBlock->toString(tabsSize);
    cifInfo = cifTexts.join(QLatin1Char('\n'));
    if (!cifInfo.isEmpty()) {
        cifInfo.prepend(QLatin1Char('\n'));
        cifInfo.append(QLatin1Char('\n'));
    } else if (commentEntity->text().isEmpty()) {
        return {};
    }

    return QString("%1 comment '%2'").arg(cifInfo, commentEntity->text());
}

/*!
 * \brief MscWriter::serializeGlobalComments Get a string representation
 * \param entity
 * \param tabsSize
 * \return
 */
QString MscWriter::serializeGlobalComments(const MscEntity *entity, int tabsSize) const
{
    if (!entity)
        return {};

    QStringList cifTexts;
    if (MscComment *comment = entity->comment()) {
        const QString globalText = comment->text();
        if (comment->isGlobal() && !globalText.isEmpty()) {
            for (const cif::CifBlockShared &cifBlock : comment->cifs()) {
                if (cifBlock->blockType() == cif::CifLine::CifType::Text)
                    cifTexts << cifBlock->toString(tabsSize);
            }
        }
    }
    if (cifTexts.isEmpty())
        return {};

    return QLatin1Char('\n') + cifTexts.join("\n") + QLatin1Char('\n');
}

/*!
 * \brief MscWriter::serializeParameters Get a string representation of the parameters of a message
 * \param message
 * \return
 */
QString MscWriter::serializeParameters(const MscMessage *message) const
{
    return message ? message->paramString() : QString();
}

/*!
 * \brief MscWriter::serializeCif Get a string representation of CIF
 * \param entity
 * \param entitySerialized
 * \param tabsSize
 * \return
 */
QString MscWriter::serializeCif(const msc::MscEntity *entity, const QString &entitySerialized, int tabsSize) const
{
    if (!entity || entitySerialized.isEmpty() || entity->cifs().isEmpty()) {
        return entitySerialized;
    }

    return entity->cifText(tabsSize) + entitySerialized;
}

} // namespace msc
