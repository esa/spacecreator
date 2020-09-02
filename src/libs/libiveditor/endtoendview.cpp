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
#include <QPushButton>

namespace aadlinterface {

struct EndToEndView::EndToEndViewPrivate {
    shared::ui::GraphicsViewBase *view { nullptr };
    QGraphicsScene *scene { nullptr };

    InterfaceDocument *document { nullptr };

    EndToEndConnections dataflow;
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

    d->view = new shared::ui::GraphicsViewBase;
    d->view->setInteractive(false);
    d->scene = new QGraphicsScene(this);
    d->view->setScene(d->scene);

    d->document = document;

    auto barLayout = new QHBoxLayout;
    barLayout->addWidget(pathLabel);
    barLayout->addWidget(pathButton);
    barLayout->addWidget(refreshButton);
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

    // Listen to path changes from the document
    connect(d->document, &InterfaceDocument::mscFileNameChanged, this, setPath);

    // Refresh the view
    connect(refreshButton, &QPushButton::clicked, this, &EndToEndView::refreshView);
}

EndToEndView::~EndToEndView()
{
    delete d;
    d = nullptr;
}

//! This emits visibleChanged after calling the standard setVisible method.
//! This is used to keep the "show e2e" menu action updated
void EndToEndView::setVisible(bool visible)
{
    const bool wasVisible = isVisible();
    QWidget::setVisible(visible);
    if (isVisible() != wasVisible) {
        Q_EMIT visibleChanged(visible);
    }
}

//! Update the view with the current model and MSC file contents
void EndToEndView::refreshView()
{
    struct InternalConnection {
        InternalConnection(const EndToEndConnections::ConnectionInsideFunction &c)
            : connection(c)
        {
        }

        EndToEndConnections::ConnectionInsideFunction connection;
        AADLInterfaceGraphicsItem *ri { nullptr };
        AADLInterfaceGraphicsItem *pi { nullptr };
    };
    QVector<InternalConnection> internalConnections;

    // We need both the ri and the pi graphics item to be able to create a connection between them
    auto updateInternalConnection = [&](const QString &instance, const QString &interface,
                                            AADLInterfaceGraphicsItem *ri, AADLInterfaceGraphicsItem *pi) {
        for (auto &ic : internalConnections) {
            if (instance == ic.connection.instance) {
                if (pi != nullptr && interface == ic.connection.interface1) {
                    // This is the ri side
                    ic.pi = pi;
                    return;
                } else if (ri != nullptr && interface == ic.connection.interface2) {
                    ic.ri = ri;
                    return;
                }
            }
        }
    };

    // Remove all the old ones
    qDeleteAll(d->scene->items());

    // Get the visible non-nested objects
    QList<aadl::AADLObject *> objects = d->document->objectsModel()->visibleObjects({});
    aadl::AADLObject::sortObjectList(objects);

    const EndToEndConnections::Dataflow dataflow = d->dataflow.dataflow();
    for (auto c : dataflow.internalConnections) {
        internalConnections << InternalConnection(c);
    };

    // Add new graphics items for each object
    QHash<shared::Id, QGraphicsItem *> items;
    for (auto obj : objects) {
        QGraphicsItem *parentItem = obj->parentObject() ? items.value(obj->parentObject()->id()) : nullptr;

        InteractiveObject *item = nullptr;
        switch (obj->aadlType()) {
        case aadl::AADLObject::Type::RequiredInterface:
            if (auto ri = qobject_cast<aadl::AADLObjectIface *>(obj)) {
                // Add the RI
                auto graphicsItem = new AADLInterfaceGraphicsItem(ri, parentItem);
                item = graphicsItem;

                auto function = ri->function();
                if (function != nullptr) {
                    // Check if this is to the environment
                    const EndToEndConnections::ConnectionWithEnvironment c { function->title(), ri->title(), false };
                    qDebug() << "Env" << c.instance << c.interface << c.toInstance;
                    if (dataflow.envConnections.contains(c)) {
                        // TODO: Add a connection from the outside
                    }

                    // Check if this is part of an internal connection
                    updateInternalConnection(c.instance, c.interface, graphicsItem, nullptr);
                }
            }
            break;
        case aadl::AADLObject::Type::ProvidedInterface:
            item = new AADLInterfaceGraphicsItem(qobject_cast<aadl::AADLObjectIface *>(obj), parentItem);
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
}

}
