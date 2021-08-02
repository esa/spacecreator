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
#include "typeassignment.h"
#include "types/userdefinedtype.h"

#include <QStandardItem>

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

    static QStringList headers = { tr("Field"), tr("Type"), tr("Value"), tr("Optional") };
    setHorizontalHeaderLabels(headers);

    ItemMap itemMap = createModelItems(asn1Item->type());

    setItem(0, MODEL_NAME_INDEX, itemMap["item"]);
    setItem(0, MODEL_TYPE_INDEX, itemMap["type"]);
    setItem(0, MODEL_VALUE_INDEX, itemMap["value"]);
    setItem(0, MODEL_IS_OPTIONAL_INDEX, itemMap["present"]);
}

/*!
 * \brief Asn1ItemModel::createModelItems Create items for the tree view from the map
 * \param asn1Item The map with the items
 * \return The created item map
 */
Asn1ItemModel::ItemMap Asn1ItemModel::createModelItems(const Asn1Acn::Types::Type *asn1Item, const QString &name)
{
    ItemMap itemMap;
    QString typeLimit;
    QStandardItem *valueItem = nullptr;

    QStandardItem *nameItem = new QStandardItem(name.isEmpty() ? asn1Item->identifier() : name);
    nameItem->setEditable(false);

    const QVariantMap &values = asn1Item->parameters();

    switch (asn1Item->typeEnum()) {
    case Asn1Acn::Types::Type::INTEGER:
    case Asn1Acn::Types::Type::REAL:
        valueItem = createNumberItem(asn1Item);

        if (values.contains(Asn1Acn::ASN1_MIN) && values.contains(Asn1Acn::ASN1_MAX)) {
            typeLimit = QString(" (%1..%2)")
                                .arg(values[Asn1Acn::ASN1_MIN].toString(), values[Asn1Acn::ASN1_MAX].toString());
        }
        break;
    case Asn1Acn::Types::Type::BOOLEAN:
        valueItem = createBoolItem(asn1Item);
        break;
    case Asn1Acn::Types::Type::SEQUENCE:
        valueItem = createChildItems(asn1Item, nameItem);
        break;
    case Asn1Acn::Types::Type::SEQUENCEOF:
        valueItem = createSequenceOfItem(asn1Item, nameItem);
        if (values.contains(Asn1Acn::ASN1_MIN) && values.contains(Asn1Acn::ASN1_MAX)) {
            if (values[Asn1Acn::ASN1_MIN] == values[Asn1Acn::ASN1_MAX])
                typeLimit = QString(tr(" Size(%1)")).arg(values[Asn1Acn::ASN1_MIN].toString());
            else
                typeLimit = QString(tr(" Size(%1..%2)"))
                                    .arg(values[Asn1Acn::ASN1_MIN].toString(), values[Asn1Acn::ASN1_MAX].toString());
        }
        break;
    case Asn1Acn::Types::Type::ENUMERATED:
        valueItem = createEnumeratedItem(asn1Item);
        break;
    case Asn1Acn::Types::Type::CHOICE:
        valueItem = createChoiceItem(asn1Item, nameItem);
        break;
    case Asn1Acn::Types::Type::STRING:
    case Asn1Acn::Types::Type::OCTETSTRING:
    case Asn1Acn::Types::Type::BITSTRING:
    case Asn1Acn::Types::Type::NUMERICSTRING:
    case Asn1Acn::Types::Type::IA5STRING:
        valueItem = createItem(asn1Item);
        if (values.contains(Asn1Acn::ASN1_MIN) && values.contains(Asn1Acn::ASN1_MAX)) {
            if (values[Asn1Acn::ASN1_MIN] == values[Asn1Acn::ASN1_MAX])
                typeLimit = QString(tr(" Length(%1)")).arg(values[Asn1Acn::ASN1_MIN].toString());
            else
                typeLimit = QString(tr(" Length(%1..%2)"))
                                    .arg(values[Asn1Acn::ASN1_MIN].toString(), values[Asn1Acn::ASN1_MAX].toString());
        }
        break;
    case Asn1Acn::Types::Type::USERDEFINED: {
        auto userType = static_cast<const Asn1Acn::Types::UserdefinedType *>(asn1Item);
        if (userType->referencedType()) {
            return createModelItems(userType->referencedType()->type(), asn1Item->identifier());
        } else if (!asn1Item->children().empty()) {
            valueItem = createItem(asn1Item->children().at(0).get());
        }
        break;
    }
    default:
        if (!asn1Item->children().empty()) {
            valueItem = createChildItems(asn1Item, nameItem);
            valueItem->setEditable(false);
        }
    }

    if (!valueItem) {
        valueItem = createItem(asn1Item);
    }

    QStandardItem *typeItem = new QStandardItem(asn1Item->typeName() + typeLimit);
    typeItem->setData(QBrush(QColor("gray")), Qt::ForegroundRole);
    typeItem->setEditable(false);

    itemMap["item"] = nameItem;
    itemMap["type"] = typeItem;
    itemMap["value"] = valueItem;
    itemMap["present"] = createPresentItem(asn1Item);

    return itemMap;
}

/*!
 * \brief Asn1ItemModel::createNumberItem Create an item with the type number
 * \param asn1Item
 * \return
 */
QStandardItem *Asn1ItemModel::createNumberItem(const Asn1Acn::Types::Type *asn1Item)
{
    // set default value (min range):
    const QVariantMap &values = asn1Item->parameters();
    return createItem(asn1Item, values[Asn1Acn::ASN1_MIN].toString());
}

