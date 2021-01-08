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

#include "asn1reader.h"
#include "astxmlparser.h"
#include "chartlayoutmanager.h"
#include "commands/cmdpastechart.h"
#include "documentitemmodel.h"
#include "exceptions.h"
#include "hierarchyviewmodel.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "mscchart.h"
#include "msccommandsstack.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"
#include "mscreader.h"
#include "mscwriter.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QGraphicsScene>
#include <QImage>
#include <QMimeData>
#include <QPainter>
#include <QUndoStack>

namespace msc {

const QLatin1String MainModel::MscChartMimeType("application/mscchart");

struct MainModelPrivate {
    explicit MainModelPrivate(MainModel *q)
        : m_mscModel(new MscModel())
        , m_chartLayoutManager(&m_undoStack)
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

    MscCommandsStack m_undoStack;
    MscModel *m_mscModel = nullptr; /// model of the msc data
    QStringList m_mscErrorMessages;
    ChartLayoutManager m_chartLayoutManager; /// model for the chart UI
    HierarchyViewModel m_hierarchyModel; /// model of the graphical document UI
    DocumentItemModel *m_documentItemModel = nullptr; /// model of the document tree
    QPointer<msc::MscDocument> m_selectedDocument;
    QString m_currentFilePath;
    QFileSystemWatcher *m_asn1Watcher = nullptr;
};

/*!
  \class msc::MainModel

  This is the main model for the msc chart view.
*/

/*!
 * \brief MainModel::MainModel Constructor
 * \param parent Object parent
 */
MainModel::MainModel(QObject *parent)
    : QObject(parent)
    , d(new MainModelPrivate(this))
{
    connect(&d->m_hierarchyModel, &HierarchyViewModel::documentDoubleClicked, this, &MainModel::showChartFromDocument);
    connect(&d->m_hierarchyModel, &HierarchyViewModel::selectedDocumentChanged, this, &MainModel::setSelectedDocument);

    connect(d->m_mscModel, &msc::MscModel::dataChanged, this, &MainModel::modelDataChanged);
    connect(&d->m_chartLayoutManager, &ChartLayoutManager::cifDataChanged, d->m_mscModel, &msc::MscModel::dataChanged);

    initialModel();
    d->m_documentItemModel->setUndoStack(&(d->m_undoStack));
    d->m_hierarchyModel.setUndoStack(&(d->m_undoStack));
    showFirstChart();
}

MainModel::~MainModel()
{
    clearMscModel();
}

/*!
 * \brief MainModel::initialModel Create the initial model.
 */
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
    d->m_undoStack.clear();
}

/*!
 * \brief MainModel::graphicsScene Get the current graphics scene the model is shown in
 * \return The graphics scene
 */
QGraphicsScene *MainModel::graphicsScene() const
{
    return d->m_chartLayoutManager.graphicsScene();
}

/*!
 * \brief MainModel::hierarchyScene Get the scene of the hierarchy view
 * \return the scene
 */
QGraphicsScene *MainModel::hierarchyScene() const
{
    return d->m_hierarchyModel.graphicsScene();
}

/*!
 * \brief MainModel::documentItemModel Get the model for the document
 * \return the model
 */
msc::DocumentItemModel *MainModel::documentItemModel() const
{
    return d->m_documentItemModel;
}

/*!
 * \brief MainModel::mscErrorMessages Get the list of error messages from the MSC parser
 * \return List of error strings
 */
QStringList MainModel::mscErrorMessages() const
{
    return d->m_mscErrorMessages;
}

/*!
 * \brief MainModel::chartViewModel Get the chart model
 * \return The chart model
 */
ChartLayoutManager &MainModel::chartViewModel() const
{
    return d->m_chartLayoutManager;
}

/*!
 * \brief MainModel::hierarchyViewModel Get the hierarchy view model
 * \return The hierarchy view model
 */
HierarchyViewModel &MainModel::hierarchyViewModel() const
{
    return d->m_hierarchyModel;
}

/*!
 * \brief MainModel::modelText Get the full MSC text of the model as it would look if saved to disk
 * \return The full MSC file contents
 */
QString MainModel::modelText() const
{
    msc::MscWriter mscWriter;
    return mscWriter.modelText(d->m_mscModel);
}

/*!
 * \brief MainModel::mscModel Get the MSC model
 * \return
 */
MscModel *MainModel::mscModel() const
{
    return d->m_mscModel;
}

/*!
 * \brief MainModel::chartText Get the full MSC text of a chart as it would look if saved to disk
 * \return The full MSC file contents of a chart
 */
