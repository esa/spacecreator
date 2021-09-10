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

#include "asn1itemdelegate.h"

#include "asn1editorconst.h"
#include "types/type.h"

#include <QComboBox>
#include <QPainter>
#include <QSize>
#include <QSpinBox>
#include <QTextEdit>
#include <limits>

namespace asn1 {

Asn1ItemDelegate::Asn1ItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void Asn1ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    painter->save();

    QPen pen(Qt::lightGray);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    painter->setPen(pen);

    QRect rc(option.rect);
    if (index.column() == 0)
        rc.setLeft(0);

    painter->drawLines({ QLineF(rc.left(), rc.bottom(), rc.right(), rc.bottom()),
            QLineF(rc.right(), rc.top(), rc.right(), rc.bottom()) });

    painter->restore();
}

QSize Asn1ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index) + QSize(3, 3);
}

QWidget *Asn1ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    QWidget *editor = nullptr;
    auto asnType = static_cast<Asn1Acn::Types::Type::ASN1Type>(index.data(ASN1TYPE_ROLE).toInt());

    switch (asnType) {
    case Asn1Acn::Types::Type::SEQUENCEOF: {
        QSpinBox *spinBox = new QSpinBox(parent);

        if (index.data(MIN_RANGE_ROLE).isValid())
            spinBox->setMinimum(index.data(MIN_RANGE_ROLE).toInt());

        if (index.data(MAX_RANGE_ROLE).isValid())
            spinBox->setMaximum(index.data(MAX_RANGE_ROLE).toInt());

        editor = spinBox;
        break;
    }
    case Asn1Acn::Types::Type::INTEGER:
    case Asn1Acn::Types::Type::REAL: {
        QDoubleSpinBox *spinBox = new QDoubleSpinBox(parent);
        if (asnType == Asn1Acn::Types::Type::INTEGER) {
            spinBox->setDecimals(0);
        }

        spinBox->setMinimum(index.data(MIN_RANGE_ROLE).isValid() ? index.data(MIN_RANGE_ROLE).toDouble()
                                                                 : std::numeric_limits<double>::min());
        spinBox->setMaximum(index.data(MAX_RANGE_ROLE).isValid() ? index.data(MAX_RANGE_ROLE).toDouble()
                                                                 : std::numeric_limits<double>::max());
        spinBox->setValue(0);
        editor = spinBox;
        break;
    }
    case Asn1Acn::Types::Type::ENUMERATED:
    case Asn1Acn::Types::Type::CHOICE:
    case Asn1Acn::Types::Type::BOOLEAN: {
        QComboBox *comboBox = new QComboBox(parent);
        comboBox->addItems(index.data(CHOICE_LIST_ROLE).toStringList());
        comboBox->setCurrentIndex(0);
        editor = comboBox;

        break;
    }
    default:
        editor = new QTextEdit(parent);
    }

    return editor;
}

void Asn1ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto asnType = static_cast<Asn1Acn::Types::Type::ASN1Type>(index.data(ASN1TYPE_ROLE).toInt());

    switch (asnType) {
    case Asn1Acn::Types::Type::SEQUENCEOF:
        qobject_cast<QSpinBox *>(editor)->setValue(index.data().toInt());
        break;
    case Asn1Acn::Types::Type::ENUMERATED:
    case Asn1Acn::Types::Type::CHOICE:
    case Asn1Acn::Types::Type::BOOLEAN:
        qobject_cast<QComboBox *>(editor)->setCurrentText(index.data().toString());
        break;
    case Asn1Acn::Types::Type::INTEGER:
    case Asn1Acn::Types::Type::REAL:
        qobject_cast<QDoubleSpinBox *>(editor)->setValue(index.data().toDouble());
        break;
    default:
        qobject_cast<QTextEdit *>(editor)->setText(index.data().toString());
    }
}

void Asn1ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto asnType = static_cast<Asn1Acn::Types::Type::ASN1Type>(index.data(ASN1TYPE_ROLE).toInt());
    QVariant value;

    switch (asnType) {
    case Asn1Acn::Types::Type::SEQUENCEOF:
        value = qobject_cast<QSpinBox *>(editor)->value();
        break;
    case Asn1Acn::Types::Type::ENUMERATED:
    case Asn1Acn::Types::Type::CHOICE:
    case Asn1Acn::Types::Type::BOOLEAN:
        value = qobject_cast<QComboBox *>(editor)->currentText();
        break;
    case Asn1Acn::Types::Type::INTEGER:
    case Asn1Acn::Types::Type::REAL:
        value = qobject_cast<QDoubleSpinBox *>(editor)->value();
        break;
    default:
        value = qobject_cast<QTextEdit *>(editor)->toPlainText();
    }

    model->setData(index, value);

    if (asnType == Asn1Acn::Types::Type::SEQUENCEOF)
        Q_EMIT sequenceOfSizeChanged(index.sibling(index.row(), 0), value, index.data(MAX_RANGE_ROLE));

    if (asnType == Asn1Acn::Types::Type::CHOICE)
        Q_EMIT choiceFieldChanged(index.sibling(index.row(), 0), index.data(CHOICE_LIST_ROLE).toList().size(),
                qobject_cast<QComboBox *>(editor)->currentIndex());
}

void Asn1ItemDelegate::updateEditorGeometry(
        QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect editorRect;

    if (static_cast<Asn1Acn::Types::Type::ASN1Type>(index.data(ASN1TYPE_ROLE).toInt()) == Asn1Acn::Types::Type::STRING)
        editorRect = QRect(option.rect.x(), option.rect.y(), option.rect.width(), qMax(option.rect.height(), 100));
    else
        editorRect = option.rect;

    editor->setGeometry(editorRect);
}

} // namespace asn1
