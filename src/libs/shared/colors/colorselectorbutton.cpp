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

#include "colorselectorbutton.h"

#include <QColorDialog>

namespace shared {

ColorSelectorButton::ColorSelectorButton(QWidget *parent)
    : QToolButton(parent)
{
    connect(this, &QToolButton::clicked, this, &ColorSelectorButton::onClicked);
}

QColor ColorSelectorButton::color() const
{
    return m_color;
}

void ColorSelectorButton::setColor(const QColor &color, AcceptPolicy accept)
{
    if (accept == AcceptPolicy::ValidColors && !color.isValid())
        return;

    if (color != m_color) {
        m_color = color;

        QPixmap px(48, 48);
        px.fill(m_color);
        setIcon(px);

        Q_EMIT colorChanged(m_color);
    }
}

void ColorSelectorButton::onClicked()
{
    const QColor c = QColorDialog::getColor(m_color, this, tr("Select the color"), QColorDialog::ShowAlphaChannel);
    if (c.isValid())
        setColor(c);
}

}
