/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "actionsmanager.h"

#include "common.h"
#include "errorhub.h"
#include "extprocmonitor.h"
#include "interfacedocument.h"
#include "ivexporter.h"
#include "ivfunction.h"
#include "ivinterface.h"
#include "ivobject.h"
#include "qfiledialog.h"
#include "standardpaths.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFontDatabase>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenu>
#include <QMessageBox>
#include <QMetaObject>
#include <QObject>
#include <QProcess>
#include <QUndoStack>
#include <QVersionNumber>

struct ExternalArgHolder {
    enum Type
    {
        CWD,
        Attr,
        Prop,
        Param,
        ProjectPath,
        FilePath,
    };

    QString key;
    QString description;
    Type target;
};

static QVector<ExternalArgHolder> externalArgs(bool namedKey)
{
    const QString name = namedKey ? QLatin1String("name") : QString();
    QVector<ExternalArgHolder> args;
    args.append({ "$FILE", QObject::tr("Path to the file should be processed"), ExternalArgHolder::Type::FilePath });
    args.append({ "$TASTE3", QObject::tr("Path to the Taste binary"), ExternalArgHolder::Type::CWD });
    args.append(
            { "$PRJ_FOLDER", QObject::tr("Path to the Taste current project"), ExternalArgHolder::Type::ProjectPath });
    args.append({ "$attr_" + name, QObject::tr("Value of the selected object's attribute [name]"),
            ExternalArgHolder::Type::Attr });
    args.append({ "$prop_" + name, QObject::tr("Value of the selected object's property [name]"),
            ExternalArgHolder::Type::Prop });
    args.append({ "$param_" + name,
            QObject::tr("Value of the selected Interface's parameter (or Function's context parameter) [name]."),
            ExternalArgHolder::Type::Param });
    return args;
}

