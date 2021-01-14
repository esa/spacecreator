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

#pragma once

#include <QMap>
#include <QStandardItemModel>
#include <memory>

class QStandardItem;

namespace Asn1Acn {
namespace Types {
class Type;
}
class TypeAssignment;
}

namespace asn1 {

class Asn1ItemModel : public QStandardItemModel
{
    Q_OBJECT

public:
    Asn1ItemModel(QObject *parent = nullptr);
    Asn1ItemModel(const std::unique_ptr<Asn1Acn::TypeAssignment> &asn1Item, QObject *parent = nullptr);

    void setAsn1Model(const std::unique_ptr<Asn1Acn::TypeAssignment> &asn1Item);

private:
    using ItemMap = QMap<QString, QStandardItem *>;

    ItemMap createModelItems(const Asn1Acn::Types::Type *asn1Item, const QString &name = {});

    QStandardItem *createNumberItem(const Asn1Acn::Types::Type *asn1Item);
    QStandardItem *createBoolItem(const Asn1Acn::Types::Type *asn1Item);
    QStandardItem *createChildItems(const Asn1Acn::Types::Type *asn1Item, QStandardItem *parent);
    QStandardItem *createSequenceOfItem(const Asn1Acn::Types::Type *asn1Item, QStandardItem *parent);
    QStandardItem *createEnumeratedItem(const Asn1Acn::Types::Type *asn1Item);
    QStandardItem *createChoiceItem(const Asn1Acn::Types::Type *asn1Item, QStandardItem *parent);
    QStandardItem *createItem(const Asn1Acn::Types::Type *asn1Item, const QString &text = QString());

    QStandardItem *createPresentItem(const Asn1Acn::Types::Type *asn1Item);
};

} // namespace asn1
