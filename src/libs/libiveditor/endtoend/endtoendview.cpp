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

#include "endtoendconnections.h"
#include "interfacedocument.h"
#include "itemeditor/common/ivutils.h"
#include "itemeditor/graphicsitemhelpers.h"
#include "itemeditor/ivconnectiongraphicsitem.h"
#include "itemeditor/ivconnectiongroupgraphicsitem.h"
#include "itemeditor/ivflowconnectiongraphicsitem.h"
#include "itemeditor/ivfunctiongraphicsitem.h"
#include "itemeditor/ivinterfacegraphicsitem.h"
#include "itemeditor/ivinterfacegroupgraphicsitem.h"
#include "ivconnection.h"
#include "ivconnectionchain.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivinterfacegroup.h"
#include "ivmodel.h"
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

namespace ive {

struct EndToEndView::EndToEndViewPrivate {
    Ui::EndToEndView *ui { new Ui::EndToEndView };
    QGraphicsScene *scene { nullptr };

    InterfaceDocument *document { nullptr };
    msc::MscModel *model { nullptr };

    EndToEndConnections dataflow;
    LeafDocumentsModel *leafDocuments { nullptr };

    QString lastExportPath;

    std::function<msc::MscModel *(QString)> mscModelFetcher = [](QString) { return nullptr; };
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
            bool match = refreshView();
            if (!match) {
                QMessageBox::information(
                        this, tr("File mismatch"), tr("The IV file and the MSC file do not seem to match."));
            }
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

/*!
   Sets a function that returns the MscModel for a given filename
 */
void EndToEndView::setMscDataFetcher(std::function<msc::MscModel *(QString)> fetcher)
{
    d->mscModelFetcher = fetcher;
}

/*!
   Update the view with the current model and MSC file contents
   @return Returns true, if at least one matching connection was found
*/
bool EndToEndView::refreshView()
{
    struct InternalConnection {
        EndToEndConnections::ConnectionInsideFunction connection;
        IVInterfaceGraphicsItem *ri { nullptr };
        IVInterfaceGraphicsItem *pi { nullptr };
    };
    QVector<InternalConnection> internalConnections;

    // Remove all the old ones
    qDeleteAll(d->scene->items());

    // Get the visible non-nested objects
    QList<ivm::IVObject *> objects = d->document->objectsModel()->visibleObjects({});
    ivm::IVObject::sortObjectList(objects);

    const QList<ivm::IVConnectionChain *> chains = ivm::IVConnectionChain::build(*d->document->objectsModel());

    msc::MscDocument *doc = nullptr;
    if (d->ui->leafDocsView->currentIndex().isValid()) {
        QVariant currentData = d->ui->leafDocsView->currentIndex().data(QObjectListModel::ObjectRole);
        doc = currentData.value<msc::MscDocument *>();
    }
    const EndToEndConnections::Dataflow dataflow = doc ? d->dataflow.dataflow(doc) : d->dataflow.dataflow();
    for (auto c : dataflow.internalConnections) {
        internalConnections << InternalConnection { c };
    };

    bool foundConnection = false;
    QVector<IVInterfaceGraphicsItem *> interfaceItems;

    // Add new graphics items for each object
    QHash<shared::Id, QGraphicsItem *> items;
    shared::ui::VEInteractiveObject *rootItem = nullptr;
    for (auto obj : objects) {
        const int lowestLevel = gi::nestingLevel(d->document->objectsModel()->rootObject()) + 1;
        const int objectLevel = gi::nestingLevel(obj);
        const bool isRootOrRootChild = obj->id() == d->document->objectsModel()->rootObjectId()
                || (d->document->objectsModel()->rootObject()
                        && obj->parentObject() == d->document->objectsModel()->rootObject());
        if ((objectLevel < lowestLevel || objectLevel > (lowestLevel + gi::kNestingVisibilityLevel))
                && !isRootOrRootChild) {
            continue;
        }

        QGraphicsItem *parentItem = obj->parentObject() ? items.value(obj->parentObject()->id()) : nullptr;

        shared::ui::VEInteractiveObject *item = nullptr;
        switch (obj->type()) {
        case ivm::IVObject::Type::InterfaceGroup:
            if (parentItem) {
                if (auto ifaceGroup = qobject_cast<ivm::IVInterfaceGroup *>(obj)) {
                    // Add the Interface
                    auto graphicsItem = new IVInterfaceGroupGraphicsItem(ifaceGroup, parentItem);
                    item = graphicsItem;

                    if (auto function = ifaceGroup->function()) {
                        // Check if this is part of an internal connection
                        QStringList labels;
                        for (auto iface : ifaceGroup->entities()) {
                            if (iface->direction() == ivm::IVInterface::InterfaceType::Required) {
                                const QStringList labelsOriginal =
                                        iface->as<ivm::IVInterfaceRequired *>()->ifaceLabelList();
                                for (const auto &label : labelsOriginal) {
                                    labels << label.trimmed().toLower();
                                }
                                const QString title = function->title().trimmed().toLower();
                                for (auto &internalConnection : internalConnections) {
                                    if (title == internalConnection.connection.instance
                                            && labels.contains(internalConnection.connection.interface2)) {
                                        internalConnection.ri = graphicsItem;
                                    }
                                }
                            } else if (iface->direction() == ivm::IVInterface::InterfaceType::Provided) {
                                const QString interface =
                                        iface->as<ivm::IVInterfaceProvided *>()->ifaceLabel().trimmed().toLower();
                                const QString title = function->title().trimmed().toLower();
                                for (auto &internalConnection : internalConnections) {
                                    if (title == internalConnection.connection.instance
                                            && interface == internalConnection.connection.interface1) {
                                        // This is the pi side
                                        internalConnection.pi = graphicsItem;
                                    }
                                }
                            } else {
                                break;
                            }
                        }
                    }
                }
            }
            break;
        case ivm::IVObject::Type::RequiredInterface:
            if (parentItem && !obj->isGrouped()) {
                if (auto reqIface = qobject_cast<ivm::IVInterfaceRequired *>(obj)) {
                    // Add the RI
                    auto graphicsItem = new IVInterfaceGraphicsItem(reqIface, parentItem);
                    interfaceItems.append(graphicsItem);
                    item = graphicsItem;

                    if (auto function = reqIface->function()) {
                        // Check if this is part of an internal connection
                        const QStringList labelsOriginal = reqIface->ifaceLabelList();
                        QStringList labels;
                        for (const auto &label : labelsOriginal) {
                            labels << label.trimmed().toLower();
                        }
                        const QString title = function->title().trimmed().toLower();
                        for (auto &internalConnection : internalConnections) {
                            if (title == internalConnection.connection.instance
                                    && labels.contains(internalConnection.connection.interface2)) {
                                internalConnection.ri = graphicsItem;
                            }
                        }
                    }
                }
            }
            break;
        case ivm::IVObject::Type::ProvidedInterface:
            if (parentItem && !obj->isGrouped()) {
                if (auto provIface = qobject_cast<ivm::IVInterfaceProvided *>(obj)) {
                    // Add the PI
                    auto graphicsItem = new IVInterfaceGraphicsItem(provIface, parentItem);
                    interfaceItems.append(graphicsItem);
                    item = graphicsItem;

                    if (auto function = provIface->function()) {
                        // Check if this is part of an internal connection
                        const QString interface = provIface->ifaceLabel().trimmed().toLower();
                        const QString title = function->title().trimmed().toLower();
                        for (auto &internalConnection : internalConnections) {
                            if (title == internalConnection.connection.instance
                                    && interface == internalConnection.connection.interface1) {
                                // This is the pi side
                                internalConnection.pi = graphicsItem;
                            }
                        }
                    }
                }
            }
            break;
        case ivm::IVObject::Type::ConnectionGroup:
        case ivm::IVObject::Type::Connection:
            if (auto connection = qobject_cast<ivm::IVConnection *>(obj)) {
                auto findGroupObject = [&](ivm::IVInterface *iface) {
                    auto it = std::find_if(objects.cbegin(), objects.cend(), [iface](ivm::IVObject *obj) {
                        if (obj->type() == ivm::IVObject::Type::InterfaceGroup) {
                            for (auto entity : obj->as<ivm::IVInterfaceGroup *>()->entities()) {
                                if (entity->id() == iface->id()) {
                                    return true;
                                }
                            }
                        }
                        return false;
                    });
                    return it != objects.cend() ? (*it)->as<ivm::IVInterfaceGroup *>() : nullptr;
                };

                ivm::IVInterface *ifaceStart = connection->sourceInterface();
                if (ifaceStart && ifaceStart->isGrouped()
                        && ifaceStart->type() != ivm::IVObject::Type::InterfaceGroup) {
                    ifaceStart = findGroupObject(ifaceStart);
                }
                auto startItem = qgraphicsitem_cast<IVInterfaceGraphicsItem *>(
                        ifaceStart ? items.value(ifaceStart->id()) : nullptr);

                ivm::IVInterface *ifaceEnd = connection->targetInterface();
                if (ifaceEnd && ifaceEnd->isGrouped() && ifaceEnd->type() != ivm::IVObject::Type::InterfaceGroup) {
                    ifaceEnd = findGroupObject(ifaceEnd);
                }
                auto endItem =
                        qgraphicsitem_cast<IVInterfaceGraphicsItem *>(ifaceEnd ? items.value(ifaceEnd->id()) : nullptr);

                if (connection->type() == ivm::IVObject::Type::ConnectionGroup) {
                    const QList<QPointer<ivm::IVConnection>> groupedConnections =
                            connection->as<ivm::IVConnectionGroup *>()->groupedConnections();
                    if (std::any_of(groupedConnections.cbegin(), groupedConnections.cend(),
                                [&](const QPointer<ivm::IVConnection> &groupedConnection) {
                                    return !groupedConnection.isNull()
                                            && EndToEndConnections::isInDataflow(dataflow, chains, groupedConnection);
                                })) {
                        item = new IVFlowConnectionGraphicsItem(connection, startItem, endItem, parentItem);
                        foundConnection = true;
                    } else {
                        item = new IVConnectionGroupGraphicsItem(connection->as<ivm::IVConnectionGroup *>(),
                                qgraphicsitem_cast<IVInterfaceGroupGraphicsItem *>(startItem),
                                qgraphicsitem_cast<IVInterfaceGroupGraphicsItem *>(endItem), parentItem);
                    }
                } else if (!obj->isGrouped()) {
                    if (EndToEndConnections::isInDataflow(dataflow, chains, connection)) {
                        item = new IVFlowConnectionGraphicsItem(connection, startItem, endItem, parentItem);
                        foundConnection = true;
                    } else {
                        item = new IVConnectionGraphicsItem(connection, startItem, endItem, parentItem);
                    }
                }
            }
            break;
        case ivm::IVObject::Type::Function:
            item = new IVFunctionGraphicsItem(qobject_cast<ivm::IVFunction *>(obj), parentItem);
            if (obj->isRootObject()) {
                rootItem = item;
            }
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

    const shared::ColorHandler colorHandler =
            shared::ColorManager::instance()->colorsForItem(shared::ColorManager::HandledColors::ConnectionFlow);
    for (const auto &internalConnection : internalConnections) {
        if (internalConnection.pi != nullptr && internalConnection.ri != nullptr) {
            auto item = new QGraphicsPathItem;
            item->setPen(colorHandler.pen());
            QPainterPath path;
            path.moveTo(internalConnection.pi->scenePos());
            path.lineTo(internalConnection.ri->scenePos());
            item->setPath(path);
            item->setZValue(ZOrder.InternalConnection);
            d->scene->addItem(item);
        }
    }

    for (IVInterfaceGraphicsItem *ifItem : interfaceItems) {
        ifItem->updateLabel();
    }
    if (rootItem) {
        rootItem->instantLayoutUpdate();
    }

    // Set the scene rect based on what we show
    d->scene->setSceneRect(d->scene->itemsBoundingRect());

    return foundConnection || !internalConnections.isEmpty();
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
    if (d->model && d->model->parent() == this) {
        delete d->model;
        d->model = nullptr;
    }

    d->model = d->mscModelFetcher(fileName);
    if (d->model == nullptr) {
        msc::MscReader reader;
        try {
            d->model = reader.parseFile(fileName);
            d->model->setParent(this);
        } catch (...) {
            QMessageBox::warning(this, tr("Can't load msc file"),
                    tr("Can't load the msc file\n%1\n\n%2").arg(fileName, reader.getErrorMessages().join('\n')));
            return;
        }
    }

    d->document->setMscFileName(fileName);
    d->dataflow.setPath(fileName);
    QFileInfo info(fileName);
    d->ui->pathLabel->setText(tr("MSC file: %1").arg(info.fileName()));
    d->leafDocuments->fillModel(d->model->documents().first());
    d->ui->leafDocsView->setCurrentIndex(d->leafDocuments->index(0, 0));
}

}
