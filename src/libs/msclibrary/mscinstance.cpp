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

#include "mscinstance.h"

#include "cif/cifblockfactory.h"
#include "cif/ciflines.h"

#include <QDebug>
#include <QStringList>

namespace msc {

MscInstance::MscInstance(QObject *parent)
    : MscInstance({}, parent)
{
}

MscInstance::MscInstance(const QString &name, QObject *parent)
    : MscEntity(name, parent)
{
}

/*!
   In an older standard it says:
   \<kind denominator\> ::= system | block | process | service
   But in newest ones it's just a \<name\>
 */
const QString &MscInstance::denominator() const
{
    return m_denominator;
}

void MscInstance::setDenominator(const QString &denominator)
{
    if (denominator == m_denominator) {
        return;
    }

    m_denominator = denominator;
    Q_EMIT denominatorChanged(m_denominator);
    Q_EMIT denominatorOrKindChanged(denominatorAndKind());
    Q_EMIT dataChanged();
}

const QString &MscInstance::kind() const
{
    return m_kind;
}

void MscInstance::setKind(const QString &kind)
{
    if (kind == m_kind) {
        return;
    }

    m_kind = kind;
    Q_EMIT kindChanged(m_kind);
    Q_EMIT denominatorOrKindChanged(denominatorAndKind());
    Q_EMIT dataChanged();
}

QString MscInstance::denominatorAndKind() const
{
    return QString("%1 %2").arg(m_denominator, m_kind).trimmed();
}

void MscInstance::setDenominatorAndKind(const QString &value)
{
    bool denominatorUpdated = false;
    bool kindUpdated = false;

    QString denominator;
    QString kind;
    splitDenominatorKind(value, denominator, kind);
    if (denominator != m_denominator) {
        denominatorUpdated = true;
        m_denominator = denominator;
    }
    if (kind != m_kind) {
        kindUpdated = true;
        m_kind = kind;
    }

    if (denominatorUpdated)
        Q_EMIT denominatorChanged(m_denominator);
    if (kindUpdated)
        Q_EMIT kindChanged(m_kind);
    if (denominatorUpdated || kindUpdated) {
        Q_EMIT denominatorOrKindChanged(denominatorAndKind());
        Q_EMIT dataChanged();
    }
}

void MscInstance::splitDenominatorKind(const QString &text, QString &denominator, QString &kind)
{
    QStringList tokens = text.split(" ");
    if (tokens.size() > 1) {
        const QString denom = tokens.at(0);
        if (denom.compare("system", Qt::CaseInsensitive) == 0 || denom.compare("block", Qt::CaseInsensitive) == 0
                || denom.compare("process", Qt::CaseInsensitive) == 0
                || denom.compare("service", Qt::CaseInsensitive) == 0) {
            tokens.removeAt(0);
            kind = tokens.join(" ");
            denominator = denom;
            return;
        }
    }
    kind = text;
    denominator.clear();
}

const QString &MscInstance::inheritance() const
{
    return m_inheritance;
}

void MscInstance::setInheritance(const QString &inheritance)
{
    if (inheritance == m_inheritance) {
        return;
    }

    m_inheritance = inheritance;
    Q_EMIT inheritanceChanged(m_inheritance);
    Q_EMIT dataChanged();
}

MscEntity::EntityType MscInstance::entityType() const
{
    return MscEntity::EntityType::Instance;
}

MscInstance *MscInstance::explicitCreator() const
{
    return m_explicitCreator;
}
void MscInstance::setExplicitCreator(MscInstance *creator)
{
    if (m_explicitCreator == creator)
        return;

    m_explicitCreator = creator;
    Q_EMIT dataChanged();
    Q_EMIT explicitCreatorChanged(m_explicitCreator);
}

bool MscInstance::explicitStop() const
{
    return m_explicitStop;
}
void MscInstance::setExplicitStop(bool stop)
{
    if (m_explicitStop == stop)
        return;

    m_explicitStop = stop;
    Q_EMIT explicitStopChanged(m_explicitStop);
    Q_EMIT dataChanged();
}

cif::CifBlockShared MscInstance::cifInstance() const
{
    return cifBlockByType(cif::CifLine::CifType::Instance);
}

void MscInstance::setCifGeometry(const QVector<QPoint> &cifGeometry)
{
    using namespace cif;

    CifBlockShared instanceCif = cifInstance();
    if (!instanceCif) {
        instanceCif = CifBlockFactory::createBlockInstance();
        addCif(instanceCif);
    }

    if (!instanceCif->hasPayloadFor(CifLine::CifType::Instance)) {
        instanceCif->addLine(CifLineShared(new CifLineInstance()));
    }

    if (cifGeometry.isEmpty())
        clearCifs();
    else {
        const QVector<QPoint> points { cifGeometry.at(0), cifGeometry.at(1), cifGeometry.at(2) };
        instanceCif->setPayload(QVariant::fromValue(points), CifLine::CifType::Instance);
    }

    Q_EMIT cifGeometryChanged();
    Q_EMIT dataChanged();
}

QVector<QPoint> MscInstance::cifGeometry() const
{
    using namespace cif;
    if (const CifBlockShared &instanceCif = cifInstance()) {
        const QVector<QPoint> &cifData = instanceCif->payload(CifLine::CifType::Instance).value<QVector<QPoint>>();
        return cifData;
    }

    return QVector<QPoint>();
}

} // namespace msc
