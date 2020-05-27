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

#include "baseitems/graphicsview.h"
#include "common.h"

#include <QPointer>
#include <QQueue>

class QMenu;
class QToolBar;
class QUndoStack;

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

class InterfaceDocument : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceDocument(QObject *parent = nullptr);
    ~InterfaceDocument() override;

    void init();

    void fillToolBar(QToolBar *toolBar);

    QGraphicsScene *scene() const;
    QWidget *view() const;
    QUndoStack *commandsStack() const;

    bool create(const QString &path = QString());
    bool load(const QString &path);
    bool save(const QString &path);
    void close();

    QString path() const;

    bool isDirty() const;

    QString title() const;

    QMenu *customMenu() const;

    const QHash<shared::Id, aadl::AADLObject *> &objects() const;
    QString supportedFileExtensions() const;

Q_SIGNALS:
    void dirtyChanged(bool dirty);
    void titleChanged();

public Q_SLOTS:
    void onSavedExternally(const QString &filePath, bool saved);
    void setObjects(const QVector<aadl::AADLObject *> &objects);

private Q_SLOTS:
    void updateDirtyness();
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
    void setPath(const QString &path);

    void resetDirtyness();

    bool loadImpl(const QString &path);

    QVector<QAction *> initActions();

    QWidget *createView();
    QGraphicsScene *createScene();

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

    void showNIYGUI(const QString &title = QString());
    void updateSceneRect();

    struct InterfaceDocumentPrivate;
    InterfaceDocumentPrivate *d;
};

}
