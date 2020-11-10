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

#include "editorcore.h"
#include "graphicsview.h"
#include "mscdocument.h"

#include <QStackedWidget>
#include <QVector>
#include <memory>

namespace aadl {
class AADLObjectConnection;
class AADLObjectFunction;
}

namespace shared {
class UndoCommand;
}

namespace msc {
class AadlSystemChecks;
class ActionCreatorTool;
class BaseTool;
class CommentCreatorTool;
class ConditionCreatorTool;
class CoregionCreatorTool;
class EntityDeleteTool;
class InstanceCreatorTool;
class InstanceStopTool;
class MainModel;
class MessageCreatorTool;
class MscChart;
class MscCommandsStack;
class PointerTool;
class TimerCreatorTool;

class MainWindow;

class MSCEditorCore : public shared::EditorCore
{
    Q_OBJECT
    Q_PROPERTY(msc::MSCEditorCore::ViewMode viewMode READ viewMode WRITE setViewMode NOTIFY viewModeChanged)

public:
    enum class ViewMode
    {
        CHART,
        HIERARCHY
    };
    Q_ENUM(ViewMode);

    explicit MSCEditorCore(QObject *parent = nullptr);
    ~MSCEditorCore();

    msc::MainModel *mainModel() const;

    void setPluginActive(bool active) override;

    void setViews(
            QStackedWidget *centerView, shared::ui::GraphicsViewBase *chartView, msc::GraphicsView *hierarchyView);
    QStackedWidget *centerView();
    shared::ui::GraphicsViewBase *chartView() override;
    msc::GraphicsView *hierarchyView();

    void initChartTools();
    void initHierarchyViewActions();
    void initConnections();

    void addToolBars(QMainWindow *window) override;
    void addMenuFileActions(QMenu *menu, QMainWindow *window) override;
    void addMenuEditActions(QMenu *menu, QMainWindow *window) override;
    void addMenuViewActions(QMenu *menu, QMainWindow *window) override;
    void addMenuHelpActions(QMenu *menu, QMainWindow *window) override;

    QToolBar *mscToolBar();
    QToolBar *hierarchyToolBar();
    void showToolbars(bool show);

    void populateCommandLineArguments(shared::CommandLineParser *parser) const override;

    QAction *actionCopy() { return m_actionCopy; }
    QAction *actionPaste() { return m_actionPaste; }
    msc::EntityDeleteTool *deleteTool() { return m_deleteTool; }
    msc::BaseTool *activeTool() const;

    QAction *actionShowDocument() { return m_actionShowDocument; }
    QAction *actionShowHierarchy() { return m_actionShowHierarchy; }

    QVector<QAction *> chartActions() const;
    QVector<QAction *> hierarchyActions() const;

    QAction *createActionCopy(MainWindow *window);
    QAction *createActionPaste(MainWindow *window);

    msc::AadlSystemChecks *aadlChecker() const;

    ViewMode viewMode();

    QUndoStack *undoStack() const override;
    msc::MscCommandsStack *commandsStack() const;

    bool renameAsnFile(const QString &oldName, const QString &newName) override;
    void changeMscInstanceName(const QString &oldName, const QString &name);
    void changeMscMessageName(
            const QString &oldName, const QString &newName, const QString &sourceName, const QString &targetName);
    void removeMscInstances(aadl::AADLObjectFunction *aaldFunction);
    void removeMscMessages(aadl::AADLObjectConnection *aadlConnection);

    QString filePath() const override;
    bool save() override;

public Q_SLOTS:
    void setViewMode(ViewMode mode);
    void showDocumentView(bool show);
    void showHierarchyView(bool show);
    void activateDefaultTool();
    void selectCurrentChart();
    void checkGlobalComment();
    void openMessageDeclarationEditor(QWidget *parentwidget);

Q_SIGNALS:
    void viewModeChanged(ViewMode);
    void nameChanged(QObject *entity, const QString &oldName, shared::UndoCommand *command);

private Q_SLOTS:
    void updateMscToolbarActionsChecked();
    void updateHierarchyActions();
    void addDocument(msc::MscDocument::HierarchyType type);

private:
    std::unique_ptr<msc::MainModel> m_model;
    std::unique_ptr<msc::AadlSystemChecks> m_aadlChecks;

    ViewMode m_viewMode = ViewMode::CHART;

    QPointer<QStackedWidget> m_centerView = nullptr;
    QPointer<shared::ui::GraphicsViewBase> m_chartView = nullptr;
    QPointer<msc::GraphicsView> m_hierarchyView = nullptr;

    QPointer<QToolBar> m_mscToolBar = nullptr;
    QPointer<QToolBar> m_hierarchyToolBar = nullptr;

    QAction *m_actionScreenshot = nullptr;
    QAction *m_editSeparator = nullptr;
    QAction *m_actionCopy = nullptr;
    QAction *m_actionPaste = nullptr;
    QAction *m_actionShowDocument = nullptr;
    QAction *m_actionShowHierarchy = nullptr;

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

    bool m_toolbarsVisible = true;
};

}
