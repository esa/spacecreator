/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#pragma once

#include "common.h"
#include "document/abstracttabdocument.h"

#include <QPointer>
#include <QQueue>
#include <baseitems/graphicsview.h>

class QAction;
class QActionGroup;
class QMutex;

namespace aadl {
class AADLObject;
class AADLObjectsModel;
}

namespace aadlinterface {
class CreatorTool;
class AADLCommentGraphicsItem;
class AADLConnectionGraphicsItem;
class AADLFunctionGraphicsItem;
class AADLFunctionTypeGraphicsItem;
class AADLInterfaceGraphicsItem;
class InterfaceTabGraphicsScene;
}

namespace document {

class InterfaceTabDocument : public AbstractTabDocument
{
    Q_OBJECT
public:
    explicit InterfaceTabDocument(QObject *parent = nullptr);
    ~InterfaceTabDocument() override;

    QString title() const override;
    QMenu *customMenu() const override;

    const QHash<shared::Id, aadl::AADLObject *> &objects() const;
    QString supportedFileExtensions() const override;

public Q_SLOTS:
    void setObjects(const QVector<aadl::AADLObject *> &objects);

protected:
    bool createImpl(const QString &path = QString()) override;
    bool loadImpl(const QString &path) override;
    bool saveImpl(const QString &path) override;
    void closeImpl() override;

    QVector<QAction *> initActions() override;

    QWidget *createView() override;
    QGraphicsScene *createScene() override;

protected Q_SLOTS:
    void onActionCreateFunctionType();
    void onActionCreateFunction();
    void onActionCreateProvidedInterface();
    void onActionCreateRequiredInterface();
    void onActionCreateComment();
    void onActionGroupConnections();
    void onActionCreateConnection();
    void onActionRemoveItem();
    void onActionZoomIn();
    void onActionZoomOut();
    void onActionExitToRootFunction();
    void onActionExitToParentFunction();

    void onAADLObjectAdded(aadl::AADLObject *object);
    void onAADLObjectRemoved(aadl::AADLObject *object);
    void onItemClicked();
    void onItemDoubleClicked();
    void onRootObjectChanged(shared::Id rootId);

    void onAttributesManagerRequested();
    void onDataTypesMenuInvoked();
    void onColorSchemeMenuInvoked();
    void onDynContextEditorMenuInvoked();

    void showPropertyEditor(aadl::AADLObject *obj);

    void showInfoMessage(const QString &title, const QString &message);

private:
    QGraphicsItem *createItemForObject(aadl::AADLObject *obj);
    aadlinterface::AADLFunctionGraphicsItem *rootItem() const;
    void updateItem(QGraphicsItem *item);

    void updateComment(aadlinterface::AADLCommentGraphicsItem *comment);
    void updateInterface(aadlinterface::AADLInterfaceGraphicsItem *iface);
    void updateFunction(aadlinterface::AADLFunctionGraphicsItem *function);
    void updateFunctionType(aadlinterface::AADLFunctionTypeGraphicsItem *functionType);
    void updateConnection(aadlinterface::AADLConnectionGraphicsItem *connection);

    void clearScene();
    void changeRootItem(shared::Id id);

private:
    aadlinterface::InterfaceTabGraphicsScene *m_graphicsScene { nullptr };
    QPointer<aadlinterface::GraphicsView> m_graphicsView { nullptr };
    aadl::AADLObjectsModel *m_model { nullptr };
    QActionGroup *m_actionGroup { nullptr };
    QAction *m_actCreateFunctionType { nullptr };
    QAction *m_actCreateFunction { nullptr };
    QAction *m_actCreateProvidedInterface { nullptr };
    QAction *m_actCreateRequiredInterface { nullptr };
    QAction *m_actCreateComment { nullptr };
    QAction *m_actGroupConnections { nullptr };
    QAction *m_actCreateConnection { nullptr };
    QAction *m_actRemove { nullptr };
    QAction *m_actZoomIn { nullptr };
    QAction *m_actZoomOut { nullptr };
    QAction *m_actExitToRoot { nullptr };
    QAction *m_actExitToParent { nullptr };

    aadlinterface::CreatorTool *m_tool { nullptr };
    QHash<shared::Id, QGraphicsItem *> m_items;

    QMutex *m_mutex { nullptr };
    QQueue<aadl::AADLObject *> m_rmQueu;
    const QRectF m_desktopGeometry;
    QRectF m_prevItemsRect;

    void showNIYGUI(const QString &title = QString());
    void updateSceneRect();
};

}
