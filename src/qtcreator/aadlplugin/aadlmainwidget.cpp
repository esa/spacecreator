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

namespace AadlPlugin {

AadlMainWidget::AadlMainWidget(QWidget *parent)
    : QWidget(parent)
    , m_plugin(new aadlinterface::IVEditorCore(this))
{
    initUi();

    QUndoStack *currentStack { nullptr };
    currentStack = m_plugin->document()->commandsStack();
    if (currentStack) {
        if (m_plugin->undoGroup()->stacks().contains(currentStack)) {
            m_plugin->undoGroup()->addStack(currentStack);
        }
        m_plugin->undoGroup()->setActiveStack(currentStack);
    } else {
        m_plugin->undoGroup()->removeStack(m_plugin->undoGroup()->activeStack());
    }

    aadlinterface::cmd::CommandsStack::setCurrent(currentStack);
}

AadlMainWidget::~AadlMainWidget() { }

bool AadlMainWidget::load(const QString &filename)
{
    m_plugin->document()->load(filename);
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
    return m_plugin->document()->isDirty();
}

QUndoStack *AadlMainWidget::undoStack()
{
    return m_plugin->document()->commandsStack();
}

QString AadlMainWidget::textContents() const
{
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
    m_plugin->minimapView()->setVisible(visible);
}

void AadlMainWidget::onAttributesManagerRequested()
{
    m_plugin->document()->onAttributesManagerRequested();
}

void AadlMainWidget::onColorSchemeMenuInvoked()
{
    m_plugin->document()->onColorSchemeMenuInvoked();
}

void AadlMainWidget::onDynContextEditorMenuInvoked()
{
    m_plugin->document()->onDynContextEditorMenuInvoked();
}

aadlinterface::IVEditorCore *AadlMainWidget::ivPlugin() const
{
    return m_plugin;
}

void AadlMainWidget::initUi()
{
    setLayout(new QVBoxLayout(this));
    layout()->addWidget(m_plugin->document()->view());
    layout()->setMargin(0);

    m_actions = m_plugin->document()->initActions();

    m_plugin->setupMiniMap();
}

}
