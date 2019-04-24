/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mainmodel.h"

#include "chartviewmodel.h"
#include "commands/common/commandsstack.h"
#include "documentitemmodel.h"
#include "exceptions.h"
#include "hierarchyviewmodel.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscfile.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "mscwriter.h"

#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QGraphicsScene>
#include <QImage>
#include <QMimeData>
#include <QPainter>
#include <QUndoStack>

using namespace msc;

const QLatin1String MainModel::MscChartMimeType("application/mscchart");

struct MainModelPrivate {
    explicit MainModelPrivate(MainModel *q)
        : m_mscModel(new MscModel())
        , m_documentItemModel(new DocumentItemModel(q))
        , m_currentFilePath(
#if defined(QT_DEBUG) && defined(Q_OS_WIN)
                  QDir(QDir::current().path() + QString("/../../esa/examples")).path()
#else
                  "../../msceditor/examples"
#endif // QT_DEBUG and Q_OS_WIN
          )
    {
    }
    ~MainModelPrivate()
    {
        delete m_mscModel;
        delete m_documentItemModel;
    }

    MscModel *m_mscModel = nullptr; /// model of the msc data
    QStringList m_mscErrorMessages;
    ChartViewModel m_chartModel; /// model for the chart UI
    HierarchyViewModel m_hierarchyModel; /// model of the graphical document UI
    DocumentItemModel *m_documentItemModel = nullptr; /// model of the document tree
    QPointer<msc::MscDocument> m_selectedDocument;
    QString m_currentFilePath;

    QUndoStack m_undoStack;
    int m_lastSavedUndoId = 0;
};

MainModel::MainModel(QObject *parent)
    : QObject(parent)
    , d(new MainModelPrivate(this))
{
    connect(&d->m_hierarchyModel, &HierarchyViewModel::documentDoubleClicked, this, &MainModel::showChartFromDocument);
    connect(&d->m_hierarchyModel, &HierarchyViewModel::selectedDocumentChanged, this, &MainModel::setSelectedDocument);

    connect(d->m_mscModel, &msc::MscModel::dataChanged, this, &MainModel::modelDataChanged);
    connect(&d->m_chartModel, &ChartViewModel::cifDataChanged, this, &MainModel::modelDataChanged);

    initialModel();
    showFirstChart();

    msc::cmd::CommandsStack::setCurrent(&d->m_undoStack);
}

MainModel::~MainModel()
{
    clearMscModel();
}

void MainModel::initialModel()
{
    auto model = new MscModel();
    auto doc = new MscDocument(tr("Untitled_Document"));
    doc->setHierarchyType(MscDocument::HierarchyAnd);

    // leaf type document
    auto leafDoc = new MscDocument(tr("Untitled_Leaf"));
    leafDoc->setHierarchyType(MscDocument::HierarchyLeaf);
    leafDoc->addChart(new MscChart(tr("Untitled_MSC")));

    doc->addDocument(leafDoc);
    model->addDocument(doc);
    setNewModel(model);

    setCurrentFilePath("");
    clearUndoStack();
}

QGraphicsScene *MainModel::graphicsScene() const
{
    return d->m_chartModel.graphicsScene();
}

QGraphicsScene *MainModel::hierarchyScene() const
{
    return d->m_hierarchyModel.graphicsScene();
}

msc::DocumentItemModel *MainModel::documentItemModel() const
{
    return d->m_documentItemModel;
}

QStringList MainModel::mscErrorMessages() const
{
    return d->m_mscErrorMessages;
}

ChartViewModel &MainModel::chartViewModel() const
{
    return d->m_chartModel;
}

HierarchyViewModel &MainModel::hierarchyViewModel() const
{
    return d->m_hierarchyModel;
}

QString MainModel::modelText() const
{
    msc::MscWriter mscWriter;
    return mscWriter.modelText(d->m_mscModel);
}

MscModel *MainModel::mscModel() const
{
    return d->m_mscModel;
}

QString MainModel::chartText(const MscChart *chart) const
{
    msc::MscWriter mscWriter;
    return mscWriter.serialize(chart);
}

void MainModel::setSelectedDocument(MscDocument *document)
{
    if (d->m_selectedDocument == document)
        return;

    d->m_selectedDocument = document;
    d->m_hierarchyModel.setSelectedDocument(d->m_selectedDocument);
    Q_EMIT selectedDocumentChanged(d->m_selectedDocument);
}

MscDocument *MainModel::selectedDocument() const
{
    return d->m_selectedDocument;
}

void MainModel::setCurrentFilePath(const QString &filePath)
{
    if (filePath == d->m_currentFilePath)
        return;

    d->m_currentFilePath = filePath;
    Q_EMIT currentFilePathChanged(d->m_currentFilePath);
}

