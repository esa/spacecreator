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
class MSCEditorCore;
}

namespace shared {
class ActionsBar;
}

namespace spctr {
class MscModelStorage;

class MscMainWidget : public QWidget
{
    Q_OBJECT
public:
    MscMainWidget(QWidget *parent = nullptr);
    ~MscMainWidget();

    bool init(QSharedPointer<msc::MSCEditorCore> plugin);

    QSharedPointer<msc::MSCEditorCore> mscCore() const;

private Q_SLOTS:
    void showChart(const QModelIndex &index);
    void showSelection(const QModelIndex &current, const QModelIndex &previous);
    void showAsn1Errors(const QStringList &faultyMessages);
    void onViewModeChanged();
    void openAsn1Dialog();

private:
    void init();
    void initConnections();

    shared::ActionsBar *m_chartToolBar = nullptr;
    shared::ActionsBar *m_documentToolBar = nullptr;

    msc::DocumentTreeView *m_documentTree = nullptr;
    QPushButton *m_aadlSwitch = nullptr;
    QPushButton *m_asn1Switch = nullptr;
    QPushButton *m_asn1Select = nullptr;

    QSharedPointer<msc::MSCEditorCore> m_plugin;
};

}
