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

#include "ivconnectionlayertype.h"
#include "ivinterface.h"
#include "ivobject.h"

#include <QObject>
#include <memory>

namespace ivm {

struct IVConnectionPrivate;
class IVConnection : public IVObject
{
    Q_OBJECT

public:
    explicit IVConnection(IVInterface *ifaceSource, IVInterface *ifaceTarget, QObject *parent = nullptr);
    ~IVConnection() override;

    QString sourceName() const;
    IVObject *source() const;

    QString targetName() const;
    IVObject *target() const;

    QString sourceInterfaceName() const;
    IVInterface *sourceInterface() const;

    QString targetInterfaceName() const;
    IVInterface *targetInterface() const;

    void setInheritPI();
    void unsetInheritPI();

    IVConnectionLayerType *layer() const;

    bool isProtected() const;

    template<class T>
    static inline T selectIface(IVInterface *a, IVInterface *b)
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
        IVInterface::InterfaceType m_ifaceDirection;
        inline bool isReady() const { return !m_functionName.isEmpty() && !m_interfaceName.isEmpty(); }
        inline void reset()
        {
            m_functionName.clear();
            m_interfaceName.clear();
        }
    };

    void setDelayedStart(const IVConnection::EndPointInfo &info);
    const IVConnection::EndPointInfo &delayedStart() const;
    void setDelayedEnd(const EndPointInfo &end);
    const IVConnection::EndPointInfo &delayedEnd() const;

    enum class ConnectionType
    {
        NotAConnection = 0,
        RI2PI,
        PI2RI,
        RI2RI,
        PI2PI
    };
    IVConnection::ConnectionType connectionType() const;

    bool isOneDirection() const;

    QString name() const;
    QVector<shared::InterfaceParameter> params() const;

protected:
    explicit IVConnection(
            const IVObject::Type t, IVInterface *ifaceSource, IVInterface *ifaceTarget, QObject *parent = nullptr);
    bool lookupEndpointsPostponed();
    bool needPostponedInit() const;

private:
    const std::unique_ptr<IVConnectionPrivate> d;

    enum class InheritPIChange
    {
        Inherit = 0,
        NotInherit
    };
    void handleInheritPIChange(IVConnection::InheritPIChange inheritance);
    void handleProvidedTitleChanged(const QString &title);
    void handleInheritPIChange(bool enabled);

    void clearPostponedEndpoints();

    IVObject *findFunction(const IVConnection::EndPointInfo &info) const;
    IVInterface *findIface(const IVConnection::EndPointInfo &info, IVObject *parentObject) const;
};

}

QDebug operator<<(QDebug debug, const ivm::IVConnection &connection);
