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

#include "breadcrumbwidget.h"

namespace shared {

BreadcrumbWidget::BreadcrumbWidget(QWidget *parent)
    : QLabel(parent)
{
    connect(this, &QLabel::linkActivated, this, &BreadcrumbWidget::levelClicked);
    move(parent->pos() + QPoint(5, 5));
}

void BreadcrumbWidget::setLevels(const QList<Level> &levels)
{
    if (levels.isEmpty()) {
        setText(QString());
        return;
    }

    static const QString entry = u"<a href='%1'>%2</a>"_qs;
    static const QString separator = u" &gt; "_qs;

    // Without some link text it won't become clickable, so '/' it is.
    QString text = entry.arg(QLatin1String("/"), tr("Root"));
    text.append(separator);
    for (auto level = levels.begin(), end = levels.end(); level != end; ++level) {
        const bool last = level + 1 == end;
        // Don't add an href address, so it is not clickable: it's the current level.
        text.append(entry.arg(last ? QString() : level->link, level->name));
        if (!last)
            text.append(separator);
    }
    setText(text);
    resize(sizeHint());
}

} // namespace shared
