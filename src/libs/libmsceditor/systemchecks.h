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

#include <QObject>
#include <QPair>
#include <QStringList>
#include <QVector>

namespace ivm {
class IVObject;
class IVFunction;
class IVConnection;
}

namespace msc {
class MscChart;
class MscInstance;
class MscMessage;
class MscMessageDeclaration;

/*!
   Base class to check MSC data against a iv system
   The actual checks are to be implemented in a derived class
 */
class SystemChecks : public QObject
{
    Q_OBJECT
public:
    SystemChecks(QObject *parent = nullptr);
    virtual ~SystemChecks();

    virtual QStringList functionsNames() const;
    virtual QStringList connectionNames() const;

    virtual bool checkInstance(const msc::MscInstance *instance) const;
    virtual bool checkMessage(const msc::MscMessage *message) const;

    virtual QVector<QPair<msc::MscChart *, msc::MscInstance *>> checkInstanceNames() const;
    virtual QVector<QPair<msc::MscChart *, msc::MscInstance *>> checkInstanceRelations() const;

    virtual QVector<QPair<msc::MscChart *, msc::MscMessage *>> checkMessages() const;

    virtual QStringList connectionNamesFromTo(const QString &sourceName, const QString &targetName) const;

    virtual bool correspond(const ivm::IVObject *ivObj, const msc::MscInstance *instance) const;
    virtual bool correspond(const ivm::IVFunction *ivFunc, const msc::MscInstance *instance) const;
    virtual bool correspond(const ivm::IVConnection *connection, const msc::MscMessage *message) const;

    virtual QVector<msc::MscMessageDeclaration *> allConnectionsAsDeclaration() const;

    virtual Qt::CaseSensitivity stringSensitivity() const;

    virtual bool hasValidSystem() const;

Q_SIGNALS:
    void ivDataReset();
};

} // namespace msc
