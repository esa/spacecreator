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

#include "ivobject.h"
#include "parameter.h"

#include <QObject>
#include <QVector>
#include <memory>

namespace ivm {

class IVFunction;
class IVConnection;
class IVComment;
class IVInterface;
class IVInterfaceGroup;
class IVConnectionGroup;

struct IVFunctionTypePrivate;

class IVFunctionType : public IVObject
{
    Q_OBJECT

public:
    explicit IVFunctionType(
            const QString &title = QString(), QObject *parent = nullptr, const shared::Id &id = shared::InvalidId);
    ~IVFunctionType() override;

    QVector<IVObject *> children() const;
    bool addChild(IVObject *child);
    bool removeChild(IVObject *child);

    bool postInit(QString *warning = nullptr) override;

    QVector<IVFunctionType *> functionTypes() const;
    QVector<IVFunction *> functions() const;
    QVector<IVConnection *> connections() const;
    QVector<IVConnectionGroup *> connectionGroups() const;
    QVector<IVComment *> comments() const;
    QVector<IVInterface *> interfaces() const;
    QVector<IVInterface *> allInterfaces() const;
    QVector<IVInterface *> ris() const;
    QVector<IVInterface *> pis() const;
    QVector<IVInterfaceGroup *> interfaceGroups() const;
    bool hasNestedChildren() const;
    bool hasInterface(const QString &name, Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;

    QVector<ContextParameter> contextParams() const;
    ContextParameter contextParam(const QString &name) const;
    void addContextParam(const ContextParameter &param);
    bool removeContextParam(const ContextParameter &param);
    void clearContextParams();
    void setContextParams(const QVector<ContextParameter> &params);

    QVector<QPointer<IVFunction>> instances() const;
    void rememberInstance(IVFunction *function);
    void forgetInstance(IVFunction *function);

Q_SIGNALS:
    void contextParamsChanged();
    void childAdded(const shared::Id &id);
    void childRemoved(const shared::Id &id);

protected:
    explicit IVFunctionType(const IVObject::Type t, const QString &title = QString(), QObject *parent = nullptr,
            const shared::Id &id = shared::InvalidId);

private:
    const std::unique_ptr<IVFunctionTypePrivate> d;
};

}
