#include "mscplugin.h"

#include "commandlineparser.h"
#include "commands/common/commandsstack.h"
#include "graphicsview.h"
#include "hierarchyviewmodel.h"
#include "mainmodel.h"
#include "mainwindow.h"
#include "mscdocument.h"
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
#include <QMainWindow>
#include <QMenu>
#include <QMimeData>
#include <QStackedWidget>
#include <QToolBar>
#include <QUndoGroup>
#include <QUndoStack>

namespace msc {

static const char *HIERARCHY_TYPE_TAG = "hierarchyTag";

/*!
 * \class MSCPlugin
 * \brief Has most of the code for the MSC Editor UI and the underlying library
 */

MSCPlugin::MSCPlugin(QObject *parent)
    : shared::Plugin(parent)
    , m_model(new msc::MainModel())
    , m_mscToolBar(new QToolBar(tr("MSC")))
    , m_hierarchyToolBar(new QToolBar(tr("Hierarchy")))
{
    m_mscToolBar->setObjectName("mscTools");
    m_mscToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    m_mscToolBar->setVisible(m_toolbarsVisible);
    m_hierarchyToolBar->setObjectName("hierarchyTools");
    m_hierarchyToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    m_hierarchyToolBar->setVisible(m_toolbarsVisible);
}

MSCPlugin::~MSCPlugin() { }

MainModel *MSCPlugin::mainModel() const
{
    return m_model.get();
}

void MSCPlugin::setPluginActive(bool active)
{
    m_actionScreenshot->setVisible(active);
    m_editSeparator->setVisible(active);
    m_actionCopy->setVisible(active);
    m_actionPaste->setVisible(active);
}

void MSCPlugin::setViews(
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

QStackedWidget *MSCPlugin::centerView()
{
    return m_centerView;
}

shared::ui::GraphicsViewBase *MSCPlugin::chartView()
{
    return m_chartView;
}

GraphicsView *MSCPlugin::hierarchyView()
{
    return m_hierarchyView;
}

void MSCPlugin::initChartTools()
{
    Q_ASSERT(m_chartView != nullptr);

    m_pointerTool = new msc::PointerTool(nullptr, this);
    m_tools.append(m_pointerTool);

    m_instanceCreatorTool = new msc::InstanceCreatorTool(&(m_model->chartViewModel()), nullptr, this);
    m_tools.append(m_instanceCreatorTool);

    m_instanceStopTool = new msc::InstanceStopTool(nullptr, this);
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
            connect(creatorTool, &msc::BaseCreatorTool::created, this, &MSCPlugin::activateDefaultTool);
        }

        toolsActions->addAction(toolAction);
        connect(toolAction, &QAction::toggled, this, &MSCPlugin::updateMscToolbarActionsChecked);
    }

    m_defaultToolAction = toolsActions->actions().first();
    m_defaultToolAction->setVisible(false);

    m_deleteTool = new msc::EntityDeleteTool(&(m_model->chartViewModel()), m_chartView, this);
    m_deleteTool->setCurrentChart(m_model->chartViewModel().currentChart());
    connect(&m_model->chartViewModel(), &msc::ChartLayoutManager::currentChartChanged, m_deleteTool,
            &msc::EntityDeleteTool::setCurrentChart);

    activateDefaultTool();
}

void MSCPlugin::initHierarchyViewActions()
{
    Q_ASSERT(m_hierarchyView != nullptr);

    auto addAction = [&](msc::MscDocument::HierarchyType type, const QString &title, const QPixmap &icon) {
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
            msc::MscDocument::HierarchyType type =
                    sender()->property(HIERARCHY_TYPE_TAG).value<msc::MscDocument::HierarchyType>();
            addDocument(type);
        });
    };

    addAction(msc::MscDocument::HierarchyAnd, tr("Hierarchy And"), QPixmap(":/icons/document_and.png"));
    addAction(msc::MscDocument::HierarchyOr, tr("Hierarchy Or"), QPixmap(":/icons/document_or.png"));
    addAction(msc::MscDocument::HierarchyParallel, tr("Hierarchy Parallel"), QPixmap(":/icons/document_parallel.png"));
    addAction(msc::MscDocument::HierarchyIs, tr("Hierarchy Is"), QPixmap(":/icons/document_is_scenario.png"));
    addAction(msc::MscDocument::HierarchyRepeat, tr("Hierarchy Repeat"), QPixmap(":/icons/document_repeat.png"));
    addAction(
            msc::MscDocument::HierarchyException, tr("Hierarchy Exception"), QPixmap(":/icons/document_exception.png"));
    addAction(msc::MscDocument::HierarchyLeaf, tr("Hierarchy Leaf"), QPixmap(":/icons/document_leaf.png"));
}

