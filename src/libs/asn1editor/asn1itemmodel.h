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
#include <optional>

class QStandardItem;

namespace Asn1Acn {
namespace Constraints {
template<typename ValueType>
class Constraint;
template<typename ValueType>
class ConstraintList;
template<typename ValueType>
class WithConstraints;
}
namespace Types {
class Boolean;
class Choice;
class Enumerated;
class Sequence;
class SequenceOf;
class Type;
}
class IntegerValue;
template<typename T>
class Range;
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

    template<typename ValueType>
    QStandardItem *createNumberItem(
            const Asn1Acn::Constraints::WithConstraints<ValueType> *asn1Item, QString &typeLimit);
    QStandardItem *createBoolItem(const Asn1Acn::Types::Boolean *asn1Item);
    template<typename ValueType>
    QStandardItem *createStringItem(
            const Asn1Acn::Constraints::WithConstraints<ValueType> *asn1Item, QString &typeLimit);
    QStandardItem *createSequenceItem(const Asn1Acn::Types::Sequence *asn1Item, QStandardItem *parent);
    QStandardItem *createSequenceOfItem(
            const Asn1Acn::Types::SequenceOf *asn1Item, QStandardItem *parent, QString &typeLimit);
    QStandardItem *createEnumeratedItem(const Asn1Acn::Types::Enumerated *asn1Item);
    QStandardItem *createChoiceItem(const Asn1Acn::Types::Choice *asn1Item, QStandardItem *parent);

    QStandardItem *createPresentItem();
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    template<typename ValueType>
    std::optional<Asn1Acn::Range<typename ValueType::Type>> combineRanges(
            const Asn1Acn::Constraints::ConstraintList<ValueType> &constraintList);
    template<typename ValueType>
    std::optional<Asn1Acn::Range<typename ValueType::Type>> combineRange(
            const Asn1Acn::Constraints::Constraint<ValueType> *constraint);

    template<typename ValueType>
    std::optional<Asn1Acn::Range<int64_t>> combineSizes(
            const Asn1Acn::Constraints::ConstraintList<ValueType> &constraintList);
    template<typename ValueType>
    std::optional<Asn1Acn::Range<int64_t>> combineSize(const Asn1Acn::Constraints::Constraint<ValueType> *constraint);
};

} // namespace asn1
