/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QAction>
#include <QPointer>
#include <QStandardItem>
#include <QVector>
#include <QWidget>

namespace shared {
class BreadcrumbWidget;
class VEObject;
}

namespace Ui {
class IVAppWidget;
}

namespace ivm {
class IVObject;
class IVFunctionType;
}

namespace ive {

class IVImporter;
class GraphicsView;
class InterfaceDocument;
class IVCreatorTool;
class IVEditorCore;

/*!
   The main widget including all of the IV editor UI
 */
class IVAppWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IVAppWidget(IVEditorCore *core, QWidget *parent = nullptr);
    ~IVAppWidget();

    GraphicsView *graphicsView() const;

    void centerView();

    QAction *actionDelete() const;

public Q_SLOTS:
    void showArchetypeManager();

private Q_SLOTS:
    void showContextMenuForSharedTypesView(const QPoint &pos);
    void showContextMenuForComponentsLibraryView(const QPoint &pos);
    void showContextMenuForIVModel(const QPoint &pos);
    void showAvailableLayers(const QPoint &pos);
    void renameSelectedLayer(QStandardItem *item);
    void copyItems();
    void cutItems();
    void wrapItems();
    void exportToClipboard(const QList<ivm::IVObject *> &objects, QMimeData *mimeData = nullptr);
    void showPropertyEditor(const shared::Id &id);
    void showEditAttributesDialog();

    void enterNestedView(const shared::Id &id);
    void onItemDoubleClicked(const shared::Id &id);
    void onItemCreated(const shared::Id &id);
    void onRootObjectChanged(const shared::Id &rootId);
    void checkActionsFromSelection();
    void editExternalEntity(const QString &path);
    void hideSelectedRows();
    void hideUnselectedRows();
    void showAllRows();
    void showSelectedRows();

private:
    void initGraphicsView();
    void initModelView();
    void initImportView();
    void initSharedView();
    void initLayerView();

    QVector<QAction *> initActions();
    QVector<QAction *> initViewActions();

    QList<ivm::IVObject *> selectedObjects() const;

    ivm::IVFunctionType *functionAtPosition(QPointF &sceneDropPoint);
    Ui::IVAppWidget *ui = nullptr;
    shared::BreadcrumbWidget *m_breadCrumb = nullptr;
    IVCreatorTool *m_tool { nullptr };
    QAction *m_actCreateConnectionGroup { nullptr };
    QAction *m_actUngroupConnection { nullptr };
    QAction *m_actDeleteConnectionGroup { nullptr };
    QAction *m_actEditAttributes { nullptr };
    QAction *m_actRemove { nullptr };
    QAction *m_actZoomIn { nullptr };
    QAction *m_actZoomOut { nullptr };
    QAction *m_actExitToRoot { nullptr };
    QAction *m_actExitToParent { nullptr };
    QAction *m_actEnterNestedView { nullptr };
    QAction *m_actShrinkScene { nullptr };

    QVector<QAction *> m_toolbarActions;
    QVector<QAction *> m_viewActions;

    QPointer<IVEditorCore> m_ivCore;
    QPointer<InterfaceDocument> m_document;
    IVImporter *m_importer;
};

} // namespace ive
