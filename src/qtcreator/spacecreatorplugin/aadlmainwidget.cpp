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
#include "asn1dialog.h"
#include "commandsstack.h"
#include "interface/commands/commandsfactory.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"
#include "xmldocexporter.h"

#include <QAction>
#include <QBuffer>
#include <QDebug>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QUndoGroup>
#include <QUndoStack>

namespace spctr {

AadlMainWidget::AadlMainWidget(AadlModelStorage *aadlStorage, QWidget *parent)
    : QWidget(parent)
    , m_aadlStorage(aadlStorage)
{
}

AadlMainWidget::~AadlMainWidget()
{
    if (m_plugin && m_plugin->document()->view() && m_plugin->document()->view()->parent() == this) {
        m_plugin->document()->view()->setParent(nullptr);
    }
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

QVector<QAction *> AadlMainWidget::toolActions() const
{
    return m_actions;
}

/*!
   Show the dialog to display and edit the uased ASN1 file
 */
void AadlMainWidget::showAsn1Dialog()
{
    if (m_aadlStorage.isNull()) {
        return;
    }

    aadlinterface::Asn1Dialog dialog;
    QFileInfo fi(m_plugin->document()->path());
    fi.setFile(fi.absolutePath() + "/" + m_plugin->document()->asn1FileName());
    dialog.setFile(fi);
    dialog.show();
    int result = dialog.exec();
    if (result == QDialog::Accepted) {
        if (m_plugin->document()->asn1FileName() != dialog.fileName()) {
            QVariantList params { QVariant::fromValue(m_plugin->document()), QVariant::fromValue(dialog.fileName()) };
            QUndoCommand *command =
                    aadlinterface::cmd::CommandsFactory::create(aadlinterface::cmd::ChangeAsn1File, params);
            if (command) {
                aadlinterface::cmd::CommandsStack::current()->push(command);
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

    setLayout(new QVBoxLayout(this));
    m_plugin->document()->init();
    layout()->addWidget(m_plugin->document()->view());
    layout()->setMargin(0);

    m_actions = m_plugin->document()->initActions();

    m_plugin->setupMiniMap();

    aadlinterface::cmd::CommandsStack::setCurrent(m_plugin->document()->commandsStack());

    connect(m_plugin->document()->commandsStack(), &QUndoStack::cleanChanged, this,
            [&]() { Q_EMIT dirtyChanged(isDirty()); });
}

}