namespace ive {

/*!
 *\class ive::ActionsManager
 * \brief The application wide manager for script-able actions.
 * Used to: store action instances in runtime, deploy the default file from resources (if necessary), populate the
 *context menu and to actually invoke the internal QAction or external application.
 */
ActionsManager *ActionsManager::m_instance = nullptr;

/*!
 * Returns path to a dir that contains custom actions descriptions.
 * The value is something like ~/.local/share/ESA/IV Editor/contextMenu
 */
QString ActionsManager::storagePath()
{
    return QString("%1/contextMenu/")
            .arg(shared::StandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
}

/*!
 * \brief ActionsManager::populateMenu
 * Adds the appropriate actions into \a menu, managing its enablement based on action conditions and the \a currObj.
 * Initiates connection from QAction to the related handler slot.
 */
void ActionsManager::registerDeploymentFilesCallback(std::function<QStringList()> cb)
{
    instance()->m_deploymentFilesCallBack = cb;
}

void ActionsManager::populateMenu(QMenu *menu, ivm::IVObject *currObj, InterfaceDocument *doc)
{
    if (!menu)
        return;

    menu->addSeparator();

    static const QString kBuildAction = QLatin1String("Build");
    for (const Action &actHandler : qAsConst(instance()->m_actions)) {
        if (actHandler.m_title == kBuildAction)
            continue;

        QAction *act = new QAction(actHandler.m_title, menu);
        act->setData(QVariant::fromValue(currObj));
        const bool enabled(actHandler.isAcceptable(currObj));
        act->setEnabled(enabled);
        if (enabled) {
            QObject::connect(act, &QAction::triggered, doc, [actHandler, act, doc]() {
                if (triggerActionHidden(actHandler)) {
                    return;
                }
                if (!actHandler.m_internalActName.isEmpty())
                    triggerActionInternal(actHandler);
                else if (!actHandler.m_externalApp.isEmpty()) {
                    triggerActionExternal(
                            actHandler, act ? act->data().value<ivm::IVObject *>() : nullptr, doc, doc->path());
                } else {
                    QMessageBox::warning(nullptr, QObject::tr("Custom action"),
                            QObject::tr("No internal or external action provided by %1").arg(actHandler.m_title));
                }
            });
        }
        menu->addAction(act);
    }

    auto it = instance()->m_actions.cend();
    if (instance()->m_deploymentFilesCallBack) {
        it = std::find_if(m_instance->m_actions.cbegin(), m_instance->m_actions.cend(),
                [](const Action &action) { return kBuildAction == action.m_title; });
    }

    if (it != instance()->m_actions.cend()) {
        QMenu *buildMenu = new QMenu(QObject::tr("Build"), menu);
        for (const QString &dvFilePath : instance()->m_deploymentFilesCallBack()) {
            QAction *act = new QAction(QFileInfo(dvFilePath).fileName(), menu);
            act->setData(QVariant::fromValue(currObj));
            QObject::connect(act, &QAction::triggered, doc, [actHandler = *it, act, doc, dvFilePath]() {
                if (triggerActionHidden(actHandler)) {
                    return;
                }
                if (!actHandler.m_internalActName.isEmpty())
                    triggerActionInternal(actHandler);
                else if (!actHandler.m_externalApp.isEmpty()) {
                    triggerActionExternal(
                            actHandler, act ? act->data().value<ivm::IVObject *>() : nullptr, doc, dvFilePath);
                }
            });
            buildMenu->addAction(act);
        }
        menu->addMenu(buildMenu);
    }
}

ActionsManager *ActionsManager::instance()
{
    if (!m_instance)
        m_instance = new ActionsManager;
    return m_instance;
}

ActionsManager::ActionsManager()
    : m_actions()
{
    init();
}

void ActionsManager::init()
{
    deployDefaults();
    loadFiles();
}

void ActionsManager::deployDefaults()
{
    const QString targetDir = storagePath();
    const QString targetFile("contextmenu.json");
    const QString targetFilePath(targetDir + targetFile);

    shared::ensureDirExists(targetDir);

    if (!QFileInfo::exists(targetFilePath)) {
        shared::copyFile(":/defaults/resources/" + targetFile, targetFilePath);
    }
}

QStringList ActionsManager::listUserFiles()
{
    QStringList res;
    const QString &targetDir = storagePath();

    QDir dir(targetDir);
    if (!dir.exists())
        return res;

    const QStringList &names = dir.entryList({ "*.json" }, QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);
    for (const QString &name : names) {
        res.append(targetDir + name);
    }

    return res;
}

void ActionsManager::loadFiles()
{
    m_actions.clear();
    for (const QString &file : ActionsManager::listUserFiles()) {
        loadActions(file);
    }
}

void ActionsManager::reload()
{
    instance()->loadFiles();
}

void ActionsManager::loadActions(const QString &fromFile)
{
    const QVector<Action> &actions = parseFile(fromFile);
    if (!actions.isEmpty()) {
        m_actions.append(actions);
    }
}

QVector<Action> ActionsManager::parseFile(const QString &filePath, QString *errorHandler)
{
    shared::ErrorHub::clearFileErrors(filePath);
    QVector<Action> res;

    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const QString &msg = QString("Can't open file %1 - %2").arg(filePath, f.errorString());
        shared::ErrorHub::addError(shared::ErrorItem::Error, f.errorString(), filePath);
        if (errorHandler)
            *errorHandler = msg;
        return res;
    }
    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(f.readAll(), &err);
    if (err.error != QJsonParseError::NoError) {
        const QString &msg = QString("JSON parsing %1 failed: %2").arg(err.errorString(), filePath);
        shared::ErrorHub::addError(
                shared::ErrorItem::Error, QString("JSON parsing %1 failed: %2").arg(err.errorString()), filePath);
        if (errorHandler)
            *errorHandler = msg;
        return res;
    }

    QVersionNumber version(0, 0);

    QJsonArray jArr;
    if (doc.isArray()) {
        jArr = doc.array();
    } else {
        if (doc.isObject()) {
            QJsonObject jObj = doc.object();
            if (!jObj.contains("version")) {
                shared::ErrorHub::addError(shared::ErrorItem::Error, "JSON content does not have a version", filePath);
                return {};
            }
            if (jObj.contains("actions")) {
                jArr = jObj["actions"].toArray();
            }
            version = QVersionNumber::fromString(jObj["version"].toString());
            jArr = jObj["actions"].toArray();
        } else {
            shared::ErrorHub::addError(shared::ErrorItem::Error, "JSON content does not match", filePath);
            return {};
        }
    }

    for (const auto &obj : qAsConst(jArr)) {
        res.append(Action(obj.toObject()));
    }

    return res;
}

bool ActionsManager::saveFile(const QString &filePath, const QVector<Action> &actions)
{
    QJsonObject jobj;
    jobj.insert("version", "1.0");

    QJsonArray jArr;
    for (const Action &act : actions) {
        jArr.append(act.toJson());
    }
    jobj.insert("actions", jArr);

    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qWarning() << "Can't open file" << filePath << "for writing:" << f.errorString();
        return false;
    }
    f.write(QJsonDocument(jobj).toJson());

