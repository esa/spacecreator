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
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivobject.h"
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

static void setConfiguredEditorData(QWidget *editor, const QVariant &displayValue, const QVariant::Type type)
{
    if (type == QVariant::Invalid)
        return;

    switch (type) {
    case QVariant::Bool:
        if (auto checkBox = qobject_cast<QCheckBox *>(editor)) {
            checkBox->setChecked(
                    QString::compare(displayValue.toString(), QLatin1String("true"), Qt::CaseInsensitive) == 0);
        }
        break;
    case QVariant::Int:
        if (auto spinBox = qobject_cast<QSpinBox *>(editor)) {
            spinBox->setValue(displayValue.toInt());
        }
        break;
    case QVariant::Double:
        if (auto doubleSpinBox = qobject_cast<QDoubleSpinBox *>(editor)) {
            doubleSpinBox->setValue(displayValue.toDouble());
        }
        break;
    case QVariant::String:
        if (auto lineEdit = qobject_cast<QLineEdit *>(editor)) {
            lineEdit->setText(displayValue.toString());
        }
        break;
    case QVariant::StringList:
        if (auto comboBox = qobject_cast<QComboBox *>(editor)) {
            comboBox->setCurrentText(displayValue.toString());
        }
        break;
    default:
        break;
    }
}

static QWidget *createConfiguredEditor(
        const QString &attribute, const QVariant &editValue, const QVariant &validator, QWidget *parent)
{
    if (!editValue.isValid())
        return nullptr;

    QWidget *editor { nullptr };
    switch (editValue.type()) {
    case QVariant::Int: {
        auto spinBox = new QSpinBox(parent);
        if (!validator.isNull() && validator.canConvert<QPair<int, int>>()) {
            const auto limits = validator.value<QPair<int, int>>();
            spinBox->setMinimum(limits.first);
            spinBox->setMaximum(limits.second);
        }
        editor = spinBox;
        break;
    }
    case QVariant::String: {
        auto lineEdit = new QLineEdit(parent);
        QRegularExpression re;
        if (attribute == ivm::meta::Props::token(ivm::meta::Props::Token::name)) {
            re.setPattern(ivm::IVNameValidator::namePatternUI());
        } else if (!validator.isNull() && validator.canConvert<QString>()) {
            re.setPattern(validator.toString());
        }
        lineEdit->setValidator(new QRegularExpressionValidator(re, lineEdit));
        editor = lineEdit;
        break;
    }
    case QVariant::Double: {
        auto doubleSpinBox = new QDoubleSpinBox(parent);
        if (!validator.isNull() && validator.canConvert<QPair<qreal, qreal>>()) {
            const auto limits = validator.value<QPair<qreal, qreal>>();
            doubleSpinBox->setMinimum(limits.first);
            doubleSpinBox->setMaximum(limits.second);
        }
        editor = doubleSpinBox;
        break;
    }
    case QVariant::Bool: {
        editor = new QCheckBox(parent);
        break;
    }
    case QVariant::StringList: {
        auto comboBox = new QComboBox(parent);
        comboBox->addItems(editValue.toStringList());
        editor = comboBox;
        break;
    }
    default:
        return nullptr;
    }
    Q_ASSERT(editor);
    if (editor) {
        editor->setFocusPolicy(Qt::StrongFocus);
    }
    return editor;
}

QWidget *AttributeDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        QWidget *editor = createConfiguredEditor(PropertiesListModel::tokenNameFromIndex(index),
                index.data(PropertiesListModel::EditRole), index.data(PropertiesListModel::ValidatorRole), parent);
        if (editor) {
            editor->setEnabled(index.flags().testFlag(Qt::ItemIsEnabled));
        }
        return editor;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void AttributeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.isValid()) {
        setConfiguredEditorData(
                editor, index.data(PropertiesListModel::DataRole), index.data(PropertiesListModel::EditRole).type());
        return;
    }

    return QStyledItemDelegate::setEditorData(editor, index);
}

void AttributeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QVariant data;
    switch (index.data(PropertiesListModel::EditRole).type()) {
    case QVariant::Bool:
        if (auto checkBox = qobject_cast<QCheckBox *>(editor)) {
            data = checkBox->isChecked();
        }
        break;
    case QVariant::Int:
        if (auto spinBox = qobject_cast<QSpinBox *>(editor)) {
            spinBox->interpretText();
            data = spinBox->value();
        }
        break;
    case QVariant::Double:
        if (auto doubleSpibBox = qobject_cast<QDoubleSpinBox *>(editor)) {
            doubleSpibBox->interpretText();
            data = doubleSpibBox->value();
        }
        break;
    case QVariant::String:
        if (auto lineEdit = qobject_cast<QLineEdit *>(editor)) {
            data = lineEdit->text();
        }
        break;
    case QVariant::StringList:
        if (auto comboBox = qobject_cast<QComboBox *>(editor)) {
            data = comboBox->currentText();
        }
        break;
    default:
        break;
    }
    if (data.isValid()) {
        if (!model->setData(index, data, PropertiesListModel::DataRole)) {
            setConfiguredEditorData(editor, index.data(PropertiesListModel::DataRole),
                    index.data(PropertiesListModel::EditRole).type());
        }
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

} // namespace ive
