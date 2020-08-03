#include "endtoendview.h"

#include "aadlobjectconnection.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "interface/aadlconnectiongraphicsitem.h"
#include "interface/aadlfunctiongraphicsitem.h"
#include "interface/aadlinterfacegraphicsitem.h"
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

    aadl::AADLObjectsModel *objectsModel { nullptr };

    QString mscFilePath;
    QString dir;
};

EndToEndView::EndToEndView(aadl::AADLObjectsModel *objectsModel, QWidget *parent)
    : QDialog(parent)
    , d(new EndToEndViewPrivate)
{
    setWindowTitle(tr("End To End Dataflow"));
    setAttribute(Qt::WA_DeleteOnClose, false);
    setAttribute(Qt::WA_QuitOnClose, false);

    auto pathLabel = new QLabel(tr("MSC file: -"));
    auto pathButton = new QPushButton(tr("&Choose MSC file"));
    auto refreshButton = new QPushButton(tr("&Refresh view"));

    d->view = new shared::ui::GraphicsViewBase;
    d->view->setInteractive(false);
    d->scene = new QGraphicsScene(this);
    d->view->setScene(d->scene);

    d->objectsModel = objectsModel;

    auto barLayout = new QHBoxLayout;
    barLayout->addWidget(pathLabel);
    barLayout->addWidget(pathButton);
    barLayout->addWidget(refreshButton);
    barLayout->addStretch(1);
    auto layout = new QVBoxLayout(this);
    layout->addLayout(barLayout);
    layout->addWidget(d->view);

    connect(pathButton, &QPushButton::clicked, this, [=]() {
        const QString path = QFileDialog::getOpenFileName(
                this, tr("Choose MSC file"), d->dir, tr("MSC files (*.msc);;All files (*)"));
        if (!path.isEmpty()) {
            d->mscFilePath = path;

            QFileInfo info(path);
            pathLabel->setText(tr("MSC file: %1").arg(info.fileName()));
            d->dir = info.path();

            refreshButton->click();
        }
    });

    connect(refreshButton, &QPushButton::clicked, this, &EndToEndView::refreshView);
}

EndToEndView::~EndToEndView()
{
    delete d;
    d = nullptr;
}

void EndToEndView::setVisible(bool visible)
{
    const bool wasVisible = isVisible();
    QWidget::setVisible(visible);
    if (isVisible() != wasVisible) {
        Q_EMIT visibleChanged(visible);
    }
}

void EndToEndView::refreshView()
{
    // Remove all the old ones
    qDeleteAll(d->items.values());
    d->items.clear();

    // Get the visible non-nested objects
    QList<aadl::AADLObject *> objects = d->objectsModel->visibleObjects({});
    aadl::AADLObject::sortObjectList(objects);

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
            d->scene->addItem(item);
            item->updateFromEntity();
        }
    }
}

}
