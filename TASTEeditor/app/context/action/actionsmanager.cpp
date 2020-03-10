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

#include "app/common.h"
#include "extprocmonitor.h"
#include "tab_aadl/aadlobject.h"
#include "tab_aadl/aadlobjectfunction.h"
#include "tab_aadl/aadlobjectiface.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenu>
#include <QMessageBox>
#include <QMetaObject>
#include <QObject>
#include <QProcess>
#include <QStandardPaths>
#include <iomanip>
#include <iostream>

namespace taste3 {
namespace ctx {

ActionsManager *ActionsManager::m_instance = nullptr;

const QVector<ActionsManager::ExternalArgHolder> ActionsManager::externalArgs = {
    { "$TASTE3", QObject::tr("Path to the Taste binary"), ExternalArgHolder::Type::CWD },
    { "$attr_", QObject::tr("Value of the selected object's attribute [name]"), ExternalArgHolder::Type::Attr },
    { "$prop_", QObject::tr("Value of the selected object's property [name]"), ExternalArgHolder::Type::Prop },
    { "$param_", QObject::tr("Value of the selected Interface's parameter (or Function's context parameter) [name]."),
      ExternalArgHolder::Type::Param },
};

QString ActionsManager::storagePath()
{
    static const QString targetDir =
            QString("%1/contextMenu/").arg(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    return targetDir;
};

void ActionsManager::populateMenu(QMenu *menu, taste3::aadl::AADLObject *currObj)
{
    if (!menu)
        return;

    menu->addSeparator();

    for (const Action &actHandler : instance()->m_actions) {
        QAction *act = new QAction(actHandler.m_title, menu);
        act->setData(QVariant::fromValue(currObj));
        const bool enabled(actHandler.isAcceptable(currObj));
        act->setEnabled(enabled);
        if (enabled) {
            QObject::connect(act, &QAction::triggered, [actHandler, act]() {
                if (!actHandler.m_internalActName.isEmpty())
                    triggerActionInternal(actHandler);
                else if (!actHandler.m_externalApp.isEmpty()) {
                    triggerActionExternal(actHandler, act ? act->data().value<taste3::aadl::AADLObject *>() : nullptr);
                } else {
                    QMessageBox::warning(
                            nullptr, QObject::tr("Custom action"),
                            QObject::tr("No internal or extrernal action provided by %1").arg(actHandler.m_title));
                }
            });
        }
        menu->addAction(act);
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
    static const QString &targetDir = storagePath();
    static const QString targetFile("contextmenu.json");
    static const QString targetFilePath(targetDir + targetFile);

    common::ensureDirExists(targetDir);

    if (!QFileInfo::exists(targetFilePath))
        common::copyResourceFile(":/defaults/app/resources/" + targetFile, targetFilePath);
}

QStringList ActionsManager::listUserFiles()
{
    QStringList res;
    const QString &targetDir = storagePath();

    QDir dir(targetDir);
    if (!dir.exists())
        return res;

    const QStringList &names = dir.entryList({ "*.json" }, QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);
    for (const QString &name : names)
        res.append(targetDir + name);

    return res;
}

void ActionsManager::loadFiles()
{
    m_actions.clear();
    for (const QString &file : ActionsManager::listUserFiles())
        loadActions(file);
}

void ActionsManager::reload()
{
    instance()->loadFiles();
}

void ActionsManager::loadActions(const QString &fromFile)
{
    const QVector<Action> &actions = parseFile(fromFile);
    if (!actions.isEmpty())
        m_actions.append(actions);
}

QVector<Action> ActionsManager::parseFile(const QString &filePath, QString *errorHandler)
{
    QVector<Action> res;

    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const QString &msg = QString("Can't open file %1 - %2").arg(filePath, f.errorString());
        qWarning() << msg;
        if (errorHandler)
            *errorHandler = msg;
        return res;
    }
    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(f.readAll(), &err);
    if (err.error != QJsonParseError::NoError) {
        const QString &msg = QString("JSON parsing %1 failed: %2").arg(err.errorString(), filePath);
        qWarning() << msg;
        if (errorHandler)
            *errorHandler = msg;
        return res;
    }

    const QJsonArray &root = doc.array();
    for (auto obj : root)
        res.append(Action(obj.toObject()));

    return res;
}

bool ActionsManager::registerScriptableAction(QAction *action, const QString &key, const QString &description)
{
    if (!action || key.isEmpty() || instance()->m_qactions.contains(key))
        return false;

    instance()->m_qactions.insert(key, { action, key, description });

    QObject::connect(action, &QObject::destroyed, [key]() { instance()->m_qactions.remove(key); });

    return true;
}

void ActionsManager::listRegisteredActions()
{
    const QMap<QString, ctx::ActionsManager::ScriptableActionHandler> &actions =
            ctx::ActionsManager::scriptableActions();
    const QStringList &names = actions.keys();
    const int titleLength = std::max_element(names.cbegin(), names.cend(), [](const QString &lhs, const QString &rhs) {
                                return lhs.length() < rhs.length();
                            })->length();

    std::cout << qPrintable(QObject::tr("Available actions:")) << std::endl;
    for (const ctx::ActionsManager::ScriptableActionHandler &h : actions)
        std::cout << std::left << std::setw(titleLength) << qPrintable(h.m_title) << "\t- "
                  << qPrintable(h.m_description) << std::endl;
}

QMap<QString, ActionsManager::ScriptableActionHandler> ActionsManager::scriptableActions()
{
    return instance()->m_qactions;
}

QStringList ActionsManager::scriptableActionNames()
{
    return scriptableActions().keys();
}

void ActionsManager::triggerActionInternal(const Action &act)
{
    const QMap<QString, ctx::ActionsManager::ScriptableActionHandler> &actions =
            ctx::ActionsManager::scriptableActions();
    if (actions.contains(act.m_internalActName)) {
        qDebug() << "Triggering action:" << act.m_internalActName;
        actions.value(act.m_internalActName).m_action->trigger();
    } else {
        qWarning() << "Scriptable action" << act.m_internalActName << "not found";
    }
}

QString ActionsManager::replaceKeyHolder(const QString &text, const taste3::aadl::AADLObject *aadlObj)
{
    if (text.isEmpty() || !aadlObj)
        return {};

    if (text[0] != '$')
        return text;

    for (const ActionsManager::ExternalArgHolder &holder : externalArgs) {
        const QString name = text.mid(holder.key.size());
        switch (holder.target) {
        case ActionsManager::ExternalArgHolder::CWD: {
            if (text == holder.key)
                return qApp->applicationDirPath();
            break;
        }
        case ActionsManager::ExternalArgHolder::Attr: {
            if (text.startsWith(holder.key)) {
                return aadlObj->attr(name).toString();
            }
            break;
        }
        case ActionsManager::ExternalArgHolder::Prop: {
            if (text.startsWith(holder.key)) {
                return aadlObj->prop(name).toString();
            }
            break;
        }
        case ActionsManager::ExternalArgHolder::Param: {
            if (text.startsWith(holder.key)) {
                switch (aadlObj->aadlType()) {
                case aadl::AADLObject::Type::Interface: {
                    if (const aadl::AADLObjectIface *iface = aadlObj->as<const aadl::AADLObjectIface *>()) {
                        const aadl::IfaceParameter &ifaceParam = iface->param(name);
                        if (!ifaceParam.isNull())
                            return ifaceParam.toString();
                    }
                    break;
                }
                case aadl::AADLObject::Type::Function:
                case aadl::AADLObject::Type::FunctionType: {
                    if (const aadl::AADLObjectFunctionType *fn = aadlObj->as<const aadl::AADLObjectFunctionType *>()) {
                        const aadl::ContextParameter &ctxParam = fn->contextParam(name);
                        if (!ctxParam.isNull())
                            return ctxParam.toString();
                    }
                    break;
                }
                default:
                    break;
                }
            }
            break;
        }
        }
    }

    return QString(); // clear the placeholder if no data found
}

void ActionsManager::triggerActionExternal(const Action &act, const taste3::aadl::AADLObject *aadlObj)
{
    if (!act.m_externalApp.isEmpty()) {
        QStringList params = act.m_externalAppParams;
        params.prepend(act.m_externalAppCwd);

        for (QString &param : params)
            param = replaceKeyHolder(param, aadlObj);

        const QString cwd = params.takeFirst();

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

QStringList ActionsManager::externalArgsHoldersDescr()
{
    QStringList result;
    for (auto t : externalArgs) {
        QString uiName = t.key;
        uiName.replace(QLatin1String("_"), QLatin1String("_name"));
        result.append(QString("%1 - %2").arg(uiName, t.description));
    }
    return result;
}

} // ns ctx
} // ns taste3
