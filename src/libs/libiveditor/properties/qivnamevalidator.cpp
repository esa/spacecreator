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
along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "qivnamevalidator.h"

#include "ivnamevalidator.h"
#include "ivobject.h"

namespace ive {

QIVNameValidator::QIVNameValidator(ivm::IVObject *object, QObject *parent)
    : QValidator { parent }
    , m_object(object)
{
}

QValidator::State QIVNameValidator::validate(QString &input, int &pos) const
{
    if (m_secondValidator) {
        QValidator::State result = m_secondValidator->validate(input, pos);
        if (result != QValidator::Acceptable) {
            return result;
        }
    }

    return ivm::IVNameValidator::isAcceptableName(m_object, input) ? QValidator::Acceptable : QValidator::Intermediate;
}

void QIVNameValidator::fixup(QString &input) const
{
    if (m_secondValidator) {
        m_secondValidator->fixup(input);
    } else {
        QValidator::fixup(input);
    }
}

void QIVNameValidator::setSecondValidator(const QValidator *validator)
{
    m_secondValidator = const_cast<QValidator *>(validator);
}

}
