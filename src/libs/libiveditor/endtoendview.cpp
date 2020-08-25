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
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QLabel>
#include <QPushButton>

namespace aadlinterface {

struct EndToEndView::EndToEndViewPrivate {
    shared::ui::GraphicsViewBase *view { nullptr };
    QGraphicsScene *scene { nullptr };
    QHash<shared::Id, QGraphicsItem *> items;

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
    // Remove all the old ones
    qDeleteAll(d->items.values());
    d->items.clear();

    // Get the visible non-nested objects
    QList<aadl::AADLObject *> objects = d->document->objectsModel()->visibleObjects({});
    aadl::AADLObject::sortObjectList(objects);

    QVector<QPair<QString, QString>> dataflow = d->dataflow.dataflow();

    // Add new graphics items for each object
    for (auto obj : objects) {
        QGraphicsItem *parentItem = obj->parentObject() ? d->items.value(obj->parentObject()->id()) : nullptr;

        InteractiveObject *item = nullptr;
        switch (obj->aadlType()) {
        case aadl::AADLObject::Type::RequiredInterface:
        case aadl::AADLObject::Type::ProvidedInterface:
            item = new AADLInterfaceGraphicsItem(qobject_cast<aadl::AADLObjectIface *>(obj), parentItem);
            break;
        case aadl::AADLObject::Type::Connection:
            if (auto connection = qobject_cast<aadl::AADLObjectConnection *>(obj)) {
                aadl::AADLObjectIface *ifaceStart = connection->sourceInterface();
                auto startItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                        ifaceStart ? d->items.value(ifaceStart->id()) : nullptr);

                aadl::AADLObjectIface *ifaceEnd = connection->targetInterface();
                auto endItem = qgraphicsitem_cast<AADLInterfaceGraphicsItem *>(
                        ifaceEnd ? d->items.value(ifaceEnd->id()) : nullptr);

                auto i = new AADLConnectionGraphicsItem(connection, startItem, endItem, parentItem);
                item = i;
                if (/* TODO: item is on path */ true) {
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
            d->items.insert(obj->id(), item);
            if (item->parentItem() == nullptr) {
                // Only items without a parent should be added to the scene or we get a warning
                d->scene->addItem(item);
            }
            item->updateFromEntity();
        }
    }
}

}