    return true;
}

/*!
 * \brief ActionsManager::registerScriptableAction
 * Marks the \a action as script able and makes it available by \a key.
 * The \a description is optional.
 * Returns \c false if \a action is null, the \a key is empty already used.
 */
bool ActionsManager::registerScriptableAction(QAction *action, const QString &key, const QString &description)
{
    if (!action || key.isEmpty() || instance()->m_qactions.contains(key))
        return false;

    instance()->m_qactions.insert(key, { action, key, description });

    QObject::connect(action, &QObject::destroyed, [key]() { instance()->m_qactions.remove(key); });

    return true;
}

QString ActionsManager::listRegisteredActions()
{
    QString result = QObject::tr("Available actions:");

    const QMap<QString, ActionsManager::ScriptableActionHandler> &actions = ActionsManager::scriptableActions();
    const QStringList &names = actions.keys();
    const int titleLength = std::max_element(names.cbegin(), names.cend(), [](const QString &lhs, const QString &rhs) {
        return lhs.length() < rhs.length();
    })->length();

    for (const ActionsManager::ScriptableActionHandler &h : actions) {
        QString padding;
        padding.fill(' ', titleLength - h.m_title.length());
        result += QString("\n%1%2 - %3").arg(h.m_title, padding, h.m_description);
    }
    return result;
}

QMap<QString, ActionsManager::ScriptableActionHandler> ActionsManager::scriptableActions()
{
    return instance()->m_qactions;
}

void ActionsManager::clearActions()
{
    m_actions.clear();
    m_qactions.clear();
}

QStringList ActionsManager::scriptableActionNames()
{
    return scriptableActions().keys();
}

/*!
 * \brief ActionsManager::triggerActionInternal
 * Handler for "internal" actions. Triggers a QAction previously registered with
 * ActionsManager::registerScriptableAction()
 */
void ActionsManager::triggerActionInternal(const Action &act)
{
    const QMap<QString, ActionsManager::ScriptableActionHandler> &actions = ActionsManager::scriptableActions();
    if (actions.contains(act.m_internalActName)) {
        actions.value(act.m_internalActName).m_action->trigger();
    } else {
        qWarning() << "Scriptable action" << act.m_internalActName << "not found";
    }
}

QString ActionsManager::replaceKeyHolder(
        const QString &text, const ivm::IVObject *ivObj, const QString &projectDir, const QString &filepath)
{
    if (text.isEmpty() || text[0] != '$') {
        return text;
    }

    for (const ExternalArgHolder &holder : externalArgs(false)) {
        const QString name = text.mid(holder.key.size());
        switch (holder.target) {
        case ExternalArgHolder::FilePath:
            if (text == holder.key)
                return QFileInfo(filepath).absoluteFilePath();
            break;
        case ExternalArgHolder::ProjectPath:
            if (text == holder.key)
                return QFileInfo(projectDir).absolutePath();
            break;
        case ExternalArgHolder::CWD:
            if (text == holder.key)
                return qApp->applicationDirPath();
            break;
        case ExternalArgHolder::Attr:
            if (text.startsWith(holder.key) && ivObj) {
                return ivObj->entityAttributeValue<QString>(name);
            }
            break;
        case ExternalArgHolder::Prop:
            if (text.startsWith(holder.key) && ivObj) {
                return ivObj->entityAttributeValue<QString>(name);
            }
            break;
        case ExternalArgHolder::Param:
            if (text.startsWith(holder.key) && ivObj) {
                switch (ivObj->type()) {
                case ivm::IVObject::Type::RequiredInterface:
                case ivm::IVObject::Type::ProvidedInterface:
                    if (const ivm::IVInterface *iface = ivObj->as<const ivm::IVInterface *>()) {
                        const shared::InterfaceParameter &ifaceParam = iface->param(name);
                        if (!ifaceParam.isNull())
                            return ifaceParam.toString();
                    }
                    break;
                case ivm::IVObject::Type::Function:
                case ivm::IVObject::Type::FunctionType:
                    if (const ivm::IVFunctionType *fn = ivObj->as<const ivm::IVFunctionType *>()) {
                        const shared::ContextParameter &ctxParam = fn->contextParam(name);
                        if (!ctxParam.isNull())
                            return ctxParam.toString();
                    }
                    break;
                default:
                    break;
                }
            }
            break;
        }
    }

    return QString(); // clear the placeholder if no data found
}

