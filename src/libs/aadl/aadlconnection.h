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
#include "aadliface.h"

#include <QObject>
#include <memory>

namespace ivm {

struct AADLConnectionPrivate;
class AADLConnection : public AADLObject
{
    Q_OBJECT

public:
    explicit AADLConnection(AADLIface *ifaceSource, AADLIface *ifaceTarget, QObject *parent = nullptr);
    ~AADLConnection() override;

    QString sourceName() const;
    AADLObject *source() const;

    QString targetName() const;
    AADLObject *target() const;

    QString sourceInterfaceName() const;
    AADLIface *sourceInterface() const;

    QString targetInterfaceName() const;
    AADLIface *targetInterface() const;

    void setInheritPI();
    void unsetInheritPI();

    template<class T>
    static inline T selectIface(AADLIface *a, AADLIface *b)
    {
        T ptr { nullptr };
        if (a)
            ptr = a->as<T>();
        if (!ptr && b)
            ptr = b->as<T>();

        return ptr;
    }

    template<class T>
    inline T selectIface() const
    {
        return selectIface<T>(sourceInterface(), targetInterface());
    }

    bool postInit() override;

    struct EndPointInfo {
        QString m_functionName;
        QString m_interfaceName;
        AADLIface::IfaceType m_ifaceDirection;
        inline bool isReady() const { return !m_functionName.isEmpty() && !m_interfaceName.isEmpty(); }
    };

    void setDelayedStart(AADLConnection::EndPointInfo *start);
    void setDelayedEnd(AADLConnection::EndPointInfo *end);

    enum class ConnectionType
    {
        NotAConnection = 0,
        RI2PI,
        PI2RI,
        RI2RI,
        PI2PI
    };
    AADLConnection::ConnectionType connectionType() const;

    bool isOneDirection() const;

    QString name() const;
    QVector<IfaceParameter> params() const;

protected:
    explicit AADLConnection(const AADLObject::Type t, AADLIface *ifaceSource, AADLIface *ifaceTarget,
            QObject *parent = nullptr);
    bool lookupEndpointsPostponed();
    bool needPostponedInit() const;

private:
    const std::unique_ptr<AADLConnectionPrivate> d;

    enum class InheritPIChange
    {
        Inherit = 0,
        NotInherit
    };
    void handleInheritPIChange(AADLConnection::InheritPIChange inheritance);
    void handleProvidedTitleChanged(const QString &title);
    void handleInheritPIChange(bool enabled);

    void clearPostponedEndpoints();
};

}

QDebug operator<<(QDebug debug, const ivm::AADLConnection &connection);
