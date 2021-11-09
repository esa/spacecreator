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

#include "mscappwidget.h"

#include "commands/cmdsetasn1file.h"
#include "documentitemmodel.h"
#include "hierarchyview/hierarchyviewmodel.h"
#include "mainmodel.h"
#include "msccommandsstack.h"
#include "msceditorcore.h"
#include "mscinstance.h"
#include "mscmessage.h"
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
#include "ui_mscappwidget.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QFileInfo>
#include <QGraphicsView>
#include <QMessageBox>
#include <QMimeData>
#include <QUndoStack>

namespace msc {

static const char *HIERARCHY_TYPE_TAG = "hierarchyTag";

MscAppWidget::MscAppWidget(MSCEditorCore *mscCore, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MSCAppWidget)
    , m_mscCore(mscCore)
{
    ui->setupUi(this);

    ui->hierarchyView->setRenderHints(
            QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    ui->documentToolBar->setVisible(false);

    ui->asn1Switch->setText(mainModel()->mscModel()->dataDefinitionString());

    ui->horizontalSplitter->setStretchFactor(0, 0);
    ui->horizontalSplitter->setStretchFactor(1, 1);

    m_actionCopy = m_mscCore->createActionCopy(nullptr);
    m_actionPaste = m_mscCore->createActionPaste(nullptr);

    ui->documentTree->setModel(mainModel()->documentItemModel());
    ui->chartView->setScene(mainModel()->graphicsScene());
    ui->hierarchyView->setScene(mainModel()->hierarchyScene());

    MscDocument *doc = mainModel()->selectedDocument();
    if (!doc) {
        MscChart *chart = m_mscCore->mainModel()->chartViewModel().currentChart();
        if (chart) {
            doc = chart->parentDocument();
        }
    }

    initChartTools();
    initHierarchyViewActions();
    initConnections();

    ui->documentTree->expandAll();
    ui->documentTree->setSelectedDocument(doc);

    showDocumentView(true);
}

MscAppWidget::~MscAppWidget()
{
    if (ui->documentTree->model()) {
        disconnect(ui->documentTree->model(), nullptr, this, nullptr);
    }
    if (mainModel()) {
        disconnect(&(mainModel()->chartViewModel()), nullptr, this, nullptr);
        disconnect(mainModel(), nullptr, this, nullptr);
        disconnect(mainModel()->undoStack(), nullptr, this, nullptr);
    }
    delete ui;
}

QStackedWidget *MscAppWidget::centerView()
{
    return ui->centerView;
}

msc::GraphicsView *MscAppWidget::chartView()
{
    return ui->chartView;
}

msc::GraphicsView *MscAppWidget::hierarchyView()
{
    return ui->hierarchyView;
}

MscAppWidget::ViewMode MscAppWidget::viewMode()
{
    return m_viewMode;
}

void MscAppWidget::initChartTools()
{
    if (!m_tools.isEmpty()) {
        qDeleteAll(m_tools);
        m_tools.clear();
    }

    m_pointerTool = new msc::PointerTool(nullptr, this);
    m_tools.append(m_pointerTool);

    m_instanceCreatorTool = new msc::InstanceCreatorTool(&(mainModel()->chartViewModel()), nullptr, this);
    m_tools.append(m_instanceCreatorTool);

    m_instanceStopTool = new msc::InstanceStopTool(&(mainModel()->chartViewModel()), nullptr, this);
    m_tools.append(m_instanceStopTool);

    m_messageCreateTool = new msc::MessageCreatorTool(
            msc::MscMessage::MessageType::Message, &(mainModel()->chartViewModel()), nullptr, this);
    m_tools.append(m_messageCreateTool);

    m_createCreateTool = new msc::MessageCreatorTool(
            msc::MscMessage::MessageType::Create, &(mainModel()->chartViewModel()), nullptr, this);
    m_tools.append(m_createCreateTool);

    m_commentCreateTool = new msc::CommentCreatorTool(false, &(mainModel()->chartViewModel()), nullptr, this);
    m_tools.append(m_commentCreateTool);

    m_globalCommentCreateTool = new msc::CommentCreatorTool(true, &(mainModel()->chartViewModel()), nullptr, this);
    m_tools.append(m_globalCommentCreateTool);

    m_coregionCreateTool = new msc::CoregionCreatorTool(&(mainModel()->chartViewModel()), nullptr, this);
    m_tools.append(m_coregionCreateTool);

    m_actionCreateTool = new msc::ActionCreatorTool(&(mainModel()->chartViewModel()), nullptr, this);
    m_tools.append(m_actionCreateTool);

    m_conditionCreateTool = new msc::ConditionCreatorTool(false, &(mainModel()->chartViewModel()), nullptr, this);
    m_tools.append(m_conditionCreateTool);

    m_sharedConditionTool = new msc::ConditionCreatorTool(true, &(mainModel()->chartViewModel()), nullptr, this);
    m_tools.append(m_sharedConditionTool);

    m_startTimerCreateTool =
            new msc::TimerCreatorTool(msc::MscTimer::TimerType::Start, &(mainModel()->chartViewModel()), nullptr, this);
    m_tools.append(m_startTimerCreateTool);

    m_stopTimerCreateTool =
            new msc::TimerCreatorTool(msc::MscTimer::TimerType::Stop, &(mainModel()->chartViewModel()), nullptr, this);
    m_tools.append(m_stopTimerCreateTool);

    m_timeoutTimerCreateTool = new msc::TimerCreatorTool(
            msc::MscTimer::TimerType::Timeout, &(mainModel()->chartViewModel()), nullptr, this);
    m_tools.append(m_timeoutTimerCreateTool);

    QActionGroup *toolsActions = new QActionGroup(this);
    toolsActions->setExclusive(false);
    for (msc::BaseTool *tool : m_tools) {
        QAction *toolAction = new QAction(tool->title());
        ui->chartToolBar->addAction(toolAction);
        toolAction->setCheckable(true);
        toolAction->setIcon(tool->icon());
        toolAction->setToolTip(tr("%1: %2").arg(tool->title(), tool->description()));
        toolAction->setData(QVariant::fromValue<msc::BaseTool::ToolType>(tool->toolType()));
        toolAction->setEnabled(m_viewMode == ViewMode::CHART);
        tool->setView(chartView());
        tool->setAction(toolAction);

        if (msc::BaseCreatorTool *creatorTool = qobject_cast<msc::BaseCreatorTool *>(tool)) {
            connect(creatorTool, &msc::BaseCreatorTool::created, this, [&]() {
                chartView()->setFocus();
                activateDefaultTool();
            });
            connect(creatorTool, &msc::BaseCreatorTool::canceled, this, &MscAppWidget::activateDefaultTool);
        }

        toolsActions->addAction(toolAction);
        connect(toolAction, &QAction::toggled, this, &MscAppWidget::updateMscToolbarActionsChecked);
    }

    m_defaultToolAction = toolsActions->actions().first();
    m_defaultToolAction->setVisible(false);

    m_deleteTool = new msc::EntityDeleteTool(&(mainModel()->chartViewModel()), chartView(), this);
    m_deleteTool->setCurrentChart(mainModel()->chartViewModel().currentChart());
    connect(&mainModel()->chartViewModel(), &msc::ChartLayoutManager::currentChartChanged, m_deleteTool,
            &msc::EntityDeleteTool::setCurrentChart);

    activateDefaultTool();

    ui->chartToolBar->addSeparator();
    ui->chartToolBar->addAction(deleteTool()->action());
    ui->chartToolBar->addAction(m_mscCore->actionCopy());
    ui->chartToolBar->addAction(m_mscCore->actionPaste());
    ui->chartToolBar->addSeparator();
    for (QAction *action : initViewActions()) {
        ui->chartToolBar->addAction(action);
    }
}

void MscAppWidget::initHierarchyViewActions()
{
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
        ui->documentToolBar->addAction(action);

        m_hierarchyActions.append(action);

        connect(action, &QAction::triggered, this, [&]() {
            msc::MscDocument::HierarchyType selectedType =
                    sender()->property(HIERARCHY_TYPE_TAG).value<msc::MscDocument::HierarchyType>();
            m_mscCore->addDocument(selectedType);
        });
    };

    addAction(msc::MscDocument::HierarchyAnd, tr("Hierarchy And"), QIcon(":/icons/document_and.png"));
    addAction(msc::MscDocument::HierarchyOr, tr("Hierarchy Or"), QIcon(":/icons/document_or.png"));
    addAction(msc::MscDocument::HierarchyParallel, tr("Hierarchy Parallel"), QIcon(":/icons/document_parallel.png"));
    addAction(msc::MscDocument::HierarchyIs, tr("Hierarchy Is"), QIcon(":/icons/document_is_scenario.png"));
    addAction(msc::MscDocument::HierarchyRepeat, tr("Hierarchy Repeat"), QIcon(":/icons/document_repeat.png"));
    addAction(msc::MscDocument::HierarchyException, tr("Hierarchy Exception"), QIcon(":/icons/document_exception.png"));
    addAction(msc::MscDocument::HierarchyLeaf, tr("Hierarchy Leaf"), QIcon(":/icons/document_leaf.png"));

    ui->documentToolBar->addSeparator();
    ui->documentToolBar->addAction(deleteTool()->action());
    ui->documentToolBar->addSeparator();
    for (QAction *action : initViewActions()) {
        ui->documentToolBar->addAction(action);
    }
}

void MscAppWidget::initConnections()
{
    connect(chartView(), &msc::GraphicsView::createMessageToolRequested, this, [&]() {
        if (m_messageCreateTool) {
            m_messageCreateTool->activate();
        }
    });

    connect(&(mainModel()->chartViewModel()), &msc::ChartLayoutManager::currentChartChanged, m_mscCore,
            &MSCEditorCore::selectCurrentChart);

    connect(mainModel(), &msc::MainModel::showChartVew, this, [this]() { showDocumentView(true); });

    connect(&(mainModel()->hierarchyViewModel()), &msc::HierarchyViewModel::selectedDocumentChanged, this,
            &msc::MscAppWidget::updateHierarchyActions);
    connect(this, &msc::MscAppWidget::viewModeChanged, this, &msc::MscAppWidget::updateHierarchyActions);
    connect(&(mainModel()->hierarchyViewModel()), &msc::HierarchyViewModel::hierarchyTypeChanged, this,
            &msc::MscAppWidget::updateHierarchyActions);

    connect(ui->documentTree->selectionModel(), &QItemSelectionModel::currentChanged, this,
            &MscAppWidget::showSelection);
    connect(ui->documentTree, &QTreeView::doubleClicked, this, &MscAppWidget::showChart);

    connect(mainModel(), &msc::MainModel::selectedDocumentChanged, ui->documentTree,
            &msc::DocumentTreeView::setSelectedDocument);

    connect(mainModel()->documentItemModel(), &msc::DocumentItemModel::dataChanged, this, &MscAppWidget::showSelection);

    connect(mainModel(), &msc::MainModel::asn1FileNameChanged, ui->asn1Switch, &QPushButton::setText);

    // ASN1 view
    connect(mainModel(), &MainModel::currentFilePathChanged, this, [&](const QString &filename) {
        QFileInfo fileInfo(filename);
        if (ui->asn1View->isVisible()) {
            ui->asn1View->setDirectory(fileInfo.absolutePath());
        }
    });
    connect(mainModel(), &msc::MainModel::asn1FileNameChanged, this, [&](const QString &asnFilename) {
        if (ui->asn1View->isVisible()) {
            ui->asn1View->setFileName(asnFilename);
        }
    });
    connect(ui->asn1View, &asn1::ASN1FileView::asn1Selected, this, [this]() {
        msc::MscModel *model = mainModel()->mscModel();
        if (model->dataDefinitionString() != ui->asn1View->fileName()) {
            m_mscCore->commandsStack()->push(new cmd::CmdSetAsn1File(model, ui->asn1View->fileName(), "ASN.1"));
        }
    });
    connect(ui->asn1Switch, &QPushButton::clicked, this,
            [&]() { Q_EMIT showAsn1File(mainModel()->asn1File().absoluteFilePath()); });
    connect(ui->asn1Select, &QPushButton::clicked, this, [&]() { Q_EMIT selectAsn1(); });
    connect(ui->ivSwitch, &QPushButton::clicked, this, [&]() { Q_EMIT showInterfaceView(); });
}

QVector<QAction *> MscAppWidget::chartActions() const
{
    QVector<QAction *> actions;
    actions.reserve(m_tools.size());
    for (msc::BaseTool *tool : m_tools) {
        if (tool != m_deleteTool)
            actions.append(tool->action());
    }
    return actions;
}

QVector<QAction *> MscAppWidget::hierarchyActions() const
{
    return m_hierarchyActions;
}

PointerTool *MscAppWidget::pointerTool() const
{
    return m_pointerTool;
}

EntityDeleteTool *MscAppWidget::deleteTool()
{
    return m_deleteTool;
}

BaseTool *MscAppWidget::activeTool() const
{
    for (auto tool : m_tools) {
        if (tool->isActive()) {
            return tool;
        }
    }

    return nullptr;
}

void MscAppWidget::showAsn1View(bool show)
{
    ui->asn1View->setVisible(show);
    ui->asn1Widget->setVisible(!show);
    ui->ivSwitch->setVisible(!show);
}

void MscAppWidget::setViewMode(ViewMode mode)
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

void MscAppWidget::showDocumentView(bool show)
{
    if (show) {
        if (ui->centerView->currentWidget() == ui->chartView) {
            return;
        }

        ui->centerView->setCurrentWidget(ui->chartView);

        if (ui->documentToolBar) {
            ui->documentToolBar->hide();
        }
        if (ui->chartToolBar) {
            ui->chartToolBar->show();
        }
        for (QAction *action : chartActions()) {
            action->setEnabled(true);
            checkGlobalComment();
        }
        for (QAction *action : hierarchyActions()) {
            action->setEnabled(false);
        }

        m_actionCopy->setEnabled(true);
        msc::MscChart *chart = m_mscCore->mainModel()->chartViewModel().currentChart();
        QClipboard *clipboard = QApplication::clipboard();
        const QMimeData *mimeData = clipboard->mimeData();
        const bool clipBoardHasMscChart = mimeData ? mimeData->hasFormat(MainModel::MscChartMimeType) : false;
        if (m_actionPaste) {
            m_actionPaste->setEnabled(clipBoardHasMscChart && chart && chart->instances().isEmpty());
        }
        if (m_actionCopy) {
            m_actionCopy->setEnabled(true);
        }

        m_deleteTool->setView(ui->chartView);
        m_deleteTool->setCurrentChart(chart);

        setViewMode(ViewMode::CHART);
    } else {
        showHierarchyView(true);
    }
}

void MscAppWidget::showHierarchyView(bool show)
{
    if (show) {
        if (ui->centerView->currentWidget() == ui->hierarchyView) {
            return;
        }

        ui->centerView->setCurrentWidget(ui->hierarchyView);

        if (ui->documentToolBar) {
            ui->documentToolBar->show();
        }
        if (ui->chartToolBar) {
            ui->chartToolBar->hide();
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

        m_deleteTool->setView(ui->hierarchyView);
        m_deleteTool->setCurrentChart(nullptr);

        setViewMode(ViewMode::HIERARCHY);
    } else {
        showDocumentView(true);
    }
}

void MscAppWidget::activateDefaultTool()
{
    for (msc::BaseTool *tool : m_tools) {
        if (tool != m_pointerTool)
            tool->action()->setChecked(false);
    }
    m_pointerTool->action()->setChecked(true);
}

void MscAppWidget::checkGlobalComment()
{
    if (!m_globalCommentCreateTool) {
        return;
    }

    msc::MscChart *currentChart = m_mscCore->mainModel()->chartViewModel().currentChart();
    if (!currentChart) {
        m_globalCommentCreateTool->action()->setEnabled(false);
        return;
    }

    const bool hasInstance = currentChart && !currentChart->instances().isEmpty();
    const bool hasGlobalComment = !currentChart->commentString().isEmpty();
    m_globalCommentCreateTool->action()->setEnabled(hasInstance && !hasGlobalComment);
}

void MscAppWidget::checkInstances()
{
    // Check for names
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> resultNames;
    resultNames += m_mscCore->systemChecker()->checkInstanceNames();

    // Check for nested functions usage
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> resultRelations;
    resultRelations += m_mscCore->systemChecker()->checkInstanceRelations();

    QString text;
    if (!resultNames.isEmpty()) {
        text += tr("Following instances have no corresponding iv function:\n");
        for (auto item : resultNames) {
            if (!text.isEmpty()) {
                text += "\n";
            }
            text += QString("%1 from chart %2").arg(item.second->name(), item.first->name());
        }
    }
    if (!resultRelations.isEmpty()) {
        if (!text.isEmpty()) {
            text += "\n\n";
        }
        text += tr("Following instances are used with parent/child of nested functions:\n");
        for (auto item : resultRelations) {
            if (!text.isEmpty()) {
                text += "\n";
            }
            text += QString("%1 from chart %2").arg(item.second->name(), item.first->name());
        }
    }

    if (resultNames.isEmpty() && resultRelations.isEmpty()) {
        QMessageBox::information(nullptr, tr("All instances are ok"), tr("All instances are ok"));
    } else {
        QMessageBox::information(nullptr, tr("Non conforming instances"), text);
    }
}

void MscAppWidget::checkMessages()
{
    // check messages
    QVector<QPair<msc::MscChart *, msc::MscMessage *>> resultNames;
    resultNames += m_mscCore->systemChecker()->checkMessages();

    QString text;
    if (!resultNames.isEmpty()) {
        text += tr("Following messages have no corresponding iv connection:\n");
        for (auto item : resultNames) {
            if (!text.isEmpty()) {
                text += "\n";
            }
            text += QString("%1 from chart %2").arg(item.second->name(), item.first->name());
        }
    }
    if (resultNames.isEmpty()) {
        QMessageBox::information(nullptr, tr("All messages are ok"), tr("All messages are ok"));
    } else {
        QMessageBox::information(nullptr, tr("Non conforming messages"), text);
    }
}

void MscAppWidget::updateMscToolbarActionsChecked()
{
    if (QAction *senderAction = qobject_cast<QAction *>(sender())) {
        if (senderAction->isChecked()) {
            for (msc::BaseTool *tool : m_tools) {
                QAction *action = tool->action();
                if (action != senderAction) {
                    action->setChecked(false);
                }
            }
        }
    }
}

/*!
   Enables or disables the hierarchy actions, depending if a new document can be added or not
 */
void MscAppWidget::updateHierarchyActions()
{
    bool canAdd = true;
    if (m_viewMode != ViewMode::HIERARCHY) {
        canAdd = false;
    }
    MscDocument *parentDoc = mainModel()->selectedDocument();
    if (!parentDoc || !parentDoc->isAddChildEnable()) {
        canAdd = false;
    }

    for (QAction *action : m_hierarchyActions) {
        action->setEnabled(canAdd);
    }
}

void MscAppWidget::showChart(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    auto *obj = static_cast<QObject *>(index.internalPointer());
    if (obj == nullptr) {
        return;
    }

    if (auto document = dynamic_cast<msc::MscDocument *>(obj)) {
        if (!document->charts().empty()) {
            mainModel()->chartViewModel().setCurrentChart(document->charts()[0]);
            showDocumentView(true);
        }
    }
}

void MscAppWidget::showSelection(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    if (!current.isValid()) {
        return;
    }

    auto *obj = static_cast<QObject *>(current.internalPointer());
    if (obj == nullptr) {
        mainModel()->setSelectedDocument(nullptr);
        return;
    }

    if (chartView()) {
        static constexpr qreal padding = 110.;
        QSizeF preferredSize = chartView()->size() - QSizeF(padding, padding);
        mainModel()->chartViewModel().setPreferredChartBoxSize(preferredSize);
    }

    auto chart = dynamic_cast<msc::MscChart *>(obj);
    if (!chart) {
        if (auto document = dynamic_cast<msc::MscDocument *>(obj)) {
            if (document && document->hierarchyType() == msc::MscDocument::HierarchyLeaf) {
                if (!document->charts().isEmpty())
                    chart = document->charts().at(0);
            }
        }
    }
    if (chart) {
        mainModel()->chartViewModel().setCurrentChart(chart);
        showDocumentView(true);
    } else {
        showHierarchyView(true);

        if (auto document = dynamic_cast<msc::MscDocument *>(obj)) {
            mainModel()->setSelectedDocument(document);
        }
    }
}

MainModel *MscAppWidget::mainModel() const
{
    return m_mscCore->mainModel();
}

QVector<QAction *> MscAppWidget::initViewActions()
{
    if (!m_viewActions.isEmpty() || !m_mscCore) {
        return m_viewActions;
    }

    m_viewActions.append(m_mscCore->actionToggleMinimap());
    m_viewActions.append(m_mscCore->actionMessageDeclarations());
    m_viewActions.append(m_mscCore->actionCheckInstances());
    connect(m_mscCore->actionCheckInstances(), &QAction::triggered, this, &MscAppWidget::checkInstances);
    m_viewActions.append(m_mscCore->actionCheckMessages());
    connect(m_mscCore->actionCheckMessages(), &QAction::triggered, this, &MscAppWidget::checkMessages);
    return m_viewActions;
}

} // namespace msc
