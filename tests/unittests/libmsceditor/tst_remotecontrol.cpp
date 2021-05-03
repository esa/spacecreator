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

#include "baseitems/common/coordinatesconverter.h"
#include "chartitem.h"
#include "chartlayoutmanager.h"
#include "instanceitem.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "msclibrary.h"
#include "mscmessagedeclaration.h"
#include "mscmodel.h"
#include "remotecontrolhandler.h"
#include "remotecontrolwebserver.h"
#include "sharedlibrary.h"

#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
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
        , m_textMessageReceived(m_socket, SIGNAL(textMessageReceived(QString)))
        , m_socketError(m_socket, SIGNAL(error(QAbstractSocket::SocketError)))
        , m_chartItem(nullptr, nullptr)
    {
        msc::initMscLibrary();
        m_view.setGeometry(10, 10, 400, 400);
    }

private Q_SLOTS:
    void initTestCase()
    {
        shared::initSharedLibrary();
        m_view.setScene(m_model->graphicsScene());
        msc::CoordinatesConverter::init(m_model->graphicsScene(), &m_chartItem);
        auto converter = msc::CoordinatesConverter::instance();
        converter->setDPI(QPointF(109., 109.), QPointF(96., 96.));

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
        m_textMessageReceived.clear();
        m_socketError.clear();

        m_socket->open(QUrl(QStringLiteral("ws://localhost:%1").arg(kPort)));
        QTRY_COMPARE(m_socket->state(), QAbstractSocket::ConnectedState);
    }

    void cleanup()
    {
        m_socket->close();
        m_model->initialModel();
    }

    void testInstanceCommand()
    {
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
        QJsonObject resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
        waitForLayoutUpdate();
        QCOMPARE(m_model->chartViewModel().instanceItems().size(), 1);

        /// Testing creating second instance with the same name,
        /// command should fail
        m_socket->sendTextMessage(json);
        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());
        waitForLayoutUpdate();
        QCOMPARE(m_model->chartViewModel().instanceItems().size(), 1);

        /// Testing creating second instance with the another name,
        params[QLatin1String("name")] = QLatin1String("B");
        obj[QLatin1String("Parameters")] = params;
        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);
        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
        waitForLayoutUpdate();
        QCOMPARE(m_model->chartViewModel().instanceItems().size(), 2);

        /// Testing creating third instance at pos 1 - pushing instance B to the right - result (A C B)
        params[QLatin1String("name")] = QLatin1String("C");
        params[QLatin1String("kind")] = QLatin1String("instaaa");
        const QVector<msc::InstanceItem *> &instanceItems = m_model->chartViewModel().instanceItems();
        QCOMPARE(instanceItems.size(), 2);
        const int positionC = instanceItems.at(1)->sceneBoundingRect().toRect().x() - 1;
        params[QLatin1String("pos")] = positionC;
        obj[QLatin1String("Parameters")] = params;
        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
        waitForLayoutUpdate();
        QCOMPARE(m_model->chartViewModel().instanceItems().size(), 3);

        msc::MscChart *chart = m_model->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
        msc::MscInstance *instanceC = chart->instances().at(1);
        QCOMPARE(instanceC->name(), QString("C"));
        QCOMPARE(instanceC->kind(), QString("instaaa"));

        // Check position of instance C
        msc::InstanceItem *itemC = m_model->chartViewModel().itemForInstance(instanceC);
        const QRectF itemCRect = itemC->sceneBoundingRect();
        QCOMPARE(itemCRect.toRect().x(), positionC);

        // Check position of instance B it's pushed to the right
        msc::MscInstance *instanceB = chart->instances().at(2);
        QCOMPARE(instanceB->name(), QString("B"));
        msc::InstanceItem *itemB = m_model->chartViewModel().itemForInstance(instanceB);
        const QRectF itemBRect = itemB->sceneBoundingRect();
        QVERIFY(itemCRect.right() < itemBRect.left());
    }

    void testMessageCommand()
    {
        addTestInstances();

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
        QJsonObject resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding at least one instanceName (source) to make it valid
        params.insert(QLatin1String("srcName"), QLatin1String("A"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("dstName"), QLatin1String("XY"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding target instanceName to make command valid
        params.insert(QLatin1String("dstName"), QLatin1String("B"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testCreateCommand()
    {
        addTestInstances();

        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Message"));
        QJsonObject params;
        params.insert(QLatin1String("MessageType"), QLatin1String("Create"));
        params.insert(QLatin1String("name"), QLatin1String("start"));
        params.insert(QLatin1String("srcName"), QLatin1String("A"));
        params.insert(QLatin1String("dstName"), QLatin1String("B"));
        obj[QLatin1String("Parameters")] = params;

        QByteArray json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        QJsonObject resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testTimerCommand()
    {
        addTestInstances();

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
        QJsonObject resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("instanceName"), QLatin1String("C"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding valid instanceName but without setting any Timer Type
        /// it should fallback to msc::MscTimer::TimerType::Unknown and fail
        params[QLatin1String("instanceName")] = QLatin1String("A");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Various Timer types
        /// msc::MscTimer::TimerType::Start
        params[QLatin1String("TimerType")] = QLatin1String("Start");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// msc::MscTimer::TimerType::Stop
        params[QLatin1String("TimerType")] = QLatin1String("Stop");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// msc::MscTimer::TimerType::Timeout
        params[QLatin1String("TimerType")] = QLatin1String("Timeout");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// msc::MscTimer::TimerType::Unknown
        params[QLatin1String("TimerType")] = QLatin1String("NonExistantType");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());
    }

    void testActionCommand()
    {
        addTestInstances();

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
        QJsonObject resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("instanceName"), QLatin1String("XY"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding valid instanceName to make command successful
        params[QLatin1String("instanceName")] = QLatin1String("B");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testConditionCommand()
    {
        addTestInstances();

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
        QJsonObject resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("instanceName"), QLatin1String("XY"));
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Adding valid instanceName to make command successful
        params[QLatin1String("instanceName")] = QLatin1String("B");
        obj[QLatin1String("Parameters")] = params;

        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testMessageDeclarationCommand()
    {
        msc::MscDocument *mainDocument = m_model->mscModel()->documents().at(0);
        QCOMPARE(mainDocument->declarations().size(), 0);

        // Message declaration command without any name should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("MessageDeclaration"));
        QJsonObject params;
        obj.insert(QLatin1String("Parameters"), params);
        QByteArray json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        QJsonObject resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());
        QCOMPARE(mainDocument->declarations().size(), 0);

        // Add a real declaration
        params.insert(QLatin1String("names"), QLatin1String("reset"));
        params.insert(QLatin1String("typeRefList"), QLatin1String("Int32, UInt16"));
        obj[QLatin1String("Parameters")] = params;
        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
        QCOMPARE(mainDocument->declarations().size(), 1);
        msc::MscMessageDeclaration *declaration = mainDocument->declarations().at(0);
        const QStringList expectedNames = { "reset" };
        QCOMPARE(declaration->names(), expectedNames);
        const QStringList expectedTypes = { "Int32", "UInt16" };
        QCOMPARE(declaration->typeRefList(), expectedTypes);
    }

    void testUndoRedoCommand()
    {
        addTestInstances();

        // Add an action
        QJsonObject objAdd;
        objAdd.insert(QLatin1String("CommandType"), QLatin1String("Action"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("Action"));
        params[QLatin1String("instanceName")] = QLatin1String("B");
        objAdd.insert(QLatin1String("Parameters"), params);
        auto jsonDoc = QJsonDocument(objAdd).toJson();
        m_socket->sendTextMessage(jsonDoc);
        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        QJsonObject resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// Redo command should fail cause no one Undo was done
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Redo"));
        QByteArray json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        /// Undo
        obj[QLatin1String("CommandType")] = QLatin1String("Undo");
        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());

        /// Redo
        obj[QLatin1String("CommandType")] = QLatin1String("Redo");
        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

    void testSaveCommand()
    {
        /// Save command should fail because no filename set
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Save"));
        QByteArray json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        QJsonObject resultObj = takeFirstResultMessage();
        QVERIFY(!resultObj.value(QLatin1String("result")).toBool());

        QJsonObject params;
        params.insert(QLatin1String("fileName"), QLatin1String("remote_control.msc"));
        obj.insert(QLatin1String("Parameters"), params);
        json = QJsonDocument(obj).toJson();
        m_socket->sendTextMessage(json);

        QVERIFY(m_textMessageReceived.wait(1000));
        QCOMPARE(m_textMessageReceived.count(), 1);
        resultObj = takeFirstResultMessage();
        QVERIFY(resultObj.value(QLatin1String("result")).toBool());
    }

private:
    QJsonObject takeFirstResultMessage()
    {
        QList<QVariant> arguments = m_textMessageReceived.takeFirst();
        QString messageReceived = arguments.at(0).toString();
        return QJsonDocument::fromJson(messageReceived.toUtf8()).object();
    }

    void addTestInstances()
    {
        msc::MscChart *chart = m_model->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
        auto instanceA = new msc::MscInstance("A");
        chart->addInstance(instanceA);
        auto instanceB = new msc::MscInstance("B");
        chart->addInstance(instanceB);
    }

    void waitForLayoutUpdate()
    {
        QApplication::processEvents();
        int count = 0;
        while (m_model->chartViewModel().layoutUpdatePending() && count < 200) {
            QTest::qWait(1);
            QApplication::processEvents();
            ++count;
        }
    }

    msc::RemoteControlWebServer *m_server = nullptr;
    msc::RemoteControlHandler *m_handler = nullptr;
    msc::MainModel *m_model = nullptr;
    QWebSocket *m_socket = nullptr;
    QSignalSpy m_textMessageReceived;
    QSignalSpy m_socketError;
    QGraphicsView m_view;
    msc::ChartItem m_chartItem;
};

QTEST_MAIN(tst_RemoteControl)

#include "tst_remotecontrol.moc"
