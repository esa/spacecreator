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

#include "common.h"

#include <QApplication>
#include <QJsonObject>

namespace shared {

QVersionNumber ColorHandler::currentVersion()
{
    return QVersionNumber(2, 0);
}

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
    return d->pen;
}

QBrush ColorHandler::brush() const
{
    if (d->fillType == FillType::None) {
        return QBrush(Qt::NoBrush);
    }

    if (d->fillType == FillType::Color) {
        return QBrush(brushColor0());
    }

    QLinearGradient gradient;
    gradient.setCoordinateMode(QLinearGradient::ObjectBoundingMode);
    gradient.setColorAt(0., brushColor0());
    gradient.setColorAt(1., brushColor1());

    switch (d->fillType) {
    case FillType::GradientVertical: {
        gradient.setStart(0., 0.);
        gradient.setFinalStop(0., 1.);
        break;
    }
    case FillType::GradientHorizontal: {
        gradient.setStart(0., 0.);
        gradient.setFinalStop(1., 0.);
        break;
    }
    default:
        return QBrush(brushColor0());
    }

    return QBrush(gradient);
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
    return d->pen.widthF();
}

void ColorHandler::setPenWidth(qreal width)
{
    d->pen.setWidthF(width);
}

QColor ColorHandler::penColor() const
{
    return d->pen.color();
}

