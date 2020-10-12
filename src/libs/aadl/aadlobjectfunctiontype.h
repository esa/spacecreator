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

#include <QObject>
#include <QVector>
#include <memory>

namespace aadl {

class AADLObjectFunction;
class AADLObjectConnection;
class AADLObjectComment;
class AADLObjectIface;
class AADLObjectIfaceGroup;
class AADLObjectConnectionGroup;

struct AADLObjectFunctionTypePrivate;

class AADLObjectFunctionType : public AADLObject
{
    Q_OBJECT

public:
    explicit AADLObjectFunctionType(const QString &title = QString(), QObject *parent = nullptr);
    ~AADLObjectFunctionType() override;

    QVector<AADLObject *> children() const;
    bool addChild(AADLObject *child);
    bool removeChild(AADLObject *child);

    QVector<AADLObjectFunctionType *> functionTypes() const;
    QVector<AADLObjectFunction *> functions() const;
    QVector<AADLObjectConnection *> connections() const;
    QVector<AADLObjectConnectionGroup *> connectionGroups() const;
    QVector<AADLObjectComment *> comments() const;
    QVector<AADLObjectIface *> interfaces() const;
    QVector<AADLObjectIface *> ris() const;
    QVector<AADLObjectIface *> pis() const;
    QVector<AADLObjectIfaceGroup *> interfaceGroups() const;
    bool hasNestedChildren() const;

    QVector<ContextParameter> contextParams() const;
    ContextParameter contextParam(const QString &name) const;
    void addContextParam(const ContextParameter &param);
    bool removeContextParam(const ContextParameter &param);
    void clearContextParams();
    void setContextParams(const QVector<ContextParameter> &params);

    QVector<QPointer<AADLObjectFunction>> instances() const;
    void rememberInstance(AADLObjectFunction *function);
    void forgetInstance(AADLObjectFunction *function);

Q_SIGNALS:
    void contextParamsChanged();

protected:
    explicit AADLObjectFunctionType(
            const AADLObject::Type t, const QString &title = QString(), QObject *parent = nullptr);

private:
    const std::unique_ptr<AADLObjectFunctionTypePrivate> d;
};

typedef QVector<AADLObjectFunctionType *> AADLFunctionTypeVector;

}
