/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QAction>
#include <QPointer>
#include <QVector>
#include <QWidget>

class QStackedWidget;

namespace Ui {
class MSCAppWidget;
}

namespace msc {
class GraphicsView;
class MainModel;
class MSCEditorCore;
class ActionCreatorTool;
class BaseTool;
class CommentCreatorTool;
class ConditionCreatorTool;
class CoregionCreatorTool;
class EntityDeleteTool;
class InstanceCreatorTool;
class InstanceStopTool;
class MessageCreatorTool;
class PointerTool;
class TimerCreatorTool;

class MscAppWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(msc::MscAppWidget::ViewMode viewMode READ viewMode WRITE setViewMode NOTIFY viewModeChanged)

public:
    enum class ViewMode
    {
        CHART,
        HIERARCHY
    };
    Q_ENUM(ViewMode);

    explicit MscAppWidget(MSCEditorCore *mscCore, QWidget *parent = nullptr);
    ~MscAppWidget();

    QStackedWidget *centerView();
    GraphicsView *chartView();
    GraphicsView *hierarchyView();

    ViewMode viewMode();

    void initChartTools();
    void initHierarchyViewActions();
    void initConnections();

    QVector<QAction *> chartActions() const;
    QVector<QAction *> hierarchyActions() const;

    msc::PointerTool *pointerTool() const;
    msc::EntityDeleteTool *deleteTool();
    msc::BaseTool *activeTool() const;

    void showAsn1View(bool show);

public Q_SLOTS:
    void setViewMode(ViewMode mode);
    void showDocumentView(bool show);
    void showHierarchyView(bool show);
    void activateDefaultTool();
    void checkGlobalComment();
    void checkInstances();
    void checkMessages();

Q_SIGNALS:
    void viewModeChanged(ViewMode);
    void showAsn1File(const QString &asnFilename);
    void selectAsn1();
    void showInterfaceView();

private Q_SLOTS:
    void updateMscToolbarActionsChecked();
    void updateHierarchyActions();
    void showChart(const QModelIndex &index);
    void showSelection(const QModelIndex &current, const QModelIndex &previous);

private:
    msc::MainModel *mainModel() const;
    QVector<QAction *> initViewActions();

    Ui::MSCAppWidget *ui;
    QPointer<MSCEditorCore> m_mscCore;
    ViewMode m_viewMode = ViewMode::CHART;

    QAction *m_actionCopy = nullptr;
    QAction *m_actionPaste = nullptr;
    QAction *m_actionMessageDeclarations = nullptr;

    QVector<msc::BaseTool *> m_tools;
    QAction *m_defaultToolAction = nullptr;
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

    QVector<QAction *> m_hierarchyActions;
    QVector<QAction *> m_viewActions;
};

} // namespace msc
