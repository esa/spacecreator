﻿/*
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

#include "asn1treeview.h"

#include "asn1editorconst.h"
#include "asn1itemdelegate.h"
#include "asn1itemmodel.h"
#include "typeassignment.h"

#include <QHeaderView>
#include <QStandardItem>
#include <QStandardItemModel>

namespace asn1 {

/*!
 * \class Asn1TreeView
 *
 * This is a widget that can show the contents of an ASN.1 file in a tree view
 */
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

/*!
 * \brief Asn1TreeView::setAsn1Model Set the model of a row to \a asn1Item
 */
void Asn1TreeView::setAsn1Model(const std::unique_ptr<Asn1Acn::TypeAssignment> &asn1Item, int row)
{
    m_ItemModel.reset(new Asn1ItemModel(asn1Item, this));
    setModel(m_ItemModel.data());

    header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    hideExtraFields(m_ItemModel->item(0, 0), true, row);

    expandAll();
}

/*!
 * \brief Asn1TreeView::setAsn1Value Update the value list with the values in \a asn1Value
 */
void Asn1TreeView::setAsn1Value(const QVariantMap &asn1Value)
{
    if (model() == nullptr) {
        return;
    }

    QStandardItem *nameItem = m_ItemModel->item(0, 0);

    int row = nameItem->row();
    QString asnType = m_ItemModel->item(row, MODEL_TYPE_INDEX)->text();

    if (asnType.startsWith("sequenceOf", Qt::CaseInsensitive)) {
        int seqOfSize = asn1Value["seqofvalue"].toList().count();
        m_ItemModel->item(row, MODEL_VALUE_INDEX)->setText(QString::number(seqOfSize));
        setChildValue(nameItem, asn1Value["seqofvalue"], seqOfSize);
    } else if (asnType.startsWith("choice", Qt::CaseInsensitive)) {
        QString choiceValue = asn1Value["choice"].toMap()["name"].toString();
        m_ItemModel->item(row, MODEL_VALUE_INDEX)->setText(choiceValue);
        setChildValue(nameItem, asn1Value["choice"], -1, itemChoiceIndex(nameItem, choiceValue));
    } else if (asnType.startsWith("integer", Qt::CaseInsensitive) || asnType.startsWith("double", Qt::CaseInsensitive)
            || asnType.startsWith("real", Qt::CaseInsensitive) || asnType.startsWith("string", Qt::CaseInsensitive)
            || asnType.startsWith("enumerated", Qt::CaseInsensitive)
            || asnType.startsWith("BIT STRING", Qt::CaseInsensitive)
            || asnType.startsWith("OCTET STRING", Qt::CaseInsensitive)
            || asnType.startsWith("IA5String", Qt::CaseInsensitive)
            || asnType.startsWith("NumericString", Qt::CaseInsensitive)) {
        m_ItemModel->item(row, MODEL_VALUE_INDEX)->setText(asn1Value["value"].toString());
    } else if (asnType.startsWith("bool", Qt::CaseInsensitive)) {
        m_ItemModel->item(row, MODEL_VALUE_INDEX)->setText(asn1Value["value"].toString().toLower());
    } else if (asnType.startsWith("sequence", Qt::CaseInsensitive)) {
        setChildValue(nameItem, asn1Value["children"]);
    }

    hideExtraFields(nameItem, true, row);
    expandAll();
}

QString Asn1TreeView::getAsn1Value() const
{
    return m_ItemModel ? getItemValue(m_ItemModel->item(0, 0)) : QString();
}

void Asn1TreeView::onSequenceOfSizeChanged(const QModelIndex &index, const QVariant &value, const QVariant &maxRange)
{
    for (int x = 0; x < maxRange.toInt(); ++x)
        setRowHidden(x, index, x < value.toInt() ? false : true);

    expand(index);
}

void Asn1TreeView::onChoiceFieldChanged(const QModelIndex &index, const QVariant &length, const QVariant &currentIndex)
{
    for (int x = 0; x < length.toInt(); ++x)
        setRowHidden(x, index, x == currentIndex.toInt() ? false : true);

    expand(index);
}

