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

#include "asn1dialog.h"
#include "commandsstack.h"
#include "dveditorcore.h"
#include "dveditordocument.h"
#include "dvmainwidget.h"
#include "mainmodel.h"
#include "msceditorcore.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorprojectmanager.h"

#include <QFileInfo>
#include <QToolBar>
#include <QUndoCommand>
#include <utils/qtcassert.h>

namespace spctr {

DVQtCEditor::DVQtCEditor(SpaceCreatorProjectManager *projectManager, const QList<QAction *> &dvActions)
    : Core::IEditor()
    , m_document(new DVEditorDocument(projectManager, this))
    , m_editorWidget(new DVMainWidget)
    , m_projectManager(projectManager)
    , m_globalToolbarActions(dvActions)
{
    setContext(Core::Context(spctr::Constants::K_DV_EDITOR_ID));
    setWidget(m_editorWidget);

    connect(m_document, &spctr::DVEditorDocument::dvDataLoaded, this,
            [this](const QString &, QSharedPointer<dve::DVEditorCore> data) { m_editorWidget->init(data); });
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

QSharedPointer<dve::DVEditorCore> DVQtCEditor::dvPlugin() const
{
    return m_document->dvEditorCore();
}

QWidget *DVQtCEditor::toolBar()
{
    QSharedPointer<dve::DVEditorCore> dvCore = m_document->dvEditorCore();
    if (m_toolbar == nullptr && !dvCore.isNull()) {
        m_toolbar = new QToolBar;
        m_toolbar->addAction(dvCore->actionUndo());
        m_toolbar->addAction(dvCore->actionRedo());
        m_toolbar->addSeparator();
        for (QAction *action : qAsConst(m_globalToolbarActions)) {
            m_toolbar->addAction(action);
        }
    }

    return m_toolbar;
}

/*!
   Show the dialog to display and edit the used ASN1 file
 */
void DVQtCEditor::showAsn1Dialog()
{
    if (dvPlugin().isNull()) {
        return;
    }

    //@todo show asn1 dialog
}

}
