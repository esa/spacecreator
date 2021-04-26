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

#include "msceditorcore.h"

#include "commandlineparser.h"
#include "commands/cmddeleteentity.h"
#include "commands/cmddocumentcreate.h"
#include "commands/cmdentitynamechange.h"
#include "commands/cmdsetasn1file.h"
#include "commands/cmdsetmessagedeclarations.h"
#include "hierarchyviewmodel.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "mainmodel.h"
#include "messagedeclarationsdialog.h"
#include "mscchart.h"
#include "msccommandsstack.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessagedeclarationlist.h"
#include "mscmodel.h"
#include "systemchecks.h"
#include "tools/actioncreatortool.h"
#include "tools/basetool.h"
#include "tools/commentcreatortool.h"
#include "tools/conditioncreatortool.h"
#include "tools/coregioncreatortool.h"
#include "tools/entitydeletetool.h"
#include "tools/instancecreatortool.h"
#include "tools/instancestoptool.h"
#include "tools/messagecreatortool.h"
#include "tools/pointertool.h"
#include "tools/timercreatortool.h"
#include "ui/graphicsviewbase.h"

#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QFileInfo>
#include <QMainWindow>
#include <QMenu>
#include <QMimeData>
#include <QStackedWidget>
#include <QToolBar>
#include <QUndoGroup>
#include <QUndoStack>
#include <QUrl>

