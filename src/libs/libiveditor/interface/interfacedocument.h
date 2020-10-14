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
#include <QVariantMap>

class QMenu;
class QToolBar;
class QUndoStack;
class QAbstractItemView;
class QTreeView;

namespace Asn1Acn {
class Asn1ModelStorage;
}

namespace aadl {
class AADLObject;
class AADLObjectsModel;
}

namespace aadlinterface {

class AADLCommentGraphicsItem;
class AADLConnectionGraphicsItem;
class AADLFunctionGraphicsItem;
class AADLFunctionTypeGraphicsItem;
class AADLInterfaceGraphicsItem;
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

    QGraphicsScene *scene() const;
    QWidget *view() const;
    QTreeView *modelView() const;
    QUndoStack *commandsStack() const;

    bool create(const QString &path = QString());
    bool load(const QString &path);
    bool import();
    bool exportSelected();
    bool save(const QString &path);
    void close();

    QString path() const;

    void setAsn1FileName(const QString &asnfile);
    QString asn1FileName() const;
    QString asn1FilePath() const;

    void setMscFileName(const QString &mscFile);
    const QString &mscFileName() const;

    bool isDirty() const;

    QString title() const;

    QList<QAction *> customActions() const;

    const QHash<shared::Id, aadl::AADLObject *> &objects() const;
    aadl::AADLObjectsModel *objectsModel() const;
    aadl::AADLObjectsModel *importModel() const;

    Asn1Acn::Asn1ModelStorage *asn1DataTypes() const;

    QString supportedFileExtensions() const;

Q_SIGNALS:
    void dirtyChanged(bool dirty);
    void titleChanged();

    void mscFileNameChanged(const QString &mscFileName);

public Q_SLOTS:
    void onSavedExternally(const QString &filePath, bool saved);
    void setObjects(const QVector<aadl::AADLObject *> &objects);
    void onAttributesManagerRequested();
    void onColorSchemeMenuInvoked();
    void onDynContextEditorMenuInvoked();

private Q_SLOTS:
    void onItemClicked(shared::Id id);
    void onItemDoubleClicked(shared::Id id);

    void onDataTypesMenuInvoked();
    void showPropertyEditor(aadl::AADLObject *obj);
    void showInfoMessage(const QString &title, const QString &message);

private:
    void setPath(const QString &path);

    bool loadImpl(const QString &path);
    bool importImpl(const QString &path);

    QWidget *createView();
    QTreeView *createModelView();
    QTreeView *createImportView();

    void showNIYGUI(const QString &title = QString());

    struct InterfaceDocumentPrivate;
    InterfaceDocumentPrivate *d;
};

}
