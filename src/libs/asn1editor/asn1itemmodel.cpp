/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "asn1itemmodel.h"

#include "asn1const.h"
#include "asn1editorconst.h"
#include "asnsequencecomponent.h"
#include "constraints/logicoperators.h"
#include "constraints/rangecombiner.h"
#include "constraints/rangeconstraint.h"
#include "constraints/sizecombiner.h"
#include "constraints/sizeconstraint.h"
#include "range.h"
#include "typeassignment.h"
#include "types/bitstring.h"
#include "types/boolean.h"
#include "types/choice.h"
#include "types/enumerated.h"
#include "types/ia5string.h"
#include "types/integer.h"
#include "types/numericstring.h"
#include "types/octetstring.h"
#include "types/real.h"
#include "types/sequence.h"
#include "types/sequenceof.h"
#include "types/userdefinedtype.h"
#include "values.h"

#include <QStandardItem>
#include <iostream>

namespace asn1 {

/*!
 * \class Asn1ItemModel
 *
 * This class is the model for the \sa Asn1TreeView.
 */
Asn1ItemModel::Asn1ItemModel(QObject *parent)
    : QStandardItemModel(1, 4, parent)
{
}

Asn1ItemModel::Asn1ItemModel(const std::unique_ptr<Asn1Acn::TypeAssignment> &asn1Item, QObject *parent)
    : Asn1ItemModel(parent)
{
    setAsn1Model(asn1Item);
}

/*!
 * \brief Asn1ItemModel::setAsn1Model Populate the values in the model from this map.
 * \param asn1Item
 */
void Asn1ItemModel::setAsn1Model(const std::unique_ptr<Asn1Acn::TypeAssignment> &asn1Item)
{
    clear();

    static QStringList headers = { tr("Field"), tr("Type"), tr("Value"), tr("Present") };
    setHorizontalHeaderLabels(headers);

    ItemMap itemMap = createModelItems(asn1Item->type());

    setItem(0, MODEL_NAME_INDEX, itemMap["item"]);
    setItem(0, MODEL_TYPE_INDEX, itemMap["type"]);
    setItem(0, MODEL_VALUE_INDEX, itemMap["value"]);
    setItem(0, MODEL_IS_PRESENT_INDEX, itemMap["present"]);
}

/*!
 * \brief Asn1ItemModel::createModelItems Create items for the tree view from the map
 * \param asn1Item The map with the items
 * \return The created item map
 */
Asn1ItemModel::ItemMap Asn1ItemModel::createModelItems(const Asn1Acn::Types::Type *asn1Item, const QString &name, QStandardItem *nameItem)
{
    ItemMap itemMap;
    QString typeLimit;
    QStandardItem *valueItem = nullptr;

    if (!nameItem) {
        nameItem = new QStandardItem(name.isEmpty() ? asn1Item->identifier() : name);
        nameItem->setEditable(false);
    }

    switch (asn1Item->typeEnum()) {
    case Asn1Acn::Types::Type::INTEGER: {
        const auto *integerItem = dynamic_cast<const Asn1Acn::Types::Integer *>(asn1Item);
        valueItem = createNumberItem<Asn1Acn::IntegerValue>(integerItem, typeLimit);
        valueItem->setData(integerItem->typeEnum(), ASN1TYPE_ROLE);
        break;
    }
    case Asn1Acn::Types::Type::REAL: {
        const auto *realItem = dynamic_cast<const Asn1Acn::Types::Real *>(asn1Item);
        valueItem = createNumberItem<Asn1Acn::RealValue>(realItem, typeLimit);
        valueItem->setData(realItem->typeEnum(), ASN1TYPE_ROLE);
        break;
    }
    case Asn1Acn::Types::Type::BOOLEAN: {
        const auto *boolItem = dynamic_cast<const Asn1Acn::Types::Boolean *>(asn1Item);
        valueItem = createBoolItem(boolItem);
        break;
    }
    case Asn1Acn::Types::Type::SEQUENCE: {
        const auto *sequenceItem = dynamic_cast<const Asn1Acn::Types::Sequence *>(asn1Item);
        valueItem = createSequenceItem(sequenceItem, nameItem);
        break;
    }
    case Asn1Acn::Types::Type::SEQUENCEOF: {
        const auto *sequenceOfItem = dynamic_cast<const Asn1Acn::Types::SequenceOf *>(asn1Item);
        valueItem = createSequenceOfItem(sequenceOfItem, nameItem, typeLimit);
        break;
    }
    case Asn1Acn::Types::Type::ENUMERATED: {
        const auto *enumeratedItem = dynamic_cast<const Asn1Acn::Types::Enumerated *>(asn1Item);
        valueItem = createEnumeratedItem(enumeratedItem);
        break;
    }
    case Asn1Acn::Types::Type::CHOICE: {
        const auto *choiceItem = dynamic_cast<const Asn1Acn::Types::Choice *>(asn1Item);
        valueItem = createChoiceItem(choiceItem, nameItem);
        break;
    }
    case Asn1Acn::Types::Type::BITSTRING: {
        const auto *bitStringItem = dynamic_cast<const Asn1Acn::Types::BitString *>(asn1Item);
        valueItem = createStringItem<Asn1Acn::BitStringValue>(bitStringItem, typeLimit);
        valueItem->setData(asn1Item->typeEnum(), ASN1TYPE_ROLE);
        break;
    }
    case Asn1Acn::Types::Type::IA5STRING: {
        const auto *ia5StringItem = dynamic_cast<const Asn1Acn::Types::IA5String *>(asn1Item);
        valueItem = createStringItem<Asn1Acn::StringValue>(ia5StringItem, typeLimit);
        valueItem->setData(asn1Item->typeEnum(), ASN1TYPE_ROLE);
        break;
    }
    case Asn1Acn::Types::Type::NUMERICSTRING: {
        const auto *numericStringItem = dynamic_cast<const Asn1Acn::Types::NumericString *>(asn1Item);
        valueItem = createStringItem<Asn1Acn::StringValue>(numericStringItem, typeLimit);
        valueItem->setData(asn1Item->typeEnum(), ASN1TYPE_ROLE);
        break;
    }
    case Asn1Acn::Types::Type::OCTETSTRING: {
        const auto *octetStringItem = dynamic_cast<const Asn1Acn::Types::OctetString *>(asn1Item);
        valueItem = createStringItem<Asn1Acn::OctetStringValue>(octetStringItem, typeLimit);
        valueItem->setData(asn1Item->typeEnum(), ASN1TYPE_ROLE);
        break;
    }
    case Asn1Acn::Types::Type::USERDEFINED: {
        auto userType = static_cast<const Asn1Acn::Types::UserdefinedType *>(asn1Item);
        if (userType->type()) {
            return createModelItems(userType->type(), nameItem->text(), nameItem);
        }
        break;
    }
    default:
        std::cerr << "Unhandled asn1 type\n";
        break;
    }

    QStandardItem *typeItem = new QStandardItem(asn1Item->typeName() + typeLimit);
    typeItem->setData(QBrush(QColor("gray")), Qt::ForegroundRole);
    typeItem->setEditable(false);

    itemMap["item"] = nameItem;
    itemMap["type"] = typeItem;
    itemMap["value"] = valueItem;
    itemMap["present"] = createPresentItem();

    return itemMap;
}

/*!
 * \brief Asn1ItemModel::createNumberItem Create an item with the type number
 * \param asn1Item
 * \return
 */
template<typename ValueType>
QStandardItem *Asn1ItemModel::createNumberItem(
        const Asn1Acn::Constraints::WithConstraints<ValueType> *asn1Item, QString &typeLimit)
{
    QStandardItem *item = new QStandardItem("");

    const auto &constraints = asn1Item->constraints();
    const auto range = Asn1Acn::Constraints::RangeCombiner<ValueType>::combineRanges(&constraints);

    if (range) {
        item->setText(QString::number(range->begin()));
        item->setData(QVariant::fromValue(range->begin()), MIN_RANGE_ROLE);
        item->setData(QVariant::fromValue(range->end()), MAX_RANGE_ROLE);

        typeLimit = QString(" (%1..%2)").arg(QString::number(range->begin())).arg(QString::number(range->end()));
    }

    return item;
}

/*!
 * \brief Asn1ItemModel::createBoolItem  Create an item with the type bool
 * \param asn1Item
 * \return
 */
QStandardItem *Asn1ItemModel::createBoolItem(const Asn1Acn::Types::Boolean *asn1Item)
{
    static const QVariantList choices { QString("true"), QString("false") };

    QStandardItem *item = new QStandardItem("false");
    item->setData(asn1Item->typeEnum(), ASN1TYPE_ROLE);
    item->setData(choices, CHOICE_LIST_ROLE);

    return item;
}

template<typename ValueType>
QStandardItem *Asn1ItemModel::createStringItem(
        const Asn1Acn::Constraints::WithConstraints<ValueType> *asn1Item, QString &typeLimit)
{
    QStandardItem *item = new QStandardItem("");

    const auto &constraints = asn1Item->constraints();
    const auto range = Asn1Acn::Constraints::SizeCombiner<ValueType>::combineSizes(&constraints);

    if (range) {
        item->setData(QVariant::fromValue(range->begin()), MIN_RANGE_ROLE);
        item->setData(QVariant::fromValue(range->end()), MAX_RANGE_ROLE);

        if (range->isSingleItem()) {
            typeLimit = QString(tr(" Length(%1)")).arg(range->begin());
        } else {
            typeLimit = QString(tr(" Length(%1..%2)")).arg(range->begin()).arg(range->end());
        }
    }

    return item;
}

/*!
 * \brief Asn1ItemModel::createSequenceItem Create an item with the type sequence
 * \param asn1Item
 * \param parent
 * \return
 */
QStandardItem *Asn1ItemModel::createSequenceItem(const Asn1Acn::Types::Sequence *asn1Item, QStandardItem *parent)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;

