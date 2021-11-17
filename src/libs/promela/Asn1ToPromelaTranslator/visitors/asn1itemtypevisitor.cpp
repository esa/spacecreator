/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
 *
 * Thips library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "asn1itemtypevisitor.h"

#include "asn1constraintvisitor.h"
#include "asn1sequencecomponentvisitor.h"

#include <asn1library/asn1/types/bitstring.h>
#include <asn1library/asn1/types/boolean.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/enumerated.h>
#include <asn1library/asn1/types/ia5string.h>
#include <asn1library/asn1/types/integer.h>
#include <asn1library/asn1/types/labeltype.h>
#include <asn1library/asn1/types/null.h>
#include <asn1library/asn1/types/numericstring.h>
#include <asn1library/asn1/types/octetstring.h>
#include <asn1library/asn1/types/real.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <asn1library/asn1/values.h>
#include <conversion/common/escaper/escaper.h>

using Asn1Acn::BitStringValue;
using Asn1Acn::IntegerValue;
using Asn1Acn::OctetStringValue;
using Asn1Acn::StringValue;
using Asn1Acn::Types::BitString;
using Asn1Acn::Types::Boolean;
using Asn1Acn::Types::Choice;
using Asn1Acn::Types::Enumerated;
using Asn1Acn::Types::EnumeratedItem;
using Asn1Acn::Types::IA5String;
using Asn1Acn::Types::Integer;
using Asn1Acn::Types::LabelType;
using Asn1Acn::Types::Null;
using Asn1Acn::Types::NumericString;
using Asn1Acn::Types::OctetString;
using Asn1Acn::Types::Real;
using Asn1Acn::Types::Sequence;
using Asn1Acn::Types::SequenceOf;
using Asn1Acn::Types::UserdefinedType;
using conversion::Escaper;
using promela::model::ArrayType;
using promela::model::BasicType;
using promela::model::DataType;
using promela::model::Declaration;
using promela::model::PromelaModel;
using promela::model::TypeAlias;
using promela::model::Utype;
using promela::model::UtypeRef;
using promela::model::ValueDefinition;

namespace promela::translator {
Asn1ItemTypeVisitor::Asn1ItemTypeVisitor(PromelaModel &promelaModel, std::optional<QString> baseTypeName, QString name)
    : m_promelaModel(promelaModel)
    , m_baseTypeName(std::move(baseTypeName))
    , m_name(std::move(name))
{
}

const std::optional<::promela::model::DataType> &Asn1ItemTypeVisitor::getResultDataType() const noexcept
{
    return m_resultDataType;
}

void Asn1ItemTypeVisitor::visit(const Boolean &type)
{
    Q_UNUSED(type);
    const QString typeName = constructTypeName(m_name);
    m_promelaModel.addTypeAlias(TypeAlias(typeName, BasicType::BOOLEAN));
    m_resultDataType = DataType(UtypeRef(typeName));
}

void Asn1ItemTypeVisitor::visit(const Null &type)
{
    Q_UNUSED(type);
    const QString typeName = constructTypeName(m_name);
    m_promelaModel.addTypeAlias(TypeAlias(typeName, BasicType::BIT));
    m_resultDataType = DataType(UtypeRef(typeName));
}

void Asn1ItemTypeVisitor::visit(const BitString &type)
{
    Asn1ConstraintVisitor<BitStringValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);
    const QString utypeName = constructTypeName(m_name);
    Utype utype = Utype(utypeName);

    utype.addField(Declaration(DataType(ArrayType(constraintVisitor.getMaxSize(), BasicType::BIT)), "data"));

    if (constraintVisitor.getMaxSize() != constraintVisitor.getMinSize()) {
        utype.addField(Declaration(DataType(BasicType::INT), "length"));
    }

    m_promelaModel.addUtype(utype);

    m_resultDataType = DataType(UtypeRef(utypeName));
}

void Asn1ItemTypeVisitor::visit(const OctetString &type)
{
    Asn1ConstraintVisitor<OctetStringValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);
    const QString utypeName = constructTypeName(m_name);
    Utype utype = Utype(utypeName);

    utype.addField(Declaration(DataType(ArrayType(constraintVisitor.getMaxSize(), BasicType::BYTE)), "data"));

    if (constraintVisitor.getMaxSize() != constraintVisitor.getMinSize()) {
        utype.addField(Declaration(DataType(BasicType::INT), "length"));
    }

    m_promelaModel.addUtype(utype);

    m_resultDataType = DataType(UtypeRef(utypeName));
}

void Asn1ItemTypeVisitor::visit(const IA5String &type)
{
    Asn1ConstraintVisitor<StringValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);
    const QString utypeName = constructTypeName(m_name);
    Utype utype = Utype(utypeName);

    utype.addField(Declaration(DataType(ArrayType(constraintVisitor.getMaxSize(), BasicType::BYTE)), "data"));

    if (constraintVisitor.getMaxSize() != constraintVisitor.getMinSize()) {
        utype.addField(Declaration(DataType(BasicType::INT), "length"));
    }

    m_promelaModel.addUtype(utype);

    m_resultDataType = DataType(UtypeRef(utypeName));
}

void Asn1ItemTypeVisitor::visit(const NumericString &type)
{
    Q_UNUSED(type);
}

void Asn1ItemTypeVisitor::visit(const Enumerated &type)
{
    const QString typeName = constructTypeName(m_name);

    QList<EnumeratedItem> elements = type.items().values();
    std::sort(elements.begin(), elements.end(),
            [](const EnumeratedItem &lhs, const EnumeratedItem &rhs) { return lhs.index() < rhs.index(); });

    for (const EnumeratedItem &element : elements) {
        QString enumName = QString("%1_%2").arg(typeName).arg(Escaper::escapePromelaName(element.name()));
        m_promelaModel.addValueDefinition(ValueDefinition(enumName, element.value()));
    }

    m_promelaModel.addTypeAlias(TypeAlias(typeName, BasicType::INT));
    m_resultDataType = DataType(UtypeRef(typeName));
}

