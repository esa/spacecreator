/*
   Copyright (C) 2018 - 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscqtceditor.h"

#include "msceditorcore.h"
#include "msceditordocument.h"
#include "mscmainwidget.h"
#include "mscmodelstorage.h"
#include "spacecreatorpluginconstants.h"
#include "tools/entitydeletetool.h"

#include <QDebug>
#include <QToolBar>

namespace spctr {

MscQtCEditor::MscQtCEditor(MscModelStorage *mscStorage)
    : Core::IEditor()
    , m_editorWidget(new MscMainWidget)
{
    setContext(Core::Context(spctr::Constants::K_MSC_EDITOR_ID));
    m_document = new MscEditorDocument(mscStorage, this);
    setWidget(m_editorWidget);

    connect(m_document, &spctr::MscEditorDocument::mscDataLoaded, this,
            [this](const QString &, QSharedPointer<msc::MSCEditorCore> data) { m_editorWidget->init(data); });
}

MscQtCEditor::~MscQtCEditor()
{
    m_editorWidget->deleteLater();
}

Core::IDocument *MscQtCEditor::document() const
{
    return m_document;
}

MscEditorDocument *MscQtCEditor::mscDocument() const
{
    return m_document;
}

QSharedPointer<msc::MSCEditorCore> MscQtCEditor::mscEditorCore() const
{
    return m_document->mscEditorCore();
}

QWidget *MscQtCEditor::toolBar()
{
    QSharedPointer<msc::MSCEditorCore> mscCore = m_document->mscEditorCore();
    if (m_toolbar == nullptr && !mscCore.isNull()) {
        m_toolbar = new QToolBar;
        m_toolbar->addAction(mscCore->actionUndo());
        m_toolbar->addAction(mscCore->actionRedo());
        m_toolbar->addSeparator();
        m_toolbar->addAction(mscCore->deleteTool()->action());
        m_toolbar->addSeparator();
        m_toolbar->addAction(mscCore->actionCopy());
        m_toolbar->addAction(mscCore->actionPaste());
    }

    return m_toolbar;
}

}
