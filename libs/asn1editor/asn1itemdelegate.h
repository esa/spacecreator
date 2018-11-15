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

#ifndef ASN1ITEMDELEGATE_H
#define ASN1ITEMDELEGATE_H

#include <QStyledItemDelegate>

namespace asn1 {

const int ASN1TYPE = Qt::UserRole;
const int MIN_RANGE = Qt::UserRole + 1;
const int MAX_RANGE = Qt::UserRole + 2;
const int CHOICE_LIST = Qt::UserRole + 3;
const int INTMAP = Qt::UserRole + 4;
const int PLOTTERS = Qt::UserRole + 5;
const int OPTIONAL = Qt::UserRole + 6;
const int ALWAYS_PRESENT = Qt::UserRole + 7;
const int ALWAYS_ABSENT = Qt::UserRole + 8;

class Asn1ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    Asn1ItemDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
Q_SIGNALS:
    void sequenceOfSizeChanged(const QModelIndex &index, QVariant value, QVariant maxRange);
    void choiceFieldChanged(const QModelIndex &index, QVariant lenght, QVariant currentIndex);
};

} // namespace asn1

#endif // ASN1ITEMDELEGATE_H
