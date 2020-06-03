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

#include "mainwidget.h"

#include "asn1fileview.h"
#include "chartviewmodel.h"
#include "documentitemmodel.h"
#include "documenttreeview.h"
#include "graphicsview.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscmodel.h"
#include "tools/actioncreatortool.h"
#include "tools/commentcreatortool.h"
#include "tools/conditioncreatortool.h"
#include "tools/coregioncreatortool.h"
#include "tools/entitydeletetool.h"
#include "tools/hierarchycreatortool.h"
#include "tools/instancecreatortool.h"
#include "tools/instancestoptool.h"
#include "tools/messagecreatortool.h"
#include "tools/pointertool.h"
#include "tools/timercreatortool.h"

#include <QAction>
#include <QDebug>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QImage>
#include <QSplitter>
#include <QStackedWidget>
#include <QUndoStack>
#include <QtWidgets/QHeaderView>
#include <coreplugin/minisplitter.h>

namespace MscPlugin {
namespace Common {
static const char *HIERARCHY_TYPE_TAG = "hierarchyTag";

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , m_model(new msc::MainModel(this))
{
    initUi();

    m_documentTree->setModel(m_model->documentItemModel());
    m_asn1Widget->setModel(m_model->mscModel());
    m_graphicsView->setScene(m_model->graphicsScene());
    m_hierarchyView->setBackgroundBrush(QImage(":/resources/resources/texture.png"));
    m_hierarchyView->setScene(m_model->hierarchyScene());

    initActions();
    initTools();
    initDocumentViewActions();
    initConnections();

    showDocumentView(true);
}

MainWidget::~MainWidget()
{
    if (m_documentTree->model()) {
        disconnect(m_documentTree->model(), nullptr, this, nullptr);
    }
    disconnect(&(m_model->chartViewModel()), nullptr, this, nullptr);
}

bool MainWidget::load(const QString &filename)
{
    m_model->chartViewModel().setPreferredChartBoxSize(QSizeF());

    const bool ok = m_model->loadFile(filename);
    if (ok) {
        selectCurrentChart();
        m_graphicsView->setZoom(100);
    }

    return ok;
}

bool MainWidget::save()
{
    return m_model->saveMsc(m_model->currentFilePath());
}

QString MainWidget::errorMessage() const
{
    return m_model->mscErrorMessages().join("\n");
}

void MainWidget::setFileName(const QString &filename)
{
    m_model->setCurrentFilePath(filename);
}

bool MainWidget::isDirty() const
{
    return m_model->needSave();
}

QUndoStack *MainWidget::undoStack()
{
    return m_model->undoStack();
}

QString MainWidget::textContents() const
{
    return m_model->modelText();
}

QAction *MainWidget::actionToolDelete() const
{
    return m_deleteTool->action();
}

QVector<QAction *> MainWidget::toolActions() const
{
    QVector<QAction *> actions;
    actions.reserve(m_tools.size());
    for (msc::BaseTool *tool : m_tools) {
        if (tool != m_deleteTool)
            actions.append(tool->action());
    }
    return actions;
}

QVector<QAction *> MainWidget::hierarchyActions() const
{
    QVector<QAction *> actions;
    actions.reserve(m_tools.size());
    for (msc::BaseTool *tool : m_hierarchyTools) {
        actions.append(tool->action());
    }
    return actions;
}

void MainWidget::selectCurrentChart()
{
    msc::MscChart *chart = m_model->chartViewModel().currentChart();
    if (m_deleteTool) {
        m_deleteTool->setCurrentChart(chart);
    }
}

void MainWidget::showChart(const QModelIndex &index)
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
            m_model->chartViewModel().fillView(document->charts()[0]);
            showDocumentView(true);
        }
    }
}

