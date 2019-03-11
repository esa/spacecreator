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

#include <QCoreApplication>
#include <QWebSocket>
#include <QJsonDocument>
#include <QtTest>

#include "mainwindow.h"

static const int kPort = 34622;

class tst_RemoteControl : public QObject
{
    Q_OBJECT
public:
private Q_SLOTS:

    void initTestCase() {
        window = new MainWindow;
        QVERIFY(window->processCommandLineArg(CommandLineParser::Positional::StartRemoteControl,
                                      QString::number(34622)));
        socket = new QWebSocket(QString(), QWebSocketProtocol::Version::VersionLatest, window);
    }

    void testInstanceCommand() {
        QSignalSpy socketConnectedSpy(socket, SIGNAL(connected()));
        QSignalSpy textMessageReceived(socket, SIGNAL(textMessageReceived(QString)));
        QSignalSpy socketError(socket, SIGNAL(error(QAbstractSocket::SocketError)));

        socket->open(QUrl(QStringLiteral("ws://localhost:%1").arg(kPort)));

        QTRY_COMPARE(socketConnectedSpy.count(), 1);
        QCOMPARE(socketError.count(), 0);
        QCOMPARE(socket->state(), QAbstractSocket::ConnectedState);
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Instance"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("A"));
        obj.insert(QLatin1String("Parameters"), params);
        QByteArray json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        QList<QVariant> arguments = textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// Testing creating second instance with the same name,
        /// command should fail
        socket->sendTextMessage(json);
        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Testing creating second instance with the another name,
        params[QLatin1String("name")] = QLatin1String("B");
        obj[QLatin1String("Parameters")] = params;
        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testMessageCommand() {
        QCOMPARE(socket->state(), QAbstractSocket::ConnectedState);

        QSignalSpy socketConnectedSpy(socket, SIGNAL(connected()));
        QSignalSpy textMessageReceived(socket, SIGNAL(textMessageReceived(QString)));
        QSignalSpy socketError(socket, SIGNAL(error(QAbstractSocket::SocketError)));

        /// Message command without any instanceName should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Message"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("MSG"));
        obj.insert(QLatin1String("Parameters"), params);
        QByteArray json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        QList<QVariant> arguments = textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding at least one instanceName (source) to make it valid
        params.insert(QLatin1String("srcName"), QLatin1String("A"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("dstName"), QLatin1String("C"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding target instanceName to make command valid
        params.insert(QLatin1String("dstName"), QLatin1String("B"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// Adding msc::MscMessage::MessageType::Create
        params.insert(QLatin1String("MessageType"), QLatin1String("Create"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testTimerCommand() {
        QCOMPARE(socket->state(), QAbstractSocket::ConnectedState);

        QSignalSpy socketConnectedSpy(socket, SIGNAL(connected()));
        QSignalSpy textMessageReceived(socket, SIGNAL(textMessageReceived(QString)));
        QSignalSpy socketError(socket, SIGNAL(error(QAbstractSocket::SocketError)));

        /// Timer command without any instanceName should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Timer"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("Timer"));
        obj.insert(QLatin1String("Parameters"), params);
        QByteArray json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        QList<QVariant> arguments = textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("instanceName"), QLatin1String("C"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding valid instanceName but without setting any Timer Type
        /// it should fallback to msc::MscTimer::TimerType::Unknown and fail
        params[QLatin1String("instanceName")] = QLatin1String("A");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Various Timer types
        /// msc::MscTimer::TimerType::Start
        params[QLatin1String("TimerType")] = QLatin1String("Start");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// msc::MscTimer::TimerType::Stop
        params[QLatin1String("TimerType")] = QLatin1String("Stop");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// msc::MscTimer::TimerType::Timeout
        params[QLatin1String("TimerType")] = QLatin1String("Timeout");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// msc::MscTimer::TimerType::Unknown
        params[QLatin1String("TimerType")] = QLatin1String("NonExistantType");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());
    }

    void testActionCommand() {
        QCOMPARE(socket->state(), QAbstractSocket::ConnectedState);

        QSignalSpy socketConnectedSpy(socket, SIGNAL(connected()));
        QSignalSpy textMessageReceived(socket, SIGNAL(textMessageReceived(QString)));
        QSignalSpy socketError(socket, SIGNAL(error(QAbstractSocket::SocketError)));

        /// Action command without any instanceName should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Action"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("Action"));
        obj.insert(QLatin1String("Parameters"), params);
        QByteArray json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        QList<QVariant> arguments = textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("instanceName"), QLatin1String("C"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding valid instanceName to make command successful
        params[QLatin1String("instanceName")] = QLatin1String("B");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testConditionCommand() {
        QCOMPARE(socket->state(), QAbstractSocket::ConnectedState);

        QSignalSpy socketConnectedSpy(socket, SIGNAL(connected()));
        QSignalSpy textMessageReceived(socket, SIGNAL(textMessageReceived(QString)));
        QSignalSpy socketError(socket, SIGNAL(error(QAbstractSocket::SocketError)));

        /// Condition command without any instanceName should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Condition"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("Condition"));
        obj.insert(QLatin1String("Parameters"), params);
        QByteArray json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        QList<QVariant> arguments = textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("instanceName"), QLatin1String("C"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding valid instanceName to make command successful
        params[QLatin1String("instanceName")] = QLatin1String("B");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testUndoRedoCommand() {
        QCOMPARE(socket->state(), QAbstractSocket::ConnectedState);

        QSignalSpy socketConnectedSpy(socket, SIGNAL(connected()));
        QSignalSpy textMessageReceived(socket, SIGNAL(textMessageReceived(QString)));
        QSignalSpy socketError(socket, SIGNAL(error(QAbstractSocket::SocketError)));

        /// Redo command should fail cause no one Undo was done
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Redo"));
        QByteArray json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        QList<QVariant> arguments = textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Undo
        obj[QLatin1String("CommandType")] = QLatin1String("Undo");
        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// Redo
        obj[QLatin1String("CommandType")] = QLatin1String("Redo");
        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testSaveCommand() {
        QCOMPARE(socket->state(), QAbstractSocket::ConnectedState);

        QSignalSpy socketConnectedSpy(socket, SIGNAL(connected()));
        QSignalSpy textMessageReceived(socket, SIGNAL(textMessageReceived(QString)));
        QSignalSpy socketError(socket, SIGNAL(error(QAbstractSocket::SocketError)));

        /// Save command should fail because no filename set
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Save"));
        QByteArray json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        QList<QVariant> arguments = textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();

        QJsonObject resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        QJsonObject params;
        params.insert(QLatin1String("fileName"), QLatin1String("remote_control.msc"));
        obj.insert(QLatin1String("Parameters"), params);
        json = QJsonDocument(obj).toJson();
        socket->sendTextMessage(json);

        QVERIFY(textMessageReceived.wait(1000));
        QCOMPARE(textMessageReceived.count(), 1);
        arguments = textMessageReceived.takeFirst();
        messageReceived = arguments.at(0).toString();

        resultObj = QJsonDocument::fromJson(messageReceived.toUtf8()).object();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void cleanupTestCase() {
        socket->close();

        delete window;
        window = nullptr;
    }

private:
    MainWindow *window;
    QWebSocket *socket;
};


QTEST_MAIN(tst_RemoteControl)

#include "tst_remotecontrol.moc"
