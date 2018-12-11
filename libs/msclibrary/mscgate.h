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

#pragma once

#include "mscelement.h"
#include <mscinstance.h>

#include <QPointer>
#include <QVariantList>

namespace msc {

class MscGate : public MscElement
{
    Q_OBJECT
    Q_PROPERTY(MscGate::Direction direction READ direction WRITE setDirection)
    Q_PROPERTY(MscInstance *instance READ instance WRITE setInstance)
    Q_PROPERTY(QString instanceName READ instanceName WRITE setInstanceName)
    Q_PROPERTY(QString paramName READ paramName WRITE setParamName)
    Q_PROPERTY(QVariantList params READ params WRITE setParams)

public:
    enum class Direction {
        In = 0,
        Out
    };

    explicit MscGate(QObject *parent = nullptr);
    MscGate(const QString &name, QObject *parent = nullptr);

    MscGate::Direction direction() const;
    MscInstance *instance() const;
    QString instanceName() const;
    QString paramName() const;
    QVariantList params() const;

    chart::Element elementType() const override;

public Q_SLOTS:
    void setDirection(MscGate::Direction dir);
    void setInstance(MscInstance *instance);
    void setInstanceName(const QString &instanceName);
    void setParamName(const QString &name);
    void setParams(const QVariantList &params);

private:
    Direction m_direction = MscGate::Direction::In;
    QPointer<MscInstance> m_instance = nullptr;
    QString m_instanceName;
    QString m_paramName;
    QVariantList m_params;
};
} // ns name
