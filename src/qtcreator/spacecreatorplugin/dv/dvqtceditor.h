/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QPointer>
#include <QSharedPointer>
#include <editormanager/ieditor.h>

class QToolBar;

namespace dve {
class DVEditorCore;
}

namespace spctr {

class DVEditorDocument;
class DVMainWidget;
class SpaceCreatorProjectManager;

class DVQtCEditor : public Core::IEditor
{
    Q_OBJECT

public:
    DVQtCEditor(SpaceCreatorProjectManager *projectManager, const QList<QAction *> &dvActions);
    ~DVQtCEditor();

    Core::IDocument *document() const override;
    DVEditorDocument *dvDocument() const;
    QSharedPointer<dve::DVEditorCore> dvPlugin() const;

    QWidget *toolBar() override;

public Q_SLOTS:
    void showAsn1Dialog();

private:
    DVEditorDocument *m_document = nullptr;
    QPointer<QToolBar> m_toolbar = nullptr;
    DVMainWidget *m_editorWidget = nullptr;
    QPointer<SpaceCreatorProjectManager> m_projectManager;
    QList<QAction *> m_globalToolbarActions;
};

}
