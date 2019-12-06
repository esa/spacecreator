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

#include "aadlcommonprops.h"

namespace taste3 {
namespace aadl {

static int sProvidedCounter = 0;
static int sRequiredCounter = 0;

struct AADLObjectIfacePrivate {
    AADLObjectIfacePrivate(AADLObjectIface::IfaceType dir)
        : m_direction(dir)
    {
    }
    const AADLObjectIface::IfaceType m_direction;
    QVector<IfaceParameter> m_paramasIn = {};
    QVector<IfaceParameter> m_paramasOut = {};
};

AADLObjectIface::AADLObjectIface(AADLObjectIface::IfaceType direction, const QString &title, AADLObject *parent)
    : AADLObject(title, parent)
    , d(new AADLObjectIfacePrivate(direction))
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

QString AADLObjectIface::kind() const
{
    return attr(meta::Props::token(meta::Props::Token::kind)).toString();
}

bool AADLObjectIface::setKind(const QString &kind)
{
    if (this->kind() != kind) {
        setAttr(meta::Props::token(meta::Props::Token::kind), kind);
        return true;
    }

    return false;
}

qint32 AADLObjectIface::period() const
{
    return attr(meta::Props::token(meta::Props::Token::period)).toInt();
}

bool AADLObjectIface::setPeriod(qint32 period)
{
    if (this->period() != period) {
        setAttr(meta::Props::token(meta::Props::Token::period), period);
        return true;
    }

    return false;
}

qint32 AADLObjectIface::wcet() const
{
    return attr(meta::Props::token(meta::Props::Token::wcet)).toInt();
}

bool AADLObjectIface::setWcet(qint32 wcet)
{
    if (this->wcet() != wcet) {
        setAttr(meta::Props::token(meta::Props::Token::wcet), wcet);
        return true;
    }
    return false;
}

qint32 AADLObjectIface::queueSize() const
{
    return attr(meta::Props::token(meta::Props::Token::queue_size)).toInt();
}

bool AADLObjectIface::setQueueSize(qint32 size)
{
    if (queueSize() != size) {
        setAttr(meta::Props::token(meta::Props::Token::queue_size), size);
        return true;
    }
    return false;
}

QVector<IfaceParameter> AADLObjectIface::paramsIn() const
{
    return d->m_paramasIn;
}

void AADLObjectIface::setParamsIn(const QVector<IfaceParameter> &params) const
{
    d->m_paramasIn = params;
}

void AADLObjectIface::addParamIn(const IfaceParameter &param) const
{
    d->m_paramasIn.append(param);
}

QVector<IfaceParameter> AADLObjectIface::paramsOut() const
{
    return d->m_paramasOut;
}

void AADLObjectIface::setParamsOut(const QVector<IfaceParameter> &params) const
{
    d->m_paramasOut = params;
}

void AADLObjectIface::addParamOut(const IfaceParameter &param) const
{
    d->m_paramasOut.append(param);
}

void AADLObjectIface::addParam(const IfaceParameter &param)
{
    if (param.direction() == IfaceParameter::Direction::In)
        addParamIn(param);
    else
        addParamOut(param);
}

QString AADLObjectIface::rcmOperationKind() const
{
    return prop(meta::Props::token(meta::Props::Token::RCMoperationKind)).toString();
}

bool AADLObjectIface::setRcmOperationKind(const QString &kind)
{
    if (rcmOperationKind() != kind) {
        setProp(meta::Props::token(meta::Props::Token::RCMoperationKind), kind);
        return true;
    }
    return false;
}

QString AADLObjectIface::deadline() const
{
    return prop(meta::Props::token(meta::Props::Token::Deadline)).toString();
}

bool AADLObjectIface::setDeadline(const QString &deadline)
{
    if (this->deadline() != deadline) {
        setProp(meta::Props::token(meta::Props::Token::Deadline), deadline);
        return true;
    }
    return false;
}

QString AADLObjectIface::rcmPeriod() const
{
    return prop(meta::Props::token(meta::Props::Token::RCMperiod)).toString();
}

bool AADLObjectIface::setRcmPeriod(const QString &period)
{
    if (rcmPeriod() != period) {
        setProp(meta::Props::token(meta::Props::Token::RCMperiod), period);
        return true;
    }
    return false;
}

QString AADLObjectIface::interfaceName() const
{
    return prop(meta::Props::token(meta::Props::Token::InterfaceName)).toString();
}

bool AADLObjectIface::setInterfaceName(const QString &name)
{
    if (interfaceName() != name) {
        setProp(meta::Props::token(meta::Props::Token::InterfaceName), name);
        return true;
    }
    return false;
}

bool AADLObjectIface::labelInheritance() const
{
    return prop(meta::Props::token(meta::Props::Token::labelInheritance)).toBool();
}

bool AADLObjectIface::setLabelInheritance(bool label)
{
    if (labelInheritance() != label) {
        setProp(meta::Props::token(meta::Props::Token::labelInheritance), label);
        return true;
    }
    return false;
}

AADLObjectIfaceProvided::AADLObjectIfaceProvided(AADLObject *parent)
    : AADLObjectIface(IfaceType::Provided, tr("PI_%1").arg(++sProvidedCounter), parent)
{
}

AADLObjectIfaceProvided::AADLObjectIfaceProvided(const QString &title, AADLObject *parent)
    : AADLObjectIface(IfaceType::Provided, title, parent)
{
}

AADLObjectIfaceRequired::AADLObjectIfaceRequired(AADLObject *parent)
    : AADLObjectIface(IfaceType::Required, tr("RI_%1").arg(++sRequiredCounter), parent)
{
}

AADLObjectIfaceRequired::AADLObjectIfaceRequired(const QString &title, AADLObject *parent)
    : AADLObjectIface(IfaceType::Required, title, parent)
{
}

} // ns aadl
} // ns taste3
