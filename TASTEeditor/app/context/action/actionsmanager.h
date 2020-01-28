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

#include <QVector>

class QAction;

namespace taste3 {
namespace aadl {
class AADLObject;
} // ns aadl
} // ns taste3

class QMenu;
namespace taste3 {
namespace ctx {

class ActionsManager
{
public:
    static void populateMenu(QMenu *menu, taste3::aadl::AADLObject *currObj);
    static bool registerScriptableAction(QAction *action, const QString &key, const QString &description);
    static void listRegisteredActions();
    static ActionsManager *instance();

private:
    struct ScriptableActionHandler {
        QAction *m_action;
        QString m_title;
        QString m_description;
    };

    static ActionsManager *m_instance;

    ActionsManager();
    void init();

    void deployDefaults();
    void loadFiles();
    void loadActions(const QString &fromFile);

    static void triggerActionInternal(const Action &act);
    static void triggerActionExternal(const Action &act);

    static QMap<QString, ActionsManager::ScriptableActionHandler> scriptableActions();

    QVector<Action> m_actions;
    QMap<QString, ScriptableActionHandler> m_qactions;
};

} // ns ctx
} // ns taste3