void MainWidget::showSelection(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    if (!current.isValid()) {
        return;
    }

    auto *obj = static_cast<QObject *>(current.internalPointer());
    if (obj == nullptr) {
        m_model->setSelectedDocument(nullptr);
        return;
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
        m_model->chartViewModel().fillView(chart);
        showDocumentView(true);
    } else {
        showHierarchyView(true);

        if (auto document = dynamic_cast<msc::MscDocument *>(obj)) {
            bool canNewChild = true;
            if (document->hierarchyType() == msc::MscDocument::HierarchyType::HierarchyLeaf)
                canNewChild = false;
            if (document->documents().size() > 0
                    && (document->hierarchyType() == msc::MscDocument::HierarchyType::HierarchyRepeat
                            || document->hierarchyType() == msc::MscDocument::HierarchyType::HierarchyIs
                            || document->hierarchyType() == msc::MscDocument::HierarchyType::HierarchyException)) {
                canNewChild = false;
            }
            for (QAction *action : hierarchyActions()) {
                action->setEnabled(canNewChild);
            }

            m_model->setSelectedDocument(document);
        }
    }
}

void MainWidget::showDocumentView(bool show)
{
    if (show) {
        m_centerView->setCurrentWidget(m_graphicsView);

        m_actCopy->setEnabled(true);

        msc::MscChart *chart = m_model->chartViewModel().currentChart();
        m_deleteTool->setView(m_graphicsView);
        m_deleteTool->setCurrentChart(chart);

        for (QAction *action : toolActions()) {
            action->setEnabled(true);
        }
        for (QAction *action : hierarchyActions()) {
            action->setEnabled(false);
        }
    }
}

void MainWidget::showHierarchyView(bool show)
{
    if (show) {
        m_centerView->setCurrentWidget(m_hierarchyView);

        m_actCopy->setEnabled(false);
        m_actPaste->setEnabled(false);

        m_deleteTool->setView(m_hierarchyView);
        m_deleteTool->setCurrentChart(nullptr);

        for (QAction *action : toolActions()) {
            action->setEnabled(false);
        }
        for (QAction *action : hierarchyActions()) {
            action->setEnabled(true);
        }
    }
}

void MainWidget::initUi()
{
    m_centerView = new QStackedWidget(this);
    m_graphicsView = new msc::GraphicsView();
    m_centerView->addWidget(m_graphicsView);
    m_hierarchyView = new msc::GraphicsView();
    m_centerView->addWidget(m_hierarchyView);

    m_documentTree = new msc::DocumentTreeView(this);
    m_documentTree->header()->setVisible(true);

    m_asn1Widget = new msc::ASN1FileView(this);

    m_leftVerticalSplitter = new Core::MiniSplitter(Qt::Vertical);
    m_leftVerticalSplitter->addWidget(m_documentTree);
    m_leftVerticalSplitter->addWidget(m_asn1Widget);
    m_leftVerticalSplitter->setStretchFactor(0, 1);
    m_leftVerticalSplitter->setStretchFactor(1, 0);

    m_horizontalSplitter = new Core::MiniSplitter(Qt::Horizontal);
    m_horizontalSplitter->addWidget(m_leftVerticalSplitter);
    m_horizontalSplitter->addWidget(m_centerView);
    m_horizontalSplitter->setStretchFactor(0, 0);
    m_horizontalSplitter->setStretchFactor(1, 1);

    setLayout(new QVBoxLayout);
    layout()->addWidget(m_horizontalSplitter);
    layout()->setMargin(0);
}

void MainWidget::initActions()
{
    m_actCopy = new QAction(tr("Copy Diagram"), this);
    m_actCopy->setShortcut(QKeySequence::Copy);
    m_actCopy->setIcon(QIcon::fromTheme("edit-copy"));
    connect(m_actCopy, &QAction::triggered, m_model, &msc::MainModel::copyCurrentChart);

    m_actPaste = new QAction(tr("Paste:"), this);
    m_actPaste->setShortcut(QKeySequence::Paste);
    m_actPaste->setIcon(QIcon::fromTheme("edit-paste"));
    connect(m_actPaste, &QAction::triggered, m_model, &msc::MainModel::pasteChart);
}