/*!
 * \brief Asn1TreeView::hideExtraFields Hide a value row
 * \param item
 * \param hide
 * \param row
 */
void Asn1TreeView::hideExtraFields(const QStandardItem *item, bool hide, int row)
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(this->model());
    auto asnType = static_cast<Asn1Acn::Types::Type::ASN1Type>(
            model->item(row, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt());

    if (asnType == Asn1Acn::Types::Type::CHOICE && hide) {
        const QVariantList &choices = model->item(row, MODEL_VALUE_INDEX)->data(CHOICE_LIST_ROLE).toList();
        onChoiceFieldChanged(model->item(row)->index(), choices.size(),
                choices.indexOf(model->item(row, MODEL_VALUE_INDEX)->text()));
    } else if (asnType == Asn1Acn::Types::Type::SEQUENCEOF && hide) {
        onSequenceOfSizeChanged(model->item(row)->index(), model->item(row, MODEL_VALUE_INDEX)->text(),
                model->item(row, MODEL_VALUE_INDEX)->data(MAX_RANGE_ROLE));
    }

    for (int x = 0; x < item->rowCount(); ++x) {
        hideExtraFields(item->child(x));

        asnType = static_cast<Asn1Acn::Types::Type::ASN1Type>(
                item->child(x, MODEL_VALUE_INDEX)->data(ASN1TYPE_ROLE).toInt());
        if (asnType == Asn1Acn::Types::Type::SEQUENCEOF)
            onSequenceOfSizeChanged(item->child(x)->index(), item->child(x, MODEL_VALUE_INDEX)->text(),
                    item->child(x, MODEL_VALUE_INDEX)->data(MAX_RANGE_ROLE));
        else if (asnType == Asn1Acn::Types::Type::CHOICE) {
            const QVariantList &choices = item->child(x, MODEL_VALUE_INDEX)->data(CHOICE_LIST_ROLE).toList();
            onChoiceFieldChanged(item->child(x)->index(), choices.size(),
                    choices.indexOf(item->child(x, MODEL_VALUE_INDEX)->text()));
        }
    }
}

/*!
 * \brief Asn1TreeView::setChildRowValue Update the value of a child row
 * \param rootItem
 * \param childIndex
 * \param asn1Value
 */
void Asn1TreeView::setChildRowValue(const QStandardItem *rootItem, int childIndex, const QVariant &asn1Value)
{
    const QStandardItem *childItem = rootItem->child(childIndex, MODEL_TYPE_INDEX);
    const QString asnType = childItem ? rootItem->child(childIndex, MODEL_TYPE_INDEX)->text() : "";
    QStandardItem *child = rootItem->child(childIndex, MODEL_VALUE_INDEX);
    if (!child) {
        return;
    }
    const QString defaultValue = child->data(DEFAULT_ROLE).toString();

    if (asnType.startsWith("integer", Qt::CaseInsensitive) || asnType.startsWith("double", Qt::CaseInsensitive)
            || asnType.startsWith("real", Qt::CaseInsensitive) || asnType.startsWith("string", Qt::CaseInsensitive)
            || asnType.startsWith("enumerated", Qt::CaseInsensitive)
            || asnType.startsWith("BIT STRING", Qt::CaseInsensitive)
            || asnType.startsWith("OCTET STRING", Qt::CaseInsensitive)
            || asnType.startsWith("IA5String", Qt::CaseInsensitive)
            || asnType.startsWith("NumericString", Qt::CaseInsensitive)) {
        child->setText(asn1Value.toMap()["value"].toString());
    } else if (asnType.startsWith("bool", Qt::CaseInsensitive)) {
        child->setText(asn1Value.toMap()["value"].toString().toLower());
    } else if (asnType.startsWith("sequenceOf", Qt::CaseInsensitive)
            || asnType.startsWith("sequence of", Qt::CaseInsensitive)) {
        int seqOfSize = asn1Value.toMap()["seqofvalue"].toList().count();
        seqOfSize = ensureValueInRange(seqOfSize, child);
        child->setText(QString::number(seqOfSize));
        setChildValue(rootItem->child(childIndex), asn1Value.toMap()["seqofvalue"], seqOfSize);
    } else if (asnType.startsWith("sequence", Qt::CaseInsensitive)) {
        setChildValue(rootItem->child(childIndex), asn1Value.toMap()["children"]);
    } else if (asnType.startsWith("choice", Qt::CaseInsensitive)) {
        const QString choiceValue = asn1Value.toMap()["choice"].toMap()["name"].toString();
        child->setText(choiceValue);
        setChildRowValue(rootItem->child(childIndex), itemChoiceIndex(rootItem->child(childIndex), choiceValue),
                asn1Value.toMap()["choice"]);
    }
    if (child->text().isEmpty()) {
        if (!defaultValue.isEmpty()) {
            child->setText(defaultValue);
        }
    } else if (auto present = rootItem->child(childIndex, MODEL_IS_PRESENT_INDEX)) {
        if (present->isCheckable()) {
            present->setCheckState(Qt::Checked);
        }
    }
}

