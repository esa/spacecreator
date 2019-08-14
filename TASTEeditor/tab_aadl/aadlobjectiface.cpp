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

#include "aadlobjectiface.h"

namespace taste3 {
namespace aadl {

struct AADLObjectIfacePrivate {

    AADLObjectIfacePrivate(AADLObjectIface::IfaceType dir, AADLObject *parent)
        : m_direction(dir)
        , m_holder(parent)
    {
    }
    const AADLObjectIface::IfaceType m_direction;
    const QPointer<AADLObject> m_holder;
    QString m_kind = {};
    qint32 m_period = 0;
    qint32 m_wcet = 0;
    qint32 m_queueSize = 0;

    QVector<IfaceParam> m_paramasIn = {};
    QVector<IfaceParam> m_paramasOut = {};

    QString m_rcmOperationKind;
    QString m_deadline;
    QString m_rcmPeriod;
    QVector<qint32> m_coordinates;
    QString m_interfaceName;
    bool m_labelInheritance;
};

AADLObjectIface::AADLObjectIface(AADLObjectIface::IfaceType direction, const QString &title, AADLObject *parent)
    : AADLObject(title, parent)
    , d(new AADLObjectIfacePrivate(direction, parent))
{
}

AADLObjectIface::~AADLObjectIface() {}

AADLObject::AADLObjectType AADLObjectIface::aadlType() const
{
    return AADLObjectType::AADLIface;
}

AADLObjectIface::IfaceType AADLObjectIface::direction() const
{
    return d->m_direction;
}

bool AADLObjectIface::isProvided() const
{
    return direction() == IfaceType::Provided;
}
bool AADLObjectIface::isRequired() const
{
    return direction() == IfaceType::Required;
}

AADLObject *AADLObjectIface::holder() const
{
    return d->m_holder;
}

QString AADLObjectIface::kind() const
{
    return d->m_kind;
}

bool AADLObjectIface::setKind(const QString &kind)
{
    if (d->m_kind != kind) {
        d->m_kind = kind;
        return true;
    }

    return false;
}

qint32 AADLObjectIface::period() const
{
    return d->m_period;
}

bool AADLObjectIface::setPeriod(qint32 period)
{
    if (period != d->m_period) {
        d->m_period = period;
        return true;
    }

    return false;
}

qint32 AADLObjectIface::wcet() const
{
    return d->m_wcet;
}

bool AADLObjectIface::setWcet(qint32 wcet)
{
    if (d->m_wcet != wcet) {
        d->m_wcet = wcet;
        return true;
    }
    return false;
}

qint32 AADLObjectIface::queueSize() const
{
    return d->m_queueSize;
}

bool AADLObjectIface::setQueueSize(qint32 size)
{
    if (d->m_queueSize != size) {
        d->m_queueSize = size;
        return true;
    }
    return false;
}

QVector<IfaceParam> AADLObjectIface::paramsIn() const
{
    return d->m_paramasIn;
}

void AADLObjectIface::setParamsIn(const QVector<IfaceParam> &params) const
{
    d->m_paramasIn = params;
}

void AADLObjectIface::addParamIn(const IfaceParam &param) const
{
    d->m_paramasIn.append(param);
}

QVector<IfaceParam> AADLObjectIface::paramsOut() const
{
    return d->m_paramasOut;
}

void AADLObjectIface::setParamsOut(const QVector<IfaceParam> &params) const
{
    d->m_paramasOut = params;
}

void AADLObjectIface::addParamOut(const IfaceParam &param) const
{
    d->m_paramasOut.append(param);
}

QString AADLObjectIface::rcmOperationKind() const
{
    return d->m_rcmOperationKind;
}

bool AADLObjectIface::setRcmOperationKind(const QString &kind)
{
    if (rcmOperationKind() != kind) {
        d->m_rcmOperationKind = kind;
        return true;
    }
    return false;
}

QString AADLObjectIface::deadline() const
{
    return d->m_deadline;
}

bool AADLObjectIface::setDeadline(const QString &deadline)
{
    if (this->deadline() != deadline) {
        d->m_deadline = deadline;
        return true;
    }
    return false;
}

QString AADLObjectIface::rcmPeriod() const
{
    return d->m_rcmPeriod;
}

bool AADLObjectIface::setRcmPeriod(const QString &period)
{
    if (rcmPeriod() != period) {
        d->m_rcmPeriod = period;
        return true;
    }
    return false;
}

QVector<qint32> AADLObjectIface::coordinates() const
{
    return d->m_coordinates;
}

bool AADLObjectIface::setCoordinates(const QVector<qint32> &coords)
{
    if (coordinates() != coords) {
        d->m_coordinates = coords;
        return true;
    }
    return false;
}

QString AADLObjectIface::interfaceName() const
{
    return d->m_interfaceName;
}

bool AADLObjectIface::setInterfaceName(const QString &name)
{
    if (interfaceName() != name) {
        d->m_interfaceName = name;
        return true;
    }
    return false;
}

bool AADLObjectIface::labelInheritance() const
{
    return d->m_labelInheritance;
}

bool AADLObjectIface::setLabelInheritance(bool label)
{
    if (labelInheritance() != label) {
        d->m_labelInheritance = label;
        return true;
    }
    return false;
}

AADLObjectIfaceProvided::AADLObjectIfaceProvided(AADLObject *parent)
    : AADLObjectIface(IfaceType::Provided, QString(), parent)
{
}

AADLObjectIfaceRequired::AADLObjectIfaceRequired(AADLObject *parent)
    : AADLObjectIface(IfaceType::Required, QString(), parent)
{
}

} // ns aadl
} // ns taste3
