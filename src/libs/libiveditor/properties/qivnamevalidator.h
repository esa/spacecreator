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

#pragma once

#include <QPointer>
#include <QValidator>

namespace ivm {
class IVObject;
}

namespace ive {

/**
 * QValidator for text input to check for valid names/titles for IVObjects
 */
class QIVNameValidator : public QValidator
{
public:
    explicit QIVNameValidator(ivm::IVObject *object, QObject *parent = nullptr);

    State validate(QString &input, int &pos) const override;
    void fixup(QString &) const override;

    void setSecondValidator(const QValidator *validator);

private:
    QPointer<ivm::IVObject> m_object;
    QPointer<QValidator> m_secondValidator;
};

}
