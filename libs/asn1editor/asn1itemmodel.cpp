/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QStandardItem>

namespace asn1 {

Asn1ItemModel::Asn1ItemModel(QObject *parent)
    : QStandardItemModel(1, 4, parent)
{
}

Asn1ItemModel::Asn1ItemModel(const QVariantMap &asn1Item, QObject *parent)
    : Asn1ItemModel(parent)
{
    setAsn1Model(asn1Item);
}

void Asn1ItemModel::setAsn1Model(const QVariantMap &asn1Item)
{
    clear();

    static QStringList headers = QStringList() << tr("Field") << tr("Type") << tr("Value") << tr("Optional");
    setHorizontalHeaderLabels(headers);

    ItemMap itemMap = createModelItems(asn1Item, false);

    setItem(0, MODEL_NAME_INDEX, itemMap["item"]);
    setItem(0, MODEL_TYPE_INDEX, itemMap["type"]);
    setItem(0, MODEL_VALUE_INDEX, itemMap["value"]);
}

Asn1ItemModel::ItemMap Asn1ItemModel::createModelItems(const QVariantMap &asn1Item, bool isPresentItem)
{
    ItemMap itemMap;
    QString typeLimit;
    QStandardItem *valueItem;
    static QMap<ASN1Type, QString> asn1TypeStringMap{
        { INTEGER, "integer" },       { DOUBLE, "double" },         { BOOL, "bool" },     { SEQUENCE, "sequence" },
        { SEQUENCEOF, "sequenceOf" }, { ENUMERATED, "enumerated" }, { CHOICE, "choice" }, { STRING, "string" }
    };

    QStandardItem *nameItem = new QStandardItem(asn1Item[ASN1_NAME].toString());
    nameItem->setEditable(false);
    auto asnType = static_cast<ASN1Type>(asn1Item[ASN1_TYPE].toInt());

    if (asnType == INTEGER || asnType == DOUBLE) {
        valueItem = createNumberItem(asn1Item);

        if (asn1Item.contains(ASN1_MIN) && asn1Item.contains(ASN1_MAX))
            typeLimit = QString(" (%1..%2)").arg(asn1Item[ASN1_MIN].toString(), asn1Item[ASN1_MAX].toString());
    } else if (asnType == BOOL)
        valueItem = createBoolItem(asn1Item);
    else if (asnType == SEQUENCE)
        valueItem = createSequenceItem(asn1Item, nameItem);
    else if (asnType == SEQUENCEOF) {
        valueItem = createSequenceOfItem(asn1Item, nameItem);

        if (asn1Item.contains(ASN1_MIN) && asn1Item.contains(ASN1_MAX)) {
            if (asn1Item[ASN1_MIN] == asn1Item[ASN1_MAX])
                typeLimit = QString(tr(" Size(%1)")).arg(asn1Item[ASN1_MIN].toString());
            else
                typeLimit =
                        QString(tr(" Size(%1..%2)")).arg(asn1Item[ASN1_MIN].toString(), asn1Item[ASN1_MAX].toString());
        }
    } else if (asnType == ENUMERATED)
        valueItem = createEnumeratedItem(asn1Item);
    else if (asnType == CHOICE)
        valueItem = createChoiceItem(asn1Item, nameItem);
    else if (asnType == STRING) {
        valueItem = createItem(asn1Item);

        if (asn1Item.contains(ASN1_MIN) && asn1Item.contains(ASN1_MAX)) {
            if (asn1Item[ASN1_MIN] == asn1Item[ASN1_MAX])
                typeLimit = QString(tr(" Length(%1)")).arg(asn1Item[ASN1_MIN].toString());
            else
                typeLimit = QString(tr(" Length(%1..%2)"))
                                    .arg(asn1Item[ASN1_MIN].toString(), asn1Item[ASN1_MAX].toString());
        }
    } else
        valueItem = new QStandardItem();

    QStandardItem *typeItem = new QStandardItem(asn1TypeStringMap[asnType] + typeLimit);
    typeItem->setData(QBrush(QColor("gray")), Qt::ForegroundRole);
    typeItem->setEditable(false);

    itemMap["item"] = nameItem;
    itemMap["type"] = typeItem;
    itemMap["value"] = valueItem;

    if (isPresentItem)
        itemMap["present"] = createPresentItem(asn1Item);

    return itemMap;
}

QStandardItem *Asn1ItemModel::createNumberItem(QVariantMap asn1Item)
{
    // set default value (min range):
    return createItem(asn1Item, asn1Item[ASN1_MIN].toString());
}

QStandardItem *Asn1ItemModel::createBoolItem(QVariantMap asn1Item)
{
    static const QVariantList choices{ QString("true"), QString("false") };

    QStandardItem *item = createItem(asn1Item, "false");
    item->setData(choices, CHOICE_LIST_ROLE);

    return item;
}

QStandardItem *Asn1ItemModel::createSequenceItem(QVariantMap asn1Item, QStandardItem *parent)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;

