/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "attributedelegate.h"

#include "aadlcommonprops.h"
#include "aadlnamevalidator.h"
#include "aadlobject.h"
#include "aadlobjectsmodel.h"
#include "interface/properties/propertieslistmodel.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSpinBox>

namespace ive {

AttributeDelegate::AttributeDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

static QWidget *createConfiguredEditor(
        const QVariant &displayValue, const QVariant &editValue, QWidget *parent, const QString &attribute)
{
    if (editValue.type() == QVariant::Invalid)
        return nullptr;

    switch (editValue.type()) {
    case QVariant::Int: {
        auto editor = new QSpinBox(parent);
        editor->setMinimum(std::numeric_limits<int>::min());
        editor->setMaximum(std::numeric_limits<int>::max());
        editor->setValue(displayValue.toInt());
        return editor;
    }
    case QVariant::String: {
        auto editor = new QLineEdit(parent);
        if (attribute == aadl::meta::Props::token(aadl::meta::Props::Token::name)) {
            QRegularExpression re(aadl::AADLNameValidator::namePatternUI());
            editor->setValidator(new QRegularExpressionValidator(re, editor));
        }
        editor->setText(displayValue.toString());
        return editor;
    }
    case QVariant::Double: {
        auto editor = new QDoubleSpinBox(parent);
        editor->setMinimum(std::numeric_limits<double>::min());
        editor->setMaximum(std::numeric_limits<double>::max());
        editor->setValue(displayValue.toDouble());
        return editor;
    }
    case QVariant::Bool: {
        auto editor = new QCheckBox(parent);
        editor->setChecked(QString::compare(displayValue.toString(), QLatin1String("YES"), Qt::CaseInsensitive)
                || QString::compare(displayValue.toString(), QLatin1String("true"), Qt::CaseInsensitive));
        return editor;
    }
    case QVariant::StringList: {
        auto editor = new QComboBox(parent);
        const QStringList items = editValue.toStringList();
        editor->addItems(items);
        editor->setCurrentText(displayValue.toString());
        return editor;
    }
    default:
        return nullptr;
    }
}

QWidget *AttributeDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == PropertiesListModel::ColumnValue) {
        if (const auto pModel = qobject_cast<const QStandardItemModel *>(index.model())) {
            if (QStandardItem *item = pModel->itemFromIndex(index)) {
                QModelIndex attrIdx = index.siblingAtColumn(PropertiesListModel::ColumnTitle);
                return createConfiguredEditor(item->data(Qt::EditRole),
                        item->data(PropertiesListModel::PropertyDataRole), parent, attrIdx.data().toString());
            }
        }
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void AttributeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == PropertiesListModel::ColumnValue) {
        if (const auto pModel = qobject_cast<const QStandardItemModel *>(index.model())) {
            if (QStandardItem *item = pModel->itemFromIndex(index)) {
                if (item->data(PropertiesListModel::PropertyDataRole).type() == QVariant::StringList) {
                    if (QComboBox *cb = qobject_cast<QComboBox *>(editor)) {
                        cb->setCurrentText(item->data(Qt::EditRole).toString());
                    }
                }
            }
        }
    }

    return QStyledItemDelegate::setEditorData(editor, index);
}

} // namespace ive
