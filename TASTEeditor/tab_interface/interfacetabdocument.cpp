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

#include "baseitems/graphicsview.h"
#include "creatortool.h"
#include "interfacetabgraphicsscene.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsView>
#include <tab_aadl/aadlobjectsmodel.h>

#define WARN_NOT_IMPLEMENTED qWarning() << Q_FUNC_INFO << "Not implemented yet."

namespace taste3 {
namespace document {

InterfaceTabDocument::InterfaceTabDocument(QObject *parent)
    : AbstractTabDocument(parent)
    , m_model(new aadl::AADLObjectsModel(this))
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
        m_graphicsView = new GraphicsView;
    m_graphicsView->setScene(m_graphicsScene);
    return m_graphicsView;
}

QGraphicsScene *InterfaceTabDocument::createScene()
{
    if (!m_graphicsScene) {
        m_graphicsScene = new InterfaceTabGraphicsScene(this);
        m_graphicsScene->setSceneRect(QRectF(0, 0, 640, 480));
    }
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
    if (!m_tool) {
        m_tool = new aadl::CreatorTool(m_graphicsView, m_model, this);
        connect(m_tool, &aadl::CreatorTool::created, this, [this]() {
            if (QAction *currentAction = m_actionGroup->checkedAction())
                currentAction->setChecked(false);
            m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::Pointer);
        });
    }

    if (!m_actionGroup) {
        m_actionGroup = new QActionGroup(this);
        m_actionGroup->setExclusive(true);
    }

    if (!m_actCreateContainer) {
        m_actCreateContainer = new QAction(tr("Function Type"));
        m_actCreateContainer->setCheckable(true);
        m_actCreateContainer->setActionGroup(m_actionGroup);
        connect(m_actCreateContainer, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateContainer);
        m_actCreateContainer->setIcon(QIcon(":/tab_interface/toolbar/icns/container.svg"));
    }

    if (!m_actCreateFunction) {
        m_actCreateFunction = new QAction(tr("Function"));
        m_actCreateFunction->setCheckable(true);
        m_actCreateFunction->setActionGroup(m_actionGroup);
        connect(m_actCreateFunction, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateFunction);

        m_actCreateFunction->setIcon(QIcon(":/tab_interface/toolbar/icns/function.svg"));
    }

    if (!m_actCreateProvidedInterface) {
        m_actCreateProvidedInterface = new QAction(tr("Provided Interface"));
        m_actCreateProvidedInterface->setCheckable(true);
        m_actCreateProvidedInterface->setActionGroup(m_actionGroup);
        connect(m_actCreateProvidedInterface, &QAction::triggered, this,
                &InterfaceTabDocument::onActionCreateProvidedInterface);
        m_actCreateProvidedInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/pi.svg"));
    }

    if (!m_actCreateRequiredInterface) {
        m_actCreateRequiredInterface = new QAction(tr("Required Interface"));
        m_actCreateRequiredInterface->setCheckable(true);
        m_actCreateRequiredInterface->setActionGroup(m_actionGroup);
        connect(m_actCreateRequiredInterface, &QAction::triggered, this,
                &InterfaceTabDocument::onActionCreateRequiredInterface);
        m_actCreateRequiredInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/ri.svg"));
    }

    if (!m_actCreateComment) {
        m_actCreateComment = new QAction(tr("Comment"));
        m_actCreateComment->setCheckable(true);
        m_actCreateComment->setActionGroup(m_actionGroup);
        connect(m_actCreateComment, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateComment);
        m_actCreateComment->setIcon(QIcon(":/tab_interface/toolbar/icns/comment.svg"));
    }

    if (!m_actGroupConnections) {
        m_actGroupConnections = new QAction(tr("Group"));
        m_actGroupConnections->setCheckable(true);
        m_actGroupConnections->setActionGroup(m_actionGroup);
        connect(m_actGroupConnections, &QAction::triggered, this, &InterfaceTabDocument::onActionGroupConnections);
        m_actGroupConnections->setIcon(QIcon(":/tab_interface/toolbar/icns/group.svg"));
    }

    if (!m_actCreateConnection) {
        m_actCreateConnection = new QAction(tr("Connection"));
        m_actCreateConnection->setCheckable(true);
        m_actCreateConnection->setActionGroup(m_actionGroup);
        connect(m_actCreateConnection, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateConnection);
        m_actCreateConnection->setIcon(QIcon(":/tab_interface/toolbar/icns/connection.svg"));
    }

    return { m_actCreateContainer, m_actCreateFunction,   m_actCreateProvidedInterface, m_actCreateRequiredInterface,
             m_actCreateComment,   m_actGroupConnections, m_actCreateConnection };
}

void InterfaceTabDocument::onActionCreateContainer()
{
    m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::Container);
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateFunction()
{
    m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::Function);
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateProvidedInterface()
{
    m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::ProvidedInterface);
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateRequiredInterface()
{
    m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::RequiredInterface);
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateComment()
{
    m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::Comment);
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionGroupConnections()
{
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateConnection()
{
    m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::Connection);
    WARN_NOT_IMPLEMENTED;
}

} // ns document
} // ns taste3
