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
#include "itemeditor/graphicsview.h"

#include <QPointer>
#include <QQueue>
#include <QVariantMap>

class QMenu;
class QToolBar;
class QUndoStack;
class QAbstractItemView;
class QTreeView;
class QItemSelection;

namespace Asn1Acn {
class Asn1ModelStorage;
class Asn1SystemChecks;
}

namespace shared {
class VEObject;
}

namespace ivm {
class IVObject;
class IVInterface;
class IVModel;
}

namespace ive {
namespace cmd {
class CommandsStack;
}

class IVExporter;
class IVItemModel;
class IVCommentGraphicsItem;
class IVConnectionGraphicsItem;
class IVFunctionGraphicsItem;
class IVFunctionTypeGraphicsItem;
class IVInterfaceGraphicsItem;
class InterfaceTabGraphicsScene;

class InterfaceDocument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString mscFileName READ mscFileName WRITE setMscFileName NOTIFY mscFileNameChanged)

public:
    explicit InterfaceDocument(QObject *parent = nullptr);
    ~InterfaceDocument() override;

    void init();
    QVector<QAction *> initActions();

    void fillToolBar(QToolBar *toolBar);

    IVExporter *exporter() const;

    QGraphicsScene *scene() const;
    shared::ui::GraphicsViewBase *graphicsView() const;
    QWidget *view() const;

    QUndoStack *undoStack() const;
    cmd::CommandsStack *commandsStack() const;

    bool create(const QString &path = QString());
    bool load(const QString &path);
    bool loadAvailableComponents();
    bool exportSelectedFunctions();
    bool exportSelectedType();
    void close();

    QString path() const;
    void setPath(const QString &path);

    void setAsn1FileName(const QString &asnfile);
    QString asn1FileName() const;
    QString asn1FilePath() const;

    void setMscFileName(const QString &mscFile);
    const QString &mscFileName() const;
    QString mscFilePath() const;

    bool isDirty() const;

    QString title() const;

    QList<QAction *> customActions() const;

    QHash<shared::Id, shared::VEObject *> objects() const;
    ivm::IVModel *objectsModel() const;
    ivm::IVModel *importModel() const;
    IVItemModel *itemsModel() const;

    void setAsn1Check(Asn1Acn::Asn1SystemChecks *check);

    QString supportedFileExtensions() const;

    bool checkInterfaceAsn1Compliance(const ivm::IVInterface *interface) const;
    bool checkAllInterfacesForAsn1Compliance();

Q_SIGNALS:
    void dirtyChanged(bool dirty);
    void titleChanged();

    void asn1FileNameChanged(const QString &asn1FileName);
    void mscFileNameChanged(const QString &mscFileName);

public Q_SLOTS:
    void onSavedExternally(const QString &filePath, bool saved);
    void setObjects(const QVector<ivm::IVObject *> &objects);
    void onAttributesManagerRequested();
    void onColorSchemeMenuInvoked();
    void onDynContextEditorMenuInvoked();

private Q_SLOTS:
    void onItemClicked(const shared::Id &id);
    void onItemDoubleClicked(const shared::Id &id);
    void onItemCreated(const shared::Id &id);

    void onDataTypesMenuInvoked();
    void prepareEntityNameForEditing(const shared::Id &id);
    void showPropertyEditor(const shared::Id &id);
    void showInfoMessage(const QString &title, const QString &message);
    void importEntity(const shared::Id &id, const QPointF &sceneDropPoint);
    void instantiateEntity(const shared::Id &id, const QPointF &sceneDropPoint);

    void copyItems();
    void cutItems();
    void pasteItems();
    void pasteItems(const QPointF &sceneDropPoint);

    void showContextMenuForIVModel(const QPoint &pos);

private:
    bool exportImpl(const QString &path, const QList<shared::VEObject *> &objects);
    bool loadImpl(const QString &path);
    QString getComponentName(const QStringList &exportNames);
    QList<shared::VEObject *> prepareSelectedObjectsForExport(QString &name, bool silent = false);
    bool loadComponentModel(ivm::IVModel *model, const QString &path);

    QWidget *createGraphicsView();
    QTreeView *createModelView();
    QTreeView *createImportView();
    QTreeView *createSharedView();

    void onSceneSelectionChanged(const QList<shared::Id> &selectedObjects);
    void onViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void showNIYGUI(const QString &title = QString());

    QWidget *window();

    struct InterfaceDocumentPrivate;
    InterfaceDocumentPrivate *d;
};

}
