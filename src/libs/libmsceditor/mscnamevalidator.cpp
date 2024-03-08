/*
  Copyright (C) 2024 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscnamevalidator.h"

#include "common.h"
#include "mscentity.h"

#include <QRegularExpressionValidator>

namespace msc {

MscNameValidator::MscNameValidator(QObject *parent)
    : QValidator { parent }
{
}

QValidator::State MscNameValidator::validate(QString &input, int &pos) const
{
    if (shared::isForbiddenName(input)) {
        return QValidator::Intermediate;
    }

    auto regexValidator = QRegularExpressionValidator(msc::MscEntity::nameVerifier(), parent());
    return regexValidator.validate(input, pos);
}

void MscNameValidator::fixup(QString &input) const { }

} // namespace msc
