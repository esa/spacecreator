/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvobject.h"
#include "parameter.h"

#include <QVector>

namespace dvm {

class DVSystemInterface : public DVObject
{
    Q_OBJECT
    Q_PROPERTY(bool isProvided READ isProvided)
    Q_PROPERTY(bool isRequired READ isRequired)
    Q_PROPERTY(QVector<shared::InterfaceParameter> params READ params)

public:
    enum class InterfaceType
    {
        Required,
        Provided,
    };
    Q_ENUM(InterfaceType)

    explicit DVSystemInterface(DVObject *parent = nullptr);

    void setInterfaceType(InterfaceType type);
    InterfaceType interfaceType() const;
    bool isProvided() const;
    bool isRequired() const;

    QVector<shared::InterfaceParameter> params() const;
    shared::InterfaceParameter param(const QString &name) const;
    void setParams(const QVector<shared::InterfaceParameter> &params);
    void addParam(const shared::InterfaceParameter &param);

Q_SIGNALS:
    void paramsChanged();

private:
    InterfaceType m_interfaceType = InterfaceType::Provided;
    QVector<shared::InterfaceParameter> m_parameters = {};
};

} // namespace dvm

Q_DECLARE_METATYPE(dvm::DVSystemInterface *)
