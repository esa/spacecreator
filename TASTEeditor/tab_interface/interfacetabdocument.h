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

#include "app/common.h"
#include "document/abstracttabdocument.h"

#include <QGraphicsView>
#include <QPointer>
#include <baseitems/graphicsview.h>

namespace taste3 {
class InterfaceTabGraphicsScene;

namespace aadl {
class CreatorTool;
class AADLObject;
class AADLObjectsModel;

class AADLCommentGraphicsItem;
class AADLConnectionGraphicsItem;
class AADLFunctionGraphicsItem;
class AADLFunctionTypeGraphicsItem;
class AADLInterfaceGraphicsItem;

} // namespace aadl

namespace document {

class InterfaceTabDocument : public AbstractTabDocument
{
    Q_OBJECT
public:
    explicit InterfaceTabDocument(QObject *parent = nullptr);
    ~InterfaceTabDocument() override;

    QString title() const override;
    QMenu *customMenu() const override;

    const QHash<common::Id, aadl::AADLObject *> &objects() const;
    QString supportedFileExtensions() const override;

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
    void onItemClicked();
    void onItemDoubleClicked();
    void onRootObjectChanged(common::Id rootId);

    void onAttributesManagerRequested();
    void onDataTypesMenuInvoked();
    void onColorSchemeMenuInvoked();
    void onDynContextEditorMenuInvoked();

    void showPropertyEditor(aadl::AADLObject *obj);

private:
    QGraphicsItem *createItemForObject(aadl::AADLObject *obj);
    aadl::AADLFunctionGraphicsItem *rootItem() const;
    void updateItem(QGraphicsItem *item);

    void updateComment(aadl::AADLCommentGraphicsItem *comment);
    void updateInterface(aadl::AADLInterfaceGraphicsItem *iface);
    void updateFunction(aadl::AADLFunctionGraphicsItem *function);
    void updateFunctionType(aadl::AADLFunctionTypeGraphicsItem *functionType);
    void updateConnection(aadl::AADLConnectionGraphicsItem *connection);

    void clearScene();
    void changeRootItem(common::Id id);

private:
    InterfaceTabGraphicsScene *m_graphicsScene { nullptr };
    QPointer<GraphicsView> m_graphicsView { nullptr };
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

    aadl::CreatorTool *m_tool { nullptr };
    QHash<common::Id, QGraphicsItem *> m_items;

    void showNIYGUI(const QString &title = QString());
};

} // ns document
} // ns taste3
