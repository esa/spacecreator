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

#include "commandsstack.h"
#include "interface/interfacetabdocument.h"
#include "iveditorplugin.h"
#include "xmldocexporter.h"

#include <QAction>
#include <QDebug>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QImage>
#include <QSplitter>
#include <QStackedWidget>
#include <QUndoGroup>
#include <QUndoStack>
#include <QtWidgets/QHeaderView>
#include <coreplugin/minisplitter.h>

namespace AadlPlugin {

AadlMainWidget::AadlMainWidget(QWidget *parent)
    : QWidget(parent)
    , m_plugin(new aadlinterface::IVEditorPlugin(this))
    , m_document(new aadlinterface::InterfaceDocument(this))
{
    m_document->init();
    initUi();

    QUndoStack *currentStack { nullptr };
    currentStack = m_document->commandsStack();
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
    m_document->load(filename);
    return true;
}

bool AadlMainWidget::save()
{
    return aadlinterface::XmlDocExporter::exportDocSilently(m_document, {}, {});
}

void AadlMainWidget::setFileName(const QString &filename)
{
    Q_UNUSED(filename)
}

bool AadlMainWidget::isDirty() const
{
    // @todo return if the user edited the model
    return false;
}

QUndoStack *AadlMainWidget::undoStack()
{
    return m_document->commandsStack();
}

QString AadlMainWidget::textContents() const
{
    // @todo return the model as xml
    return "";
}

QVector<QAction *> AadlMainWidget::toolActions() const
{
    return m_actions;
}

void AadlMainWidget::initUi()
{
    setLayout(new QVBoxLayout(this));
    layout()->addWidget(m_document->view());
    layout()->setMargin(0);

    m_actions = m_document->initActions();
}

}
