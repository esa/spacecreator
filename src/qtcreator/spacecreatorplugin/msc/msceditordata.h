/*
   Copyright (C) 2018 - 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QPointer>
#include <QSharedPointer>

class QAction;
class QUndoGroup;

namespace msc {
class MSCEditorCore;
}

namespace Core {
class IEditor;
}

namespace spctr {

class MscContext;
class SpaceCreatorProjectManager;

class MscEditorData : public QObject
{
    Q_OBJECT
public:
    MscEditorData(
            SpaceCreatorProjectManager *projectManager, const QList<QAction *> &mscActions, QObject *parent = nullptr);
    ~MscEditorData() override;

    Core::IEditor *createEditor();

public Q_SLOTS:
    void setMinimapVisible(bool visible);

private Q_SLOTS:
    void onCurrentEditorChanged(Core::IEditor *editor);

private:
    MscContext *m_context = nullptr;
    QUndoGroup *m_undoGroup = nullptr;

    bool m_minimapVisible = false;

    QPointer<SpaceCreatorProjectManager> m_projectManager;
    QList<QAction *> m_mscActions;
};

}
