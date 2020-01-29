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

#pragma once

#include "aadlobject.h"
#include "aadlparameter.h"

#include <QMetaType>
#include <QObject>
#include <QPointer>
#include <QVector>
#include <memory>

namespace taste3 {
namespace aadl {

class AADLObjectFunction;
struct AADLObjectIfacePrivate;

class AADLObjectIface : public AADLObject
{
    Q_OBJECT
    Q_PROPERTY(AADLObjectIface::IfaceType direction READ direction)

public:
    enum class IfaceType
    {
        Required = 0,
        Provided
    };
    Q_ENUM(IfaceType)

    static constexpr IfaceType DefaultDirection { IfaceType::Required };

    ~AADLObjectIface() override;

    AADLObject::AADLObjectType aadlType() const override;

    AADLObjectIface::IfaceType direction() const;

    bool isProvided() const;
    bool isRequired() const;

    QString kind() const;
    bool setKind(const QString &kind);

    qint32 period() const;
    bool setPeriod(qint32 period);

    qint32 wcet() const;
    bool setWcet(qint32 wcet);

    qint32 queueSize() const;
    bool setQueueSize(qint32 size);

    QVector<IfaceParameter> params() const;
    void setParams(const QVector<IfaceParameter> &params);
    void addParam(const IfaceParameter &param);

    QString rcmOperationKind() const;
    bool setRcmOperationKind(const QString &kind);

    QString deadline() const;
    bool setDeadline(const QString &deadline);

    QString rcmPeriod() const;
    bool setRcmPeriod(const QString &period);

    QString interfaceName() const;
    bool setInterfaceName(const QString &name);

    bool labelInheritance() const;
    bool setLabelInheritance(bool label);

    AADLObjectFunction *function() const;

protected:
    explicit AADLObjectIface(AADLObjectIface::IfaceType direction, const QString &title, AADLObject *parent = nullptr);
    explicit AADLObjectIface(const common::Id &id, AADLObjectIface::IfaceType direction, const QString &title,
                             AADLObject *parent = nullptr);

private:
    const std::unique_ptr<AADLObjectIfacePrivate> d;
};

class AADLObjectIfaceProvided : public AADLObjectIface
{
    Q_OBJECT
public:
    explicit AADLObjectIfaceProvided(AADLObject *parent = nullptr);
    explicit AADLObjectIfaceProvided(const QString &title, AADLObject *parent = nullptr);
    explicit AADLObjectIfaceProvided(const common::Id &id, const QString &title = QString(),
                                     AADLObject *parent = nullptr);
};

class AADLObjectIfaceRequired : public AADLObjectIface
{
    Q_OBJECT
public:
    explicit AADLObjectIfaceRequired(AADLObject *parent = nullptr);
    explicit AADLObjectIfaceRequired(const QString &title, AADLObject *parent = nullptr);
    explicit AADLObjectIfaceRequired(const common::Id &id, const QString &title = QString(),
                                     AADLObject *parent = nullptr);
};

AADLObjectIface *createIface(AADLObjectIface::IfaceType direction, const common::Id &id);

typedef QVector<AADLObjectIface *> AADLIfacesVector;

} // ns aadl
} // ns taste3

Q_DECLARE_METATYPE(taste3::aadl::AADLObjectIface::IfaceType);
