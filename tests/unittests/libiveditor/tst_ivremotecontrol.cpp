/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "interfacedocument.h"
#include "iveditor.h"
#include "remotecontrol/remotecontrolhandler.h"
#include "remotecontrol/remotecontrolwebserver.h"
#include "sharedlibrary.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QWebSocket>
#include <QtTest>

static const int kPort = 34633;

class tst_IvRemoteControl : public QObject
{
    Q_OBJECT
public:
    enum class SendResult
    {
        EXPECT_PASS = 0,
        EXPECT_FAIL = 1
    };

    tst_IvRemoteControl()
        : m_server(new shared::RemoteControlWebServer(this))
        , m_handler(new ive::RemoteControlHandler(this))
        , m_document(new ive::InterfaceDocument(this))
        , m_socket(new QWebSocket(QString(), QWebSocketProtocol::Version::VersionLatest, this))
        , m_textMessageReceived(m_socket, SIGNAL(textMessageReceived(QString)))
        , m_socketError(m_socket, SIGNAL(error(QAbstractSocket::SocketError)))
    {
        ive::initIVEditor();
    }

private Q_SLOTS:
    void initTestCase()
    {
        shared::initSharedLibrary();

        connect(m_server, &shared::RemoteControlWebServer::commandReceived, m_handler,
                &ive::RemoteControlHandler::handleMessage);
        connect(m_handler, &ive::RemoteControlHandler::commandDone, m_server,
                &shared::RemoteControlWebServer::commandDone);

        m_server->start(kPort);
    }

    void init()
    {
        m_handler->setIvDocument(m_document);
        m_textMessageReceived.clear();
        m_socketError.clear();

        m_socket->open(QUrl(QStringLiteral("ws://localhost:%1").arg(kPort)));
        QTRY_COMPARE(m_socket->state(), QAbstractSocket::ConnectedState);
    }

    void cleanup() { m_socket->close(); }

    void testLoadCommandFail()
    {
        QCOMPARE(m_socketError.count(), 0);
        QCOMPARE(m_socket->state(), QAbstractSocket::ConnectedState);

        QJsonObject obj;
        obj[QLatin1String("CommandType")] = QLatin1String("Load");
        QJsonObject params;
        params.insert("filename", "dummy");
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj, SendResult::EXPECT_FAIL);
    }

private:
    QJsonObject takeFirstResultMessage()
    {
        QList<QVariant> arguments = m_textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();
        return QJsonDocument::fromJson(messageReceived.toUtf8()).object();
    }

    void sendCommand(const QJsonObject &obj, SendResult passes = SendResult::EXPECT_PASS)
    {
        QByteArray json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);
        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        QJsonObject resultObj = takeFirstResultMessage();
        const bool ok = passes == SendResult::EXPECT_PASS;
        QCOMPARE(resultObj.value(QLatin1String("result")).toBool(), ok);
        waitForLayoutUpdate();
    }

    void waitForLayoutUpdate()
    {
        QApplication::processEvents();
        QTest::qWait(1);
        QApplication::processEvents();
    }

    shared::RemoteControlWebServer *m_server = nullptr;
    ive::RemoteControlHandler *m_handler = nullptr;
    ive::InterfaceDocument *m_document = nullptr;
    QWebSocket *m_socket = nullptr;
    QSignalSpy m_textMessageReceived;
    QSignalSpy m_socketError;
};

QTEST_MAIN(tst_IvRemoteControl)

#include "tst_ivremotecontrol.moc"