void Asn1ItemTypeVisitor::visit(const Choice &type)
{
    const QString utypeName = constructTypeName(m_name);
    const QString nestedUtypeName = constructTypeName(QString("%1_data").arg(m_name));
    Utype utype(utypeName);
    // QSet<QString> componentNames;
    Utype nestedUtype(nestedUtypeName, true);
    const QString none = QString("%1_NONE").arg(utypeName);
    m_promelaModel.addValueDefinition(ValueDefinition(none, 0));
    int32_t index = 1;
    for (const std::unique_ptr<Asn1Acn::Types::ChoiceAlternative> &component : type.components()) {
        // componentNames.insert(component->name());
        Asn1ItemTypeVisitor nestedVisitor(m_promelaModel, utypeName, component->name());
        component->type()->accept(nestedVisitor);
        std::optional<DataType> nestedDataType = nestedVisitor.getResultDataType();

        const QString fieldPresent =
                QString("%1_%2_PRESENT").arg(utypeName).arg(Escaper::escapePromelaName(component->name()));
        m_promelaModel.addValueDefinition(ValueDefinition(fieldPresent, index));
        ++index;
        nestedUtype.addField(Declaration(nestedDataType.value(), Escaper::escapePromelaName(component->name())));
    }

    m_promelaModel.addUtype(nestedUtype);

    utype.addField(Declaration(DataType(UtypeRef(nestedUtypeName)), "data"));
    utype.addField(Declaration(DataType(BasicType::INT), "selection"));
    m_promelaModel.addUtype(utype);

    m_resultDataType = DataType(UtypeRef(utypeName));
}

void Asn1ItemTypeVisitor::visit(const Sequence &type)
{
    const QString nestedUtypeName = constructTypeName(m_name);
    Utype nestedUtype(nestedUtypeName);

    QList<QString> optionalFields;

    for (const std::unique_ptr<Asn1Acn::SequenceComponent> &component : type.components()) {
        Asn1SequenceComponentVisitor componentVisitor(m_promelaModel, nestedUtype, nestedUtypeName, optionalFields);
        component->accept(componentVisitor);
    }

    if (!optionalFields.isEmpty()) {
        const QString existUtypeName = QString("%1_exist").arg(nestedUtypeName);
        Utype existUtype(existUtypeName);
        for (const QString &field : optionalFields) {
            existUtype.addField(Declaration(DataType(BasicType::BOOLEAN), field));
        }
        m_promelaModel.addUtype(existUtype);
        nestedUtype.addField(Declaration(DataType(UtypeRef(existUtypeName)), "exist"));
    }

    if (nestedUtype.getFields().isEmpty()) {
        nestedUtype.addField(Declaration(DataType(BasicType::BIT), "dummy"));
    }

    m_promelaModel.addUtype(nestedUtype);

    m_resultDataType = DataType(UtypeRef(nestedUtypeName));
}

void Asn1ItemTypeVisitor::visit(const SequenceOf &type)
{
    Asn1ConstraintVisitor<IntegerValue> constraintVisitor;
    type.constraints().accept(constraintVisitor);

    const QString utypeName = constructTypeName(m_name);
    Utype utype = Utype(utypeName);

    Asn1ItemTypeVisitor itemTypeVisitor(m_promelaModel, utypeName, "item");
    type.itemsType()->accept(itemTypeVisitor);
    DataType dataType = itemTypeVisitor.getResultDataType().value();

    if (dataType.isBasicType()) {
        utype.addField(
                Declaration(DataType(ArrayType(constraintVisitor.getMaxSize(), dataType.getBasicType())), "data"));
    } else if (dataType.isUtypeReference()) {
        utype.addField(
                Declaration(DataType(ArrayType(constraintVisitor.getMaxSize(), dataType.getUtypeReference())), "data"));
    }

    if (constraintVisitor.getMaxSize() != constraintVisitor.getMinSize()) {
        utype.addField(Declaration(DataType(BasicType::INT), "length"));
    }

    type.itemsType();
    m_promelaModel.addUtype(utype);

    m_resultDataType = DataType(UtypeRef(utypeName));
}

void Asn1ItemTypeVisitor::visit(const Real &type)
{
    Q_UNUSED(type);
    const QString typeName = constructTypeName(m_name);
    m_promelaModel.addTypeAlias(TypeAlias(typeName, BasicType::FLOAT));
    m_resultDataType = DataType(UtypeRef(typeName));
}

void Asn1ItemTypeVisitor::visit(const LabelType &type)
{
    Q_UNUSED(type);
}

void Asn1ItemTypeVisitor::visit(const Integer &type)
{
    Q_UNUSED(type);
    const QString typeName = constructTypeName(m_name);
    m_promelaModel.addTypeAlias(TypeAlias(typeName, BasicType::INT));
    m_resultDataType = DataType(UtypeRef(typeName));
}

void Asn1ItemTypeVisitor::visit(const UserdefinedType &type)
{
    const QString typeName = constructTypeName(m_name);
    m_promelaModel.addTypeAlias(TypeAlias(typeName, UtypeRef(Escaper::escapePromelaName(type.typeName()))));
    m_resultDataType = DataType(UtypeRef(typeName));
}

QString Asn1ItemTypeVisitor::constructTypeName(QString name)
{
    if (m_baseTypeName.has_value()) {
        return QString("%1_%2").arg(m_baseTypeName.value()).arg(Escaper::escapePromelaName(m_name));
    }
    return Escaper::escapePromelaName(name);
}
}
