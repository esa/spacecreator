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

#include "msceditorcore.h"

#include <QPointer>
#include <QSharedPointer>
#include <QStackedWidget>
#include <QWidget>
#include <memory>

class QAction;
class QGraphicsView;
class QPushButton;
class QSplitter;
class QUndoStack;
class QVBoxLayout;

namespace msc {
class DocumentTreeView;
class GraphicsView;
}

namespace shared {
class ActionsBar;
}

namespace spctr {
class SpaceCreatorProjectImpl;

class MscMainWidget : public QWidget
{
    Q_OBJECT
public:
    MscMainWidget(QWidget *parent = nullptr);
    ~MscMainWidget();

    bool init(MSCEditorCorePtr plugin, SpaceCreatorProjectImpl *project);

    MSCEditorCorePtr mscCore() const;

private Q_SLOTS:
    void openAsn1Dialog();

private:
    void init();

    MSCEditorCorePtr m_plugin;
    QPointer<SpaceCreatorProjectImpl> m_project;
};

}