namespace msc {

static const char *HIERARCHY_TYPE_TAG = "hierarchyTag";

/*!
 * \class MSCEditorCore
 * \brief Has most of the code for the MSC Editor UI and the underlying library
 */

MSCEditorCore::MSCEditorCore(QObject *parent)
    : shared::EditorCore(parent)
    , m_model(new msc::MainModel())
{
    setSystemChecker(new SystemChecks(this));

    connect(m_model->commandsStack(), &msc::MscCommandsStack::nameChanged, this, &msc::MSCEditorCore::nameChanged);
}

MSCEditorCore::~MSCEditorCore() { }

MainModel *MSCEditorCore::mainModel() const
{
    return m_model.get();
}

void MSCEditorCore::setViews(
        QStackedWidget *centerView, shared::ui::GraphicsViewBase *chartView, GraphicsView *hierarchyView)
{
    m_centerView = centerView;
    m_chartView = chartView;
    m_hierarchyView = hierarchyView;

    if (m_chartView) {
        m_chartView->setScene(mainModel()->graphicsScene());
    }
    if (m_hierarchyView) {
        m_hierarchyView->setScene(mainModel()->hierarchyScene());
    }
}

QStackedWidget *MSCEditorCore::centerView()
{
    return m_centerView;
}

shared::ui::GraphicsViewBase *MSCEditorCore::chartView()
{
    return m_chartView;
}

GraphicsView *MSCEditorCore::hierarchyView()
{
    return m_hierarchyView;
}

void MSCEditorCore::initChartTools()
{
    Q_ASSERT(m_chartView != nullptr);

    if (!m_tools.isEmpty()) {
        qDeleteAll(m_tools);
        m_tools.clear();
    }

    m_pointerTool = new msc::PointerTool(nullptr, this);
    m_tools.append(m_pointerTool);

    m_instanceCreatorTool = new msc::InstanceCreatorTool(&(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_instanceCreatorTool);

    m_instanceStopTool = new msc::InstanceStopTool(&(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_instanceStopTool);

    m_messageCreateTool = new msc::MessageCreatorTool(
            msc::MscMessage::MessageType::Message, &(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_messageCreateTool);

    m_createCreateTool = new msc::MessageCreatorTool(
            msc::MscMessage::MessageType::Create, &(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_createCreateTool);

    m_commentCreateTool = new msc::CommentCreatorTool(false, &(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_commentCreateTool);

    m_globalCommentCreateTool = new msc::CommentCreatorTool(true, &(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_globalCommentCreateTool);

    m_coregionCreateTool = new msc::CoregionCreatorTool(&(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_coregionCreateTool);

    m_actionCreateTool = new msc::ActionCreatorTool(&(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_actionCreateTool);

    m_conditionCreateTool = new msc::ConditionCreatorTool(false, &(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_conditionCreateTool);

    m_sharedConditionTool = new msc::ConditionCreatorTool(true, &(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_sharedConditionTool);

    m_startTimerCreateTool =
            new msc::TimerCreatorTool(msc::MscTimer::TimerType::Start, &(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_startTimerCreateTool);

    m_stopTimerCreateTool =
            new msc::TimerCreatorTool(msc::MscTimer::TimerType::Stop, &(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_stopTimerCreateTool);

    m_timeoutTimerCreateTool =
            new msc::TimerCreatorTool(msc::MscTimer::TimerType::Timeout, &(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_timeoutTimerCreateTool);

    QActionGroup *toolsActions = new QActionGroup(this);
    toolsActions->setExclusive(false);
    for (msc::BaseTool *tool : m_tools) {
        QAction *toolAction = mscToolBar()->addAction(tool->title());
        toolAction->setCheckable(true);
        toolAction->setIcon(tool->icon());
        toolAction->setToolTip(tr("%1: %2").arg(tool->title(), tool->description()));
        toolAction->setData(QVariant::fromValue<msc::BaseTool::ToolType>(tool->toolType()));
        toolAction->setEnabled(m_viewMode == ViewMode::CHART);
        tool->setView(m_chartView);
        tool->setAction(toolAction);

        if (msc::BaseCreatorTool *creatorTool = qobject_cast<msc::BaseCreatorTool *>(tool)) {
            connect(creatorTool, &msc::BaseCreatorTool::created, this, [&]() {
                m_chartView->setFocus();
                activateDefaultTool();
            });
            connect(creatorTool, &msc::BaseCreatorTool::canceled, this, &MSCEditorCore::activateDefaultTool);
        }

        toolsActions->addAction(toolAction);
        connect(toolAction, &QAction::toggled, this, &MSCEditorCore::updateMscToolbarActionsChecked);
    }

    m_defaultToolAction = toolsActions->actions().first();
    m_defaultToolAction->setVisible(false);

    m_deleteTool = new msc::EntityDeleteTool(&(m_model->chartViewModel()), m_chartView, this);
    m_deleteTool->setCurrentChart(m_model->chartViewModel().currentChart());
    connect(&m_model->chartViewModel(), &msc::ChartLayoutManager::currentChartChanged, m_deleteTool,
            &msc::EntityDeleteTool::setCurrentChart);

    activateDefaultTool();
}

void MSCEditorCore::initHierarchyViewActions()
{
    Q_ASSERT(m_hierarchyView != nullptr);

    if (!m_hierarchyActions.isEmpty()) {
        qDeleteAll(m_hierarchyActions);
        m_hierarchyActions.clear();
    }

    auto addAction = [&](msc::MscDocument::HierarchyType type, const QString &title, const QIcon &icon) {
        QAction *action = new QAction(title, this);
        action->setProperty(HIERARCHY_TYPE_TAG, type);
        action->setIcon(icon);
        action->setToolTip(title);
        action->setEnabled(m_viewMode == ViewMode::HIERARCHY);
        if (m_hierarchyToolBar) {
            m_hierarchyToolBar->addAction(action);
        }

        m_hierarchyActions.append(action);

        connect(action, &QAction::triggered, this, [&]() {
            msc::MscDocument::HierarchyType selectedType =
                    sender()->property(HIERARCHY_TYPE_TAG).value<msc::MscDocument::HierarchyType>();
            addDocument(selectedType);
        });
    };

    addAction(msc::MscDocument::HierarchyAnd, tr("Hierarchy And"), QIcon(":/icons/document_and.png"));
    addAction(msc::MscDocument::HierarchyOr, tr("Hierarchy Or"), QIcon(":/icons/document_or.png"));
    addAction(msc::MscDocument::HierarchyParallel, tr("Hierarchy Parallel"), QIcon(":/icons/document_parallel.png"));
    addAction(msc::MscDocument::HierarchyIs, tr("Hierarchy Is"), QIcon(":/icons/document_is_scenario.png"));
    addAction(msc::MscDocument::HierarchyRepeat, tr("Hierarchy Repeat"), QIcon(":/icons/document_repeat.png"));
    addAction(msc::MscDocument::HierarchyException, tr("Hierarchy Exception"), QIcon(":/icons/document_exception.png"));
    addAction(msc::MscDocument::HierarchyLeaf, tr("Hierarchy Leaf"), QIcon(":/icons/document_leaf.png"));
}

void MSCEditorCore::initConnections()
{
    if (m_connectionsDone) {
        return;
    }

    Q_ASSERT(m_chartView != nullptr);

    if (auto chartview = qobject_cast<GraphicsView *>(m_chartView)) {
        connect(chartview, &msc::GraphicsView::createMessageToolRequested, this, [&]() {
            if (m_messageCreateTool) {
                m_messageCreateTool->activate();
            }
        });
    }

    connect(&(m_model->chartViewModel()), &msc::ChartLayoutManager::currentChartChanged, this,
            &MSCEditorCore::selectCurrentChart);
    connect(&(m_model->chartViewModel()), &msc::ChartLayoutManager::initialNameAccepted, this,
            [this](MscEntity *entity) {
                if (!entity) {
                    return;
                }
                Q_EMIT nameChanged(entity, entity->name(), nullptr);
            });

    connect(m_model.get(), &msc::MainModel::showChartVew, this, [this]() { showDocumentView(true); });

    connect(&(m_model->hierarchyViewModel()), &msc::HierarchyViewModel::selectedDocumentChanged, this,
            &msc::MSCEditorCore::updateHierarchyActions);
    connect(this, &msc::MSCEditorCore::viewModeChanged, this, &msc::MSCEditorCore::updateHierarchyActions);
    connect(&(m_model->hierarchyViewModel()), &msc::HierarchyViewModel::hierarchyTypeChanged, this,
            &msc::MSCEditorCore::updateHierarchyActions);

    m_connectionsDone = true;
}

void MSCEditorCore::addToolBars(QMainWindow *window)
{
    window->addToolBar(mainToolBar());
    window->addToolBar(Qt::LeftToolBarArea, mscToolBar());
    window->addToolBar(Qt::LeftToolBarArea, hierarchyToolBar());
}

QToolBar *MSCEditorCore::mscToolBar()
{
    if (!m_mscToolBar) {
        m_mscToolBar = new QToolBar(tr("MSC"));
        m_mscToolBar->setObjectName("mscTools");
        m_mscToolBar->setAllowedAreas(Qt::AllToolBarAreas);
        m_mscToolBar->setVisible(m_toolbarsVisible);
    }
    return m_mscToolBar;
}

QToolBar *MSCEditorCore::hierarchyToolBar()
{
    if (!m_hierarchyToolBar) {
        m_hierarchyToolBar = new QToolBar(tr("Hierarchy"));
        m_hierarchyToolBar->setObjectName("hierarchyTools");
        m_hierarchyToolBar->setAllowedAreas(Qt::AllToolBarAreas);
        m_hierarchyToolBar->setVisible(m_toolbarsVisible);
    }
    return m_hierarchyToolBar;
}

void MSCEditorCore::showToolbars(bool show)
{
    m_toolbarsVisible = show;
    if (m_mscToolBar) {
        m_mscToolBar->setVisible(m_toolbarsVisible);
    }
    if (m_hierarchyToolBar) {
        m_hierarchyToolBar->setVisible(m_toolbarsVisible);
    }
}

void MSCEditorCore::populateCommandLineArguments(shared::CommandLineParser *parser) const
{
    parser->handlePositional(shared::CommandLineParser::Positional::OpenFileMsc);
    parser->handlePositional(shared::CommandLineParser::Positional::DbgOpenMscExamplesChain);
    parser->handlePositional(shared::CommandLineParser::Positional::DropUnsavedChangesSilently);
}

BaseTool *MSCEditorCore::activeTool() const
{
    for (auto tool : m_tools) {
        if (tool->isActive()) {
            return tool;
        }
    }

    return nullptr;
}

QAction *MSCEditorCore::actionMessageDeclarations()
{
    if (!m_actionMessageDeclarations) {
        m_actionMessageDeclarations =
                new QAction(QIcon(QLatin1String(":/icons/message_declarations.svg")), tr("Show messages ..."));
        connect(m_actionMessageDeclarations, &QAction::triggered, this,
                [this]() { openMessageDeclarationEditor(nullptr); });
    }
    return m_actionMessageDeclarations;
}

QVector<QAction *> MSCEditorCore::chartActions() const
{
    QVector<QAction *> actions;
    actions.reserve(m_tools.size());
    for (msc::BaseTool *tool : m_tools) {
        if (tool != m_deleteTool)
            actions.append(tool->action());
    }
    return actions;
}

QVector<QAction *> MSCEditorCore::hierarchyActions() const
{
    return m_hierarchyActions;
}

QAction *MSCEditorCore::createActionCopy(QMainWindow *window)
{
    if (m_actionCopy == nullptr) {
        if (window != nullptr) {
            m_actionCopy = new QAction(tr("Copy:"), this);
            m_actionCopy->setIcon(QIcon::fromTheme("edit-copy"));
            m_actionCopy->setMenu(new QMenu(window));
            m_actionCopy->menu()->addAction(
                    tr("Copy Diagram"), m_model.get(), &msc::MainModel::copyCurrentChart, QKeySequence::Copy);
            m_actionCopy->menu()->addAction(
                    tr("Copy as Picture"), m_model.get(), &msc::MainModel::copyCurrentChartAsPicture);
        } else {
            m_actionCopy = new QAction(tr("Copy Diagram"), this);
            m_actionCopy->setShortcut(QKeySequence::Copy);
            m_actionCopy->setIcon(QIcon::fromTheme("edit-copy"));
            connect(m_actionCopy, &QAction::triggered, m_model.get(), &msc::MainModel::copyCurrentChart);
        }
    }
    return m_actionCopy;
}

QAction *MSCEditorCore::createActionPaste(QMainWindow *window)
{
    if (m_actionPaste == nullptr) {
        if (window) {
            m_actionPaste = new QAction(tr("Paste:"), window);
            m_actionPaste->setShortcut(QKeySequence::Paste);
            m_actionPaste->setIcon(QIcon::fromTheme("edit-paste"));
            connect(m_actionPaste, &QAction::triggered, m_model.get(), &msc::MainModel::pasteChart);
        } else {
            m_actionPaste = new QAction(tr("Paste:"), this);
            m_actionPaste->setShortcut(QKeySequence::Paste);
            m_actionPaste->setIcon(QIcon::fromTheme("edit-paste"));
            connect(m_actionPaste, &QAction::triggered, m_model.get(), &msc::MainModel::pasteChart);
        }
    }
    return m_actionPaste;
}

/*!
   Sets the checker for iv consistency
 */
void MSCEditorCore::setSystemChecker(SystemChecks *checker)
{
    if (checker == nullptr) {
        return;
    }

    if (m_systemChecks && m_systemChecks->parent() == this) {
        delete m_systemChecks;
    }
    m_systemChecks = checker;
    m_model->chartViewModel().setSystemChecker(m_systemChecks);
}

/*!
   Returns the checker for iv consistency
 */
SystemChecks *MSCEditorCore::systemChecker() const
{
    return m_systemChecks;
}

MSCEditorCore::ViewMode MSCEditorCore::viewMode()
{
    return m_viewMode;
}

QUndoStack *MSCEditorCore::undoStack() const
{
    return m_model->undoStack();
}

msc::MscCommandsStack *MSCEditorCore::commandsStack() const
{
    return m_model->commandsStack();
}

/*!
   Changes the asn1 referenceto \p newName if the existing one if pointing to \p oldName
 */
bool MSCEditorCore::renameAsnFile(const QString &oldName, const QString &newName)
{
    QFileInfo oldFile(oldName);
    const QString oldFileName = oldFile.fileName();
    QFileInfo newFile(newName);
    const QString newFileName = newFile.fileName();

    MscModel *mscModel = m_model->mscModel();
    if (mscModel->dataDefinitionString() == oldFileName) {
        commandsStack()->push(new cmd::CmdSetAsn1File(mscModel, newFileName, "ASN.1"));

        Q_EMIT editedExternally(this);
        return true;
    }
    return false;
}

/*!
   Changes all instances that have the name \p oldName to have the new name \p name
 */
void MSCEditorCore::changeMscInstanceName(const QString &oldName, const QString &name)
{
    bool updated = false;
    for (msc::MscChart *chart : m_model->mscModel()->allCharts()) {
        for (msc::MscInstance *instance : chart->instances()) {
            if (instance->name() == oldName) {
                msc::MscCommandsStack *undo = commandsStack();
                auto cmd = new msc::cmd::CmdEntityNameChange(instance, name, nullptr);
                undo->push(cmd);
                updated = true;
            }
        }
    }
    if (updated) {
        Q_EMIT editedExternally(this);
    }
}

/*!
   Changes all messages that have the name \p oldName to have the new name \p newName, if the source and target have the
   names \p sourceName and \p targetName
 */
void MSCEditorCore::changeMscMessageName(
        const QString &oldName, const QString &newName, const QString &sourceName, const QString &targetName)
{
    bool updated = false;
    for (msc::MscChart *chart : m_model->mscModel()->allCharts()) {
        for (msc::MscMessage *message : chart->messages()) {
            if (message->name() == oldName) {
                const QString messageSource = message->sourceInstance() ? message->sourceInstance()->name() : "";
                const QString messageTarget = message->targetInstance() ? message->targetInstance()->name() : "";
                if (messageSource == sourceName && messageTarget == targetName) {
                    msc::MscCommandsStack *undo = commandsStack();
                    auto cmd = new msc::cmd::CmdEntityNameChange(message, newName, nullptr);
                    undo->push(cmd);
                    updated = true;
                }
            }
        }
    }
    if (updated) {
        Q_EMIT editedExternally(this);
    }
}

/*!
   Removes all instance that are corresponding to the function \p ivFunction
 */
void MSCEditorCore::removeMscInstances(ivm::IVFunction *ivFunction)
{
    bool updated = false;
    msc::MscCommandsStack *undo = commandsStack();
    for (msc::MscChart *chart : m_model->mscModel()->allCharts()) {
        for (msc::MscInstance *instance : chart->instances()) {
            if (m_systemChecks->correspond(ivFunction, instance)) {
                auto cmd = new msc::cmd::CmdDeleteEntity({ instance }, chart);
                undo->push(cmd);
                updated = true;
            }
        }
    }

    if (updated) {
        Q_EMIT editedExternally(this);
    }
}

/*!
   Removes all messages that are corresponding to the connection \p ivConnection
 */
void MSCEditorCore::removeMscMessages(ivm::IVConnection *ivConnection)
{
    bool updated = false;
    msc::MscCommandsStack *undo = commandsStack();
    for (msc::MscChart *chart : m_model->mscModel()->allCharts()) {
        for (msc::MscMessage *message : chart->messages()) {
            if (m_systemChecks->correspond(ivConnection, message)) {
                auto cmd = new msc::cmd::CmdDeleteEntity({ message }, chart);
                undo->push(cmd);
                updated = true;
            }
        }
    }

    if (updated) {
        Q_EMIT editedExternally(this);
    }
}

/*!
   Returns a list of all corresponding instances for iv function \p IVFunction.
 */
QList<MscInstance *> MSCEditorCore::correspondingInstances(ivm::IVFunction *ivFunction) const
{
    QList<MscInstance *> corresponds;
    for (msc::MscChart *chart : m_model->mscModel()->allCharts()) {
        for (msc::MscInstance *instance : chart->instances()) {
            if (m_systemChecks->correspond(ivFunction, instance)) {
                corresponds.append(instance);
            }
        }
    }

    return corresponds;
}

/*!
   Returns a list of all corresponding messages for iv connection \p ivConnection.
 */
QList<MscMessage *> MSCEditorCore::correspondingMessages(ivm::IVConnection *ivConnection) const
{
    QList<MscMessage *> corresponds;
    for (msc::MscChart *chart : m_model->mscModel()->allCharts()) {
        for (msc::MscMessage *message : chart->messages()) {
            if (m_systemChecks->correspond(ivConnection, message)) {
                corresponds.append(message);
            }
        }
    }
    return corresponds;
}

QString MSCEditorCore::filePath() const
{
    return m_model->currentFilePath();
}

bool MSCEditorCore::save()
{
    if (!m_model) {
        return false;
    }

    return m_model->saveMsc(m_model->currentFilePath());
}

void MSCEditorCore::setViewMode(MSCEditorCore::ViewMode mode)
{
    if (mode == m_viewMode) {
        return;
    }

    m_viewMode = mode;
    if (m_viewMode == ViewMode::CHART) {
        showDocumentView(true);
    } else {
        showHierarchyView(true);
    }

    Q_EMIT viewModeChanged(m_viewMode);
}

void MSCEditorCore::showDocumentView(bool show)
{
    if (show) {
        if (m_centerView->currentWidget() == m_chartView) {
            return;
        }

        m_centerView->setCurrentWidget(m_chartView);

        if (m_hierarchyToolBar) {
            m_hierarchyToolBar->hide();
        }
        if (m_mscToolBar && m_toolbarsVisible) {
            m_mscToolBar->show();
        }
        for (QAction *action : chartActions()) {
            action->setEnabled(true);
            checkGlobalComment();
        }
        for (QAction *action : hierarchyActions()) {
            action->setEnabled(false);
        }

        actionCopy()->setEnabled(true);
        msc::MscChart *chart = mainModel()->chartViewModel().currentChart();
        QClipboard *clipboard = QApplication::clipboard();
        const QMimeData *mimeData = clipboard->mimeData();
        const bool clipBoardHasMscChart = mimeData ? mimeData->hasFormat(MainModel::MscChartMimeType) : false;
        if (m_actionPaste) {
            m_actionPaste->setEnabled(clipBoardHasMscChart && chart && chart->instances().isEmpty());
        }
        if (m_actionCopy) {
            m_actionCopy->setEnabled(true);
        }

        m_deleteTool->setView(m_chartView);
        m_deleteTool->setCurrentChart(chart);

        setViewMode(ViewMode::CHART);
    } else {
        showHierarchyView(true);
    }
}

void MSCEditorCore::showHierarchyView(bool show)
{
    if (show) {
        if (m_centerView->currentWidget() == m_hierarchyView) {
            return;
        }

        m_centerView->setCurrentWidget(m_hierarchyView);

        if (m_hierarchyToolBar && m_toolbarsVisible) {
            m_hierarchyToolBar->show();
        }
        if (m_mscToolBar) {
            m_mscToolBar->hide();
        }
        for (QAction *action : chartActions()) {
            action->setEnabled(false);
        }
        for (QAction *action : hierarchyActions()) {
            action->setEnabled(true);
        }

        if (m_actionCopy) {
            m_actionCopy->setEnabled(false);
        }
        if (m_actionPaste) {
            m_actionPaste->setEnabled(false);
        }

        m_deleteTool->setView(m_hierarchyView);
        m_deleteTool->setCurrentChart(nullptr);

        setViewMode(ViewMode::HIERARCHY);
    } else {
        showDocumentView(true);
    }
}

void MSCEditorCore::activateDefaultTool()
{
    for (msc::BaseTool *tool : m_tools) {
        if (tool != m_pointerTool)
            tool->action()->setChecked(false);
    }
    m_pointerTool->action()->setChecked(true);
}

/*!
 * \brief msc::MSCEditorCore::selectCurrentChart Set the current chart as the currently selected.
 */
void MSCEditorCore::selectCurrentChart()
{
    msc::MscChart *chart = m_model->chartViewModel().currentChart();
    if (chart != nullptr) {
        connect(chart, &msc::MscEntity::commentChanged, this, &msc::MSCEditorCore::checkGlobalComment,
                Qt::UniqueConnection);
    }
    checkGlobalComment();
}

void MSCEditorCore::checkGlobalComment()
{
    if (!m_globalCommentCreateTool) {
        return;
    }

    msc::MscChart *currentChart = m_model->chartViewModel().currentChart();
    if (!currentChart) {
        m_globalCommentCreateTool->action()->setEnabled(false);
        return;
    }

    const bool hasInstance = currentChart && !currentChart->instances().isEmpty();
    const bool hasGlobalComment = !currentChart->commentString().isEmpty();
    m_globalCommentCreateTool->action()->setEnabled(hasInstance && !hasGlobalComment);
}

void MSCEditorCore::openMessageDeclarationEditor(QWidget *parentwidget)
{
    msc::MscModel *model = mainModel()->mscModel();
    if (!model) {
        return;
    }

    QVector<msc::MscDocument *> docs = model->documents();
    if (docs.isEmpty()) {
        return;
    }

    MessageDeclarationsDialog dialog(
            docs.at(0)->messageDeclarations(), model, commandsStack(), m_systemChecks, parentwidget);
    dialog.setFileName(model->dataDefinitionString());
    dialog.setIVConnectionNames(m_systemChecks->connectionNames());
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        commandsStack()->beginMacro("Edit message declarations");
        commandsStack()->push(new cmd::CmdSetMessageDeclarations(docs.at(0), dialog.declarations()));
        commandsStack()->push(new cmd::CmdSetAsn1File(model, dialog.fileName(), "ASN.1"));
        commandsStack()->endMacro();
    }
}

void MSCEditorCore::updateMscToolbarActionsChecked()
{
    if (!m_mscToolBar) {
        return;
    }

    if (QAction *senderAction = qobject_cast<QAction *>(sender()))
        if (senderAction->isChecked()) {
            for (QAction *action : m_mscToolBar->actions())
                if (action != senderAction) {
                    action->setChecked(false);
                }
        }
}

/*!
   Enables or disables the hierarchy actions, depending if a new document can be added or not
 */
void MSCEditorCore::updateHierarchyActions()
{
    bool canAdd = true;
    if (m_viewMode != ViewMode::HIERARCHY) {
        canAdd = false;
    }
    MscDocument *parentDoc = m_model->selectedDocument();
    if (!parentDoc || !parentDoc->isAddChildEnable()) {
        canAdd = false;
    }

    for (QAction *action : m_hierarchyActions) {
        action->setEnabled(canAdd);
    }
}

/*!
   Adds a MSC document if the hierarchy view is active and a "non-leaf" document is selected
 */
void MSCEditorCore::addDocument(MscDocument::HierarchyType type)
{
    if (m_viewMode != ViewMode::HIERARCHY) {
        return;
    }
    MscDocument *parentDoc = m_model->hierarchyViewModel().selectedDocument();
    if (!parentDoc) {
        return;
    }
    if (!parentDoc->isAddChildEnable()) {
        return;
    }

    MscDocument *document = new MscDocument(QObject::tr("Document_%1").arg(parentDoc->documents().size()));
    document->setHierarchyType(type);

    commandsStack()->push(new cmd::CmdDocumentCreate(document, parentDoc));

    m_model->setSelectedDocument(document);
}

QUrl MSCEditorCore::helpPage() const
{
    return QUrl("https://taste.tuxfamily.org/wiki/index.php?title=MSC_tutorial");
}
}
