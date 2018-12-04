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

class QStandardItem;

namespace asn1 {

class Asn1ItemDelegate;
class Asn1ItemModel;

class Asn1TreeView : public QTreeView
{
    Q_OBJECT

public:
    Asn1TreeView(QWidget *parent = nullptr);

    void setAsn1Model(const QVariantMap &asn1Item, int row = 0);
    void setAsn1Value(const QVariantMap &asn1Value);
    QString getAsn1Value() const;

private Q_SLOTS:
    void onSequenceOfSizeChanged(const QModelIndex &index, const QVariant value, const QVariant maxRange);
    void onChoiceFieldChanged(const QModelIndex &index, const QVariant length, const QVariant currentIndex);

private:
    void hideExtraFields(const QStandardItem *item, bool hide = false, int row = 0);

    void setChildValue(const QStandardItem *rootItem, const QVariant &asn1Value, int seqOfSize = -1, int choiceRow = -1);
    void setChildRowValue(const QStandardItem *rootItem, int childIndex, const QVariant &asn1Value);

    QVariantMap findValue(const QString &name, const QVariantMap &asn1Value) const;
    int itemChoiceIndex(const QStandardItem *item, const QString &name) const;

    QString getItemValue(const QStandardItem *item, const QString &separator = "") const;

private:
    using ItemModelPtr = QSharedPointer<Asn1ItemModel>;

    Asn1ItemDelegate *m_itemDelegate;
    ItemModelPtr m_ItemModel;
};

} // namespace asn1

#endif // ASN1TREEVIEW_H
