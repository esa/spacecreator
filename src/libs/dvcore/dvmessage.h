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

#include "dvnode.h"
#include "dvobject.h"

namespace dvm {

/**
 * Represents a message between two node
 * @note the message can go both ways. So fromNode/toNodes can be swapped (names kept for backwards compatibility)
 */
class DVMessage : public DVObject
{
    Q_OBJECT
    Q_PROPERTY(QString titleUI READ titleUI)
    Q_PROPERTY(QString fromFunction READ fromFunction WRITE setFromFunction)
    Q_PROPERTY(QStringList fromFunctionPath READ fromFunctionPath)
    Q_PROPERTY(QString fromInterface READ fromInterface WRITE setFromInterface)
    Q_PROPERTY(dvm::DVNode *fromNode READ fromNode)
    Q_PROPERTY(QString toFunction READ toFunction WRITE setToFunction)
    Q_PROPERTY(QStringList toFunctionPath READ toFunctionPath)
    Q_PROPERTY(QString toInterface READ toInterface WRITE setToInterface)
    Q_PROPERTY(dvm::DVNode *toNode READ toNode)
    Q_PROPERTY(QString resolvedTargetFunction READ resolvedTargetFunction)
    Q_PROPERTY(QString resolvedTargetInterface READ resolvedTargetInterface)

public:
    explicit DVMessage(QObject *parent = nullptr, const shared::Id &id = shared::InvalidId);

    bool postInit() override;

    QString titleUI() const override;

    QString fromFunction() const;
    void setFromFunction(const QString &from);
    QStringList fromFunctionPath() const;

    QString fromInterface() const;
    void setFromInterface(const QString &from);

    DVNode *fromNode() const;

    QString toFunction() const;
    void setToFunction(const QString &to);
    QStringList toFunctionPath() const;

    QString toInterface() const;
    void setToInterface(const QString &to);

    DVNode *toNode() const;

    QString resolvedTargetFunction() const;
    QString resolvedTargetInterface() const;

private:
    QStringList pathOfFunction(const QString &functionName, DVNode *node) const;
    DVModel *getModel() const;
};

} // namespace dvm

Q_DECLARE_METATYPE(dvm::DVMessage *)
