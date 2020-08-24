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

#include "mainmodel.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmodel.h"
#include "remotecontrolhandler.h"
#include "remotecontrolwebserver.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QWebSocket>
#include <QtTest>

static const int kPort = 34622;

class tst_RemoteControl : public QObject
{
    Q_OBJECT
public:
    tst_RemoteControl()
        : m_server(new msc::RemoteControlWebServer(this))
        , m_handler(new msc::RemoteControlHandler(this))
        , m_model(new msc::MainModel(this))
        , m_socket(new QWebSocket(QString(), QWebSocketProtocol::Version::VersionLatest, this))
        , m_socketConnectedSpy(m_socket, SIGNAL(connected()))
        , m_textMessageReceived(m_socket, SIGNAL(textMessageReceived(QString)))
        , m_socketError(m_socket, SIGNAL(error(QAbstractSocket::SocketError)))
    {
    }

private Q_SLOTS:

    void initTestCase()
    {
        m_model->initialModel();
        m_handler->setModel(m_model);
        connect(m_server, &msc::RemoteControlWebServer::executeCommand, m_handler,
                &msc::RemoteControlHandler::handleRemoteCommand);
        connect(m_handler, &msc::RemoteControlHandler::commandDone, m_server,
                &msc::RemoteControlWebServer::commandDone);

        m_server->start(kPort);
    }

    void init()
    {
        m_socketConnectedSpy.clear();
        m_textMessageReceived.clear();
        m_socketError.clear();
    }

    void testInstanceCommand()
    {
        m_socket->open(QUrl(QStringLiteral("ws://localhost:%1").arg(kPort)));

        QTRY_COMPARE(m_socketConnectedSpy.count(), 1);
        QCOMPARE(m_socketError.count(), 0);
        QCOMPARE(m_socket->state(), QAbstractSocket::ConnectedState);
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Instance"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("A"));
        obj.insert(QLatin1String("Parameters"), params);
        QByteArray json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        QList<QVariant> arguments = m_textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// Testing creating second instance with the same name,
        /// command should fail
        m_socket->sendTextMessage(json);
        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Testing creating second instance with the another name,
        params[QLatin1String("name")] = QLatin1String("B");
        obj[QLatin1String("Parameters")] = params;
        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// Testing creating third instance at pos 1
        params[QLatin1String("name")] = QLatin1String("C");
        params[QLatin1String("kind")] = QLatin1String("instaaa");
        params[QLatin1String("pos")] = 1;
        obj[QLatin1String("Parameters")] = params;
        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        msc::MscChart *chart = m_model->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
        msc::MscInstance *instanceC = chart->instances().at(1);
        QCOMPARE(instanceC->name(), QString("C"));
        QCOMPARE(instanceC->kind(), QString("instaaa"));
    }

    void testMessageCommand()
    {
        QCOMPARE(m_socket->state(), QAbstractSocket::ConnectedState);

        /// Message command without any instanceName should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Message"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("MSG"));
        obj.insert(QLatin1String("Parameters"), params);
        QByteArray json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        QList<QVariant> arguments = m_textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding at least one instanceName (source) to make it valid
        params.insert(QLatin1String("srcName"), QLatin1String("A"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("dstName"), QLatin1String("XY"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding target instanceName to make command valid
        params.insert(QLatin1String("dstName"), QLatin1String("B"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// Adding msc::MscMessage::MessageType::Create
        params.insert(QLatin1String("MessageType"), QLatin1String("Create"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testTimerCommand()
    {
        QCOMPARE(m_socket->state(), QAbstractSocket::ConnectedState);

        /// Timer command without any instanceName should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Timer"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("Timer"));
        obj.insert(QLatin1String("Parameters"), params);
        QByteArray json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        QList<QVariant> arguments = m_textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("instanceName"), QLatin1String("C"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding valid instanceName but without setting any Timer Type
        /// it should fallback to msc::MscTimer::TimerType::Unknown and fail
        params[QLatin1String("instanceName")] = QLatin1String("A");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Various Timer types
        /// msc::MscTimer::TimerType::Start
        params[QLatin1String("TimerType")] = QLatin1String("Start");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// msc::MscTimer::TimerType::Stop
        params[QLatin1String("TimerType")] = QLatin1String("Stop");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// msc::MscTimer::TimerType::Timeout
        params[QLatin1String("TimerType")] = QLatin1String("Timeout");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// msc::MscTimer::TimerType::Unknown
        params[QLatin1String("TimerType")] = QLatin1String("NonExistantType");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());
    }

    void testActionCommand()
    {
        QCOMPARE(m_socket->state(), QAbstractSocket::ConnectedState);

        /// Action command without any instanceName should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Action"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("Action"));
        obj.insert(QLatin1String("Parameters"), params);
        QByteArray json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        QList<QVariant> arguments = m_textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("instanceName"), QLatin1String("XY"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding valid instanceName to make command successful
        params[QLatin1String("instanceName")] = QLatin1String("B");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testConditionCommand()
    {
        QCOMPARE(m_socket->state(), QAbstractSocket::ConnectedState);

        /// Condition command without any instanceName should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Condition"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("Condition"));
        obj.insert(QLatin1String("Parameters"), params);
        QByteArray json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        QList<QVariant> arguments = m_textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("instanceName"), QLatin1String("XY"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding valid instanceName to make command successful
        params[QLatin1String("instanceName")] = QLatin1String("B");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testUndoRedoCommand()
    {
        QCOMPARE(m_socket->state(), QAbstractSocket::ConnectedState);

        /// Redo command should fail cause no one Undo was done
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Redo"));
        QByteArray json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        QList<QVariant> arguments = m_textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Undo
        obj[QLatin1String("CommandType")] = QLatin1String("Undo");
        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// Redo
        obj[QLatin1String("CommandType")] = QLatin1String("Redo");
        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testSaveCommand()
    {
        QCOMPARE(m_socket->state(), QAbstractSocket::ConnectedState);

        /// Save command should fail because no filename set
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Save"));
        QByteArray json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        QList<QVariant> arguments = m_textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        QJsonObject params;
        params.insert(QLatin1String("fileName"), QLatin1String("remote_control.msc"));
        obj.insert(QLatin1String("Parameters"), params);
        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        arguments = m_textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void cleanupTestCase()
    {
        m_socket->close();
        delete m_socket;
        m_socket = nullptr;

        delete m_server;
        m_server = nullptr;

        delete m_handler;
        m_handler = nullptr;

        delete m_model;
        m_model = nullptr;
    }

private:
    msc::RemoteControlWebServer *m_server = nullptr;
    msc::RemoteControlHandler *m_handler = nullptr;
    msc::MainModel *m_model = nullptr;
    QWebSocket *m_socket = nullptr;
    QSignalSpy m_socketConnectedSpy;
    QSignalSpy m_textMessageReceived;
    QSignalSpy m_socketError;
};

QTEST_MAIN(tst_RemoteControl)

#include "tst_remotecontrol.moc"
