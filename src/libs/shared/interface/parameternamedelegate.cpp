/*
   Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public License
along with this program. If not, see
<https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "parameternamedelegate.h"

#include "common.h"
#include "ui/errorindicatinglineedit.h"

#include <QValidator>

namespace shared {

/**
 * Validator for valid names of parameter names
 */
class NameValidator : public QValidator
{
public:
    explicit NameValidator(QObject *parent = nullptr)
        : QValidator(parent)
    {
    }

    State validate(QString &input, int &pos) const override
    {
        Q_UNUSED(pos);

        // check for forbidden characters
        static const QRegularExpression re(shared::namePatternUI);
        const QRegularExpressionMatch match = re.match(input);
        if (!match.hasMatch()) {
            return QValidator::Invalid;
        }

        // check for forbidden names etc.
        return isValidName(input) ? QValidator::Acceptable : QValidator::Intermediate;
    }
};

ParameterNameDelegate::ParameterNameDelegate(QObject *parent)
    : QStyledItemDelegate { parent }
{
}

QWidget *ParameterNameDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto lineEdit = new ErrorIndicatingLineEdit(parent);
    lineEdit->setText(index.data().toString());

    auto validator = new NameValidator(lineEdit);
    lineEdit->setValidator(validator);

    return lineEdit;
}

} // namespace shared
