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

#include "endtoendview.h"

#include "aadlconnectionchain.h"
#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "baseitems/common/aadlutils.h"
#include "endtoendconnections.h"
#include "interface/aadlconnectiongraphicsitem.h"
#include "interface/aadlflowconnectiongraphicsitem.h"
#include "interface/aadlfunctiongraphicsitem.h"
#include "interface/aadlinterfacegraphicsitem.h"
#include "interface/interfacedocument.h"
#include "leafdocumentsmodel.h"
#include "mscmodel.h"
#include "mscreader.h"
#include "ui/graphicsviewbase.h"
#include "ui_endtoendview.h"

#include <QBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QLabel>
#include <QListView>
#include <QMessageBox>
#include <QPushButton>
#include <memory>

namespace aadlinterface {

struct EndToEndView::EndToEndViewPrivate {
    Ui::EndToEndView *ui { new Ui::EndToEndView };
    QGraphicsScene *scene { nullptr };

    InterfaceDocument *document { nullptr };
    std::unique_ptr<msc::MscModel> model;

    EndToEndConnections dataflow;
    LeafDocumentsModel *leafDocuments { nullptr };

    QString lastExportPath;
};

//! Initialize this with the current document
EndToEndView::EndToEndView(InterfaceDocument *document, QWidget *parent)
    : QDialog(parent)
    , d(new EndToEndViewPrivate)
{
    d->ui->setupUi(this);
    setMscFiles({});

    setAttribute(Qt::WA_DeleteOnClose, false);
    setAttribute(Qt::WA_QuitOnClose, false);

    d->ui->view->setInteractive(false);
    d->scene = new QGraphicsScene(this);
    d->ui->view->setScene(d->scene);
    d->document = document;

    d->leafDocuments = new LeafDocumentsModel(this);
    d->ui->leafDocsView->setModel(d->leafDocuments);

    connect(d->ui->pathButton, &QPushButton::clicked, this, [this]() {
        QFileInfo fi(d->document->mscFileName());
        const QString dir = fi.path();
        const QString path =
                QFileDialog::getOpenFileName(this, tr("Choose MSC file"), dir, tr("MSC files (*.msc);;All files (*)"));
        if (!path.isEmpty()) {
            setMscFile(path);
        }
    });
    connect(d->ui->fileSelectBox, &QComboBox::currentTextChanged, this, &EndToEndView::setMscFile);

    connect(d->ui->exportButton, &QPushButton::clicked, this, &EndToEndView::exportToPng);

    // Listen to path changes from the document
    connect(d->document, &InterfaceDocument::mscFileNameChanged, this, &EndToEndView::setMscFile);

    // Refresh the view
    connect(d->ui->refreshButton, &QPushButton::clicked, this, &EndToEndView::refreshView);

    connect(d->ui->leafDocsView->selectionModel(), &QItemSelectionModel::currentChanged, this,
            &EndToEndView::refreshView);

    if (parent != nullptr) {
        // Size this a bit smaller than the parent
        const int margin = 10;
        resize(parent->width() - margin * 2, parent->height() - margin * 2);
        move(parent->x() + margin, parent->y() + margin);
    }
}

EndToEndView::~EndToEndView()
{
    delete d;
    d = nullptr;
}

//! This emits visibleChanged after calling the standard setVisible method.
//! And it refreshes the view if this is now visible.
//! This is used to keep the "show e2e" menu action updated.
void EndToEndView::setVisible(bool visible)
{
    const bool wasVisible = isVisible();
    QWidget::setVisible(visible);
    if (isVisible() != wasVisible) {
        Q_EMIT visibleChanged(visible);

        if (isVisible()) {
            refreshView();
        }
    }
}

void EndToEndView::setMscFiles(const QStringList &files)
{
    d->ui->fileSelectBox->clear();
    d->ui->fileSelectBox->addItems(files);

    const bool showFileButton = files.isEmpty();
    d->ui->pathButton->setVisible(showFileButton);
    d->ui->fileSelectBox->setVisible(!showFileButton);
}

//! Update the view with the current model and MSC file contents
void EndToEndView::refreshView()
{
    struct InternalConnection {
        EndToEndConnections::ConnectionInsideFunction connection;
        AADLInterfaceGraphicsItem *ri { nullptr };
        AADLInterfaceGraphicsItem *pi { nullptr };
    };
    QVector<InternalConnection> internalConnections;

    // Remove all the old ones
    qDeleteAll(d->scene->items());

    // Get the visible non-nested objects
    QList<aadl::AADLObject *> objects = d->document->objectsModel()->visibleObjects({});
    aadl::AADLObject::sortObjectList(objects);

    const QList<aadl::AADLConnectionChain *> chains = aadl::AADLConnectionChain::build(*d->document->objectsModel());

    msc::MscDocument *doc = nullptr;
    if (d->ui->leafDocsView->currentIndex().isValid()) {
        QVariant currentData = d->ui->leafDocsView->currentIndex().data(QObjectListModel::ObjectRole);
        doc = currentData.value<msc::MscDocument *>();
    }
    const EndToEndConnections::Dataflow dataflow = doc ? d->dataflow.dataflow(doc) : d->dataflow.dataflow();
    for (auto c : dataflow.internalConnections) {
        internalConnections << InternalConnection { c };
    };

    // Add new graphics items for each object
    QHash<shared::Id, QGraphicsItem *> items;
    for (auto obj : objects) {
        QGraphicsItem *parentItem = obj->parentObject() ? items.value(obj->parentObject()->id()) : nullptr;

        InteractiveObject *item = nullptr;
        switch (obj->aadlType()) {
        case aadl::AADLObject::Type::RequiredInterface:
            if (parentItem) {
                if (auto ri = qobject_cast<aadl::AADLObjectIfaceRequired *>(obj)) {
                    // Add the RI
                    auto graphicsItem = new AADLInterfaceGraphicsItem(ri, parentItem);
                    graphicsItem->init();
                    item = graphicsItem;

                    if (auto function = ri->function()) {
                        // Check if this is part of an internal connection
                        const QStringList labelsOriginal = ri->ifaceLabelList();
                        QStringList labels;
                        for (auto l : labelsOriginal) {
                            labels << l.trimmed().toLower();
                        }
                        const QString title = function->title().trimmed().toLower();
                        for (auto &ic : internalConnections) {
                            if (title == ic.connection.instance && labels.contains(ic.connection.interface2)) {
                                ic.ri = graphicsItem;
                            }
                        }
                    }
                }
            }
            break;
        case aadl::AADLObject::Type::ProvidedInterface:
            if (parentItem) {
                if (auto pi = qobject_cast<aadl::AADLObjectIfaceProvided *>(obj)) {
                    // Add the PI
                    auto graphicsItem = new AADLInterfaceGraphicsItem(pi, parentItem);
                    item = graphicsItem;

                    if (auto function = pi->function()) {
                        // Check if this is part of an internal connection
                        const QString interface = pi->ifaceLabel().trimmed().toLower();
                        const QString title = function->title().trimmed().toLower();
                        for (auto &ic : internalConnections) {
                            if (title == ic.connection.instance && interface == ic.connection.interface1) {
                                // This is the ri side
                                ic.pi = graphicsItem;
                            }
                        }
                    }
                }
            }
            break;
        case aadl::AADLObject::Type::Connection:
            if (auto connection = qobject_cast<aadl::AADLObjectConnection *>(obj)) {
                aadl::AADLObjectIface *ifaceStart = connection->sourceInterface();
                auto startItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                        ifaceStart ? items.value(ifaceStart->id()) : nullptr);

                aadl::AADLObjectIface *ifaceEnd = connection->targetInterface();
                auto endItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                        ifaceEnd ? items.value(ifaceEnd->id()) : nullptr);

                if (EndToEndConnections::isInDataflow(dataflow, chains, connection)) {
                    item = new AADLFlowConnectionGraphicsItem(connection, startItem, endItem, parentItem);
                } else {
                    item = new AADLConnectionGraphicsItem(connection, startItem, endItem, parentItem);
                }
            }
            break;
        case aadl::AADLObject::Type::Function:
            item = new AADLFunctionGraphicsItem(qobject_cast<aadl::AADLObjectFunction *>(obj), parentItem);
            break;
        default:
            break;
        }

