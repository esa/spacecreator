#include <QComboBox>
#include <QSpinBox>
#include <QTextEdit>

#include "asn1itemdelegate.h"

Asn1ItemDelegate::Asn1ItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

QSize Asn1ItemDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(100, 25);
}

QWidget *Asn1ItemDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &,
                                        const QModelIndex &index) const
{
    QWidget *editor = nullptr;
    QString asnType = index.data(ASN1TYPE).toString();

    if (asnType == "integer" || asnType == "sequenceOf") {
        editor = new QSpinBox(parent);
        qobject_cast<QSpinBox*>(editor)->setMinimum(index.data(MIN_RANGE).toInt());
        qobject_cast<QSpinBox*>(editor)->setMaximum(index.data(MAX_RANGE).toInt());
    }
    else if (asnType == "double") {
        editor = new QDoubleSpinBox(parent);
        qobject_cast<QDoubleSpinBox*>(editor)->setMinimum(index.data(MIN_RANGE).toDouble());
        qobject_cast<QDoubleSpinBox*>(editor)->setMaximum(index.data(MAX_RANGE).toDouble());
    }
    else if (asnType == "enumerated" ||
             asnType == "choice" ||
             asnType == "bool") {
        editor = new QComboBox(parent);

        QVariantList enumVals = index.data(CHOICE_LIST).toList();
        for (const auto &val : enumVals)
            qobject_cast<QComboBox*>(editor)->addItem(val.toString());
    }
    else if (asnType == "string") {
        editor = new QTextEdit(parent);
    }

    return editor;
}

void Asn1ItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString asnType = index.data(ASN1TYPE).toString();

    if (asnType == "integer" || asnType == "sequenceOf")
        qobject_cast<QSpinBox*>(editor)->setValue(index.data().toInt());
    else if (asnType == "enumerated" ||
             asnType == "choice" ||
             asnType == "bool")
        qobject_cast<QComboBox*>(editor)->setCurrentText(index.data().toString());
    else if (asnType == "double")
        qobject_cast<QDoubleSpinBox*>(editor)->setValue(index.data().toDouble());
    else if (asnType == "string")
        qobject_cast<QTextEdit*>(editor)->setText(index.data().toString());
}

void Asn1ItemDelegate::setModelData(QWidget *editor,
                                    QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QString asnType = index.data(ASN1TYPE).toString();
    QVariant value;

    if (asnType == "integer" || asnType == "sequenceOf")
        value = qobject_cast<QSpinBox*>(editor)->value();
    else if (asnType == "enumerated" ||
             asnType == "choice" ||
             asnType == "bool")
        value = qobject_cast<QComboBox*>(editor)->currentText();
    else if (asnType == "double")
        value = qobject_cast<QDoubleSpinBox*>(editor)->value();
    else if (asnType == "string")
        value = qobject_cast<QTextEdit*>(editor)->toPlainText();

    model->setData(index, value);

    if (asnType == "sequenceOf")
        const_cast< Asn1ItemDelegate* >(this)->emit sequenceOfSizeChanged(index.sibling(index.row(), 0),
                                                                          value,
                                                                          index.data(MAX_RANGE));

    if (asnType == "choice")
        const_cast< Asn1ItemDelegate* >(this)->emit choiceFieldChanged(index.sibling(index.row(), 0),
                                                                       index.data(CHOICE_LIST).toList().size(),
                                                                       qobject_cast<QComboBox*>(editor)->currentIndex());
}

void Asn1ItemDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    QRect editorRect;

    QString asnType = index.data(ASN1TYPE).toString();
    if (asnType == "string")
        editorRect = QRect(option.rect.x(), option.rect.y(),
                  option.rect.width(), qMax(option.rect.height(), 100));
    else
        editorRect = option.rect;

    editor->setGeometry(editorRect);
}