/*!
 * \brief Asn1TreeView::setChildValue Update the value of a child
 * \param rootItem
 * \param asn1Value
 * \param seqOfSize
 * \param choiceRow
 */
void Asn1TreeView::setChildValue(const QStandardItem *rootItem, const QVariant &asn1Value, int seqOfSize, int choiceRow)
{
    if (!asn1Value.isValid())
        return;

    if (choiceRow != -1) {
        // asn1Value = map
        setChildRowValue(rootItem, choiceRow, asn1Value);
    } else if (rootItem->hasChildren()) {
        const QVariantList childData = asn1Value.toList();
        for (int idx = 0; idx < rootItem->rowCount(); ++idx) {
            const QString name = rootItem->child(idx, MODEL_NAME_INDEX)->text();
            QVariant value;
            if (!name.isEmpty()) {
                auto it = std::find_if(childData.cbegin(), childData.cend(), [name](const QVariant &value) {
                    if (!value.isValid())
                        return false;

                    return value.toMap().value(QLatin1String("name")).toString() == name;
                });
                if (it != childData.cend()) {
                    value = *it;
                }
            } else {
                value = childData.value(idx);
            }

            setChildRowValue(rootItem, idx, value);
        }
    }
}

/*!
 * \brief Asn1TreeView::findValue Find all values with \a name in \a asn1Value
 * \return A map of the values
 */
QVariantMap Asn1TreeView::findValue(const QString &name, const QVariantMap &asn1Value) const
{
    QVariantMap result;

    if (asn1Value["name"] == name)
        return asn1Value;

    if (asn1Value.contains("children")) {
        for (const QVariant &child : asn1Value["children"].toList()) {
            result = findValue(name, child.toMap());
            if (result.size())
                break;
        }
    }

    if (asn1Value.contains("choice"))
        result = findValue(name, asn1Value["choice"].toMap());

    return result;
}

/*!
 * \brief Asn1TreeView::itemChoiceIndex Find the index with \a name
 * \param item the item to search in
 * \param name the name to search for
 * \return the index of the row or 0 if none was found
 */
int Asn1TreeView::itemChoiceIndex(const QStandardItem *item, const QString &name) const
{
    int choiceIndex = 0;

    for (choiceIndex = 0; choiceIndex < item->rowCount(); ++choiceIndex) {
        if (name == item->child(choiceIndex, MODEL_NAME_INDEX)->text()) {
            break;
        }
    }

    return choiceIndex;
}

/*!
 * \brief Asn1TreeView::getItemValue Get a string representation of an item
 * \param item The item to stringify
 * \param separator The separator between the values
 * \return The generated string
 */
