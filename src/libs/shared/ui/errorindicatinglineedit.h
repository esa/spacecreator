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

#include <QLineEdit>

namespace shared {

/**
 * A QLineEdit that visually highlights, when the current text is no valid input.
 * This does need to have a proper QValidator set.
 */
class ErrorIndicatingLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit ErrorIndicatingLineEdit(QWidget *parent = nullptr);
    explicit ErrorIndicatingLineEdit(const QString &text, QWidget *parent = nullptr);

protected Q_SLOTS:
    virtual void updateErrorIndication();
};

} // namespace shared
