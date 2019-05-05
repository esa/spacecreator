/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscentity.h"
#include "mscmessage.h"

#include <QObject>
#include <QRectF>
#include <QString>
#include <QVector>

namespace msc {

class MscAction;
class MscCondition;
class MscDocument;
class MscInstance;
class MscInstanceEvent;
class MscGate;
class MscTimer;

class MscChart : public MscEntity
{
    Q_OBJECT
public:
    explicit MscChart(QObject *parent = nullptr);
    explicit MscChart(const QString &name, QObject *parent = nullptr);
    ~MscChart() override;

    const QVector<MscInstance *> &instances() const;
    MscInstance *makeInstance(const QString &name = QString(), int index = -1);
    void addInstance(MscInstance *instance, int index = -1);
    void removeInstance(MscInstance *instance);

    MscInstance *instanceByName(const QString &name) const;

    const QVector<MscInstanceEvent *> &instanceEvents() const;
    QVector<MscInstanceEvent *> eventsForInstance(MscInstance *instance) const;
    void addInstanceEvent(MscInstanceEvent *instanceEvent, int eventIndex = -1);
    void removeInstanceEvent(MscInstanceEvent *instanceEvent);
    MscMessage *messageByName(const QString &name) const;

    const QVector<MscGate *> &gates() const;
    void addGate(MscGate *gate);
    void removeGate(MscGate *gate);

    MscDocument *parentDocument() const;

    MscEntity::EntityType entityType() const override;

    void updateInstanceOrder(MscInstance *instance, int pos);
    void updateActionPos(MscAction *action, MscInstance *newInstance, int eventPos);
    void updateConditionPos(MscCondition *condition, MscInstance *newInstance, int eventPos);
    void updateTimerPos(MscTimer *timer, MscInstance *newInstance, int eventPos);
    void updateMessageTarget(MscMessage *message, MscInstance *newInstance, int eventPos, MscMessage::EndType endType);

    QRect cifRect() const;
    void setCifRect(const QRect &rect);
    int maxInstanceNameNumber() const;
    int setInstanceNameNumbers(int nextNumber);

    QString createUniqueInstanceName() const;

public Q_SLOTS:
    void resetTimerRelations(MscTimer *timer);

Q_SIGNALS:
    void instanceAdded(MscInstance *instance, int pos);
    void instanceRemoved(MscInstance *instance);
    void instanceOrderChanged(MscInstance *instance, int from, int to);
    void instanceEventAdded(MscInstanceEvent *message);
    void instanceEventRemoved(MscInstanceEvent *message);
    void eventMoved();
    void messageRetargeted();
    void gateAdded(MscGate *gate);
    void gateRemoved(MscGate *gate);
    void rectChanged();
    void globalCommentTextChanged(const QString &text, const QString &hash);
    void globalCommentRectChanged(const QRect rect, const QString &hash);

private:
    bool moveEvent(MscInstanceEvent *event, int newIndex);

    /*!
       Return a QVector with all events in this chart of the given type
     */
    template<typename T>
    QVector<T *> allEvents()
    {
        QVector<T *> events;
        for (auto ev : m_instanceEvents) {
            if (auto obj = qobject_cast<T *>(ev))
                events.append(obj);
        }
        return events;
    }

    QVector<MscInstance *> m_instances;
    QVector<MscInstanceEvent *> m_instanceEvents;
    QVector<MscGate *> m_gates;

    cif::CifBlockShared cifMscDoc() const;
};

} // namespace msc
