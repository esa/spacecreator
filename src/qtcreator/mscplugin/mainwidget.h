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

class QAction;
class QGraphicsView;
class QSplitter;
class QUndoStack;

namespace msc {
class ASN1FileView;
class ActionCreatorTool;
class BaseTool;
class CommentCreatorTool;
class ConditionCreatorTool;
class CoregionCreatorTool;
class DocumentTreeView;
class EntityDeleteTool;
class GraphicsView;
class InstanceCreatorTool;
class InstanceStopTool;
class MainModel;
class MessageCreatorTool;
class PointerTool;
class TimerCreatorTool;
}

namespace MscPlugin {
namespace Common {

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

    QAction *actionCopy() const { return m_actCopy; }
    QAction *actionPaste() const { return m_actPaste; }
    QAction *actionToolDelete() const;
    QVector<QAction *> toolActions() const;
    QVector<QAction *> hierarchyActions() const;

public Q_SLOTS:
    void selectCurrentChart();

Q_SIGNALS:
    void dirtyChanged(bool dirty);

private Q_SLOTS:
    void showChart(const QModelIndex &index);
    void showSelection(const QModelIndex &current, const QModelIndex &previous);
    void showDocumentView(bool show);
    void showHierarchyView(bool show);

private:
    void initUi();
    void initActions();
    void initTools();
    void initDocumentViewActions();
    void initConnections();
    void activateDefaultTool();

    QSplitter *m_horizontalSplitter = nullptr;
    QSplitter *m_leftVerticalSplitter = nullptr;

    QStackedWidget *m_centerView = nullptr;
    msc::GraphicsView *m_graphicsView;
    msc::GraphicsView *m_hierarchyView;

    msc::DocumentTreeView *m_documentTree = nullptr;
    msc::ASN1FileView *m_asn1Widget = nullptr;

    msc::MainModel *m_model = nullptr;

    QAction *m_actCopy = nullptr;
    QAction *m_actPaste = nullptr;

    QVector<msc::BaseTool *> m_tools;
    msc::EntityDeleteTool *m_deleteTool = nullptr;
    msc::PointerTool *m_pointerTool = nullptr;
    msc::InstanceCreatorTool *m_instanceCreatorTool = nullptr;
    msc::InstanceStopTool *m_instanceStopTool = nullptr;
    msc::MessageCreatorTool *m_messageCreateTool = nullptr;
    msc::MessageCreatorTool *m_createCreateTool = nullptr;
    msc::CommentCreatorTool *m_commentCreateTool = nullptr;
    msc::CommentCreatorTool *m_globalCommentCreateTool = nullptr;
    msc::CoregionCreatorTool *m_coregionCreateTool = nullptr;
    msc::ActionCreatorTool *m_actionCreateTool = nullptr;
    msc::ConditionCreatorTool *m_conditionCreateTool = nullptr;
    msc::ConditionCreatorTool *m_sharedConditionTool = nullptr;
    msc::TimerCreatorTool *m_startTimerCreateTool = nullptr;
    msc::TimerCreatorTool *m_stopTimerCreateTool = nullptr;
    msc::TimerCreatorTool *m_timeoutTimerCreateTool = nullptr;

    QVector<msc::BaseTool *> m_hierarchyTools;
};

} // namespace Common
} // namespace MscPlugin
