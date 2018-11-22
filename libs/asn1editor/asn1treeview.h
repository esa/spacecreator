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
    using ItemMap = QMap<QString, QStandardItem *>;

    ItemMap createModelItems(QVariantMap asn1Item);

    QStandardItem *createNumberItem(QVariantMap asn1Item);
    QStandardItem *createBoolItem(QVariantMap asn1Item);
    QStandardItem *createSequenceItem(QVariantMap asn1Item, QStandardItem *parent);
    QStandardItem *createSequenceOfItem(QVariantMap asn1Item, QStandardItem *parent);
    QStandardItem *createEnumeratedItem(QVariantMap asn1Item);
    QStandardItem *createChoiceItem(QVariantMap asn1Item, QStandardItem *parent);
    QStandardItem *createStringItem(QVariantMap asn1Item);

    QStandardItem *createPresentItem(QVariantMap asn1Item);

    void hideExtraFields(const QStandardItem *item, bool hide = false, int row = 0);

private:
    Asn1ItemDelegate *m_itemDelegate;
    ItemPtr m_nameItem;
    ItemPtr m_typeItem;
    ItemPtr m_valueItem;
};

} // namespace asn1

#endif // ASN1TREEVIEW_H
