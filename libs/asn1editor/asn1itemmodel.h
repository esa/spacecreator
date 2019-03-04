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

#ifndef ASN1ITEMMODEL_H
#define ASN1ITEMMODEL_H

#include <QStandardItemModel>

class QStandardItem;

namespace asn1 {

class Asn1ItemModel : public QStandardItemModel
{
    Q_OBJECT

public:
    Asn1ItemModel(QObject *parent = nullptr);
    Asn1ItemModel(const QVariantMap &asn1Item, QObject *parent = nullptr);

    void setAsn1Model(const QVariantMap &asn1Item);

private:
    using ItemMap = QMap<QString, QStandardItem *>;

    ItemMap createModelItems(const QVariantMap &asn1Item, bool isPresentItem = true);

    QStandardItem *createNumberItem(const QVariantMap &asn1Item);
    QStandardItem *createBoolItem(const QVariantMap &asn1Item);
    QStandardItem *createSequenceItem(const QVariantMap &asn1Item, QStandardItem *parent);
    QStandardItem *createSequenceOfItem(const QVariantMap &asn1Item, QStandardItem *parent);
    QStandardItem *createEnumeratedItem(const QVariantMap &asn1Item);
    QStandardItem *createChoiceItem(const QVariantMap &asn1Item, QStandardItem *parent);
    QStandardItem *createItem(const QVariantMap &asn1Item, const QString &text = QString());

    QStandardItem *createPresentItem(const QVariantMap &asn1Item);
};

} // namespace asn1

#endif // ASN1ITEMMODEL_H
