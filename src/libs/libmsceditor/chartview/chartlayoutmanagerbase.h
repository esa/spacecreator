/*
   Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "msccommandsstack.h"
#include "systemchecks.h"

#include <QObject>
#include <QPointer>

namespace msc {

class ActionItem;
class ChartItem;
class CommentItem;
class ConditionItem;
class CoregionItem;
class InstanceItem;
class InteractiveObject;
class MessageItem;
class TimerItem;

class MscCommandsStack;
class MscComment;
class MscAction;
class MscCondition;
class MscCoregion;
class MscEntity;
class MscInstance;
class MscInstanceEvent;
class MscTimer;

class ChartLayoutManagerBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(msc::SystemChecks *systeckchecker READ systemChecker WRITE setSystemChecker NOTIFY systemCheckerChanged)

public:
    explicit ChartLayoutManagerBase(MscCommandsStack *undoStack, QObject *parent = nullptr);

    MscCommandsStack *undoStack() const;

    void setSystemChecker(SystemChecks *checker);
    SystemChecks *systemChecker() const;

    virtual ChartItem *itemForChart() const = 0;
    virtual CommentItem *itemForComment(MscComment *comment) const = 0;
    virtual InstanceItem *itemForInstance(MscInstance *instance) const = 0;
    virtual MessageItem *itemForMessage(MscMessage *message) const = 0;
    virtual ConditionItem *itemForCondition(MscCondition *condition) const = 0;
    virtual ActionItem *itemForAction(MscAction *action) const = 0;
    virtual TimerItem *itemForTimer(MscTimer *timer) const = 0;
    virtual CoregionItem *itemForCoregion(MscCoregion *coregion) const = 0;
    virtual InteractiveObject *itemForEntity(MscEntity *entity) const = 0;

    virtual int eventInstanceIndex(
            const QPointF &pt, MscInstance *instance, MscInstanceEvent *ignoreEvent = nullptr) = 0;
    virtual msc::MscInstanceEvent *eventAtPosition(const QPointF &pos) = 0;

    virtual QRectF minimalContentRect() const = 0;
    virtual QRectF actualContentRect() const = 0;

Q_SIGNALS:
    void systemCheckerChanged(msc::SystemChecks *checker);

private:
    QPointer<MscCommandsStack> m_undoStack;
    QPointer<SystemChecks> m_systemChecker;
};

} // namespace msc
