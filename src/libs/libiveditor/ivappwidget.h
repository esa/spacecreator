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
#include <QVector>
#include <QWidget>

namespace Ui {
class IVAppWidget;
}

namespace ive {
class GraphicsView;
class InterfaceDocument;
class IVCreatorTool;

/*!
   The main widget including all of the IV editor UI
 */
class IVAppWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IVAppWidget(InterfaceDocument *doc, QWidget *parent = nullptr);
    ~IVAppWidget();

    GraphicsView *graphicsView() const;

    void centerView();

private Q_SLOTS:
    void showContextMenuForIVModel(const QPoint &pos);
    void copyItems();
    void cutItems();
    void pasteItems();
    void pasteItems(const QPointF &sceneDropPoint);
    void showPropertyEditor(const shared::Id &id);
    void importEntity(const shared::Id &id, const QPointF &sceneDropPoint);
    void instantiateEntity(const shared::Id &id, const QPointF &sceneDropPoint);

    void onItemDoubleClicked(const shared::Id &id);
    void onItemCreated(const shared::Id &id);

private:
    void initGraphicsView();
    void initModelView();
    void initImportView();
    void initSharedView();

    QVector<QAction *> initActions();

    Ui::IVAppWidget *ui = nullptr;
    IVCreatorTool *m_tool { nullptr };
    QAction *m_actCreateConnectionGroup { nullptr };
    QAction *m_actRemove { nullptr };
    QAction *m_actZoomIn { nullptr };
    QAction *m_actZoomOut { nullptr };
    QAction *m_actExitToRoot { nullptr };
    QAction *m_actExitToParent { nullptr };
    QVector<QAction *> m_toolbarActions;

    QPointer<InterfaceDocument> m_document;
};

} // namespace ive
