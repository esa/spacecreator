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

#define WARN_NOT_IMPLEMENTED qWarning() << Q_FUNC_INFO << "Not implemented yet."

namespace taste3 {
namespace document {

InterfaceTabDocument::InterfaceTabDocument(QObject *parent)
    : AbstractTabDocument(parent)
    , m_graphicsScene(new InterfaceTabGraphicsScene(this))
{
    setDocScene(m_graphicsScene);
}

InterfaceTabDocument::~InterfaceTabDocument() {}

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
    if (!m_actCreateFunction) {
        m_actCreateFunction = new QAction(tr("Function"));
        connect(m_actCreateFunction, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateFunction);
    }

    if (!m_actCreateFunctionType) {
        m_actCreateFunctionType = new QAction(tr("Function Type"));
        connect(m_actCreateFunctionType, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateFunctionType);
    }

    if (!m_actCreateConnection) {
        m_actCreateConnection = new QAction(tr("Connect"));
        connect(m_actCreateConnection, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateConnection);
    }

    if (!m_actGroupConnections) {
        m_actGroupConnections = new QAction(tr("Group"));
        connect(m_actGroupConnections, &QAction::triggered, this, &InterfaceTabDocument::onActionGroupConnections);
    }

    return { m_actCreateFunction, m_actCreateFunctionType, m_actCreateConnection, m_actGroupConnections };
}

void InterfaceTabDocument::onActionCreateFunction()
{
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateFunctionType()
{
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateConnection()
{
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionGroupConnections()
{
    WARN_NOT_IMPLEMENTED;
}

} // ns document
} // ns taste3