/*!
 * \brief Asn1ItemModel::createBoolItem  Create an item with the type bool
 * \param asn1Item
 * \return
 */
QStandardItem *Asn1ItemModel::createBoolItem(const Asn1Acn::Types::Type *asn1Item)
{
    static const QVariantList choices { QString("true"), QString("false") };

    QStandardItem *item = createItem(asn1Item, "false");
    item->setData(choices, CHOICE_LIST_ROLE);

    return item;
}

/*!
 * \brief Asn1ItemModel::createSequenceItem Create an item with the type sequence
 * \param asn1Item
 * \param parent
 * \return
 */
QStandardItem *Asn1ItemModel::createChildItems(const Asn1Acn::Types::Type *asn1Item, QStandardItem *parent)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;

    for (const std::unique_ptr<Asn1Acn::Types::Type> &sequence : asn1Item->children()) {
        ItemMap chilItem = createModelItems(sequence.get());

        parent->appendRow(chilItem["item"]);

        typeItems.append(chilItem["type"]);
        valueItems.append(chilItem["value"]);
        presentItems.append(chilItem["present"]);
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
QStandardItem *Asn1ItemModel::createSequenceOfItem(const Asn1Acn::Types::Type *asn1Item, QStandardItem *parent)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;

    if (!asn1Item->children().empty()) {
        const Asn1Acn::Types::Type *type = asn1Item->children().at(0).get();

        if (type != nullptr) {
            for (int x = 0; x < asn1Item->parameters()[Asn1Acn::ASN1_MAX].toInt(); ++x) {
                ItemMap chilItem = createModelItems(type);

                chilItem["item"]->setText(QString(tr("elem%1")).arg(x + 1));
                parent->appendRow(chilItem["item"]);

                typeItems.append(chilItem["type"]);
                valueItems.append(chilItem["value"]);
                presentItems.append(chilItem["present"]);
            }
        }
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    return createNumberItem(asn1Item);
}

/*!
 * \brief Asn1ItemModel::createEnumeratedItem Create an item with the type values
 * \param asn1Item
 * \return
 */
QStandardItem *Asn1ItemModel::createEnumeratedItem(const Asn1Acn::Types::Type *asn1Item)
{
    const QVariantMap &values = asn1Item->parameters();
    return createItem(asn1Item, values[Asn1Acn::ASN1_VALUES].toList().at(0).toString());
}

/*!
 * \brief Asn1ItemModel::createChoiceItem Create an item with the type choice
 * \param asn1Item
 * \param parent
 * \return
 */
QStandardItem *Asn1ItemModel::createChoiceItem(const Asn1Acn::Types::Type *asn1Item, QStandardItem *parent)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;
    QVariantList chilsNames;

    for (const std::unique_ptr<Asn1Acn::Types::Type> &choice : asn1Item->children()) {
        ItemMap choiceItem = createModelItems(choice.get());
        parent->appendRow(choiceItem["item"]);
        typeItems.append(choiceItem["type"]);
        valueItems.append(choiceItem["value"]);
        presentItems.append(choiceItem["present"]);
        chilsNames.append(choice->identifier());
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    QStandardItem *item = createItem(asn1Item, chilsNames[0].toString());
    item->setData(chilsNames, CHOICE_LIST_ROLE);

    return item;
}

/*!
 * \brief Asn1ItemModel::createItem Create an item for the model with \a text
 * \param asn1Item Map used to set the type, min/max and choice list
 * \param text The view text
 * \return The created item
 */
QStandardItem *Asn1ItemModel::createItem(const Asn1Acn::Types::Type *asn1Item, const QString &text)
{
    QStandardItem *item = new QStandardItem(text);

    Asn1Acn::Types::Type::ASN1Type asntype = asn1Item->typeEnum();
    if (asntype == Asn1Acn::Types::Type::USERDEFINED) {
        auto userType = static_cast<const Asn1Acn::Types::UserdefinedType *>(asn1Item);
        if (userType->referencedType()) {
            asntype = userType->referencedType()->typeEnum();
        }
    }

    item->setData(asntype, ASN1TYPE_ROLE);
    const QVariantMap &values = asn1Item->parameters();
    if (values.contains(Asn1Acn::ASN1_MIN))
        item->setData(values[Asn1Acn::ASN1_MIN], MIN_RANGE_ROLE);

    if (values.contains(Asn1Acn::ASN1_MAX))
        item->setData(values[Asn1Acn::ASN1_MAX], MAX_RANGE_ROLE);

    if (values.contains(Asn1Acn::ASN1_VALUES))
        item->setData(values[Asn1Acn::ASN1_VALUES], CHOICE_LIST_ROLE);

    return item;
}

QStandardItem *Asn1ItemModel::createPresentItem(const Asn1Acn::Types::Type *asn1Item)
{
    QStandardItem *item = new QStandardItem();

    const QVariantMap &values = asn1Item->parameters();
    if (values[Asn1Acn::ASN1_IS_OPTIONAL].toBool() == true) {
        item->setCheckState(Qt::Unchecked);
        item->setCheckable(true);
    } else {
        item->setEnabled(false);
    }

    item->setData(values[Asn1Acn::ASN1_IS_OPTIONAL], OPTIONAL_ROLE);

    return item;
}

} // namespace asn1
