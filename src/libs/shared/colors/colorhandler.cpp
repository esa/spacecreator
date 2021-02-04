/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "colorhandler.h"

namespace shared {

ColorHandler::ColorHandler()
    : d(new ColorHandlerData())
{
}

ColorHandler::ColorHandler(const ColorHandler &other)
    : d(other.d)
{
}

QPen ColorHandler::pen() const
{
    return { penColor(), penWidth() };
}

QBrush ColorHandler::brush() const
{
    switch (fillType()) {
    case FillType::Gradient: {
        QLinearGradient gradient;
        gradient.setCoordinateMode(QLinearGradient::ObjectBoundingMode);
        gradient.setStart(0., 0.);
        gradient.setFinalStop(0., 1.);
        gradient.setColorAt(0., brushColor0());
        gradient.setColorAt(1., brushColor1());

        return QBrush(gradient);
    }
    default:
        return QBrush(brushColor0());
    }
}

ColorHandler::FillType ColorHandler::fillType() const
{
    return d->fillType;
}

void ColorHandler::setFillType(ColorHandler::FillType fillType)
{
    d->fillType = fillType;
}

qreal ColorHandler::penWidth() const
{
    return d->penWidth;
}

void ColorHandler::setPenWidth(qreal width)
{
    d->penWidth = width;
}

QColor ColorHandler::penColor() const
{
    return d->penColor;
}

void ColorHandler::setPenColor(const QColor &color)
{
    d->penColor = color;
}

QColor ColorHandler::brushColor0() const
{
    return d->brushColor0;
}

void ColorHandler::setBrushColor0(const QColor &color)
{
    d->brushColor0 = color;
}

QColor ColorHandler::brushColor1() const
{
    return d->brushColor1;
}

void ColorHandler::setBrushColor1(const QColor &color)
{
    d->brushColor1 = color;
}

ColorHandler ColorHandler::fromJson(const QJsonObject &jObj)
{
    ColorHandler h;
    h.setFillType(ColorHandler::FillType(jObj["fill_type"].toInt(ColorHandler::FillType::Color)));
    h.setPenWidth(jObj["pen_width"].toDouble(1.));
    h.setPenColor(QColor(jObj["pen_color"].toString("black")));
    h.setBrushColor0(QColor(jObj["brush_color0"].toString("lightGray")));
    h.setBrushColor1(QColor(jObj["brush_color1"].toString("white")));
    return h;
}

QJsonObject ColorHandler::toJson() const
{
    return {
        { "fill_type", fillType() },
        { "pen_width", penWidth() },
        { "pen_color", penColor().name(QColor::HexArgb) },
        { "brush_color0", brushColor0().name(QColor::HexArgb) },
        { "brush_color1", brushColor1().name(QColor::HexArgb) },
    };
}

} // namespace ive