QString Asn1TreeView::getItemValue(const QStandardItem *item, const QString &separator) const
{
    if (!item) {
        return {};
    }

    QString itemValue = "";

    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(this->model());

    QModelIndex itemIndex = model->indexFromItem(item);
    QModelIndex modelIndex = itemIndex.sibling(item->row(), MODEL_TYPE_INDEX);
    const QString asnType = model->itemFromIndex(modelIndex)->text();

    QString asnValue;
    modelIndex = itemIndex.sibling(item->row(), MODEL_VALUE_INDEX);
    if (modelIndex.isValid()) {
        asnValue = model->itemFromIndex(modelIndex)->text();
    }

    if (asnValue.isEmpty()) {
        auto presentIndex = itemIndex.sibling(item->row(), MODEL_IS_PRESENT_INDEX);
        if (auto presentItem = model->itemFromIndex(presentIndex)) {
            if (presentItem->isCheckable()) {
                if (presentItem->checkState() == Qt::Unchecked) {
                    return {};
                }
            }
        }
    }

    auto sequenceData = [this](const QStandardItem *item, int itemCount, const QString &separator) -> QString {
        QStringList values;
        for (int x = 0; x < itemCount; ++x) {
            const QString sequenceItemValue = getItemValue(item->child(x), separator);
            if (sequenceItemValue.isNull()) {
                continue;
            }
            values.append(sequenceItemValue);
        }
        if (!values.isEmpty()) {
            return QStringLiteral("{ %1 }").arg(values.join(QLatin1String(", ")));
        }
        return {};
    };

    if (!separator.isEmpty() && !item->text().isEmpty()) {
        itemValue = QString("%1%2 ").arg(item->text(), separator);
    }

    if (asnType.startsWith("bool", Qt::CaseInsensitive)) {
        itemValue += asnValue.toUpper();
    } else if (asnType.startsWith("choice", Qt::CaseInsensitive)) {
        itemValue += getItemValue(item->child(itemChoiceIndex(item, asnValue)), " :");
    } else if (asnType.startsWith("sequenceOf", Qt::CaseInsensitive)
            || asnType.startsWith("sequence of", Qt::CaseInsensitive)) {
        itemValue += sequenceData(item, asnValue.toInt(), QString());
    } else if (asnType.startsWith("sequence", Qt::CaseInsensitive)) {
        itemValue += sequenceData(item, item->rowCount(), QLatin1String(" "));
    } else if (asnType.startsWith("string", Qt::CaseInsensitive) || asnType.startsWith("IA5String", Qt::CaseInsensitive)
            || asnType.startsWith("NumericString", Qt::CaseInsensitive)) {
        if (!asnValue.startsWith("\"")) {
            itemValue.append("\"");
        }
        itemValue += asnValue;
        if (!asnValue.endsWith("\"")) {
            itemValue.append("\"");
        }
    } else if (asnType.startsWith("BIT STRING", Qt::CaseInsensitive)
            || asnType.startsWith("OCTET STRING", Qt::CaseInsensitive)) {
        if (!asnValue.startsWith("'")) {
            itemValue.append("'");
        }
        itemValue += asnValue;
        if (!asnValue.endsWith("'H") && !asnValue.endsWith("'B")) {
            itemValue.append("'H");
        }
    } else if (item->child(0, 0)) {
        itemValue += getItemValue(item->child(0, 0), " :");
    } else {
        //(asnType == "integer" || asnType == "double" || asnType == "enumerated")
        itemValue += asnValue;
    }

    return itemValue;
}

/*!
 * Returns \p value if within the given range. If value is samller, the range minimum is returned. If larger than max,
 * the range maximum is returned.
 * \param value The input value
 * \param valueItem The QStandardItem holds the min and max values. Accesible via the roles MIN_RANGE_ROLE and
 *  MAX_RANGE_ROLE
 * \return The value to be surely within the range
 */
int Asn1TreeView::ensureValueInRange(int value, const QStandardItem *valueItem)
{
    bool ok;
    const int min = valueItem->data(asn1::MIN_RANGE_ROLE).toInt(&ok);
    if (ok) {
        value = std::max(value, min);
    }
    const int max = valueItem->data(asn1::MAX_RANGE_ROLE).toInt(&ok);
    if (ok) {
        value = std::min(value, max);
    }
    return value;
}

} // namespace asn1