QString MainModel::chartText(const MscChart *chart) const
{
    msc::MscWriter mscWriter;
    return mscWriter.serialize(chart);
}

/*!
 * \brief MainModel::setSelectedDocument Set the currently selected document
 * \param document
 */
void MainModel::setSelectedDocument(MscDocument *document)
{
    if (d->m_selectedDocument == document) {
        return;
    }

    d->m_selectedDocument = document;
    d->m_hierarchyModel.setSelectedDocument(d->m_selectedDocument);
    Q_EMIT selectedDocumentChanged(d->m_selectedDocument);
}

/*!
 * \brief MainModel::selectedDocument Get the currently selected document.
 * \return The selected document or null.
 */
MscDocument *MainModel::selectedDocument() const
{
    return d->m_selectedDocument;
}

/*!
 * \brief MainModel::setCurrentFilePath Set the path of the current file.
 * \param filePath
 */
void MainModel::setCurrentFilePath(const QString &filePath)
{
    if (filePath == d->m_currentFilePath)
        return;

    d->m_currentFilePath = filePath;
    Q_EMIT currentFilePathChanged(d->m_currentFilePath);
}

/*!
 * \brief MainModel::currentFilePath Get the path of the current file.
 * \return
 */
const QString &MainModel::currentFilePath() const
{
    return d->m_currentFilePath;
}

/*!
 * \brief MainModel::undoStack Get the undo stack
 * \return
 */
QUndoStack *MainModel::undoStack()
{
    return d->m_undoStack.undoStack();
}

msc::MscCommandsStack *MainModel::commandsStack() const
{
    return &(d->m_undoStack);
}

/*!
 * \brief MainModel::needSave Check if the model is modified since the last save
 * \return true if it is modified
 */
bool MainModel::needSave() const
{
    return !d->m_undoStack.isClean();
}

/*!
 * \brief MainModel::showFirstChart Fill the chart model with the first chart
 */
void MainModel::showFirstChart()
{
    d->m_chartLayoutManager.setCurrentChart(firstChart());
}

/*!
 * \brief MainModel::loadFile Load an MSC file
 * \param filename
 * \return true if the loading succeeded
 */
bool MainModel::loadFile(const QString &filename)
{
    msc::MscReader reader;
    msc::MscModel *model = nullptr;

    try {
        d->m_mscErrorMessages.clear();
        model = reader.parseFile(filename, &d->m_mscErrorMessages);
    } catch (const msc::ParserException &e) {
        d->m_mscErrorMessages.append(e.errorMessage());
        for (const QString &error : d->m_mscErrorMessages) {
            qWarning() << error;
        }
        return false;
    } catch (...) {
        // print error message
        return false;
    }

    setCurrentFilePath(filename);
    setNewModel(model);
    d->m_undoStack.clear();

    return true;
}

/*!
 * \brief MainModel::saveMsc Save the MSC file
 * \param filename
 * \return True if the save succeeded
 */
bool MainModel::saveMsc(const QString &filename)
{
    msc::MscWriter mscWriter;

    bool ok = true;
    if (d->m_mscModel && (d->m_mscModel->documents().size() || d->m_mscModel->charts().size()))
        ok = mscWriter.saveModel(d->m_mscModel, filename);
    else
        ok = mscWriter.saveChart(d->m_chartLayoutManager.currentChart(), filename);

    setCurrentFilePath(filename);
    d->m_undoStack.setClean();

    return ok;
}

/*!
 * \brief MainModel::copyCurrentChart Copy the current chart text to the clipboard
 */
void MainModel::copyCurrentChart()
{
    const QString charText = chartText(d->m_chartLayoutManager.currentChart());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(MscChartMimeType, charText.toLatin1());

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->clear();
    clipboard->setMimeData(mimeData);
}

/*!
 * \brief MainModel::copyCurrentChartAsPicture Copy the current chart as an image in the clipboard
 */
void MainModel::copyCurrentChartAsPicture()
{
    QGraphicsScene *scene = d->m_chartLayoutManager.graphicsScene();

    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    scene->render(&painter);

    QApplication::clipboard()->setImage(image);
}

/*!
 * \brief MainModel::pasteChart Paste the current clipboard into the current chart, if the clipboard
 * contains a chart.
 */
void MainModel::pasteChart()
{
    msc::MscDocument *document = d->m_selectedDocument;
    if (document == nullptr) {
        return;
    }

    const QMimeData *mideData = QApplication::clipboard()->mimeData();
    if (mideData->hasFormat(MscChartMimeType)) {
        d->m_undoStack.push(new cmd::CmdPasteChart(document, mideData->data(MscChartMimeType)));
        setSelectedDocument(document);
    }
}

