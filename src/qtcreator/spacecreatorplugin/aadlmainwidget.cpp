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

#include "aadlmainwidget.h"

#include "aadlmodelstorage.h"
#include "actionsbar.h"
#include "asn1dialog.h"
#include "commandsstack.h"
#include "interface/commands/commandsfactory.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"
#include "mainmodel.h"
#include "msceditorcore.h"
#include "mscmodel.h"
#include "mscmodelstorage.h"
#include "xmldocexporter.h"

#include <QAction>
#include <QBuffer>
#include <QDebug>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSharedPointer>
#include <QUndoGroup>
#include <QUndoStack>

namespace spctr {

AadlMainWidget::AadlMainWidget(AadlModelStorage *aadlStorage, MscModelStorage *mscStorage, QWidget *parent)
    : QWidget(parent)
    , m_aadlStorage(aadlStorage)
    , m_mscStorage(mscStorage)
{
}

AadlMainWidget::~AadlMainWidget()
{
    if (m_plugin && m_plugin->document()->view() && m_plugin->document()->view()->parent() == this) {
        m_plugin->document()->view()->setParent(nullptr);
    }
    delete m_endToEndView.data();
}

bool AadlMainWidget::load(const QString &filename)
{
    m_plugin = m_aadlStorage->ivData(filename);
    init();
    Q_EMIT aadlDataLoaded(filename, m_plugin);
    return true;
}

bool AadlMainWidget::save()
{
    return aadlinterface::XmlDocExporter::exportDocSilently(m_plugin->document(), {}, {});
}

void AadlMainWidget::setFileName(const QString &filename)
{
    Q_UNUSED(filename)
}

bool AadlMainWidget::isDirty() const
{
    if (m_aadlStorage.isNull()) {
        return false;
    }
    return m_plugin->document()->isDirty();
}

QUndoStack *AadlMainWidget::undoStack()
{
    if (m_aadlStorage.isNull()) {
        return nullptr;
    }
    return m_plugin->document()->commandsStack();
}

QString AadlMainWidget::textContents() const
{
    if (m_aadlStorage.isNull()) {
        return {};
    }

    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    bool ok = aadlinterface::XmlDocExporter::exportDoc(m_plugin->document(), &buffer);
    if (ok) {
        return buffer.data();
    } else {
        return QString();
    }
}

/*!
   Show the dialog to display and edit the used ASN1 file
 */
void AadlMainWidget::showAsn1Dialog()
{
    if (m_aadlStorage.isNull()) {
        return;
    }

    aadlinterface::Asn1Dialog dialog;
    QFileInfo fi(m_plugin->document()->asn1FilePath());
    dialog.setFile(fi);
    dialog.show();
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        if (m_plugin->document()->asn1FileName() != dialog.fileName()) {
            QVariantList params { QVariant::fromValue(m_plugin->document()), QVariant::fromValue(dialog.fileName()) };
            QUndoCommand *command =
                    aadlinterface::cmd::CommandsFactory::create(aadlinterface::cmd::ChangeAsn1File, params);
            if (command) {
                aadlinterface::cmd::CommandsStack::push(command);
            }
        }
    }
}

void AadlMainWidget::setMinimapVisible(bool visible)
{
    if (m_aadlStorage.isNull()) {
        return;
    }
    m_plugin->minimapView()->setVisible(visible);
}

void AadlMainWidget::showE2EDataflow(const QStringList &mscFiles)
{
    if (m_aadlStorage.isNull()) {
        return;
    }
    if (m_endToEndView.isNull()) {
        m_endToEndView = new aadlinterface::EndToEndView(m_plugin->document());
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
        connect(m_plugin->document(), &QObject::destroyed, m_endToEndView.data(), &QObject::deleteLater);
    }
    m_endToEndView->show();
    m_endToEndView->raise();
}

void AadlMainWidget::onAttributesManagerRequested()
{
    if (m_aadlStorage.isNull()) {
        return;
    }
    m_plugin->document()->onAttributesManagerRequested();
}

void AadlMainWidget::onColorSchemeMenuInvoked()
{
    if (m_aadlStorage.isNull()) {
        return;
    }
    m_plugin->document()->onColorSchemeMenuInvoked();
}

void AadlMainWidget::onDynContextEditorMenuInvoked()
{
    if (m_aadlStorage.isNull()) {
        return;
    }
    m_plugin->document()->onDynContextEditorMenuInvoked();
}

QSharedPointer<aadlinterface::IVEditorCore> AadlMainWidget::ivPlugin() const
{
    return m_plugin;
}

void AadlMainWidget::init()
{
    if (m_aadlStorage.isNull()) {
        return;
    }

    auto layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);
    m_plugin->document()->init();

    m_aadlToolBar = new shared::ActionsBar(this);
    layout->addWidget(m_aadlToolBar);

    layout->addWidget(m_plugin->document()->view());

    m_actions = m_plugin->document()->initActions();
    for (QAction *action : m_actions) {
        m_aadlToolBar->addAction(action);
    }

    m_plugin->setupMiniMap();

    aadlinterface::cmd::CommandsStack::setCurrent(m_plugin->document()->commandsStack());

    connect(m_plugin->document()->commandsStack(), &QUndoStack::cleanChanged, this,
            [&]() { Q_EMIT dirtyChanged(isDirty()); });

    connect(m_plugin->document(), &aadlinterface::InterfaceDocument::asn1ParameterErrorDetected, this,
            &AadlMainWidget::showAsn1Errors);
}

void spctr::AadlMainWidget::showAsn1Errors(const QStringList &faultyInterfaces)
{
    QMessageBox::warning(
            this, tr("ASN1 error"), tr("Following interfaces have ASN.1 errors:") + "\n" + faultyInterfaces.join("\n"));
}

}
