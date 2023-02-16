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

#pragma once

#include <QAbstractSocket>
#include <QJsonObject>
#include <QObject>
#include <QVariantMap>

class QWebSocketServer;
class QWebSocket;

namespace shared {

class RemoteControlWebServer : public QObject
{
    Q_OBJECT
public:
    explicit RemoteControlWebServer(QObject *parent = nullptr);
    ~RemoteControlWebServer() override;

    bool start(quint16 port);

public Q_SLOTS:
    void onNewConnection();
    void processTextMessage(const QString &message);
    void socketConnected();
    void socketDisconnected();
    void error(QAbstractSocket::SocketError error);

    void commandDone(bool result, const QString &peerName, const QString &errorString);

Q_SIGNALS:
    void commandReceived(const QJsonObject &, const QString &);

private:
    QWebSocketServer *m_webSocketServer = nullptr;
    QList<QWebSocket *> m_clients;
};

}
