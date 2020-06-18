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

#include <QStackedWidget>
#include <QWidget>
#include <memory>

class QAction;
class QGraphicsView;
class QSplitter;
class QUndoStack;

namespace msc {
class ASN1FileView;
class DocumentTreeView;
class GraphicsView;
class MSCPlugin;
}

namespace MscPlugin {

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    bool load(const QString &filename);
    bool save();
    QString errorMessage() const;

    void setFileName(const QString &filename);

    bool isDirty() const;
    QUndoStack *undoStack();

    QString textContents() const;

    QAction *actionCopy() const;
    QAction *actionPaste() const;
    QAction *actionToolDelete() const;
    QVector<QAction *> toolActions() const;

    msc::MSCPlugin *mscPlugin() const;

Q_SIGNALS:
    void dirtyChanged(bool dirty);

private Q_SLOTS:
    void showChart(const QModelIndex &index);
    void showSelection(const QModelIndex &current, const QModelIndex &previous);

private:
    void initUi();
    void initActions();
    void initConnections();

    QSplitter *m_horizontalSplitter = nullptr;
    QSplitter *m_leftVerticalSplitter = nullptr;

    msc::DocumentTreeView *m_documentTree = nullptr;
    msc::ASN1FileView *m_asn1Widget = nullptr;

    std::unique_ptr<msc::MSCPlugin> m_plugin;
};

}