    QVariantList children = asn1Item[ASN1_CHILDREN].toList();

    for (const auto &child : children) {
        ItemMap chilItem = createModelItems(child.toMap());

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

QStandardItem *Asn1ItemModel::createSequenceOfItem(QVariantMap asn1Item, QStandardItem *parent)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;

    auto typeOfMap = asn1Item[ASN1_SEQOFTYPE].type() == QVariant::List
            ? asn1Item[ASN1_SEQOFTYPE].toList().value(0).toMap()
            : asn1Item[ASN1_SEQOFTYPE].toMap();

    for (int x = 0; x < asn1Item[ASN1_MAX].toInt(); ++x) {
        ItemMap chilItem = createModelItems(typeOfMap);

        chilItem["item"]->setText(QString(tr("elem%1")).arg(x + 1));
        parent->appendRow(chilItem["item"]);

        typeItems.append(chilItem["type"]);
        valueItems.append(chilItem["value"]);
        presentItems.append(chilItem["present"]);
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    return createNumberItem(asn1Item);
}

QStandardItem *Asn1ItemModel::createEnumeratedItem(QVariantMap asn1Item)
{
    return createItem(asn1Item, asn1Item[ASN1_VALUES].toList().at(0).toString());
}

QStandardItem *Asn1ItemModel::createChoiceItem(QVariantMap asn1Item, QStandardItem *parent)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;
    QVariantList chilsNames;

    QVariantList choices = asn1Item[ASN1_CHOICES].toList();
    for (const auto &choice : choices) {
        ItemMap choiceItem = createModelItems(choice.toMap());

        parent->appendRow(choiceItem["item"]);

        typeItems.append(choiceItem["type"]);
        valueItems.append(choiceItem["value"]);
        presentItems.append(choiceItem["present"]);

        chilsNames.append(choice.toMap()[ASN1_NAME]);
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    QStandardItem *item = createItem(asn1Item, chilsNames[0].toString());
    item->setData(chilsNames, CHOICE_LIST_ROLE);

    return item;
}

QStandardItem *Asn1ItemModel::createItem(QVariantMap asn1Item, const QString &text)
{
    QStandardItem *item = new QStandardItem(text);

    item->setData(asn1Item[ASN1_TYPE], ASN1TYPE_ROLE);
    if (asn1Item.contains(ASN1_MIN))
        item->setData(asn1Item[ASN1_MIN], MIN_RANGE_ROLE);

    if (asn1Item.contains(ASN1_MAX))
        item->setData(asn1Item[ASN1_MAX], MAX_RANGE_ROLE);

    if (asn1Item.contains(ASN1_VALUES))
        item->setData(asn1Item[ASN1_VALUES], CHOICE_LIST_ROLE);

    return item;
}

QStandardItem *Asn1ItemModel::createPresentItem(QVariantMap asn1Item)
{
    QStandardItem *item = new QStandardItem();

    if (asn1Item[ASN1_IS_OPTIONAL].toBool() == true) {
        item->setCheckState(Qt::Unchecked);
        item->setCheckable(true);
    }

    item->setData(asn1Item[ASN1_IS_OPTIONAL], OPTIONAL_ROLE);

    return item;
}

} // namespace asn1