/*!
 * \brief ActionsManager::triggerActionExternal
 * Handler for "external" actions.
 * Used to launch the external process with arguments, both are from \a act.
 * Replaces the keyholders by actual values of \a ivObj's attributes or parameters.
 * Creates and shows an instance of ExtProcMonitor.
 */
void ActionsManager::triggerActionExternal(
        const Action &act, const ivm::IVObject *ivObj, InterfaceDocument *doc, const QString &filepath)
{
    if (!act.m_externalApp.isEmpty()) {
        if (doc->isDirty()) {
            const QMessageBox::StandardButtons btns(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
            auto btn = QMessageBox::question(nullptr, QObject::tr("Document closing"),
                    QObject::tr("There are unsaved changes.\nWould you like to save the document?"), btns);
            if (btn == QMessageBox::Save) {
                QString savePath { filepath };
                if (savePath.isEmpty()) {
                    QFileDialog dialog(qApp->activeWindow(), QObject::tr("Export data to an XML file"));
                    dialog.setAcceptMode(QFileDialog::AcceptSave);
                    dialog.setDefaultSuffix(".xml");
                    if (dialog.exec() != QDialog::Accepted) {
                        return;
                    }
                    savePath = dialog.selectedUrls().value(0).toLocalFile();
                }
                IVExporter exporter;
                const bool ok = exporter.exportDocSilently(doc, savePath);
                if (!ok) {
                    QMessageBox::warning(
                            nullptr, QObject::tr("Save error"), QObject::tr("Unable to save the document.\nAborting"));
                    return;
                } else {
                    doc->undoStack()->setClean();
                }
            } else if (btn == QMessageBox::Cancel) {
                return;
            }
        }

        QStringList params;
        for (const QString &param : act.m_externalAppParams) {
            if (!param.isEmpty()) {
                params.append(replaceKeyHolder(param, ivObj, doc->path(), filepath));
            }
        }
        const QString cwd = replaceKeyHolder(act.m_externalAppCwd, ivObj, doc->path(), filepath);

        QWidget *mainWindow(nullptr);
        for (auto w : qApp->topLevelWidgets())
            if (w->inherits("QMainWindow")) {
                mainWindow = w;
                break;
            }

        ExtProcMonitor *mon = new ExtProcMonitor(mainWindow);
        mon->setAttribute(Qt::WA_DeleteOnClose);

        mon->start(act.m_externalApp, params, cwd);
    }
}

bool ActionsManager::triggerActionHidden(const Action &act)
{
    if (act.m_title == "List scriptable actions") {
        QMessageBox *msg = new QMessageBox(QMessageBox::Information, QObject::tr("Scriptable Actions"),
                listRegisteredActions(), QMessageBox::Ok, nullptr);
        const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        msg->setFont(fixedFont);
        msg->setAttribute(Qt::WA_DeleteOnClose);
        msg->exec();
        return true;
    }
    return false;
}

QStringList ActionsManager::externalArgsHoldersDescr()
{
    QStringList result;
    for (const auto &t : externalArgs(true)) {
        result.append(QString("%1 - %2").arg(t.key, t.description));
    }
    return result;
}

/*!
  \brief Helper function for registering the \a action as script-able, with key \a title and optional \a description.
 * The \a caller argument is used to provide reasonable debug record.
 */
void ActionsManager::registerAction(
        const QString &caller, QAction *action, const QString &title, const QString &description)
{
    if (!action) {
        qWarning() << "Null action can not be registered" << caller;
        return;
    }

    const QString &titleRef = title.isEmpty() ? action->text() : title;
    if (titleRef.isEmpty()) {
        qWarning() << "Action with no Title not be registered" << caller;
        return;
    }

    if (description.isEmpty()) {
        qWarning() << "Action with no Description not be registered" << caller;
        return;
    }

    if (!ActionsManager::registerScriptableAction(action, title, description)) {
        qWarning() << caller << "The registration of action failed; probably the duplicate key used:\n";
    }
}
}
