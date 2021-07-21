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

#include "ivqtceditor.h"

#include "asn1dialog.h"
#include "commands/cmdchangeasn1file.h"
#include "commandsstack.h"
#include "endtoend/endtoendview.h"
#include "interfacedocument.h"
#include "iveditorcore.h"
#include "iveditordocument.h"
#include "ivmainwidget.h"
#include "mainmodel.h"
#include "msceditorcore.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorprojectmanager.h"

#include <QFileInfo>
#include <QToolBar>
#include <QUndoCommand>
#include <utils/qtcassert.h>

namespace spctr {

IVQtCEditor::IVQtCEditor(SpaceCreatorProjectManager *projectManager, const QList<QAction *> &ivActions)
    : Core::IEditor()
    , m_document(new IVEditorDocument(projectManager, this))
    , m_editorWidget(new IVMainWidget)
    , m_projectManager(projectManager)
    , m_globalToolbarActions(ivActions)
{
    setContext(Core::Context(spctr::Constants::K_IV_EDITOR_ID));
    setWidget(m_editorWidget);

    connect(m_document, &spctr::IVEditorDocument::ivDataLoaded, this,
            [this](const QString &, QSharedPointer<ive::IVEditorCore> data) { m_editorWidget->init(data); });
}

IVQtCEditor::~IVQtCEditor()
{
    m_editorWidget->deleteLater();
}

Core::IDocument *IVQtCEditor::document() const
{
    return m_document;
}

IVEditorDocument *IVQtCEditor::ivDocument() const
{
    return m_document;
}

QSharedPointer<ive::IVEditorCore> IVQtCEditor::ivPlugin() const
{
    return m_document->ivEditorCore();
}

QWidget *IVQtCEditor::toolBar()
{
    QSharedPointer<ive::IVEditorCore> ivCore = m_document->ivEditorCore();
    if (m_toolbar == nullptr && !ivCore.isNull()) {
        m_toolbar = new QToolBar;
        m_toolbar->addAction(ivCore->actionUndo());
        m_toolbar->addAction(ivCore->actionRedo());
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
void IVQtCEditor::showAsn1Dialog()
{
    if (ivPlugin().isNull()) {
        return;
    }

    QSharedPointer<ive::IVEditorCore> plugin = ivPlugin();
    ive::Asn1Dialog dialog;
    QFileInfo fi(plugin->document()->asn1FilePath());
    dialog.setFile(fi);
    dialog.show();
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        if (plugin->document()->asn1FileName() != dialog.fileName()) {
            auto command = new ive::cmd::CmdChangeAsn1File(plugin->document(), dialog.fileName());
            plugin->commandsStack()->push(command);
        }
    }
}

void IVQtCEditor::showE2EDataflow(const QStringList &mscFiles)
{
    if (ivPlugin().isNull()) {
        return;
    }

    QSharedPointer<ive::IVEditorCore> plugin = ivPlugin();
    if (m_endToEndView.isNull()) {
        m_endToEndView = new ive::EndToEndView(plugin->document(), nullptr);
        m_endToEndView->setAttribute(Qt::WA_DeleteOnClose);
        std::function<msc::MscModel *(QString fileName)> fetcher = [this](QString fileName) -> msc::MscModel * {
            if (m_projectManager) {
                QSharedPointer<msc::MSCEditorCore> core = m_projectManager->mscData(fileName);
                if (core) {
                    return core->mainModel()->mscModel();
                }
            }
            return {};
        };
        m_endToEndView->setMscDataFetcher(fetcher);
        m_endToEndView->setMscFiles(mscFiles);
        connect(plugin->document(), &QObject::destroyed, m_endToEndView.data(), &QObject::deleteLater);
    }
    m_endToEndView->show();
    m_endToEndView->raise();
}

}
