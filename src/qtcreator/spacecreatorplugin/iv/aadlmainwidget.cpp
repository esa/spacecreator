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

#include "actionsbar.h"
#include "commandsstack.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"

#include <QHBoxLayout>
#include <QMessageBox>

namespace spctr {

AadlMainWidget::AadlMainWidget(QWidget *parent)
    : QWidget(parent)
{
}

AadlMainWidget::~AadlMainWidget()
{
    if (m_plugin && m_plugin->document()->view() && m_plugin->document()->view()->parent() == this) {
        m_plugin->document()->view()->setParent(nullptr);
    }
}

bool AadlMainWidget::init(QSharedPointer<aadlinterface::IVEditorCore> data)
{
    m_plugin = data;
    init();
    return true;
}

void AadlMainWidget::setMinimapVisible(bool visible)
{
    if (m_plugin.isNull()) {
        return;
    }
    m_plugin->minimapView()->setVisible(visible);
}

QSharedPointer<aadlinterface::IVEditorCore> AadlMainWidget::ivPlugin() const
{
    return m_plugin;
}

void AadlMainWidget::init()
{
    if (m_plugin.isNull()) {
        return;
    }

    auto layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);
    m_plugin->document()->init();

    layout->addWidget(m_plugin->document()->view());

    m_plugin->setupMiniMap();

    aadlinterface::cmd::CommandsStack::setCurrent(m_plugin->document()->commandsStack());

    connect(m_plugin->document(), &aadlinterface::InterfaceDocument::asn1ParameterErrorDetected, this,
            &AadlMainWidget::showAsn1Errors);
}

void spctr::AadlMainWidget::showAsn1Errors(const QStringList &faultyInterfaces)
{
    QMessageBox::warning(
            this, tr("ASN1 error"), tr("Following interfaces have ASN.1 errors:") + "\n" + faultyInterfaces.join("\n"));
}

}
