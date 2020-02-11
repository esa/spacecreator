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

#include "colormanager.h"

#include "app/common.h"
#include "baseitems/common/utils.h"
#include "settings/appoptions.h"
#include "settings/settingsmanager.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

namespace taste3 {
namespace aadl {

QPen ColorHandler::pen() const
{
    return { m_penColor, m_penWidth };
}

QBrush ColorHandler::brush() const
{
    switch (m_fillType) {
    case FillType::Gradient: {
        QLinearGradient gradient;
        gradient.setCoordinateMode(QLinearGradient::ObjectBoundingMode);
        gradient.setStart(0., 0.);
        gradient.setFinalStop(0., 1.);
        gradient.setColorAt(0., m_brushColor0);
        gradient.setColorAt(1., m_brushColor1);

        return QBrush(gradient);
    }
    default:
        return QBrush(m_brushColor0);
    }
}

ColorHandler ColorHandler::fromJson(const QJsonObject &jObj)
{
    ColorHandler h;
    h.m_fillType = ColorHandler::FillType(jObj["fill_type"].toInt(ColorHandler::FillType::Color));
    h.m_penWidth = jObj["pen_width"].toDouble(1.);
    h.m_penColor = QColor(jObj["pen_color"].toString("black"));
    h.m_brushColor0 = QColor(jObj["brush_color0"].toString("lightGray"));
    h.m_brushColor1 = QColor(jObj["brush_color1"].toString("white"));
    return h;
}

QJsonObject ColorHandler::toJson() const
{
    return {
        { "fill_type", m_fillType },
        { "pen_width", m_penWidth },
        { "pen_color", m_penColor.name(QColor::HexArgb) },
        { "brush_color0", m_brushColor0.name(QColor::HexArgb) },
        { "brush_color1", m_brushColor1.name(QColor::HexArgb) },
    };
}

ColorManager *ColorManager::m_instance = nullptr;

ColorManager::ColorManager(QObject *parent)
    : QObject(parent)
    , m_colors({
              { HandledColors::FunctionRegular, ColorHandler() },
              { HandledColors::FunctionRoot, ColorHandler() },
              { HandledColors::FunctionPartial, ColorHandler() },
              { HandledColors::FunctionType, ColorHandler() },
              { HandledColors::Iface, ColorHandler() },
              { HandledColors::Connection, ColorHandler() },
              { HandledColors::Comment, ColorHandler() },
      })
{
    const QString defaultSourcePath = prepareDefaultSource();
    QString sourcePath = AppOptions::Aadl.ColorSchemeFile.read().toString();
    if (!QFile::exists(sourcePath))
        sourcePath = defaultSourcePath;

    setSourceFile(sourcePath);
}

ColorManager *ColorManager::instance()
{
    if (!m_instance)
        m_instance = new ColorManager;

    return m_instance;
}

ColorHandler ColorManager::colorsForItem(HandledColors t)
{
    if (instance()->m_colors.contains(t))
        return instance()->m_colors.value(t);
    return ColorHandler();
}

QString ColorManager::handledColorTypeName(HandledColors t)
{
    switch (t) {
    case FunctionRegular:
        return tr("Regular Function");
    case FunctionRoot:
        return tr("Root Function");
    case FunctionPartial:
        return tr("Partial Function");
    case FunctionType:
        return tr("Function Type");
    case Iface:
        return tr("Interface");
    case Connection:
        return tr("Connection");
    case Comment:
        return tr("Comment");
    default:
        return tr("Unknown");
    }
}

bool ColorManager::setSourceFile(const QString &from)
{
    auto loadColorHandler = [this](const QJsonObject &jsonObject) {
        const ColorHandler &ch = ColorHandler::fromJson(jsonObject);
        const HandledColors colorType = HandledColors(jsonObject["color_type"].toInt(HandledColors::Unhandled));
        switch (colorType) {
        case HandledColors::Unhandled:
            return false;
        default: {
            m_colors[colorType] = ch;
            return true;
        }
        }
    };
    bool loaded(false);

    QFile jsonFile(from);
    if (jsonFile.open(QIODevice::ReadOnly)) {
        const QByteArray &jsonData = jsonFile.readAll();
        QJsonParseError jsonErrHandler;
        const QJsonDocument &jsonDoc = QJsonDocument::fromJson(jsonData, &jsonErrHandler);
        if (jsonErrHandler.error == QJsonParseError::NoError) {
            const QJsonArray &jArr = jsonDoc.array();
            for (const QJsonValue &jsonVal : jArr) {
                const QJsonObject &jsonObj = jsonVal.toObject();
                loaded = loadColorHandler(jsonObj);
                if (!loaded)
                    break;
            }
        } else {
            qWarning() << "JSON parsing failed:" << jsonErrHandler.errorString();
        }
    } else {
        qWarning() << "File open failed:" << from << jsonFile.errorString();
    }

    if (loaded) {
        m_filePath = from;
        AppOptions::Aadl.ColorSchemeFile.write(m_filePath);
        emit colorsUpdated();
    }

    return loaded;
}

QString ColorManager::sourceFile() const
{
    return m_filePath;
}

QString ColorManager::prepareDefaultSource() const
{
    static const QString jsonFileName("default_colors.json");
    const QString targetDir =
            QString("%1/colors").arg(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));

    common::ensureDirExists(targetDir);

    QString jsonFilePath = QString("%1/%2").arg(targetDir, jsonFileName);

    if (QFile::exists(jsonFilePath))
        return jsonFilePath;

    const QString &rscFilePath = QString(":/colors/%1").arg(jsonFileName);
    if (common::copyResourceFile(rscFilePath, jsonFilePath))
        return jsonFilePath;

    qWarning() << "Can't create default colors file" << jsonFilePath;
    return QString();
}

} // ns aadl
} // ns taste3
