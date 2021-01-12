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

#include "mscmainwidget.h"

#include "actionsbar.h"
#include "asn1fileview.h"
#include "chartlayoutmanager.h"
#include "commands/cmdsetasn1file.h"
#include "documentitemmodel.h"
#include "documenttreeview.h"
#include "graphicsview.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "msceditorcore.h"
#include "mscmodel.h"
#include "mscmodelstorage.h"
#include "tools/entitydeletetool.h"

#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QImage>
#include <QMessageBox>
#include <QPushButton>
#include <QSplitter>
#include <QStackedWidget>
#include <QUndoStack>
#include <QVBoxLayout>
#include <QtWidgets/QHeaderView>
#include <coreplugin/minisplitter.h>

namespace spctr {

/*!
   \brief MscEditorCore::MainWidget::MainWidget Is the main widget for the whole MSC plugin in QtCreator
 */

MscMainWidget::MscMainWidget(MscModelStorage *mscStorage, QWidget *parent)
    : QWidget(parent)
    , m_mscStorage((mscStorage))
{
    init();
}

MscMainWidget::~MscMainWidget()
{
    if (m_documentTree->model()) {
        disconnect(m_documentTree->model(), nullptr, this, nullptr);
    }
    if (!m_plugin.isNull()) {
        disconnect(&(m_plugin->mainModel()->chartViewModel()), nullptr, this, nullptr);
        disconnect(m_plugin->mainModel(), nullptr, this, nullptr);
        disconnect(m_plugin->mainModel()->undoStack(), nullptr, this, nullptr);
    }
}

bool MscMainWidget::load(const QString &filename)
{
    m_plugin = m_mscStorage->mscData(filename);
    if (!m_plugin->chartView()) {
        init();
        m_plugin->mainModel()->chartViewModel().clearScene();
        m_plugin->mainModel()->chartViewModel().updateLayout();
        m_plugin->chartView()->setZoom(100);
    }

    m_asn1Switch->setText(m_plugin->mainModel()->mscModel()->dataDefinitionString());

    if (m_documentTree) {
        m_documentTree->expandAll();
        m_documentTree->setSelectedDocument(m_plugin->mainModel()->selectedDocument());
    }
    Q_EMIT mscDataLoaded(filename, m_plugin);

    return true;
}

bool MscMainWidget::save()
{
    if (m_plugin.isNull()) {
        return false;
    }
    return m_plugin->mainModel()->saveMsc(m_plugin->mainModel()->currentFilePath());
}

QString MscMainWidget::errorMessage() const
{
    if (m_plugin.isNull()) {
        return {};
    }
    return m_plugin->mainModel()->mscErrorMessages().join("\n");
}

void MscMainWidget::setFileName(const QString &filename)
{
    if (m_plugin.isNull()) {
        return;
    }
    m_plugin->mainModel()->setCurrentFilePath(filename);
}

bool MscMainWidget::isDirty() const
{
    if (m_plugin.isNull()) {
        return false;
    }
    return m_plugin->mainModel()->needSave();
}

QUndoStack *MscMainWidget::undoStack()
{
    if (m_plugin.isNull()) {
        return {};
    }
    return m_plugin->mainModel()->undoStack();
}

QString MscMainWidget::textContents() const
{
    if (m_plugin.isNull()) {
        return {};
    }
    return m_plugin->mainModel()->modelText();
}

QAction *MscMainWidget::actionCopy() const
{
    if (m_plugin.isNull()) {
        return {};
    }
    return m_plugin->actionCopy();
}

QAction *MscMainWidget::actionPaste() const
{
    if (m_plugin.isNull()) {
        return {};
    }
    return m_plugin->actionPaste();
}

QAction *MscMainWidget::actionToolDelete() const
{
    if (m_plugin.isNull()) {
        return {};
    }
    return m_plugin->deleteTool()->action();
}

QSharedPointer<msc::MSCEditorCore> MscMainWidget::mscCore() const
{
    Q_ASSERT(!m_plugin.isNull());
    return m_plugin;
}

void MscMainWidget::showChart(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    if (m_plugin.isNull()) {
        return;
    }

    auto *obj = static_cast<QObject *>(index.internalPointer());
    if (obj == nullptr) {
        return;
    }

    if (auto document = dynamic_cast<msc::MscDocument *>(obj)) {
        if (!document->charts().empty()) {
            m_plugin->mainModel()->chartViewModel().setCurrentChart(document->charts()[0]);
            m_plugin->showDocumentView(true);
        }
    }
}

void MscMainWidget::showSelection(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    if (!current.isValid()) {
        return;
    }
    if (m_plugin.isNull()) {
        return;
    }

    auto *obj = static_cast<QObject *>(current.internalPointer());
    if (obj == nullptr) {
        m_plugin->mainModel()->setSelectedDocument(nullptr);
        return;
    }

    if (m_plugin->chartView()) {
        static constexpr qreal padding = 110.;
        QSizeF preferredSize = m_plugin->chartView()->size() - QSizeF(padding, padding);
        m_plugin->mainModel()->chartViewModel().setPreferredChartBoxSize(preferredSize);
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
        m_plugin->mainModel()->chartViewModel().setCurrentChart(chart);
        m_plugin->showDocumentView(true);
    } else {
        m_plugin->showHierarchyView(true);

        if (auto document = dynamic_cast<msc::MscDocument *>(obj)) {
            m_plugin->mainModel()->setSelectedDocument(document);
        }
    }
}

void MscMainWidget::showAsn1Errors(const QStringList &faultyMessages)
{
    QMessageBox::warning(
            this, tr("ASN1 error"), tr("Following messages have ASN.1 errors:") + "\n" + faultyMessages.join("\n"));
}

/*!
   Thw the chart of document tool bar depending of the view mode
 */
void MscMainWidget::onViewModeChanged()
{
    m_chartToolBar->setVisible(m_plugin->viewMode() == msc::MSCEditorCore::ViewMode::CHART);
    m_documentToolBar->setVisible(m_plugin->viewMode() == msc::MSCEditorCore::ViewMode::HIERARCHY);
}

void MscMainWidget::openAsn1Dialog()
{
    QString dir = QFileInfo(m_plugin->mainModel()->currentFilePath()).absolutePath();
    QString asn1File = QFileDialog::getOpenFileName(this, tr("Select a asn.1 file"), dir, "*.asn");
    if (!asn1File.isEmpty()) {
        QFileInfo fi(asn1File);
        msc::MscModel *model = m_plugin->mainModel()->mscModel();
        m_plugin->mainModel()->undoStack()->push(new msc::cmd::CmdSetAsn1File(model, fi.fileName(), "ASN.1"));
    }
}

void MscMainWidget::init()
{
    if (m_documentTree || m_plugin.isNull()) {
        // initialized already
        return;
    }

    auto viewLayout = new QHBoxLayout(this);
    viewLayout->setMargin(0);
    viewLayout->setSpacing(0);
    this->setLayout(viewLayout);

    m_plugin->showToolbars(false);

    m_chartToolBar = new shared::ActionsBar(this);
    viewLayout->addWidget(m_chartToolBar);
    m_documentToolBar = new shared::ActionsBar(this);
    viewLayout->addWidget(m_documentToolBar);
    connect(m_plugin.data(), &msc::MSCEditorCore::viewModeChanged, this, &spctr::MscMainWidget::onViewModeChanged);
    onViewModeChanged();

    auto centerView = new QStackedWidget(this);
    auto graphicsView = new msc::GraphicsView(this);
    centerView->addWidget(graphicsView);
    auto hierarchyView = new msc::GraphicsView(this);
    centerView->addWidget(hierarchyView);

    m_plugin->setViews(centerView, graphicsView, hierarchyView);

    m_leftArea = new QWidget(this);

    m_documentTree = new msc::DocumentTreeView(m_leftArea);
    m_documentTree->header()->setVisible(true);

    m_aadlSwitch = new QPushButton("Interface view", m_leftArea);
    m_aadlSwitch->setToolTip(tr("Open the file"));
    connect(m_aadlSwitch, &QPushButton::clicked, this, &spctr::MscMainWidget::showAadlFile);
    m_aadlSwitch->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    auto asn1Widget = new QWidget(m_leftArea);
    asn1Widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    auto asn1Layout = new QHBoxLayout(asn1Widget);
    asn1Layout->setMargin(0);
    asn1Widget->setLayout(asn1Layout);
    m_asn1Switch = new QPushButton("file.asn", asn1Widget);
    m_asn1Switch->setToolTip(tr("Open the file"));
    m_asn1Switch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_asn1Select = new QPushButton(tr("..."), asn1Widget);
    m_asn1Select->setToolTip(tr("Select the ASN.1 file"));
    connect(m_asn1Select, &QPushButton::clicked, this, &spctr::MscMainWidget::openAsn1Dialog);
    m_asn1Select->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    asn1Layout->addWidget(m_asn1Switch);
    asn1Layout->addWidget(m_asn1Select);

    m_leftVerticalLayout = new QVBoxLayout(m_leftArea);
    m_leftVerticalLayout->setMargin(0);
    m_leftVerticalLayout->addWidget(m_documentTree);
    m_leftVerticalLayout->addWidget(m_aadlSwitch);
    m_leftVerticalLayout->addWidget(asn1Widget);

    m_horizontalSplitter = new Core::MiniSplitter(Qt::Horizontal);
    m_horizontalSplitter->addWidget(m_leftArea);
    m_horizontalSplitter->addWidget(centerView);
    m_horizontalSplitter->setStretchFactor(0, 0);
    m_horizontalSplitter->setStretchFactor(1, 1);

    layout()->addWidget(m_horizontalSplitter);
    layout()->setMargin(0);

    m_documentTree->setModel(m_plugin->mainModel()->documentItemModel());

    m_plugin->createActionCopy(nullptr);
    m_plugin->createActionPaste(nullptr);
    m_plugin->initChartTools();
    m_plugin->initHierarchyViewActions();
    initConnections();
    m_plugin->initConnections();
    m_plugin->setupMiniMap();
    m_plugin->showDocumentView(true);

    for (QAction *chartAction : m_plugin->chartActions()) {
        m_chartToolBar->addAction(chartAction);
    }
    for (QAction *documentAction : m_plugin->hierarchyActions()) {
        m_documentToolBar->addAction(documentAction);
    }
}

void MscMainWidget::initConnections()
{
    connect(m_documentTree->selectionModel(), &QItemSelectionModel::currentChanged, this,
            &MscMainWidget::showSelection);
    connect(m_documentTree, &QTreeView::doubleClicked, this, &MscMainWidget::showChart);

    connect(m_plugin->mainModel(), &msc::MainModel::selectedDocumentChanged, m_documentTree,
            &msc::DocumentTreeView::setSelectedDocument);

    connect(m_plugin->mainModel()->documentItemModel(), &msc::DocumentItemModel::dataChanged, this,
            &MscMainWidget::showSelection);

    connect(m_plugin->mainModel()->undoStack(), &QUndoStack::cleanChanged, this,
            [&]() { Q_EMIT dirtyChanged(isDirty()); });

    connect(m_plugin->mainModel(), &msc::MainModel::asn1ParameterErrorDetected, this, &MscMainWidget::showAsn1Errors);

    connect(m_plugin->mainModel(), &msc::MainModel::asn1FileNameChanged, m_asn1Switch, &QPushButton::setText);
    connect(m_asn1Switch, &QPushButton::clicked, this,
            [&]() { Q_EMIT showAsn1File(m_plugin->mainModel()->asn1File().absoluteFilePath()); });
}

}
