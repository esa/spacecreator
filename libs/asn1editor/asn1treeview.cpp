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

#include <QHeaderView>
#include <QStandardItem>
#include <QStandardItemModel>

#include "asn1itemdelegate.h"
#include "asn1treeview.h"

namespace asn1 {

Asn1TreeView::Asn1TreeView(QWidget *parent)
    : QTreeView(parent)
{
    m_itemDelegate = new Asn1ItemDelegate(this);
    setItemDelegate(m_itemDelegate);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors(true);
    setDragEnabled(false);

    connect(m_itemDelegate, &Asn1ItemDelegate::sequenceOfSizeChanged, this, &Asn1TreeView::onSequenceOfSizeChanged);
    connect(m_itemDelegate, &Asn1ItemDelegate::choiceFieldChanged, this, &Asn1TreeView::onChoiceFieldChanged);
}

void Asn1TreeView::setAsn1Model(QVariantMap asn1Item, int row)
{
    QStandardItemModel *model = new QStandardItemModel(1, 4, this);
    QStringList headers = QStringList() << tr("Field")
                                        << tr("Type")
                                        << tr("Value")
                                        << tr("Present");
    model->setHorizontalHeaderLabels(headers);
    setModel(model);

    header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    ItemMap itemMap = createModelItems(asn1Item);

    m_nameItem.reset(itemMap["item"]);
    m_typeItem.reset(itemMap["type"]);
    m_valueItem.reset(itemMap["value"]);

    model->setItem(row, 0, m_nameItem.data());
    model->setItem(row, 1, m_typeItem.data());
    model->setItem(row, 2, m_valueItem.data());

    hideExtraFields(m_nameItem.data(), true, row);

    expandAll();
}

void Asn1TreeView::onSequenceOfSizeChanged(const QModelIndex &index, QVariant value, QVariant maxRange)
{
    for (int x = 0; x < maxRange.toInt(); ++x)
        setRowHidden(x, index, x < value.toInt() ? false : true);

    expand(index);
}

void Asn1TreeView::onChoiceFieldChanged(const QModelIndex &index, QVariant length, QVariant currentIndex)
{
    for (int x = 0; x < length.toInt(); ++x)
        setRowHidden(x, index, x == currentIndex.toInt() ? false : true);

    expand(index);
}

Asn1TreeView::ItemMap Asn1TreeView::createModelItems(QVariantMap asn1Item)
{
    ItemMap itemMap;
    QString typeLimit;
    QStandardItem *valueItem;

    QStandardItem *nameItem = new QStandardItem(asn1Item["name"].toString());
    nameItem->setEditable(false);

    if (asn1Item["type"] == "integer" || asn1Item["type"] == "double") {
        valueItem = createNumberItem(asn1Item);

        typeLimit = QString(" (%1..%2)").arg(asn1Item["min"].toString(), asn1Item["max"].toString());
    } else if (asn1Item["type"] == "bool")
        valueItem = createBoolItem(asn1Item);
    else if (asn1Item["type"] == "sequence")
        valueItem = createSequenceItem(asn1Item, nameItem);
    else if (asn1Item["type"] == "sequenceOf") {
        valueItem = createSequenceOfItem(asn1Item, nameItem);

        if (asn1Item["min"] == asn1Item["max"])
            typeLimit = QString(tr(" Size(%1)")).arg(asn1Item["min"].toString());
        else
            typeLimit = QString(tr(" Size(%1..%2)")).arg(asn1Item["min"].toString(), asn1Item["max"].toString());
    } else if (asn1Item["type"] == "enumerated")
        valueItem = createEnumeratedItem(asn1Item);
    else if (asn1Item["type"] == "choice")
        valueItem = createChoiceItem(asn1Item, nameItem);
    else if (asn1Item["type"] == "string") {
        valueItem = createStringItem(asn1Item);

        if (asn1Item["min"] == asn1Item["max"])
            typeLimit = QString(tr(" Length(%1)")).arg(asn1Item["min"].toString());
        else
            typeLimit = QString(tr(" Length(%1..%2)")).arg(asn1Item["min"].toString(), asn1Item["max"].toString());
    } else
        valueItem = new QStandardItem();

    QStandardItem *typeItem = new QStandardItem(asn1Item["type"].toString() + typeLimit);
    typeItem->setData(QBrush(QColor("gray")), Qt::ForegroundRole);
    typeItem->setEditable(false);

    itemMap["item"] = nameItem;
    itemMap["type"] = typeItem;
    itemMap["value"] = valueItem;
    itemMap["present"] = createPresentItem(asn1Item);

    return itemMap;
}

QStandardItem *Asn1TreeView::createNumberItem(QVariantMap asn1Item)
{
    // set default value (min range):
    QStandardItem *item = new QStandardItem(asn1Item["min"].toString());

    item->setData(asn1Item["type"], ASN1TYPE);
    item->setData(asn1Item["min"], MIN_RANGE);
    item->setData(asn1Item["max"], MAX_RANGE);

    return item;
}

QStandardItem *Asn1TreeView::createBoolItem(QVariantMap asn1Item)
{
    static const QVariantList choices { QString("True"), QString("False") };

    QStandardItem *item = new QStandardItem(asn1Item["default"].toString());

    item->setData(asn1Item["type"], ASN1TYPE);
    item->setData(choices, CHOICE_LIST);

    return item;
}

QStandardItem *Asn1TreeView::createSequenceItem(QVariantMap asn1Item, QStandardItem *parent)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;

