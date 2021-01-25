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

namespace ivm {

class AADLFunction;
class AADLConnection;
class AADLComment;
class AADLIface;
class AADLIfaceGroup;
class AADLConnectionGroup;

struct AADLFunctionTypePrivate;

class AADLFunctionType : public AADLObject
{
    Q_OBJECT

public:
    explicit AADLFunctionType(const QString &title = QString(), QObject *parent = nullptr);
    ~AADLFunctionType() override;

    QVector<AADLObject *> children() const;
    bool addChild(AADLObject *child);
    bool removeChild(AADLObject *child);

    QVector<AADLFunctionType *> functionTypes() const;
    QVector<AADLFunction *> functions() const;
    QVector<AADLConnection *> connections() const;
    QVector<AADLConnectionGroup *> connectionGroups() const;
    QVector<AADLComment *> comments() const;
    QVector<AADLIface *> interfaces() const;
    QVector<AADLIface *> allInterfaces() const;
    QVector<AADLIface *> ris() const;
    QVector<AADLIface *> pis() const;
    QVector<AADLIfaceGroup *> interfaceGroups() const;
    bool hasNestedChildren() const;
    bool hasInterface(const QString &name, Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;

    QVector<ContextParameter> contextParams() const;
    ContextParameter contextParam(const QString &name) const;
    void addContextParam(const ContextParameter &param);
    bool removeContextParam(const ContextParameter &param);
    void clearContextParams();
    void setContextParams(const QVector<ContextParameter> &params);

    QVector<QPointer<AADLFunction>> instances() const;
    void rememberInstance(AADLFunction *function);
    void forgetInstance(AADLFunction *function);

Q_SIGNALS:
    void contextParamsChanged();
    void childAdded(const shared::Id &id);
    void childRemoved(const shared::Id &id);

protected:
    explicit AADLFunctionType(
            const AADLObject::Type t, const QString &title = QString(), QObject *parent = nullptr);

private:
    const std::unique_ptr<AADLFunctionTypePrivate> d;
};

typedef QVector<AADLFunctionType *> AADLFunctionTypeVector;

}