void MSCPlugin::initConnections()
{
    Q_ASSERT(m_chartView != nullptr);

    if (auto chartview = qobject_cast<GraphicsView *>(m_chartView)) {
        connect(chartview, &msc::GraphicsView::createMessageToolRequested, this, [&]() {
            if (m_messageCreateTool) {
                m_messageCreateTool->activate();
            }
        });
    }

    connect(&(m_model->chartViewModel()), &msc::ChartLayoutManager::currentChartChanged, this,
            &MSCPlugin::selectCurrentChart);

    connect(m_model.get(), &msc::MainModel::showChartVew, this, [this]() { showDocumentView(true); });

    connect(&(m_model->hierarchyViewModel()), &msc::HierarchyViewModel::selectedDocumentChanged, this,
            &msc::MSCPlugin::updateHierarchyActions);
    connect(this, &msc::MSCPlugin::viewModeChanged, this, &msc::MSCPlugin::updateHierarchyActions);
    connect(&(m_model->hierarchyViewModel()), &msc::HierarchyViewModel::hierarchyTypeChanged, this,
            &msc::MSCPlugin::updateHierarchyActions);
}

void MSCPlugin::addToolBars(QMainWindow *window)
{
    window->addToolBar(mainToolBar());
    window->addToolBar(Qt::LeftToolBarArea, m_mscToolBar);
    window->addToolBar(Qt::LeftToolBarArea, m_hierarchyToolBar);
}

/*!
 * Fills the File menu with actions.
 */
void MSCPlugin::addMenuFileActions(QMenu *menu, QMainWindow *window)
{
    auto mainWindow = dynamic_cast<MainWindow *>(window);
    m_actionScreenshot = menu->addAction(QIcon(QLatin1String(":/sharedresources/icons/save.svg")),
            tr("Save Screenshot..."), mainWindow, &MainWindow::saveScreenshot, QKeySequence(Qt::ALT + Qt::Key_S));
    menu->addSeparator();
}

void MSCPlugin::addMenuEditActions(QMenu *menu, QMainWindow *window)
{
    auto mainWindow = dynamic_cast<MainWindow *>(window);

    menu->addSeparator();
    menu->addAction(m_deleteTool->action());
    m_editSeparator = menu->addSeparator();
    menu->addAction(createActionCopy(mainWindow));
    menu->addAction(createActionPaste(mainWindow));
}

void MSCPlugin::addMenuViewActions(QMenu *menu, QMainWindow *window)
{
    auto mainWindow = dynamic_cast<MainWindow *>(window);

    m_actionShowDocument = menu->addAction(tr("Show &Document"), mainWindow, &MainWindow::showDocumentView, tr("F8"));
    m_actionShowHierarchy =
            menu->addAction(tr("Show &Hierarchy"), mainWindow, &MainWindow::showHierarchyView, tr("F9"));

    m_actionShowDocument->setCheckable(true);
    m_actionShowHierarchy->setCheckable(true);
    auto group = new QActionGroup(menu);
    group->addAction(m_actionShowDocument);
    group->addAction(m_actionShowHierarchy);
    m_actionShowDocument->setChecked(true);

    menu->addSeparator();
    menu->addAction(tr("Show messages ..."), mainWindow, &MainWindow::openMessageDeclarationEditor);

    menu->addSeparator();
    auto menuWindows = menu->addMenu("Windows");
    menuWindows->addAction(mainWindow->dockWidgetDocumentToggleAction());
    menuWindows->addAction(mainWindow->mscTextViewToggleAction());
    menuWindows->addAction(mainWindow->dockWidgetAsn1ToggleAction());
}

void MSCPlugin::addMenuHelpActions(QMenu * /*menu*/, QMainWindow * /*window*/)
{
    // Do nothing
}

void MSCPlugin::showToolbars(bool show)
{
    m_toolbarsVisible = show;
    m_mscToolBar->setVisible(m_toolbarsVisible);
    m_hierarchyToolBar->setVisible(m_toolbarsVisible);
}

void MSCPlugin::populateCommandLineArguments(shared::CommandLineParser *parser) const
{
    parser->handlePositional(shared::CommandLineParser::Positional::OpenFileMsc);
    parser->handlePositional(shared::CommandLineParser::Positional::DbgOpenMscExamplesChain);
    parser->handlePositional(shared::CommandLineParser::Positional::DropUnsavedChangesSilently);
}