void MainWidget::initTools()
{
    m_deleteTool = new msc::EntityDeleteTool(&(m_model->chartViewModel()), m_graphicsView, this);
    m_deleteTool->setCurrentChart(m_model->chartViewModel().currentChart());

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
        QAction *toolAction = new QAction(tool->title(), this);
        toolAction->setCheckable(true);
        toolAction->setIcon(tool->icon());
        toolAction->setToolTip(tr("%1: %2").arg(tool->title(), tool->description()));
        toolAction->setData(QVariant::fromValue<msc::BaseTool::ToolType>(tool->toolType()));
        tool->setView(m_graphicsView);
        tool->setAction(toolAction);

        if (msc::BaseCreatorTool *creatorTool = qobject_cast<msc::BaseCreatorTool *>(tool))
            connect(creatorTool, &msc::BaseCreatorTool::created, this, &MainWidget::activateDefaultTool);

        toolsActions->addAction(toolAction);
    }
    activateDefaultTool();
}

void MainWidget::initDocumentViewActions()
{
    auto addAction = [&](msc::MscDocument::HierarchyType type) {
        // create tool
        auto tool = new msc::HierarchyCreatorTool(type, &(m_model->hierarchyViewModel()), nullptr, this);
        QAction *action = new QAction(tool->title(), tool);
        action->setProperty(HIERARCHY_TYPE_TAG, type);
        action->setCheckable(true);
        action->setIcon(tool->icon());
        action->setToolTip(tr("%1: %2").arg(tool->title(), tool->description()));
        tool->setAction(action);
        tool->setView(m_hierarchyView);

        m_hierarchyTools.append(tool);

        connect(tool, &msc::HierarchyCreatorTool::documentCreated, this, [&](msc::MscDocument *document) {
            activateDefaultTool();
            m_model->setSelectedDocument(document);
        });

        connect(action, &QAction::toggled, tool, &msc::BaseTool::setActive);
        connect(action, &QAction::toggled, this, [&]() {
            for (QAction *action : hierarchyActions())
                action->setChecked(false);
        });
    };

    addAction(msc::MscDocument::HierarchyAnd);
    addAction(msc::MscDocument::HierarchyOr);
    addAction(msc::MscDocument::HierarchyParallel);
    addAction(msc::MscDocument::HierarchyIs);
    addAction(msc::MscDocument::HierarchyRepeat);
    addAction(msc::MscDocument::HierarchyException);
    addAction(msc::MscDocument::HierarchyLeaf);
}

void MainWidget::initConnections()
{
    connect(m_documentTree->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWidget::showSelection);
    connect(m_documentTree, &QTreeView::doubleClicked, this, &MainWidget::showChart);

    connect(&(m_model->chartViewModel()), &msc::ChartViewModel::currentChartChanged, this,
            &MainWidget::selectCurrentChart);

    connect(m_model, &msc::MainModel::showChartVew, this, [this]() { showDocumentView(true); });

    connect(m_model, &msc::MainModel::selectedDocumentChanged, m_documentTree,
            &msc::DocumentTreeView::setSelectedDocument);

    connect(m_model, &msc::MainModel::modelUpdated, m_asn1Widget, &msc::ASN1FileView::setModel);

    connect(m_model->documentItemModel(), &msc::DocumentItemModel::dataChanged, this, &MainWidget::showSelection);

    connect(m_model->undoStack(), &QUndoStack::indexChanged, this, [&]() { Q_EMIT dirtyChanged(isDirty()); });
    connect(m_model, &msc::MainModel::lasteSaveUndoChange, this, [&]() { Q_EMIT dirtyChanged(isDirty()); });

    connect(m_model, &msc::MainModel::currentFilePathChanged, this, [&](const QString &filename) {
        QFileInfo fileInfo(filename);
        m_asn1Widget->setCurrentDirectory(fileInfo.absolutePath());
    });
}

void MainWidget::activateDefaultTool()
{
    for (msc::BaseTool *tool : m_tools) {
        if (tool != m_pointerTool)
            tool->action()->setChecked(false);
    }
    m_pointerTool->action()->setChecked(true);
}

} // namespace Common
} // namespace MscPlugin
