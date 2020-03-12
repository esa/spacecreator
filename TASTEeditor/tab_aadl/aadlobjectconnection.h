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

#include <QObject>
#include <memory>

namespace taste3 {
namespace aadl {

struct AADLObjectConnectionPrivate;
class AADLObjectConnection : public AADLObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ sourceName)
    Q_PROPERTY(QString target READ targetName)
    Q_PROPERTY(QString siName READ sourceInterfaceName)
    Q_PROPERTY(QString tiName READ targetInterfaceName)
    Q_PROPERTY(bool siIsRequired READ sourceInterfaceIsRequired)
    Q_PROPERTY(bool siIsProvided READ sourceInterfaceIsProvided)
    Q_PROPERTY(bool tiIsRequired READ targetInterfaceIsRequired)
    Q_PROPERTY(bool tiIsProvided READ targetInterfaceIsProvided)
public:
    explicit AADLObjectConnection(AADLObject *source, AADLObject *target, AADLObjectIface *ifaceSource,
                                  AADLObjectIface *ifaceTarget, QObject *parent = nullptr);
    ~AADLObjectConnection() override;

    QString sourceName() const;
    AADLObject *source() const;

    QString targetName() const;
    AADLObject *target() const;

    QString sourceInterfaceName() const;
    AADLObjectIface *sourceInterface() const;

    QString targetInterfaceName() const;
    AADLObjectIface *targetInterface() const;

    bool sourceInterfaceIsRequired() const;
    bool sourceInterfaceIsProvided() const;
    bool targetInterfaceIsRequired() const;
    bool targetInterfaceIsProvided() const;

    void inheritLabel();
    void uninheritLabel();

    template<class T>
    static inline T selectIface(AADLObjectIface *a, AADLObjectIface *b)
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
        AADLObjectIface::IfaceType m_ifaceDirection;
        inline bool isReady() const { return m_functionName.isEmpty() && m_interfaceName.isEmpty(); }
    };

    void setDelayedStart(AADLObjectConnection::EndPointInfo *start);
    void setDelayedEnd(AADLObjectConnection::EndPointInfo *end);

private:
    const std::unique_ptr<AADLObjectConnectionPrivate> d;

    enum class LabelInheritancePolicy
    {
        Set = 0,
        Unset
    };
    void handleLabelInheritance(AADLObjectConnection::LabelInheritancePolicy inheritance);
    void handleProvidedTitleChanged(const QString &title);
    void handleRequiredInheritancePropertyChanged(bool enabled);

    bool lookupEndpointsPostponed();
    void clearPostponedEndpoints();
};

} // ns aadl
} // ns taste3