    QVariantList children = asn1Item["children"].toList();

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

QStandardItem *Asn1TreeView::createSequenceOfItem(QVariantMap asn1Item, QStandardItem *parent)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;

    for (int x = 0; x < asn1Item["max"].toInt(); ++x) {
        ItemMap chilItem = createModelItems(asn1Item["seqoftype"].toMap());

        chilItem["item"]->setText(QString(tr("elem%1")).arg(x + 1));
        parent->appendRow(chilItem["item"]);

        typeItems.append(chilItem["type"]);
        valueItems.append(chilItem["value"]);
        presentItems.append(chilItem["present"]);
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    QStandardItem *item = new QStandardItem(asn1Item["min"].toString());

    item->setData(asn1Item["type"], ASN1TYPE);
    item->setData(asn1Item["min"], MIN_RANGE);
    item->setData(asn1Item["max"], MAX_RANGE);

    return item;
}

QStandardItem *Asn1TreeView::createEnumeratedItem(QVariantMap asn1Item)
{
    QStandardItem *item = new QStandardItem(asn1Item["values"].toString());

    item->setData(asn1Item["type"], ASN1TYPE);
    item->setData(asn1Item["values"], CHOICE_LIST);
    item->setData(asn1Item["valuesInt"], INTMAP);

    return item;
}

QStandardItem *Asn1TreeView::createChoiceItem(QVariantMap asn1Item, QStandardItem *parent)
{
    QList<QStandardItem *> typeItems;
    QList<QStandardItem *> valueItems;
    QList<QStandardItem *> presentItems;
    QVariantList chilsNames;

    QVariantList choices = asn1Item["choices"].toList();
    for (const auto &choice : choices) {
        ItemMap choiceItem = createModelItems(choice.toMap());

        parent->appendRow(choiceItem["item"]);

        typeItems.append(choiceItem["type"]);
        valueItems.append(choiceItem["value"]);
        presentItems.append(choiceItem["present"]);

        chilsNames.append(choice.toMap()["name"]);
    }

    parent->appendColumn(typeItems);
    parent->appendColumn(valueItems);
    parent->appendColumn(presentItems);

    QStandardItem *item = new QStandardItem(chilsNames[0].toString());

    item->setData(asn1Item["type"], ASN1TYPE);
    item->setData(chilsNames, CHOICE_LIST);
    item->setData(asn1Item["choiceIdx"], INTMAP);

    return item;
}

QStandardItem *Asn1TreeView::createStringItem(QVariantMap asn1Item)
{
    QStandardItem *item = new QStandardItem();

    item->setData(asn1Item["type"], ASN1TYPE);
    item->setData(asn1Item["min"], MIN_RANGE);
    item->setData(asn1Item["max"], MAX_RANGE);

    return item;
}

QStandardItem *Asn1TreeView::createPresentItem(QVariantMap asn1Item)
{
    QStandardItem *item = new QStandardItem();

    if (asn1Item["isOptional"].toBool() == true) {
        item->setCheckState(Qt::Checked);
        item->setCheckable(true);
    }

    item->setData(asn1Item["isOptional"], OPTIONAL);

    return item;
}

void Asn1TreeView::hideExtraFields(const QStandardItem *item, bool hide, int row)
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(this->model());
    QString asnType = model->item(row, 1)->text();

    if (asnType == "choice" && hide) {
        QVariantList choices = model->item(row, 2)->data(CHOICE_LIST).toList();
        onChoiceFieldChanged(model->item(row)->index(),
                             choices.size(),
                             choices.indexOf(model->item(row, 2)->text()));
    } else if (asnType.indexOf("sequenceOf") >= 0 && hide) {
        onSequenceOfSizeChanged(model->item(row)->index(),
                                model->item(row, 2)->text(),
                                model->item(row, 2)->data(MAX_RANGE));
    }

    for (int x = 0; x < item->rowCount(); ++x) {
        hideExtraFields(item->child(x));

        asnType = item->child(x, 1)->text();
        if (asnType.indexOf("sequenceOf") >= 0)
            onSequenceOfSizeChanged(item->child(x)->index(),
                                    item->child(x, 2)->text(),
                                    item->child(x, 2)->data(MAX_RANGE));
        else if (asnType == "choice") {
            QVariantList choices = item->child(x, 2)->data(CHOICE_LIST).toList();
            onChoiceFieldChanged(item->child(x)->index(),
                                 choices.size(),
                                 choices.indexOf(item->child(x, 2)->text()));
        }
    }
}

} // namespace asn1
