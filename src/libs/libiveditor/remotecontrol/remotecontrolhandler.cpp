/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "remotecontrolhandler.h"

#include "errorhub.h"
#include "interfacedocument.h"
#include "itemeditor/ivfunctiongraphicsitem.h"
#include "itemeditor/ivitemmodel.h"
#include "ivfunction.h"
#include "ivmodel.h"

#include <QMetaEnum>
#include <ivconnection.h>

namespace ive {

/*!
  \class msc::RemoteControlHandler Handles the commands sent via the shared::RemoteControlWebServer

JSON structure:

 {
     "CommandType": "command",
     "Parameters": {
         "parameter1": "parameter1Value",
         "parameter2": "parameter2Value",
         "parameter3": "parameter3Value",
         ...
     }
 }

Implemented commands and parameters list:

- **Load** - Load an existing interface view file
 + **filename** - Filename with full path to the file

- **HighlightFunction** - Highlights a function
+ **name** - Name of the function to show highlighted

- **UnHighlightFunction** - Shows a function normal
+ **name** - Name of the function to show un-highlighted. Using "*" unhighlights allfunctions.

- **HighlightConnection** - Highlights a connection
+ **fromFunction** - Name of the source function
+ **toFunction** - Name of the target function
+ **riName** - Name of the interface

- **UnHighlightConnection** - Shows a connection normal
+ **fromFunction** - Name of the source function. Using "*" unhighlights connections of all functions.
+ **toFunction** - Name of the target function. Using "*" unhighlights all connections to any function.
+ **riName** - Name of the interface. Using "*" unhighlights connections connecting the given functions.

 */
RemoteControlHandler::RemoteControlHandler(QObject *parent)
    : QObject(parent)
{
    connect(shared::ErrorHub::instance(), &shared::ErrorHub::errorAdded, this, [this](const shared::ErrorItem &error) {
        m_lastErrorMessage = shared::ErrorHub::errorDescriptions().join('\n');
        shared::ErrorHub::clearErrors();
    });
}

void RemoteControlHandler::setIvDocument(ive::InterfaceDocument *model)
{
    m_document = model;
}

/**
 * Processes a command defined in the object \p obj received from peer \p peerName
 */
void RemoteControlHandler::handleMessage(const QJsonObject &obj, const QString &peerName)
{
    const QMetaEnum qtEnum = QMetaEnum::fromType<RemoteControlHandler::CommandType>();
    const QString commandTypeStr = obj.value(qtEnum.name()).toString();
    const int commandTypeInt = qtEnum.keyToValue(commandTypeStr.toLocal8Bit().constData());
    /**
     * Loads ain interfaceview.xml file
     * @param params The key "filename" contains the full file path
     * @param errorString If not nullptr and in case of an error, an errortext is placed in here
     * @return True if the file was loaded succefully
     */

    handleRemoteCommand(static_cast<RemoteControlHandler::CommandType>(commandTypeInt),
            obj.value(QLatin1String("Parameters")).toObject().toVariantMap(), peerName);
}

/*!
 * \brief RemoteControlHandler::handleRemoteCommand Perform a remote command
 * \param commandType The type of the command to perform
 * \param params Parameters for the command
 * \param peerName The caller name
 *
 * This is the main function. Call this to perform a command.
 */
void RemoteControlHandler::handleRemoteCommand(
        RemoteControlHandler::CommandType commandType, const QVariantMap &params, const QString &peerName)
{
    if (!m_document) {
        Q_EMIT commandDone(false, peerName, QLatin1String("Empty model"));
        return;
    }
    bool result = false;

    QString errorString;
    switch (commandType) {
    case RemoteControlHandler::CommandType::Load:
        result = handleLoad(params, &errorString);
        break;
    case RemoteControlHandler::CommandType::HighlightFunction:
        result = highlightFunction(params, &errorString);
        break;
    case RemoteControlHandler::CommandType::UnHighlightFunction:
        result = unhighlightFunction(params, &errorString);
        break;
    case RemoteControlHandler::CommandType::HighlightConnection:
        result = highlightConnection(params, &errorString);
        break;
    case RemoteControlHandler::CommandType::UnHighlightConnection:
        result = unhighlightConnection(params, &errorString);
        break;
    default:
        qWarning() << "Unknown command:" << commandType;
        errorString = tr("Unknown command");
        break;
    }

    Q_EMIT commandDone(result, peerName, errorString);
}

bool RemoteControlHandler::handleLoad(const QVariantMap &params, QString *errorString)
{
    const QString fileName = params.value("filename").toString();
    bool result = m_document->load(fileName);
    if (!result && errorString) {
        *errorString = m_lastErrorMessage;
    }
    return result;
}

bool RemoteControlHandler::highlightFunction(const QVariantMap &params, QString *errorString)
{
    QList<ivm::IVFunction *> funcs = getFunction(params, errorString);

    if (funcs.isEmpty()) {
        return false;
    }

    for (ivm::IVFunction *func : funcs) {
        func->setMarked(true);
        updateParentItem(func);
    }

    return true;
}

bool RemoteControlHandler::unhighlightFunction(const QVariantMap &params, QString *errorString)
{
    QList<ivm::IVFunction *> funcs = getFunction(params, errorString);

    if (funcs.isEmpty()) {
        return false;
    }

    for (ivm::IVFunction *func : funcs) {
        func->setMarked(false);
        updateParentItem(func);
    }

    return true;
}

QList<ivm::IVFunction *> RemoteControlHandler::getFunction(const QVariantMap &params, QString *errorString) const
{
    const QString name = params.value("name").toString();
    if (name.isEmpty()) {
        if (errorString) {
            *errorString = tr("No valid function parameter 'name'");
        }
        return {};
    }

    ivm::IVModel *model = m_document->objectsModel();
    if (name == "*") {
        QList<ivm::IVFunction *> funcs;
        for (shared::VEObject *obj : model->objects()) {
            if (auto func = dynamic_cast<ivm::IVFunction *>(obj)) {
                funcs.append(func);
            }
        }
        return funcs;
    }

    ivm::IVFunction *func = model->getFunction(name, Qt::CaseInsensitive);
    if (!func) {
        if (errorString) {
            *errorString = tr("No function '%1' in the model").arg(name);
        }
        return {};
    }

    return QList<ivm::IVFunction *> { func };
}

void RemoteControlHandler::updateParentItem(ivm::IVObject *obj) const
{
    if (!obj || !obj->parentObject() || !m_document->itemsModel()) {
        return;
    }

    IVItemModel *itemsModel = m_document->itemsModel();
    ive::IVFunctionGraphicsItem *item = itemsModel->getItem<ive::IVFunctionGraphicsItem *>(obj->parentObject()->id());
    if (item) {
        item->update();
    }
}

bool RemoteControlHandler::highlightConnection(const QVariantMap &params, QString *errorString)
{
    QList<ivm::IVConnection *> connections = getConnection(params, errorString);
    if (connections.isEmpty()) {
        return false;
    }

    for (ivm::IVConnection *connection : connections) {
        connection->setMarked(true);

        updateItem(connection->source());
        updateParentItem(connection->source());
        updateItem(connection->target());
        updateParentItem(connection->target());
    }

    return true;
}

bool RemoteControlHandler::unhighlightConnection(const QVariantMap &params, QString *errorString)
{
    QList<ivm::IVConnection *> connections = getConnection(params, errorString);
    if (connections.isEmpty()) {
        return false;
    }

    for (ivm::IVConnection *connection : connections) {
        connection->setMarked(true);
        updateItem(connection->source());
        updateParentItem(connection->source());
        updateItem(connection->target());
        updateParentItem(connection->target());

        connection->setMarked(false);
    }

    return true;
}

QList<ivm::IVConnection *> RemoteControlHandler::getConnection(const QVariantMap &params, QString *errorString) const
{
    const QString fromName = params.value("fromFunction").toString();
    if (fromName.isEmpty()) {
        if (errorString) {
            *errorString = tr("No valid function parameter 'fromFunction'");
        }
        return {};
    }
    const QString toName = params.value("toFunction").toString();
    if (toName.isEmpty()) {
        if (errorString) {
            *errorString = tr("No valid function parameter 'toFunction'");
        }
        return {};
    }
    const QString riName = params.value("riName").toString();
    if (riName.isEmpty()) {
        if (errorString) {
            *errorString = tr("No valid function parameter 'riName'");
        }
        return {};
    }

    QList<ivm::IVConnection *> connections;

    ivm::IVModel *model = m_document->objectsModel();
    for (shared::VEObject *obj : model->objects()) {
        if (auto connection = dynamic_cast<ivm::IVConnection *>(obj)) {
            bool fromMatch = fromName.compare(connection->sourceName(), Qt::CaseInsensitive) == 0 || fromName == "*";
            bool toMatch = toName.compare(connection->targetName(), Qt::CaseInsensitive) == 0 || toName == "*";
            bool nameMatch =
                    riName.compare(connection->targetInterfaceName(), Qt::CaseInsensitive) == 0 || riName == "*";
            if (fromMatch && toMatch && nameMatch) {
                connections.append(connection);
            }
        }
    }

    if (connections.isEmpty()) {
        if (errorString) {
            *errorString = tr("No connection '%1' from '%2' to '%3' in the model").arg(riName, fromName, toName);
        }
    }

    return connections;
}

void RemoteControlHandler::updateItem(ivm::IVObject *obj) const
{
    if (!obj || !m_document->itemsModel()) {
        return;
    }

    IVItemModel *itemsModel = m_document->itemsModel();
    ive::IVFunctionGraphicsItem *item = itemsModel->getItem<ive::IVFunctionGraphicsItem *>(obj->id());
    if (item) {
        item->update();
    }
}

}
