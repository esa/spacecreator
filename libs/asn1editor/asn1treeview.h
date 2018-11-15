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

#ifndef ASN1TREEVIEW_H
#define ASN1TREEVIEW_H

#include <QMap>
#include <QTreeView>

class QStandardItemModel;
class QStandardItem;

namespace asn1 {

class Asn1ItemDelegate;

class Asn1TreeView : public QTreeView
{
    Q_OBJECT

public:
    Asn1TreeView(QWidget *parent = nullptr);

    void setAsn1Model(QVariantMap asn1Item, int row = 0);

private Q_SLOTS:
    void onSequenceOfSizeChanged(const QModelIndex &index, QVariant value, QVariant maxRange);
    void onChoiceFieldChanged(const QModelIndex &index, QVariant length, QVariant currentIndex);

private:
    using ItemPtr = QSharedPointer<QStandardItem>;
    using ItemMap = QMap<QString, ItemPtr>;

    ItemMap createModelItems(QVariantMap asn1Item);

    ItemPtr createNumberItem(QVariantMap asn1Item);
    ItemPtr createBoolItem(QVariantMap asn1Item);
    ItemPtr createSequenceItem(QVariantMap asn1Item, ItemPtr &parent);
    ItemPtr createSequenceOfItem(QVariantMap asn1Item, ItemPtr &parent);
    ItemPtr createEnumeratedItem(QVariantMap asn1Item);
    ItemPtr createChoiceItem(QVariantMap asn1Item, ItemPtr &parent);
    ItemPtr createStringItem(QVariantMap asn1Item);

    ItemPtr createPresentItem(QVariantMap asn1Item);

    void hideExtraFields(const QStandardItem *item, bool hide = false, int row = 0);

private:
    using ModelPtr = QSharedPointer<QStandardItemModel>;

    Asn1ItemDelegate *m_itemDelegate;
    ModelPtr m_model;
    ItemPtr m_rootTreeItem;
};

} // namespace asn1

#endif // ASN1TREEVIEW_H
