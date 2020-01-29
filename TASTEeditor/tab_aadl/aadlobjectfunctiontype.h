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
#include "aadlobjectiface.h"
#include "aadlparameter.h"

#include <QObject>
#include <QVector>
#include <memory>

namespace taste3 {
namespace aadl {

struct AADLObjectFunctionTypePrivate;
class AADLObjectFunctionType : public AADLObject
{
    Q_OBJECT
    Q_PROPERTY(QString language READ language WRITE setLanguage)
    Q_PROPERTY(QString isType READ isType)
    Q_PROPERTY(QString instanceOf READ instanceOf)
    Q_PROPERTY(QVariantList interfaces READ interfaces)
    Q_PROPERTY(QVariantList functions READ functions)
public:
    explicit AADLObjectFunctionType(const QString &title = QString(), QObject *parent = nullptr);
    ~AADLObjectFunctionType() override;

    AADLObject::AADLObjectType aadlType() const override;

    QString isType() const;
    virtual QString instanceOf() const;

    QVector<AADLObject *> children() const;
    bool addChild(AADLObject *child);
    bool removeChild(AADLObject *child);

    QVector<AADLObjectIface *> ris() const;
    bool addRI(AADLObjectIface *ri);
    bool removeRI(AADLObjectIface *ri);

    QVector<AADLObjectIface *> pis() const;
    bool addPI(AADLObjectIface *pi);
    bool removePI(AADLObjectIface *pi);

    QVariantList interfaces() const;
    bool addInterface(AADLObjectIface *iface);
    bool removeInterface(AADLObjectIface *iface);

    QVariantList functions() const;

    QString language() const;
    void setLanguage(const QString &lang);

    QStringList activeInterfaces() const;
    void setActiveInterfaces(const QStringList &ifaces);

    QVector<ContextParameter> contextParams() const;
    void addContextParam(const ContextParameter &param);
    bool removeContextParam(const ContextParameter &param);
    void clearContextParams();
    void setContextParams(const QVector<ContextParameter> &params);

private:
    const std::unique_ptr<AADLObjectFunctionTypePrivate> d;
};

typedef QVector<AADLObjectFunctionType *> AADLFunctionTypeVector;

} // ns aadl
} // ns taste3
