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

#pragma once

#include "baseitems/graphicsview.h"

#include <QComboBox>
#include <QIntValidator>
#include <QPointer>

namespace taste3 {

class ZoomValidator : public QValidator
{
    Q_OBJECT
public:
    explicit ZoomValidator(QObject *parent = nullptr);

    virtual State validate(QString &, int &) const override;
    virtual void fixup(QString &) const override;

    void setBottom(int);
    void setRange(int bottom, int top);
    void setTop(int);

    void addSuffix(QString &text) const;
    QString clearSuffix(QString &value) const;

private:
    QIntValidator *m_validator { nullptr };
};

class ZoomController : public QWidget
{
    Q_OBJECT
public:
    explicit ZoomController(QWidget *parent = nullptr);

    void setView(GraphicsView *view);

protected Q_SLOTS:
    void onCurrentIndexChanged(const QString &text);
    void setZoomLevel(qreal percent);
    void displayZoomLevel(qreal percent);

Q_SIGNALS:

private:
    QComboBox *m_combo { nullptr };
    ZoomValidator *m_validator { nullptr };
    QPointer<GraphicsView> m_view;

    void refill();
};

} // ns taste3
