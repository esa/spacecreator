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

#include "dvqtceditor.h"

#include "dveditordocument.h"
#include "dvmainwidget.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorprojectmanager.h"

#include <QToolBar>

namespace spctr {

DVQtCEditor::DVQtCEditor(SpaceCreatorProjectManager *projectManager)
    : QtCEditor()
    , m_document(new DVEditorDocument(projectManager, this))
    , m_editorWidget(new DVMainWidget(projectManager))
    , m_projectManager(projectManager)
{
    setContext(Core::Context(spctr::Constants::K_DV_EDITOR_ID));
    setWidget(m_editorWidget);

    connect(m_document, &spctr::DVEditorDocument::dvDataLoaded, this,
            [this](const QString &, DVEditorCorePtr data) { m_editorWidget->init(data); });
}

DVQtCEditor::~DVQtCEditor()
{
    m_editorWidget->deleteLater();
}

Core::IDocument *DVQtCEditor::document() const
{
    return m_document;
}

DVEditorDocument *DVQtCEditor::dvDocument() const
{
    return m_document;
}

DVEditorCorePtr DVQtCEditor::dvPlugin() const
{
    return m_document->dvEditorCore();
}

QWidget *DVQtCEditor::toolBar()
{
    DVEditorCorePtr dvCore = m_document->dvEditorCore();
    if (m_toolbar == nullptr && !dvCore.isNull()) {
        m_toolbar = new QToolBar;
        m_toolbar->addAction(dvCore->actionUndo());
        m_toolbar->addAction(dvCore->actionRedo());
    }

    return m_toolbar;
}

shared::EditorCore *DVQtCEditor::editorCore() const
{
    return m_document->dvEditorCore().get();
}

}
