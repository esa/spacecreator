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

#pragma once

#include <QBrush>
#include <QColor>
#include <QExplicitlySharedDataPointer>
#include <QFont>
#include <QObject>
#include <QPen>
#include <QSharedData>
#include <QVersionNumber>

class QJsonObject;

namespace shared {

struct ColorHandlerData;

class ColorHandler
{
    Q_GADGET
public:
    enum FillType
    {
        None = -1,
        Color = 0,
        GradientVertical,
        GradientHorizontal,
    };
    Q_ENUM(FillType);

    enum Alignment
    {
        Default = 0,
        Left = Qt::AlignLeft | Qt::AlignVCenter,
        TopLeft = Qt::AlignLeft | Qt::AlignTop,
        Top = Qt::AlignHCenter | Qt::AlignTop,
        TopRight = Qt::AlignRight | Qt::AlignTop,
        Right = Qt::AlignRight | Qt::AlignVCenter,
        BottomRight = Qt::AlignRight | Qt::AlignBottom,
        Bottom = Qt::AlignHCenter | Qt::AlignBottom,
        BottomLeft = Qt::AlignLeft | Qt::AlignBottom,
        Center = Qt::AlignCenter,
    };
    Q_ENUM(Alignment);

    static QVersionNumber currentVersion();

    ColorHandler();
    ColorHandler(const ColorHandler &other);

    QPen pen() const;
    QBrush brush() const;

    FillType fillType() const;
    void setFillType(FillType fillType);

    qreal penWidth() const;
    void setPenWidth(qreal width);

    QColor penColor() const;
    void setPenColor(const QColor &color);

    Qt::PenStyle penStyle() const;
    void setPenStyle(Qt::PenStyle style);

    QColor brushColor0() const;
    void setBrushColor0(const QColor &color);

    QColor brushColor1() const;
    void setBrushColor1(const QColor &color);

    QString group() const;
    void setGroup(const QString &group);

    QFont font() const;
    void setFont(const QFont &font);

    QColor textColor() const;
    void setTextColor(const QColor &color);

    Alignment textAlignment() const;
    void setTextAlignment(Alignment alignment);

    static ColorHandler fromJson(const QVersionNumber &version, const QJsonObject &jObj);
    QJsonObject toJson(const QVersionNumber &version = currentVersion()) const;

    void detach();

private:
    static void loadFromJson(ColorHandler *h, const QJsonObject &jObj);
    static QJsonObject storeToJson(const ColorHandler *h);

private:
    QExplicitlySharedDataPointer<ColorHandlerData> d;
};

struct ColorHandlerData : public QSharedData {
    ColorHandler::FillType fillType { ColorHandler::FillType::Color };
    QPen pen;
    QFont font;
    QColor brushColor0 { Qt::black };
    QColor brushColor1 { Qt::white };
    QColor textColor { Qt::black };
    ColorHandler::Alignment alignment = ColorHandler::Default;
    QString group { "IVE" };
};

} // namespace shared

Q_DECLARE_TYPEINFO(shared::ColorHandler, Q_MOVABLE_TYPE);