BaseTool *MSCPlugin::activeTool() const
{
    for (auto tool : m_tools) {
        if (tool->isActive()) {
            return tool;
        }
    }

    return nullptr;
}

QVector<QAction *> MSCPlugin::chartActions() const
{
    QVector<QAction *> actions;
    actions.reserve(m_tools.size());
    for (msc::BaseTool *tool : m_tools) {
        if (tool != m_deleteTool)
            actions.append(tool->action());
    }
    return actions;
}

QVector<QAction *> MSCPlugin::hierarchyActions() const
{
    return m_hierarchyActions;
}

QAction *MSCPlugin::createActionCopy(MainWindow *window)
{
    if (m_actionCopy == nullptr) {
        if (window != nullptr) {
            m_actionCopy = new QAction(tr("Copy:"), this);
            m_actionCopy->setIcon(QIcon::fromTheme("edit-copy"));
            m_actionCopy->setMenu(new QMenu(window));
            m_actionCopy->menu()->addAction(
                    tr("Copy Diagram"), window, &MainWindow::copyCurrentChart, QKeySequence::Copy);
            m_actionCopy->menu()->addAction(tr("Copy as Picture"), window, &MainWindow::copyCurrentChartAsPicture);
        } else {
            m_actionCopy = new QAction(tr("Copy Diagram"), this);
            m_actionCopy->setShortcut(QKeySequence::Copy);
            m_actionCopy->setIcon(QIcon::fromTheme("edit-copy"));
            connect(m_actionCopy, &QAction::triggered, m_model.get(), &msc::MainModel::copyCurrentChart);
        }
    }
    return m_actionCopy;
}

QAction *MSCPlugin::createActionPaste(MainWindow *window)
{
    if (m_actionPaste == nullptr) {
        if (window) {
            m_actionPaste = new QAction(tr("Paste:"), window);
            m_actionPaste->setShortcut(QKeySequence::Paste);
            m_actionPaste->setIcon(QIcon::fromTheme("edit-paste"));
            connect(m_actionPaste, &QAction::triggered, window, &MainWindow::pasteChart);
        } else {
            m_actionPaste = new QAction(tr("Paste:"), this);
            m_actionPaste->setShortcut(QKeySequence::Paste);
            m_actionPaste->setIcon(QIcon::fromTheme("edit-paste"));
            connect(m_actionPaste, &QAction::triggered, m_model.get(), &msc::MainModel::pasteChart);
        }
    }
    return m_actionPaste;
}

MSCPlugin::ViewMode MSCPlugin::viewMode()
{
    return m_viewMode;
}

void MSCPlugin::setViewMode(MSCPlugin::ViewMode mode)
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

void MSCPlugin::showDocumentView(bool show)
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

void MSCPlugin::showHierarchyView(bool show)
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

void MSCPlugin::activateDefaultTool()
{
    for (msc::BaseTool *tool : m_tools) {
        if (tool != m_pointerTool)
            tool->action()->setChecked(false);
    }
    m_pointerTool->action()->setChecked(true);
}

/*!
 * \brief msc::MSCPlugin::selectCurrentChart Set the current chart as the currently selected.
 */
void MSCPlugin::selectCurrentChart()
{
    msc::MscChart *chart = m_model->chartViewModel().currentChart();

    if (chart != nullptr) {
        if (QUndoStack *currentStack = m_model->undoStack()) {
            if (!undoGroup()->stacks().contains(currentStack)) {
                undoGroup()->addStack(currentStack);
            }
            undoGroup()->setActiveStack(currentStack);
        } else {
            undoGroup()->removeStack(undoGroup()->activeStack());
        }
        msc::cmd::CommandsStack::setCurrent(undoGroup()->activeStack());

        connect(chart, &msc::MscEntity::commentChanged, this, &msc::MSCPlugin::checkGlobalComment,
                Qt::UniqueConnection);
    }

    checkGlobalComment();
}

void MSCPlugin::checkGlobalComment()
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

void MSCPlugin::updateMscToolbarActionsChecked()
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
void MSCPlugin::updateHierarchyActions()
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
   Adds a MSC document if the hierarhy view is active and a "non-leafe" document is selected
 */
void MSCPlugin::addDocument(MscDocument::HierarchyType type)
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

    const QVariantList &cmdParams = { QVariant::fromValue<msc::MscDocument *>(document),
        QVariant::fromValue<msc::MscDocument *>(parentDoc) };
    msc::cmd::CommandsStack::push(msc::cmd::Id::CreateDocument, cmdParams);

    m_model->setSelectedDocument(document);
}

}
