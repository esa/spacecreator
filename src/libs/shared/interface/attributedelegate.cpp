/*
  Copyright (C) 2020-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "propertieslistmodel.h"
#include "ui/errorindicatinglineedit.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSpinBox>
#include <QTimer>

namespace shared {

AttributeDelegate::AttributeDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

static void setConfiguredEditorData(QWidget *editor, const QVariant &displayValue, const QVariant::Type type)
{
    if (type == QVariant::Invalid)
        return;

    // Silences some warnings on commitData being called with a widget not belonging to the view.
    QSignalBlocker blocker(editor);

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

template<typename T>
inline void connectCommitData(T *editor, QAbstractItemDelegate *context)
{
    QObject::connect(editor, &T::editingFinished, context, [context, editor]() { context->commitData(editor); });
}

template<>
inline void connectCommitData(QCheckBox *editor, QAbstractItemDelegate *context)
{
    QObject::connect(editor, &QCheckBox::stateChanged, context, [context, editor]() { context->commitData(editor); });
}

template<>
inline void connectCommitData(QComboBox *editor, QAbstractItemDelegate *context)
{
    QObject::connect(
            editor, &QComboBox::currentTextChanged, context, [context, editor]() { context->commitData(editor); });
}

static QWidget *createConfiguredEditor(QAbstractItemDelegate *context, const QString &attribute,
        const QVariant &editValue, const QVariant &validator, QWidget *parent)
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
        connectCommitData(spinBox, context);
        editor = spinBox;
        break;
    }
    case QVariant::String: {
        auto lineEdit = new ErrorIndicatingLineEdit(parent);
        QRegularExpression re;
        if (!validator.isNull() && validator.canConvert<QString>()) {
            re.setPattern(validator.toString());
        }
        lineEdit->setValidator(new QRegularExpressionValidator(re, lineEdit));
        connectCommitData(lineEdit, context);
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
        connectCommitData(doubleSpinBox, context);
        editor = doubleSpinBox;
        break;
    }
    case QVariant::Bool: {
        auto checkbox = new QCheckBox(parent);
        connectCommitData(checkbox, context);
        editor = checkbox;
        break;
    }
    case QVariant::StringList: {
        auto comboBox = new QComboBox(parent);
        comboBox->addItems(editValue.toStringList());
        connectCommitData(comboBox, context);
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
        const QString tokenName = tokenNameFromIndex(index);
        QWidget *editor = createConfiguredEditor(const_cast<AttributeDelegate *>(this),
                tokenName, index.data(PropertiesListModel::EditRole),
                index.data(PropertiesListModel::ValidatorRole), parent);
        if (editor) {
            editor->setEnabled(index.flags().testFlag(Qt::ItemIsEnabled));
        }

        /// Workaroung for #699 because of autoselecting data for QLineEdit after editor creation
        /// https://code.qt.io/cgit/qt/qtbase.git/tree/src/widgets/itemviews/qabstractitemview.cpp#n4351
        if (auto lineEdit = qobject_cast<QLineEdit *>(editor)) {
            QTimer::singleShot(0, lineEdit, [lineEdit, name = tokenName]() {
                if (QString::compare(name, QLatin1String("name"), Qt::CaseInsensitive))
                    lineEdit->deselect();
            });
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

QString AttributeDelegate::tokenNameFromIndex(const QModelIndex &index) const
{
    return PropertiesListModel::tokenNameFromIndex(index);
}

} // namespace shared
