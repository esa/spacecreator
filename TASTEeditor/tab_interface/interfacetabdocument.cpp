/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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
#include "interfacetabdocument.h"

#include "interfacetabgraphicsscene.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsView>

#define WARN_NOT_IMPLEMENTED qWarning() << Q_FUNC_INFO << "Not implemented yet."

namespace taste3 {
namespace document {

InterfaceTabDocument::InterfaceTabDocument(QObject *parent)
    : AbstractTabDocument(parent)
{
}

InterfaceTabDocument::~InterfaceTabDocument()
{
    if (m_graphicsView && !m_graphicsView->parent())
        delete m_graphicsView;
}

QWidget *InterfaceTabDocument::createView()
{
    if (!m_graphicsView)
        m_graphicsView = new QGraphicsView;
    return m_graphicsView;
}

QGraphicsScene *InterfaceTabDocument::createScene()
{
    if (!m_graphicsScene)
        m_graphicsScene = new InterfaceTabGraphicsScene(this);
    return m_graphicsScene;
}

QString InterfaceTabDocument::title() const
{
    return tr("Interface");
}

bool InterfaceTabDocument::loadImpl(const QString &path)
{
    if (path.isEmpty() || !QFileInfo(path).exists()) {
        qWarning() << Q_FUNC_INFO << "Invalid path";
        return false;
    }

    return false;
}

bool InterfaceTabDocument::saveImpl(const QString & /*path*/)
{
    return false;
}

QVector<QAction *> InterfaceTabDocument::initActions()
{
    if (!m_actCreateContainer) {
        m_actCreateContainer = new QAction(tr("Function Type"));
        connect(m_actCreateContainer, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateContainer);
        m_actCreateContainer->setIcon(QIcon(":/tab_interface/toolbar/icns/container.svg"));
    }

    if (!m_actCreateFunction) {
        m_actCreateFunction = new QAction(tr("Function"));
        connect(m_actCreateFunction, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateFunction);

        m_actCreateFunction->setIcon(QIcon(":/tab_interface/toolbar/icns/function.svg"));
    }

    if (!m_actCreateProvidedInterface) {
        m_actCreateProvidedInterface = new QAction(tr("Provided Interface"));
        connect(m_actCreateProvidedInterface, &QAction::triggered, this,
                &InterfaceTabDocument::onActionCreateProvidedInterface);
        m_actCreateProvidedInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/pi.svg"));
    }

    if (!m_actCreateRequiredInterface) {
        m_actCreateRequiredInterface = new QAction(tr("Required Interface"));
        connect(m_actCreateRequiredInterface, &QAction::triggered, this,
                &InterfaceTabDocument::onActionCreateRequiredInterface);
        m_actCreateRequiredInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/ri.svg"));
    }

    if (!m_actCreateComment) {
        m_actCreateComment = new QAction(tr("Comment"));
        connect(m_actCreateComment, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateComment);
        m_actCreateComment->setIcon(QIcon(":/tab_interface/toolbar/icns/comment.svg"));
    }

    if (!m_actGroupConnections) {
        m_actGroupConnections = new QAction(tr("Group"));
        connect(m_actGroupConnections, &QAction::triggered, this, &InterfaceTabDocument::onActionGroupConnections);
        m_actGroupConnections->setIcon(QIcon(":/tab_interface/toolbar/icns/group.svg"));
    }

    return { m_actCreateContainer,         m_actCreateFunction, m_actCreateProvidedInterface,
             m_actCreateRequiredInterface, m_actCreateComment,  m_actGroupConnections };
}

void InterfaceTabDocument::onActionCreateContainer()
{
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateFunction()
{
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateProvidedInterface()
{
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateRequiredInterface()
{
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateComment()
{
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionGroupConnections()
{
    WARN_NOT_IMPLEMENTED;
}

} // ns document
} // ns taste3
