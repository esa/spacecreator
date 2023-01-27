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
#include "mscchart.h"
#include "msccommandsstack.h"
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
    enum class SendResult
    {
        EXPECT_PASS = 0,
        EXPECT_FAIL = 1
    };

    tst_RemoteControl()
        : m_server(new msc::RemoteControlWebServer(this))
        , m_handler(new msc::RemoteControlHandler(this))
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

        connect(m_server, &msc::RemoteControlWebServer::executeCommand, m_handler,
                &msc::RemoteControlHandler::handleRemoteCommand);
        connect(m_handler, &msc::RemoteControlHandler::commandDone, m_server,
                &msc::RemoteControlWebServer::commandDone);

        m_server->start(kPort);
    }

    void init()
    {
        m_dataModel = msc::MscModel::defaultModel();
        m_undoStack = std::make_unique<msc::MscCommandsStack>();
        m_layoutManager = std::make_unique<msc::ChartLayoutManager>(m_undoStack.get());
        m_layoutManager->setCurrentChart(m_dataModel->firstChart());
        m_handler->setMscModel(m_dataModel.get());
        m_handler->setUndoStack(m_undoStack->undoStack());
        m_handler->setLayoutManager(m_layoutManager.get());
        m_handler->setChart(m_dataModel->firstChart());

        m_view.setScene(m_layoutManager->graphicsScene());
        msc::CoordinatesConverter::init(m_layoutManager->graphicsScene(), &m_chartItem);
        auto converter = msc::CoordinatesConverter::instance();
        converter->setDPI(QPointF(109., 109.), QPointF(96., 96.));

        m_textMessageReceived.clear();
        m_socketError.clear();

        m_socket->open(QUrl(QStringLiteral("ws://localhost:%1").arg(kPort)));
        QTRY_COMPARE(m_socket->state(), QAbstractSocket::ConnectedState);
    }

    void cleanup() { m_socket->close(); }

    void testInstanceCommand()
    {
        QCOMPARE(m_socketError.count(), 0);
        QCOMPARE(m_socket->state(), QAbstractSocket::ConnectedState);
        sendInstance("A");
        QCOMPARE(m_layoutManager->instanceItems().size(), 1);

        // Testing creating second instance with the same name,
        // command should fail
        sendInstance("A", "", -1, SendResult::EXPECT_FAIL);

        // Testing creating second instance with the another name,
        sendInstance("B");
        QCOMPARE(m_layoutManager->instanceItems().size(), 2);

        // Testing creating third instance at pos 1 - pushing instance B to the right - result (A C B)
        const QVector<msc::InstanceItem *> &instanceItems = m_layoutManager->instanceItems();
        const int positionC = instanceItems.at(1)->sceneBoundingRect().toRect().x() - 1;
        sendInstance("C", "instaaa", positionC);
        QCOMPARE(m_layoutManager->instanceItems().size(), 3);

        msc::MscChart *chart = m_dataModel->firstChart();
        msc::MscInstance *instanceC = chart->instances().at(1);
        QCOMPARE(instanceC->name(), QString("C"));
        QCOMPARE(instanceC->kind(), QString("instaaa"));

        // Check position of instance C
        msc::InstanceItem *itemC = m_layoutManager->itemForInstance(instanceC);
        const QRectF itemCRect = itemC->sceneBoundingRect();
        QCOMPARE(itemCRect.toRect().x(), positionC);

        // Check position of instance B it's pushed to the right
        msc::MscInstance *instanceB = chart->instances().at(2);
        QCOMPARE(instanceB->name(), QString("B"));
        msc::InstanceItem *itemB = m_layoutManager->itemForInstance(instanceB);
        const QRectF itemBRect = itemB->sceneBoundingRect();
        QVERIFY(itemCRect.right() < itemBRect.left());
    }

    void testMessageCommand()
    {
        addTestInstances();

        // Message command without any instanceName should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Message"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("MSG"));
        obj.insert(QLatin1String("Parameters"), params);
        sendCommand(obj, SendResult::EXPECT_FAIL);

        // Adding at least one instanceName (source) to make it valid
        params.insert(QLatin1String("srcName"), QLatin1String("A"));
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj);

        // Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("dstName"), QLatin1String("XY"));
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj, SendResult::EXPECT_FAIL);

        // Adding target instanceName to make command valid
        params.insert(QLatin1String("dstName"), QLatin1String("B"));
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj);
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
        sendCommand(obj);
    }

    void testTimerCommand()
    {
        addTestInstances();

        // Timer command without any instanceName should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Timer"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("Timer"));
        obj.insert(QLatin1String("Parameters"), params);
        sendCommand(obj, SendResult::EXPECT_FAIL);

        // Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("instanceName"), QLatin1String("C"));
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj, SendResult::EXPECT_FAIL);

        // Adding valid instanceName but without setting any Timer Type
        // it should fallback to msc::MscTimer::TimerType::Unknown and fail
        params[QLatin1String("instanceName")] = QLatin1String("A");
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj, SendResult::EXPECT_FAIL);

        // Various Timer types
        // msc::MscTimer::TimerType::Start
        params[QLatin1String("TimerType")] = QLatin1String("Start");
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj);

        // msc::MscTimer::TimerType::Stop
        params[QLatin1String("TimerType")] = QLatin1String("Stop");
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj);

        // msc::MscTimer::TimerType::Timeout
        params[QLatin1String("TimerType")] = QLatin1String("Timeout");
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj);

        // msc::MscTimer::TimerType::Unknown
        params[QLatin1String("TimerType")] = QLatin1String("NonExistantType");
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj, SendResult::EXPECT_FAIL);
    }

    void testActionCommand()
    {
        addTestInstances();

        // Action command without any instanceName should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Action"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("Action"));
        obj.insert(QLatin1String("Parameters"), params);
        sendCommand(obj, SendResult::EXPECT_FAIL);

        // Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("instanceName"), QLatin1String("XY"));
        obj[QLatin1String("Parameters")] = params;

        sendCommand(obj, SendResult::EXPECT_FAIL);

        // Adding valid instanceName to make command successful
        params[QLatin1String("instanceName")] = QLatin1String("B");
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj);
    }

    void testConditionCommand()
    {
        addTestInstances();

        // Condition command without any instanceName should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Condition"));
        QJsonObject params;
        params.insert(QLatin1String("name"), QLatin1String("Condition"));
        obj.insert(QLatin1String("Parameters"), params);
        sendCommand(obj, SendResult::EXPECT_FAIL);

        // Adding nonexistent instanceName to make command invalid
        params.insert(QLatin1String("instanceName"), QLatin1String("XY"));
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj, SendResult::EXPECT_FAIL);

        // Adding valid instanceName to make command successful
        params[QLatin1String("instanceName")] = QLatin1String("B");
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj);
    }

    void testMessageDeclarationCommand()
    {
        msc::MscDocument *mainDocument = m_dataModel->documents().at(0);
        QCOMPARE(mainDocument->declarations().size(), 0);

        // Message declaration command without any name should fail
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("MessageDeclaration"));
        QJsonObject params;
        obj.insert(QLatin1String("Parameters"), params);
        sendCommand(obj, SendResult::EXPECT_FAIL);
        QCOMPARE(mainDocument->declarations().size(), 0);

        // Add a real declaration
        params.insert(QLatin1String("names"), QLatin1String("reset"));
        params.insert(QLatin1String("typeRefList"), QLatin1String("Int32, UInt16"));
        obj[QLatin1String("Parameters")] = params;
        sendCommand(obj);
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
        sendCommand(objAdd);

        msc::MscChart *chart = m_dataModel->firstChart();
        QCOMPARE(chart->instanceEvents().size(), 1);

        // Redo command should fail cause no one Undo was done
        sendRedo(SendResult::EXPECT_FAIL);
        QCOMPARE(chart->instanceEvents().size(), 1);

        // Undo
        sendUndo();
        QCOMPARE(chart->instanceEvents().size(), 0);

        // Redo
        sendRedo();
        QCOMPARE(chart->instanceEvents().size(), 1);
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
        msc::MscChart *chart = m_dataModel->documents().at(0)->documents().at(0)->charts().at(0);
        auto instanceA = new msc::MscInstance("A");
        chart->addInstance(instanceA);
        auto instanceB = new msc::MscInstance("B");
        chart->addInstance(instanceB);
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

    void sendInstance(const QString &name, const QString &kind = QString(), int pos = -1,
            SendResult passes = SendResult::EXPECT_PASS)
    {
        QJsonObject obj;
        obj.insert(QLatin1String("CommandType"), QLatin1String("Instance"));
        QJsonObject params;
        params[QLatin1String("name")] = name;
        if (!kind.isEmpty()) {
            params[QLatin1String("kind")] = kind;
        }
        if (pos >= 0) {
            params[QLatin1String("pos")] = pos;
        }
        obj.insert(QLatin1String("Parameters"), params);
        sendCommand(obj, passes);
    }

    void sendUndo(SendResult passes = SendResult::EXPECT_PASS)
    {
        QJsonObject obj;
        obj[QLatin1String("CommandType")] = QLatin1String("Undo");
        sendCommand(obj, passes);
    }

    void sendRedo(SendResult passes = SendResult::EXPECT_PASS)
    {
        QJsonObject obj;
        obj[QLatin1String("CommandType")] = QLatin1String("Redo");
        sendCommand(obj, passes);
    }

    void waitForLayoutUpdate()
    {
        QApplication::processEvents();
        int count = 0;
        while (m_layoutManager->layoutUpdatePending() && count < 200) {
            QTest::qWait(1);
            QApplication::processEvents();
            ++count;
        }
    }

    msc::RemoteControlWebServer *m_server = nullptr;
    msc::RemoteControlHandler *m_handler = nullptr;
    std::unique_ptr<msc::MscModel> m_dataModel;
    std::unique_ptr<msc::MscCommandsStack> m_undoStack;
    std::unique_ptr<msc::ChartLayoutManager> m_layoutManager;
    QWebSocket *m_socket = nullptr;
    QSignalSpy m_textMessageReceived;
    QSignalSpy m_socketError;
    QGraphicsView m_view;
    msc::ChartItem m_chartItem;
};

QTEST_MAIN(tst_RemoteControl)

#include "tst_remotecontrol.moc"