        if (item != nullptr) {
            item->init();
            items.insert(obj->id(), item);
            if (item->parentItem() == nullptr) {
                // Only items without a parent should be added to the scene or we get a warning
                d->scene->addItem(item);
            }
            item->updateFromEntity();
        }
    }

    const ColorHandler colorHandler = ColorManager::colorsForItem(ColorManager::HandledColors::ConnectionFlow);
    for (auto ic : internalConnections) {
        if (ic.pi != nullptr && ic.ri != nullptr) {
            auto item = new QGraphicsPathItem;
            item->setPen(colorHandler.pen());
            QPainterPath path;
            path.moveTo(ic.pi->scenePos());
            path.lineTo(ic.ri->scenePos());
            item->setPath(path);
            item->setZValue(ZOrder.InternalConnection);
            d->scene->addItem(item);
        }
    }

    // Set the scene rect based on what we show
    d->scene->setSceneRect(d->scene->itemsBoundingRect());
}

void EndToEndView::exportToPng()
{
    QFileDialog dialog(parentWidget(), tr("Save as PNG"), d->lastExportPath, tr("PNG Images (*.png)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(".png");
    if (dialog.exec() == QDialog::Accepted) {
        const QString path = dialog.selectedUrls().value(0).toLocalFile();
        d->lastExportPath = path;

        // Create an image the size of the scene and render the scene into it
        QPixmap image(d->scene->sceneRect().size().toSize());
        image.fill(Qt::transparent);
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        d->scene->render(&painter);
        painter.end();
        if (!image.save(path)) {
            QMessageBox::critical(parentWidget(), tr("Export failed"), tr("Saving to file %1 failed").arg(path));
        }
    }
}

void EndToEndView::setMscFile(const QString &fileName)
{

    msc::MscReader reader;
    try {
        d->model.reset(reader.parseFile(fileName));
    } catch (...) {
        QMessageBox::warning(this, tr("Can't load msc file"),
                tr("Can't load the msc file\n%1\n\n%2").arg(fileName, reader.getErrorMessages().join('\n')));
        return;
    }

    d->document->setMscFileName(fileName);
    d->dataflow.setPath(fileName);
    QFileInfo info(fileName);
    d->ui->pathLabel->setText(tr("MSC file: %1").arg(info.fileName()));
    d->leafDocuments->fillModel(d->model->documents().first());
    d->ui->leafDocsView->setCurrentIndex(d->leafDocuments->index(0, 0));
}

}
