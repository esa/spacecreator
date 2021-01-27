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

#include "aadlqtceditor.h"

#include "aadleditordocument.h"
#include "aadlmainwidget.h"
#include "aadlmodelstorage.h"
#include "asn1dialog.h"
#include "commandsstack.h"
#include "endtoend/endtoendview.h"
#include "interface/commands/commandsfactory.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"
#include "mainmodel.h"
#include "msceditorcore.h"
#include "mscmodelstorage.h"
#include "spacecreatorpluginconstants.h"

#include <QFileInfo>
#include <QToolBar>
#include <QUndoCommand>
#include <utils/qtcassert.h>

namespace spctr {

AadlQtCEditor::AadlQtCEditor(
        AadlModelStorage *aadlStorage, MscModelStorage *mscStorage, const QList<QAction *> &ivActions)
    : Core::IEditor()
    , m_document(new AadlEditorDocument(aadlStorage, this))
    , m_editorWidget(new AadlMainWidget)
    , m_mscStorage(mscStorage)
    , m_globalToolbarActions(ivActions)
{
    setContext(Core::Context(spctr::Constants::K_AADL_EDITOR_ID));
    setWidget(m_editorWidget);

    connect(m_document, &spctr::AadlEditorDocument::ivDataLoaded, this,
            [this](const QString &, QSharedPointer<ive::IVEditorCore> data) { m_editorWidget->init(data); });
}

AadlQtCEditor::~AadlQtCEditor()
{
    m_editorWidget->deleteLater();
}

Core::IDocument *AadlQtCEditor::document() const
{
    return m_document;
}

AadlEditorDocument *AadlQtCEditor::ivDocument() const
{
    return m_document;
}

QSharedPointer<ive::IVEditorCore> AadlQtCEditor::ivPlugin() const
{
    return m_document->ivEditorCore();
}

QWidget *AadlQtCEditor::toolBar()
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
void AadlQtCEditor::showAsn1Dialog()
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
            QVariantList params { QVariant::fromValue(plugin->document()), QVariant::fromValue(dialog.fileName()) };
            QUndoCommand *command = ive::cmd::CommandsFactory::create(ive::cmd::ChangeAsn1File, params);
            if (command) {
                plugin->commandsStack()->push(command);
            }
        }
    }
}

void AadlQtCEditor::showE2EDataflow(const QStringList &mscFiles)
{
    if (ivPlugin().isNull()) {
        return;
    }

    QSharedPointer<ive::IVEditorCore> plugin = ivPlugin();
    if (m_endToEndView.isNull()) {
        m_endToEndView = new ive::EndToEndView(plugin->document(), nullptr);
        m_endToEndView->setAttribute(Qt::WA_DeleteOnClose);
        std::function<msc::MscModel *(QString fileName)> fetcher = [this](QString fileName) -> msc::MscModel * {
            if (m_mscStorage) {
                QSharedPointer<msc::MSCEditorCore> core = m_mscStorage->mscData(fileName);
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
