#include "endtoendview.h"

#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "endtoendconnections.h"
#include "interface/aadlconnectiongraphicsitem.h"
#include "interface/aadlfunctiongraphicsitem.h"
#include "interface/aadlinterfacegraphicsitem.h"
#include "interface/interfacedocument.h"
#include "ui/graphicsviewbase.h"

#include <QBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

namespace aadlinterface {

struct EndToEndView::EndToEndViewPrivate {
    shared::ui::GraphicsViewBase *view { nullptr };
    QGraphicsScene *scene { nullptr };

    InterfaceDocument *document { nullptr };

    EndToEndConnections dataflow;

    QString lastExportPath;
};

//! Initialize this with the current document
EndToEndView::EndToEndView(InterfaceDocument *document, QWidget *parent)
    : QDialog(parent)
    , d(new EndToEndViewPrivate)
{
    setWindowTitle(tr("End To End Dataflow"));
    setAttribute(Qt::WA_DeleteOnClose, false);
    setAttribute(Qt::WA_QuitOnClose, false);

    auto pathLabel = new QLabel;
    auto pathButton = new QPushButton(tr("&Choose MSC file"));
    auto refreshButton = new QPushButton(tr("&Refresh view"));
    auto exportButton = new QPushButton(tr("&Save as PNG"));

    d->view = new shared::ui::GraphicsViewBase;
    d->view->setInteractive(false);
    d->scene = new QGraphicsScene(this);
    d->view->setScene(d->scene);
    d->document = document;

    auto barLayout = new QHBoxLayout;
    barLayout->addWidget(pathLabel);
    barLayout->addWidget(pathButton);
    barLayout->addWidget(refreshButton);
    barLayout->addWidget(exportButton);
    barLayout->addStretch(1);
    auto layout = new QVBoxLayout(this);
    layout->addLayout(barLayout);
    layout->addWidget(d->view);

    auto setPath = [this, pathLabel](const QString &path) {
        d->document->setMscFileName(path);
        d->dataflow.setPath(path);
        QFileInfo info(path);
        if (info.exists()) {
            pathLabel->setText(tr("MSC file: %1").arg(info.fileName()));
        } else {
            pathLabel->setText(tr("MSC file: -"));
        }
    };

    connect(pathButton, &QPushButton::clicked, this, [this, setPath]() {
        QFileInfo fi(d->document->mscFileName());
        const QString dir = fi.path();
        const QString path =
                QFileDialog::getOpenFileName(this, tr("Choose MSC file"), dir, tr("MSC files (*.msc);;All files (*)"));
        if (!path.isEmpty()) {
            setPath(path);
            refreshView();
        }
    });

    connect(exportButton, &QPushButton::clicked, this, &EndToEndView::exportToPng);

    // Listen to path changes from the document
    connect(d->document, &InterfaceDocument::mscFileNameChanged, this, setPath);

    // Refresh the view
    connect(refreshButton, &QPushButton::clicked, this, &EndToEndView::refreshView);

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

    const EndToEndConnections::Dataflow dataflow = d->dataflow.dataflow();
    for (auto c : dataflow.internalConnections) {
        internalConnections << InternalConnection { c };
    };

    // This is used to draw our extra connections
    QPen connectionPen;

    // Add new graphics items for each object
    QHash<shared::Id, QGraphicsItem *> items;
    for (auto obj : objects) {
        QGraphicsItem *parentItem = obj->parentObject() ? items.value(obj->parentObject()->id()) : nullptr;

        InteractiveObject *item = nullptr;
        switch (obj->aadlType()) {
        case aadl::AADLObject::Type::RequiredInterface:
            if (auto ri = qobject_cast<aadl::AADLObjectIfaceRequired *>(obj)) {
                // Add the RI
                auto graphicsItem = new AADLInterfaceGraphicsItem(ri, parentItem);
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
            break;
        case aadl::AADLObject::Type::ProvidedInterface:
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
            break;
        case aadl::AADLObject::Type::Connection:
            if (auto connection = qobject_cast<aadl::AADLObjectConnection *>(obj)) {
                aadl::AADLObjectIface *ifaceStart = connection->sourceInterface();
                auto startItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                        ifaceStart ? items.value(ifaceStart->id()) : nullptr);

                aadl::AADLObjectIface *ifaceEnd = connection->targetInterface();
                auto endItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                        ifaceEnd ? items.value(ifaceEnd->id()) : nullptr);

                auto i = new AADLConnectionGraphicsItem(connection, startItem, endItem, parentItem);
                item = i;
                if (EndToEndConnections::isInDataflow(dataflow, connection)) {
                    i->setEndToEndDataFlowConnection();
                }

                connectionPen = item->pen();
            }
            break;
        case aadl::AADLObject::Type::Function:
            item = new AADLFunctionGraphicsItem(qobject_cast<aadl::AADLObjectFunction *>(obj), parentItem);
            break;
        default:
            break;
        }

        if (item != nullptr) {
            items.insert(obj->id(), item);
            if (item->parentItem() == nullptr) {
                // Only items without a parent should be added to the scene or we get a warning
                d->scene->addItem(item);
            }
            item->updateFromEntity();
        }
    }

    connectionPen.setWidth(5);
    for (auto ic : internalConnections) {
        if (ic.pi != nullptr && ic.ri != nullptr) {
            auto item = new QGraphicsPathItem;
            item->setPen(connectionPen);
            QPainterPath path;
            path.moveTo(ic.pi->scenePos());
            path.lineTo(ic.ri->scenePos());
            item->setPath(path);
            d->scene->addItem(item);
        }
    }

    // Set the scene rect based on what we show
    d->scene->setSceneRect(d->scene->itemsBoundingRect());
}

void EndToEndView::exportToPng()
{
    const QString path = QFileDialog::getSaveFileName(
            parentWidget(), tr("Save as PNG"), d->lastExportPath, tr("PNG Images (*.png)"));
    if (!path.isEmpty()) {
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

}
