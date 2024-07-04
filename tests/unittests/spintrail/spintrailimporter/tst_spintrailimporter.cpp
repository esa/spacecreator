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
#include <spintrail/SpinTrailModel/resettimerevent.h>
#include <spintrail/SpinTrailModel/settimerevent.h>
#include <spintrail/SpinTrailModel/spintrailmodel.h>
#include <spintrail/SpinTrailOptions/options.h>

namespace spintrail::test {
using conversion::spintrail::SpinTrailOptions;
using spintrail::importer::SpinTrailImporter;
using spintrail::model::ChannelEvent;
using spintrail::model::ContinuousSignal;
using spintrail::model::ResetTimerEvent;
using spintrail::model::SetTimerEvent;
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
        QCOMPARE(event.getChannelName(), "Actuator_reset_acc_channel");
        QCOMPARE(event.getParameters().size(), 1);
        QCOMPARE(event.getParameters().front(), "0");
    }
    {
        const std::unique_ptr<TrailEvent> &trailEvent = *iter;
        ++iter;
        QCOMPARE(trailEvent->getEventType(), TrailEvent::EventType::CONTINUOUS_SIGNAL);
        const ContinuousSignal &event = dynamic_cast<const ContinuousSignal &>(*trailEvent);
        QCOMPARE(event.getFunctionName(), "Actuator");
    }
    {
        const std::unique_ptr<TrailEvent> &trailEvent = *iter;
        ++iter;
        QCOMPARE(trailEvent->getEventType(), TrailEvent::EventType::SET_TIMER_EVENT);
        const SetTimerEvent &event = dynamic_cast<const SetTimerEvent &>(*trailEvent);
        QCOMPARE(event.getFunctionName(), "actuator");
        QCOMPARE(event.getTimerName(), "trigger");
        QCOMPARE(event.getInterval(), 1000);
    }
    {
        const std::unique_ptr<TrailEvent> &trailEvent = *iter;
        ++iter;
        QCOMPARE(trailEvent->getEventType(), TrailEvent::EventType::RESET_TIMER_EVENT);
        const ResetTimerEvent &event = dynamic_cast<const ResetTimerEvent &>(*trailEvent);
        QCOMPARE(event.getFunctionName(), "actuator");
        QCOMPARE(event.getTimerName(), "trigger");
    }
    {
        const std::unique_ptr<TrailEvent> &trailEvent = *iter;
        ++iter;
        QCOMPARE(trailEvent->getEventType(), TrailEvent::EventType::CHANNEL_EVENT);
        const ChannelEvent &event = dynamic_cast<const ChannelEvent &>(*trailEvent);
        QCOMPARE(event.getType(), ChannelEvent::Type::Send);
        QCOMPARE(event.getChannelName(), "Actuator_work_channel");
        QCOMPARE(event.getParameters().size(), 1);
        QCOMPARE(event.getParameters().front(), "11.300000");
    }
    {
        const std::unique_ptr<TrailEvent> &trailEvent = *iter;
        ++iter;
        QCOMPARE(trailEvent->getEventType(), TrailEvent::EventType::CHANNEL_EVENT);
        const ChannelEvent &event = dynamic_cast<const ChannelEvent &>(*trailEvent);
        QCOMPARE(event.getType(), ChannelEvent::Type::Recv);
        QCOMPARE(event.getChannelName(), "Actuator_work_channel");
        QCOMPARE(event.getParameters().size(), 1);
        QCOMPARE(event.getParameters().front(), "11.300000");
    }
}
}

QTEST_MAIN(spintrail::test::tst_SpinTrailImporter)

#include "tst_spintrailimporter.moc"