const QString &MainModel::currentFilePath() const
{
    return d->m_currentFilePath;
}

QUndoStack *MainModel::undoStack()
{
    return &d->m_undoStack;
}

void MainModel::clearUndoStack()
{
    d->m_undoStack.clear();
    storeCurrentUndoCommandId();
}

void MainModel::storeCurrentUndoCommandId()
{
    d->m_lastSavedUndoId = d->m_undoStack.index();
    Q_EMIT lasteSaveUndoChange();
}

bool MainModel::needSave() const
{
    return d->m_lastSavedUndoId != d->m_undoStack.index();
}

void MainModel::showFirstChart()
{
    d->m_chartModel.fillView(firstChart());
}

bool MainModel::loadFile(const QString &filename)
{
    msc::MscFile file;
    msc::MscModel *model = nullptr;

    try {
        d->m_mscErrorMessages.clear();
        model = file.parseFile(filename, &d->m_mscErrorMessages);
    } catch (const msc::ParserException &e) {
        d->m_mscErrorMessages.append(e.errorMessage());
        return false;
    } catch (...) {
        // print error message
        return false;
    }

    setNewModel(model);
    setCurrentFilePath(filename);
    clearUndoStack();

    return true;
}

bool MainModel::saveMsc(const QString &filename)
{
    msc::MscWriter mscWriter;

    bool ok = true;
    if (d->m_mscModel && (d->m_mscModel->documents().size() || d->m_mscModel->charts().size()))
        ok = mscWriter.saveModel(d->m_mscModel, filename);
    else
        ok = mscWriter.saveChart(d->m_chartModel.currentChart(), filename);

    setCurrentFilePath(filename);
    storeCurrentUndoCommandId();

    return ok;
}

void MainModel::copyCurrentChart()
{
    const QString charText = chartText(d->m_chartModel.currentChart());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(MscChartMimeType, charText.toLatin1());

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->clear();
    clipboard->setMimeData(mimeData);
}

void MainModel::copyCurrentChartAsPicture()
{
    QGraphicsScene *scene = d->m_chartModel.graphicsScene();

    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    scene->render(&painter);

    QApplication::clipboard()->setImage(image);
}

void MainModel::pasteChart()
{
    msc::MscDocument *document = d->m_selectedDocument;
    if (document == nullptr)
        return;

    const QMimeData *mideData = QApplication::clipboard()->mimeData();

    if (mideData->hasFormat(MscChartMimeType)) {
        const QVariantList &cmdParams = { QVariant::fromValue<msc::MscDocument *>(document),
                                          mideData->data(MscChartMimeType) };
        msc::cmd::CommandsStack::push(msc::cmd::Id::PasteChart, cmdParams);
        setSelectedDocument(document);
    }
}

void MainModel::showChartFromDocument(MscDocument *document)
{
    if (document->charts().isEmpty()) {
        return;
    }

    d->m_chartModel.fillView(document->charts().at(0));
    Q_EMIT showChartVew();
}

MscChart *MainModel::firstChart() const
{
    if (d->m_mscModel == nullptr) {
        return nullptr;
    }

    if (!d->m_mscModel->charts().empty()) {
        return d->m_mscModel->charts().at(0);
    }

    return firstChart(d->m_mscModel->documents());
}

MscChart *MainModel::firstChart(const QVector<MscDocument *> &docs) const
{
    for (MscDocument *doc : docs) {
        if (!doc->charts().isEmpty()) {
            return doc->charts().at(0);
        }
        auto ret = firstChart(doc->documents());
        if (ret != nullptr) {
            return ret;
        }
    }
    return nullptr;
}

void MainModel::clearMscModel()
{
    d->m_documentItemModel->setMscModel(nullptr);
    d->m_chartModel.clearScene();
    d->m_hierarchyModel.setModel(nullptr);
    if (d->m_mscModel) {
        disconnect(d->m_mscModel, nullptr, this, nullptr);
        delete d->m_mscModel;
        d->m_mscModel = nullptr;
    }
}

void MainModel::setNewModel(MscModel *model)
{
    Q_ASSERT(model != nullptr);

    clearMscModel();
    d->m_mscModel = model;

    d->m_documentItemModel->setMscModel(d->m_mscModel);

    connect(d->m_mscModel, &msc::MscModel::documentAdded, this, &MainModel::showFirstChart);
    connect(d->m_mscModel, &msc::MscModel::chartAdded, this, &MainModel::showFirstChart);
    connect(d->m_mscModel, &msc::MscModel::cleared, this, &MainModel::showFirstChart);
    connect(d->m_mscModel, &msc::MscModel::dataChanged, this, &MainModel::modelDataChanged);

    showFirstChart();
    d->m_hierarchyModel.setModel(d->m_mscModel);

    Q_EMIT modelUpdated(d->m_mscModel);
}