void ColorHandler::setPenColor(const QColor &color)
{
    d->pen.setColor(color);
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

QString ColorHandler::group() const
{
    return d->group;
}

void ColorHandler::setGroup(const QString &group)
{
    d->group = group;
}

Qt::PenStyle ColorHandler::penStyle() const
{
    return d->pen.style();
}

void ColorHandler::setPenStyle(Qt::PenStyle style)
{
    d->pen.setStyle(style);
}

QFont ColorHandler::font() const
{
    return d->font;
}

void ColorHandler::setFont(const QFont &font)
{
    d->font = font;
}

QColor ColorHandler::textColor() const
{
    return d->textColor;
}

void ColorHandler::setTextColor(const QColor &color)
{
    d->textColor = color;
}

ColorHandler::Alignment ColorHandler::textAlignment() const
{
    return d->alignment;
}

void ColorHandler::setTextAlignment(ColorHandler::Alignment alignment)
{
    d->alignment = alignment;
}

ColorHandler ColorHandler::fromJson(const QVersionNumber &version, const QJsonObject &jObj)
{
    ColorHandler h;
    switch (version.majorVersion()) {
    case 1:
        h.setFillType(ColorHandler::FillType(jObj["fill_type"].toInt(ColorHandler::FillType::Color)));
        if (jObj.contains("group")) {
            h.setGroup(jObj["group"].toString());
        }
        h.setPenWidth(jObj["pen_width"].toDouble(1.));
        h.setPenColor(QColor(jObj["pen_color"].toString("black")));
        h.setBrushColor0(QColor(jObj["brush_color0"].toString("lightGray")));
        h.setBrushColor1(QColor(jObj["brush_color1"].toString("white")));
        break;
    case 2:
        loadFromJson(&h, jObj);
        break;
    default:
        break;
    }

    return h;
}

QJsonObject ColorHandler::toJson(const QVersionNumber &version) const
{
    switch (version.majorVersion()) {
    case 1:
        return {
            { "fill_type", fillType() },
            { "group", group() },
            { "pen_width", penWidth() },
            { "pen_color", penColor().name(QColor::HexArgb) },
            { "brush_color0", brushColor0().name(QColor::HexArgb) },
            { "brush_color1", brushColor1().name(QColor::HexArgb) },
        };
    case 2:
        return storeToJson(this);
    default:
        break;
    }
    return {};
}

void ColorHandler::detach()
{
    d.detach();
}

void ColorHandler::loadFromJson(ColorHandler *h, const QJsonObject &jObj)
{
    using namespace StyleAttribute;

    if (jObj.contains("group")) {
        h->setGroup(jObj.value("group").toString());
    }

    const QJsonObject penObj = jObj.value(kPenColor.section(kStringDelemiter, -2, -2)).toObject();
    if (!penObj.isEmpty()) {
        const QString colorName = penObj.value(kPenColor.section(kStringDelemiter, -1)).toString("black");
        h->setPenColor(QColor::isValidColorName(colorName) ? QColor::fromString(colorName) : QColor(Qt::black));

        h->setPenWidth(penObj.value(kPenWidth.section(kStringDelemiter, -1)).toDouble(1.0));

        const QString style =
                penObj.value(kPenStyle.section(kStringDelemiter, -1)).toString(shared::typeName(Qt::SolidLine));
        h->setPenStyle(shared::typeFromName<Qt::PenStyle>(style));
    }

    const QJsonObject fontObj = jObj.value(kFontFamily.section(kStringDelemiter, -2, -2)).toObject();
    if (!fontObj.isEmpty()) {
        QFont font = qApp->font();
        font.setFamily(fontObj.value(kFontFamily.section(kStringDelemiter, -1)).toString(font.defaultFamily()));
        font.setPointSizeF(fontObj.value(kFontPointSize.section(kStringDelemiter, -1)).toDouble(font.pointSizeF()));
        auto weight =
                fontObj.value(kFontWeight.section(kStringDelemiter, -1)).toString(shared::typeName(font.weight()));
        font.setWeight(shared::typeFromName<QFont::Weight>(weight));
        h->setFont(font);
    }
    const QJsonObject brushObj = jObj.value(kBrushFillType.section(kStringDelemiter, -2, -2)).toObject();
    if (!brushObj.isEmpty()) {
        const auto defaultFillType = shared::typeName<ColorHandler::FillType>(ColorHandler::FillType::Color);
        const QString brushFillType =
                brushObj.value(kBrushFillType.section(kStringDelemiter, -1)).toString(defaultFillType);
        h->setFillType(shared::typeFromName<ColorHandler::FillType>(brushFillType));

        const QString brushColor0Name =
                brushObj.value(kBrushColor0.section(kStringDelemiter, -1)).toString("lightGray");
        h->setBrushColor0(QColor::isValidColorName(brushColor0Name) ? QColor::fromString(brushColor0Name)
                                                                    : QColor(Qt::lightGray));

        const QString brushColor1Name = brushObj.value(kBrushColor1.section(kStringDelemiter, -1)).toString("white");
        h->setBrushColor1(
                QColor::isValidColorName(brushColor1Name) ? QColor::fromString(brushColor1Name) : QColor(Qt::white));
    }

    const QJsonObject textObj = jObj.value(kTextColor.section(kStringDelemiter, -2, -2)).toObject();
    if (!textObj.isEmpty()) {
        const auto defaultAlignment = shared::typeName<ColorHandler::Alignment>(ColorHandler::Center);
        const QString alignment =
                textObj.value(kTextAlignment.section(kStringDelemiter, -1)).toString(defaultAlignment);
        h->setTextAlignment(shared::typeFromName<ColorHandler::Alignment>(alignment));

        const QString colorName = textObj.value(kTextColor.section(kStringDelemiter, -1)).toString("black");
        h->setTextColor(QColor::isValidColorName(colorName) ? QColor::fromString(colorName) : QColor(Qt::black));
    }
}

QJsonObject ColorHandler::storeToJson(const ColorHandler *h)
{
    using namespace StyleAttribute;

    QJsonObject pen;
    pen.insert(kPenColor.section(kStringDelemiter, -1), h->penColor().name(QColor::HexArgb));
    pen.insert(kPenWidth.section(kStringDelemiter, -1), h->penWidth());
    pen.insert(kPenStyle.section(kStringDelemiter, -1), shared::typeName(h->penStyle()));

    QJsonObject font;
    font.insert(kFontFamily.section(kStringDelemiter, -1), h->font().family());
    font.insert(kFontPointSize.section(kStringDelemiter, -1), h->font().pointSizeF());
    font.insert(kFontWeight.section(kStringDelemiter, -1), shared::typeName(h->font().weight()));

    QJsonObject brush;
    brush.insert(kBrushFillType.section(kStringDelemiter, -1), typeName(h->fillType()));
    brush.insert(kBrushColor0.section(kStringDelemiter, -1), h->brushColor0().name(QColor::HexArgb));
    brush.insert(kBrushColor1.section(kStringDelemiter, -1), h->brushColor1().name(QColor::HexArgb));

    QJsonObject text;
    text.insert(kTextColor.section(kStringDelemiter, -1), h->textColor().name(QColor::HexArgb));
    text.insert(kTextAlignment.section(kStringDelemiter, -1), typeName(h->textAlignment()));

    QJsonObject json;
    json.insert(QLatin1String("group"), h->group());
    json.insert(kPenColor.section(kStringDelemiter, -2, -2), pen);
    json.insert(kFontFamily.section(kStringDelemiter, -2, -2), font);
    json.insert(kBrushFillType.section(kStringDelemiter, -2, -2), brush);
    json.insert(kTextColor.section(kStringDelemiter, -2, -2), text);

    return json;
}

} // namespace ive
