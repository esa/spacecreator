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

class AADLObjectFunction;
struct AADLObjectFunctionTypePrivate;
class AADLObjectFunctionType : public AADLObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList interfaces READ interfaces)
    Q_PROPERTY(QVariantList functions READ nestedFunctions)
    Q_PROPERTY(QVariantList comments READ nestedComments)
public:
    explicit AADLObjectFunctionType(const QString &title = QString(), QObject *parent = nullptr);
    ~AADLObjectFunctionType() override;

    AADLObject::AADLObjectType aadlType() const override;

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
    QVector<AADLObjectIface *> interfaces() const;

    QVariantList nestedFunctions() const;
    QVariantList nestedComments() const;

    QString language() const;
    void setLanguage(const QString &lang);

    QStringList activeInterfaces() const;
    void setActiveInterfaces(const QStringList &ifaces);

    QVector<ContextParameter> contextParams() const;
    void addContextParam(const ContextParameter &param);
    bool removeContextParam(const ContextParameter &param);
    void clearContextParams();
    void setContextParams(const QVector<ContextParameter> &params);

    void setAttr(const QString &name, const QVariant &val) override;

    QVector<QPointer<AADLObjectFunction>> instances() const;
    void rememberInstance(AADLObjectFunction *function);
    void forgetInstance(AADLObjectFunction *function);

Q_SIGNALS:
    void attrChanged_isType(bool isType);
    void ifaceAdded(AADLObjectIface *iface);
    void ifaceRemoved(AADLObjectIface *iface);

private:
    const std::unique_ptr<AADLObjectFunctionTypePrivate> d;
};

typedef QVector<AADLObjectFunctionType *> AADLFunctionTypeVector;

} // ns aadl
} // ns taste3
