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

#include "asn1/file.h"
#include "asn1/typeassignment.h"
#include "asn1/types/type.h"

#include <QObject>
#include <QPointer>
#include <memory>

namespace shared {
class AbstractProject;
}

namespace Asn1Acn {
class Asn1ModelStorage;

class Asn1SystemChecks : public QObject
{
    Q_OBJECT
public:
    explicit Asn1SystemChecks(QObject *parent = nullptr);

    void setAsn1Storage(Asn1Acn::Asn1ModelStorage *storage);
    Asn1Acn::Asn1ModelStorage *asn1Storage() const;
    void setProject(shared::AbstractProject *project);

    QStringList allTypeNames() const;

    std::vector<Definitions *> definitionsList() const;

    virtual bool checkAsn1Compliance(const QString &parameter, const QString &typeName) const;
    const std::unique_ptr<Asn1Acn::TypeAssignment> &typeAssignment(const QString &text) const;

    virtual const Asn1Acn::Types::Type *typeFromName(const QString &name) const;
    bool hasType(const QString &name) const;

Q_SIGNALS:

private:
    QPointer<Asn1Acn::Asn1ModelStorage> m_storage;
    shared::AbstractProject *m_project = nullptr;
    const std::unique_ptr<Asn1Acn::TypeAssignment> m_noType;
};

} // namespace scs
