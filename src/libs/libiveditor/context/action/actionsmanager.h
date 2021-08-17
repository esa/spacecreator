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

#pragma once
#include "action.h"

#include <QMap>
#include <QVector>

class QAction;

namespace ivm {
class IVObject;
}

class QMenu;

namespace ive {

class InterfaceDocument;

class ActionsManager
{
public:
    static void registerDeploymentFilesCallback(std::function<QStringList()> cb);
    static void populateMenu(QMenu *menu, ivm::IVObject *currObj, InterfaceDocument *doc);
    static bool registerScriptableAction(QAction *action, const QString &key, const QString &description);
    static QString listRegisteredActions();
    static ActionsManager *instance();
    static QVector<Action> parseFile(const QString &filePath, QString *errorHandler = nullptr);
    static bool saveFile(const QString &filePath, const QVector<Action> &actions);
    static QStringList listUserFiles();
    static QString storagePath();
    static QStringList scriptableActionNames();
    static void reload();
    static QStringList externalArgsHoldersDescr();

    static void registerAction(const QString &caller, QAction *action, const QString &title = QString(),
            const QString &description = QString());

    struct ScriptableActionHandler {
        QAction *m_action;
        QString m_title;
        QString m_description;
    };

    static QMap<QString, ActionsManager::ScriptableActionHandler> scriptableActions();

    void clearActions();

private:
    static ActionsManager *m_instance;

    ActionsManager();
    void init();

    void deployDefaults();
    void loadFiles();
    void loadActions(const QString &fromFile);

    static void triggerActionInternal(const Action &act);
    static void triggerActionExternal(
            const Action &act, const ivm::IVObject *ivObj, InterfaceDocument *doc, const QString &filepath);
    static bool triggerActionHidden(const Action &act);

    static QString replaceKeyHolder(
            const QString &text, const ivm::IVObject *ivObj, const QString &projectDir, const QString &filepath);

    QVector<Action> m_actions;
    QMap<QString, ScriptableActionHandler> m_qactions;
    std::function<QStringList()> m_deploymentFilesCallBack;
};

}