/*!
 * \brief MainModel::showChartFromDocument Show a document in the chart view
 * \param document
 */
void MainModel::showChartFromDocument(MscDocument *document)
{
    if (document->charts().isEmpty()) {
        return;
    }

    d->m_chartLayoutManager.setCurrentChart(document->charts().at(0));
    Q_EMIT showChartVew();
}

/*!
   Reads the ASN1 data from the file referenced in the MscModel and sets it in the model
 */
void MainModel::readAsn1Types()
{
    const QFileInfo asn1FileInfo = asn1File();

    if (d->m_asn1Watcher != nullptr) {
        // Check if the directory is watched already
        if (!d->m_asn1Watcher->files().contains(asn1FileInfo.absoluteFilePath())) {
            disconnect(
                    d->m_mscModel, &msc::MscModel::dataDefinitionStringChanged, this, &msc::MainModel::readAsn1Types);
            disconnect(d->m_asn1Watcher, nullptr, this, nullptr);
            d->m_asn1Watcher->deleteLater();
            d->m_asn1Watcher = nullptr;
        }
    }

    if (asn1FileInfo.exists()) {
        Asn1Acn::Asn1Reader xmlParser;
        QStringList errorMessages;
        std::unique_ptr<Asn1Acn::File> asn1Data = xmlParser.parseAsn1File(asn1FileInfo, &errorMessages);
        QSharedPointer<Asn1Acn::File> sharedAsn1Data(asn1Data.release());
        if (errorMessages.isEmpty()) {
            d->m_mscModel->setAsn1TypesData(sharedAsn1Data);
        }
    }

    if (d->m_asn1Watcher == nullptr && asn1FileInfo.exists()) {
        d->m_asn1Watcher = new QFileSystemWatcher(this);
        d->m_asn1Watcher->addPath(asn1FileInfo.absoluteFilePath());
        connect(d->m_asn1Watcher, &QFileSystemWatcher::fileChanged, this, &msc::MainModel::readAsn1Types,
                Qt::UniqueConnection);
    }
}

/*!
 * \brief MainModel::firstChart Get the first chart
 * \return First chart pointer or null
 */
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

/*!
 * \brief MainModel::firstChart Get the first chart of the \a docs
 * \return The first chart or null
 */
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

/*!
 * \brief MainModel::clearMscModel Clear the scene.
 */
void MainModel::clearMscModel()
{
    d->m_documentItemModel->setMscModel(nullptr);
    d->m_chartLayoutManager.clearScene();
    d->m_hierarchyModel.setModel(nullptr);
    if (d->m_mscModel) {
        disconnect(&d->m_chartLayoutManager, &ChartLayoutManager::cifDataChanged, d->m_mscModel,
                &msc::MscModel::dataChanged);
        disconnect(d->m_mscModel, nullptr, this, nullptr);

        delete d->m_mscModel;
        d->m_mscModel = nullptr;
    }
}

/*!
 * \brief MainModel::setNewModel Set a new model and connect all signals and slots
 * \param model
 */
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
    connect(d->m_mscModel, &msc::MscModel::asn1ParameterErrorDetected, this,
            &msc::MainModel::asn1ParameterErrorDetected);
    connect(d->m_mscModel, &msc::MscModel::documentRemovedFrom, this, [&](msc::MscDocument *document, QObject *) {
        const msc::MscChart *currentChart = d->m_chartLayoutManager.currentChart();
        if (!currentChart || currentChart->parentDocument() == document) {
            showFirstChart();
        }
    });
    connect(&d->m_chartLayoutManager, &ChartLayoutManager::cifDataChanged, d->m_mscModel, &msc::MscModel::dataChanged);

    showFirstChart();
    d->m_hierarchyModel.setModel(d->m_mscModel);

    readAsn1Types();
    connect(d->m_mscModel, &msc::MscModel::dataDefinitionStringChanged, this, &msc::MainModel::readAsn1Types);
    connect(d->m_mscModel, &msc::MscModel::dataDefinitionStringChanged, this, &msc::MainModel::asn1FileNameChanged);

    Q_EMIT modelUpdated(d->m_mscModel);
    Q_EMIT asn1FileNameChanged(d->m_mscModel->dataDefinitionString());
}

QFileInfo MainModel::asn1File() const
{
    if (d->m_mscModel->dataDefinitionString().isEmpty()) {
        return {};
    }

    return QFileInfo(QFileInfo(d->m_currentFilePath).absolutePath() + "/" + d->m_mscModel->dataDefinitionString());
}

}
