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

#include "remotecontrolwebserver.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMetaEnum>
#include <QtWebSockets/QWebSocket>
#include <QtWebSockets/QWebSocketServer>

namespace shared {

static inline QByteArray generateResponse(bool result, const QString &errorString = QString())
{
    QJsonObject obj;
    obj.insert(QLatin1String("result"), result);
    if (!errorString.isEmpty())
        obj.insert(QLatin1String("errorString"), errorString);
    return QJsonDocument(obj).toJson();
}

/*!
\class shared::RemoteControlWebServer
\brief Handles remote control commands and arguments compounded in json packet using websocket.

After command processing returns JSON packet:

    {
        "result": True,
        "errorString": "Short error description"
    }

 */

RemoteControlWebServer::RemoteControlWebServer(QObject *parent)
    : QObject(parent)
    , m_webSocketServer(new QWebSocketServer(QLatin1String("Remote Control"), QWebSocketServer::NonSecureMode, this))
{
}

RemoteControlWebServer::~RemoteControlWebServer()
{
    m_webSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

/*!
 * \brief RemoteControlWebServer::start Start the remote listener
 * \param port The port to listen on
 * \return True if successfull
 */
bool RemoteControlWebServer::start(quint16 port)
{
    if (m_webSocketServer->isListening()) {
        if (m_webSocketServer->serverPort() == port)
            return true;
        else
            m_webSocketServer->close();
    }
    if (m_webSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Remote control server listening on port" << port;
        connect(m_webSocketServer, &QWebSocketServer::newConnection, this, &RemoteControlWebServer::onNewConnection);
        return true;
    }
    qWarning() << "Websocket server failed to start listening port:" << port << m_webSocketServer->error()
               << m_webSocketServer->errorString();
    return false;
}

/*!
 * \brief RemoteControlWebServer::onNewConnection Handle a new incoming connection
 */
void RemoteControlWebServer::onNewConnection()
{
    QWebSocket *pSocket = m_webSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &RemoteControlWebServer::processTextMessage);
    connect(pSocket, &QWebSocket::connected, this, &RemoteControlWebServer::socketConnected);
    connect(pSocket, &QWebSocket::disconnected, this, &RemoteControlWebServer::socketDisconnected);
    connect(pSocket, static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), this,
            &RemoteControlWebServer::error);

    m_clients << pSocket;
}

/*!
 * \brief RemoteControlWebServer::processTextMessage Parse and handle a remote command
 * \param message The command to perform
 */
void RemoteControlWebServer::processTextMessage(const QString &message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QJsonParseError error;
    const QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &error);
    if (QJsonParseError::NoError != error.error) {
        qWarning() << "Json document parsing error:" << error.error << error.errorString();
        if (pClient) {
            qDebug() << "Informing remote about error:"
                     << pClient->sendTextMessage(generateResponse(false, error.errorString()));
        }
        return;
    }
    const QJsonObject obj = doc.object();
    Q_EMIT commandReceived(obj, pClient ? pClient->peerName() : QString());
}

/*!
 * \brief RemoteControlWebServer::socketConnected Called when remote connection
 *  has been established. Only for debug output
 */
void RemoteControlWebServer::socketConnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Socket Connected:" << pClient;
}

/*!
 * \brief RemoteControlWebServer::socketDisconnected Perform cleanup after a disconnection
 */
void RemoteControlWebServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Socket disconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

/*!
 * \brief RemoteControlWebServer::error An error has occurred
 * \param error
 */
void RemoteControlWebServer::error(QAbstractSocket::SocketError error)
{
    const QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qWarning() << "Socket error:" << error << (pClient ? pClient->errorString() : QString());
}

/*!
 * \brief RemoteControlWebServer::commandDone Generate a response after a remote command has been handled
 * \param result
 * \param peerName
 * \param errorString
 */
void RemoteControlWebServer::commandDone(bool result, const QString &peerName, const QString &errorString)
{
    auto it = std::find_if(m_clients.constBegin(), m_clients.constEnd(),
            [peerName](const QWebSocket *socket) { return socket->peerName() == peerName; });
    if (it == m_clients.constEnd())
        return;

    (*it)->sendTextMessage(generateResponse(result, errorString));
}
}
