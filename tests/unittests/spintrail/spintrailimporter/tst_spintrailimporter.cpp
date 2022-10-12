/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include <QObject>
#include <QtTest>
#include <spintrail/SpinTrailImporter/spintrailimporter.h>
#include <spintrail/SpinTrailModel/channelevent.h>
#include <spintrail/SpinTrailModel/continuoussignal.h>
#include <spintrail/SpinTrailModel/spintrailmodel.h>
#include <spintrail/SpinTrailOptions/options.h>

namespace spintrail::test {
using conversion::spintrail::SpinTrailOptions;
using spintrail::importer::SpinTrailImporter;
using spintrail::model::ChannelEvent;
using spintrail::model::ContinuousSignal;
using spintrail::model::SpinTrailModel;
using spintrail::model::TrailEvent;

class tst_SpinTrailImporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testScenario();
};

void tst_SpinTrailImporter::testScenario()
{
    conversion::Options options;
    options.add(SpinTrailOptions::inputFilepath, "scenario.spt");
    SpinTrailImporter importer;

    const std::unique_ptr<conversion::Model> abstractModel = importer.importModel(options);
    QVERIFY(abstractModel != nullptr);
    const SpinTrailModel *model = dynamic_cast<const SpinTrailModel *>(abstractModel.get());
    QVERIFY(model != nullptr);

    QCOMPARE(model->getEvents().size(), 8);

    auto iter = model->getEvents().begin();

    {
        const std::unique_ptr<TrailEvent> &trailEvent = *iter;
        ++iter;
        QCOMPARE(trailEvent->getEventType(), TrailEvent::EventType::CHANNEL_EVENT);
        const ChannelEvent &event = dynamic_cast<const ChannelEvent &>(*trailEvent);
        QCOMPARE(event.getType(), ChannelEvent::Type::Send);
        QCOMPARE(event.getProctypeName(), "");
        QCOMPARE(event.getChannelName(), "Actuator_lock");
        QCOMPARE(event.getParameters().size(), 1);
        QCOMPARE(event.getParameters().front(), "1");
    }
    {
        const std::unique_ptr<TrailEvent> &trailEvent = *iter;
        ++iter;
        QCOMPARE(trailEvent->getEventType(), TrailEvent::EventType::CHANNEL_EVENT);
        const ChannelEvent &event = dynamic_cast<const ChannelEvent &>(*trailEvent);
        QCOMPARE(event.getType(), ChannelEvent::Type::Send);
        QCOMPARE(event.getProctypeName(), "");
        QCOMPARE(event.getChannelName(), "Controller_lock");
        QCOMPARE(event.getParameters().size(), 1);
        QCOMPARE(event.getParameters().front(), "1");
    }
    {
        const std::unique_ptr<TrailEvent> &trailEvent = *iter;
        ++iter;
        QCOMPARE(trailEvent->getEventType(), TrailEvent::EventType::CHANNEL_EVENT);
        const ChannelEvent &event = dynamic_cast<const ChannelEvent &>(*trailEvent);
        QCOMPARE(event.getType(), ChannelEvent::Type::Send);
        QCOMPARE(event.getProctypeName(), "Environ_test");
        QCOMPARE(event.getChannelName(), "Controller_test_channel");
        QCOMPARE(event.getParameters().size(), 1);
        QCOMPARE(event.getParameters().front(), "0");
    }
    {
        const std::unique_ptr<TrailEvent> &trailEvent = *iter;
        ++iter;
        QCOMPARE(trailEvent->getEventType(), TrailEvent::EventType::CHANNEL_EVENT);
        const ChannelEvent &event = dynamic_cast<const ChannelEvent &>(*trailEvent);
        QCOMPARE(event.getType(), ChannelEvent::Type::Recv);
        QCOMPARE(event.getProctypeName(), "Controller_test");
        QCOMPARE(event.getChannelName(), "Controller_lock");
        QCOMPARE(event.getParameters().size(), 1);
        QCOMPARE(event.getParameters().front(), "1");
    }
    {
        const std::unique_ptr<TrailEvent> &trailEvent = *iter;
        ++iter;
        QCOMPARE(trailEvent->getEventType(), TrailEvent::EventType::CHANNEL_EVENT);
        const ChannelEvent &event = dynamic_cast<const ChannelEvent &>(*trailEvent);
        QCOMPARE(event.getType(), ChannelEvent::Type::Recv);
        QCOMPARE(event.getProctypeName(), "Controller_test");
        QCOMPARE(event.getChannelName(), "Controller_test_channel");
        QCOMPARE(event.getParameters().size(), 1);
        QCOMPARE(event.getParameters().front(), "0");
    }
    {
        const std::unique_ptr<TrailEvent> &trailEvent = *iter;
        ++iter;
        QCOMPARE(trailEvent->getEventType(), TrailEvent::EventType::CHANNEL_EVENT);
        const ChannelEvent &event = dynamic_cast<const ChannelEvent &>(*trailEvent);
        QCOMPARE(event.getType(), ChannelEvent::Type::Send);
        QCOMPARE(event.getProctypeName(), "Controller_test");
        QCOMPARE(event.getChannelName(), "Actuator_reset_acc_channel");
        QCOMPARE(event.getParameters().size(), 1);
        QCOMPARE(event.getParameters().front(), "0");
    }
    {
        const std::unique_ptr<TrailEvent> &trailEvent = *iter;
        ++iter;
        QCOMPARE(trailEvent->getEventType(), TrailEvent::EventType::CHANNEL_EVENT);
        const ChannelEvent &event = dynamic_cast<const ChannelEvent &>(*trailEvent);
        QCOMPARE(event.getType(), ChannelEvent::Type::Send);
        QCOMPARE(event.getProctypeName(), "Controller_test");
        QCOMPARE(event.getChannelName(), "Controller_lock");
        QCOMPARE(event.getParameters().size(), 1);
        QCOMPARE(event.getParameters().front(), "1");
    }
    {
        const std::unique_ptr<TrailEvent> &trailEvent = *iter;
        ++iter;
        QCOMPARE(trailEvent->getEventType(), TrailEvent::EventType::CONTINUOUS_SIGNAL);
        const ContinuousSignal &event = dynamic_cast<const ContinuousSignal &>(*trailEvent);
        QCOMPARE(event.getFunctionName(), "Actuator");
    }
}
}

QTEST_MAIN(spintrail::test::tst_SpinTrailImporter)

#include "tst_spintrailimporter.moc"
