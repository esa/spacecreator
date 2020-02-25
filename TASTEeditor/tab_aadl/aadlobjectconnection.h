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

    AADLObjectType aadlType() const override;

    QString sourceName() const;
    AADLObject *source() const;
    void setSource(AADLObject *source);

    QString targetName() const;
    AADLObject *target() const;
    void setTarget(AADLObject *target);

    QString sourceInterfaceName() const;
    AADLObjectIface *sourceInterface() const;
    void setSourceInterface(AADLObjectIface *iface);

    QString targetInterfaceName() const;
    AADLObjectIface *targetInterface() const;
    void setTargetInterface(AADLObjectIface *iface);

    void setAttr(const QString &name, const QVariant &val) override;

    bool sourceInterfaceIsRequired() const;
    bool sourceInterfaceIsProvided() const;
    bool targetInterfaceIsRequired() const;
    bool targetInterfaceIsProvided() const;

    void inheritLabel();
    void uninheritLabel();

    template<class T>
    static inline const T selectIface(const AADLObjectIface *a, const AADLObjectIface *b)
    {
        T ptr { nullptr };
        if (a)
            ptr = a->as<T>();
        if (!ptr && b)
            ptr = b->as<T>();

        return ptr;
    }

    template<class T>
    inline const T selectIface() const
    {
        return selectIface<T>(sourceInterface(), targetInterface());
    }

    void postInit() override;

private:
    const std::unique_ptr<AADLObjectConnectionPrivate> d;
    void updateAttributes();

    enum class LabelInheritancePolicy
    {
        Set = 0,
        Unset
    };
    void handleLabelInheritance(AADLObjectConnection::LabelInheritancePolicy inheritance);
    void handleProvidedTitleChanged(const QString &title);
    void handleRequiredInheritancePropertyChanged(bool enabled);
};

} // ns aadl
} // ns taste3
