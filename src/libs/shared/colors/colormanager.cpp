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

#include "common.h"
#include "settingsmanager.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>
#include <QStandardPaths>

namespace shared {

ColorManager *ColorManager::m_instance = nullptr;
const QString ColorManager::defaultColorschemeFileName = QStringLiteral("default_colors.json");

ColorManager::ColorManager(QObject *parent)
    : QObject(parent)
    , m_colors({
              { HandledColors::FunctionRegular, ColorHandler() },
              { HandledColors::FunctionRoot, ColorHandler() },
              { HandledColors::FunctionPartial, ColorHandler() },
              { HandledColors::FunctionType, ColorHandler() },
              { HandledColors::Iface, ColorHandler() },
              { HandledColors::Connection, ColorHandler() },
              { HandledColors::ConnectionFlow, ColorHandler() },
              { HandledColors::Comment, ColorHandler() },
              { HandledColors::ConnectionGroup, ColorHandler() },
              { HandledColors::IfaceGroup, ColorHandler() },
              { HandledColors::FunctionScale, ColorHandler() },
      })
{
    const QString sourcePath = shared::SettingsManager::load<QString>(
            shared::SettingsManager::Common::ColorSchemeFile, prepareDefaultSource());

    if (!sourcePath.isEmpty() && !setSourceFile(sourcePath)) { // source file can be corrupted
        shared::copyResourceFile(defaultColorsResourceFile(), sourcePath, shared::FileCopyingMode::Overwrite);
        setSourceFile(sourcePath);
    }
}

ColorManager *ColorManager::instance()
{
    if (!m_instance)
        m_instance = new ColorManager;

    return m_instance;
}

ColorHandler ColorManager::colorsForItem(HandledColors t)
{
    return instance()->m_colors.value(t);
}

QString ColorManager::defaultColorsResourceFile()
{
    return QStringLiteral(":/colors/%1").arg(defaultColorschemeFileName);
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
    case ConnectionFlow:
        return tr("Connection Flow");
    case ConnectionGroup:
        return tr("Connection Group");
    case IfaceGroup:
        return tr("Interface Group");
    case Comment:
        return tr("Comment");
    case FunctionScale:
        return tr("Scaled nested content");
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

    if (m_colors.size() != QMetaEnum::fromType<shared::ColorManager::HandledColors>().keyCount() - 1) {
        return false;
    }

    if (loaded) {
        m_filePath = from;
        shared::SettingsManager::store<QString>(shared::SettingsManager::Common::ColorSchemeFile, m_filePath);
        Q_EMIT colorsUpdated();
    }

    return loaded;
}

QString ColorManager::sourceFile() const
{
    return m_filePath;
}

QList<ColorManager::HandledColors> ColorManager::handledColors() const
{
    return m_colors.keys();
}

QString ColorManager::prepareDefaultSource() const
{
    const QString targetDir =
            QString("%1/colors").arg(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));

    shared::ensureDirExists(targetDir);

    QString jsonFilePath = QString("%1/%2").arg(targetDir, defaultColorschemeFileName);

    if (QFile::exists(jsonFilePath))
        return jsonFilePath;

    if (shared::copyResourceFile(defaultColorsResourceFile(), jsonFilePath))
        return jsonFilePath;

    qWarning() << "Can't create default colors file" << jsonFilePath;
    return QString();
}

}