    for (const auto &sequenceComponent : asn1Item->components()) {
        const auto *asnSequenceComponent = dynamic_cast<const Asn1Acn::AsnSequenceComponent *>(sequenceComponent.get());

        if (!asnSequenceComponent) {
            continue;
        }

        ItemMap childItem = createModelItems(asnSequenceComponent->type(), asnSequenceComponent->name());
        if (asnSequenceComponent->isOptional()) {
            childItem["present"]->setEnabled(true);
            childItem["present"]->setCheckable(true);
        } else {
            childItem["present"]->setCheckState(Qt::Checked);
        }

        const std::optional<QString> defaultValue = asnSequenceComponent->defaultValue();
        if (defaultValue.has_value()) {
            childItem["value"]->setData(defaultValue.value(), DEFAULT_ROLE);
        }

        parent->appendRow(childItem["item"]);

        typeItems.append(childItem["type"]);
        valueItems.append(childItem["value"]);
        presentItems.append(childItem["present"]);
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    return new QStandardItem();
}

/*!
 * \brief Asn1ItemModel::createSequenceOfItem Create an item with the type sequence of
 * \param asn1Item
 * \param parent
 * \return
 */
QStandardItem *Asn1ItemModel::createSequenceOfItem(
        const Asn1Acn::Types::SequenceOf *asn1Item, QStandardItem *parent, QString &typeLimit)
{
    const auto &constraints = asn1Item->constraints();
    const auto range = Asn1Acn::Constraints::SizeCombiner<Asn1Acn::IntegerValue>::combineSizes(&constraints);

    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;

    if (asn1Item->itemsType()) {
        const auto *type = asn1Item->itemsType();

        if (type != nullptr) {
            for (int x = 0; x < range->end(); ++x) {
                ItemMap childItem = createModelItems(type);

                parent->appendRow(childItem["item"]);

                typeItems.append(childItem["type"]);
                valueItems.append(childItem["value"]);
                presentItems.append(childItem["present"]);
            }
        }
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    QStandardItem *item = new QStandardItem(range->begin());
    item->setData(asn1Item->typeEnum(), ASN1TYPE_ROLE);

    item->setData(QVariant::fromValue(range->begin()), MIN_RANGE_ROLE);
    item->setData(QVariant::fromValue(range->end()), MAX_RANGE_ROLE);

    if (range->isSingleItem()) {
        typeLimit = QString(tr(" Size(%1)")).arg(QString::number(range->begin()));
    } else {
        typeLimit =
                QString(tr(" Size(%1..%2)")).arg(QString::number(range->begin())).arg(QString::number(range->end()));
    }

    return item;
}

/*!
 * \brief Asn1ItemModel::createEnumeratedItem Create an item with the type values
 * \param asn1Item
 * \return
 */
QStandardItem *Asn1ItemModel::createEnumeratedItem(const Asn1Acn::Types::Enumerated *asn1Item)
{
    const auto &enumItems = asn1Item->items();

    QVariantList childNames;
    for (const auto &enumItem : enumItems) {
        childNames.append(enumItem.name());
    }

    QStandardItem *item = new QStandardItem;
    item->setData(asn1Item->typeEnum(), ASN1TYPE_ROLE);
    item->setData(childNames, CHOICE_LIST_ROLE);

    return item;
}

/*!
 * \brief Asn1ItemModel::createChoiceItem Create an item with the type choice
 * \param asn1Item
 * \param parent
 * \return
 */
QStandardItem *Asn1ItemModel::createChoiceItem(const Asn1Acn::Types::Choice *asn1Item, QStandardItem *parent)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;
    QVariantList childNames;

    for (const auto &choiceAlternative : asn1Item->components()) {
        ItemMap choiceItem = createModelItems(choiceAlternative->type(), choiceAlternative->name());
        parent->appendRow(choiceItem["item"]);
        typeItems.append(choiceItem["type"]);
        valueItems.append(choiceItem["value"]);
        presentItems.append(choiceItem["present"]);
        childNames.append(choiceAlternative->name());
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    QStandardItem *item = new QStandardItem(childNames[0].toString());
    item->setData(asn1Item->typeEnum(), ASN1TYPE_ROLE);
    item->setData(childNames, CHOICE_LIST_ROLE);

    return item;
}

QStandardItem *Asn1ItemModel::createPresentItem()
{
    QStandardItem *item = new QStandardItem();
    item->setEnabled(false);
    item->setEditable(false);

    return item;
}

bool Asn1ItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == MODEL_VALUE_INDEX) {
        if (auto item = itemFromIndex(index.siblingAtColumn(MODEL_IS_PRESENT_INDEX))) {
            if (item->isCheckable()) {
                item->setCheckState(value.isNull() ? Qt::Unchecked : Qt::Checked);
            }
        }
    } else if (index.column() == MODEL_IS_PRESENT_INDEX && role == Qt::CheckStateRole) {
        if (auto item = itemFromIndex(index.siblingAtColumn(MODEL_VALUE_INDEX))) {
            if (value.value<Qt::CheckState>() == Qt::Unchecked) {
                item->setText(QString());
            } else if (item->text().isEmpty()) {
                const QVariant defaultValue = item->data(DEFAULT_ROLE);
                if (!defaultValue.isNull()) {
                    item->setText(defaultValue.toString());
                }
            }
        }
    }
    return QStandardItemModel::setData(index, value, role);
}

} // namespace asn1
