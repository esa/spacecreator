/****************************************************************************
**
** Copyright (C) 2017-2021 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Library.
**
** Library was developed under a program and funded by
** European Space Agency.
**
** This Library is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#pragma once

#include "importedtype.h"
#include "importedvalue.h"
#include "node.h"
#include "typeassignment.h"
#include "valueassignment.h"

#include <QString>
#include <QStringList>
#include <map>
#include <memory>
#include <set>
#include <vector>

namespace Asn1Acn {

class Definitions : public Node
{
public:
    Definitions(const QString &name, const SourceLocation &location);
    Definitions(const Definitions &other);
    ~Definitions() override;

    void accept(Visitor &visitor) const override;
    void accept(MutatingVisitor &visitor) override;

    void addType(std::unique_ptr<TypeAssignment> type);
    void addValue(std::unique_ptr<ValueAssignment> value);
    void addImportedType(const ImportedType &type);
    void addImportedValue(const ImportedValue &value);

    using Types = std::vector<std::unique_ptr<TypeAssignment>>;
    using Values = std::vector<std::unique_ptr<ValueAssignment>>;
    using ImportedTypes = std::set<ImportedType>;
    using ImportedValues = std::vector<ImportedValue>;

    const Types &types() const { return m_types; }
    const TypeAssignment *type(const QString &name) const;
    TypeAssignment *type(const QString &name);
    const Values &values() const { return m_values; }
    const ValueAssignment *value(const QString &name) const;
    const ImportedTypes &importedTypes() const { return m_importedTypes; }
    const ImportedValues &importedValues() const { return m_importedValues; }

    QStringList typeAssignmentNames() const;

    template<typename Functor>
    void forAllNodes(Functor fun) const
    {
        for (const auto &type : types())
            fun(type.get());
        for (const auto &var : values())
            fun(var.get());
    }

private:
    Types m_types;
    Values m_values;
    std::map<QString, TypeAssignment *> m_typeByNameMap;
    std::map<QString, ValueAssignment *> m_valueByNameMap;
    ImportedTypes m_importedTypes;
    ImportedValues m_importedValues;
};

}
