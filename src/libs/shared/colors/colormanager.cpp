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
#include <QUrl>
#include <QVersionNumber>

namespace shared {

ColorManager *ColorManager::m_instance = nullptr;
const QString ColorManager::defaultColorschemeFileName = QStringLiteral("default_colors.json");

ColorManager::ColorManager(QObject *parent)
    : QObject(parent)
{
    setSourceFile(defaultColorsResourceFile());
    Q_ASSERT(m_colors.size() == QMetaEnum::fromType<shared::ColorManager::HandledColors>().keyCount() - 1);

    const QString sourcePath = shared::SettingsManager::load<QString>(
            shared::SettingsManager::Common::ColorSchemeFile, prepareDefaultSource());

    if (!sourcePath.isEmpty() && !setSourceFile(sourcePath)) { // source file can be corrupted
        shared::copyFile(defaultColorsResourceFile(), sourcePath, shared::FileCopyingMode::Overwrite);
        setSourceFile(sourcePath);
    }
}

ColorManager *ColorManager::instance()
{
    if (!m_instance) {
        m_instance = new ColorManager;
    }

    return m_instance;
}

ColorHandler ColorManager::colorsForItem(HandledColors t) const
{
    return m_colors.value(t);
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

    case InstanceLine:
        return tr("Instance line");
    case InstanceErrorLine:
        return tr("Instance with error");
    case InstanceEnd:
        return tr("Instance end symbol");
    case InstanceHead:
        return tr("Instance head");
    case MessageRegular:
        return tr("Regular Message");
    case MessageError:
        return tr("Message with error");
    case Action:
        return tr("Action");
    case CommentMsc:
        return tr("MSC comment");
    case Condition:
        return tr("Condition");
    case CoRegion:
        return tr("Co-Region");
    case Timer:
        return tr("Timer");
    case Chart:
        return tr("Chart");

    case Node:
        return tr("Node");
    case Processor:
        return tr("Processor");
    case Partition:
        return tr("Partition");
    case Bus:
        return tr("Bus");
    case BusConnection:
        return tr("Connection");
    case Device:
        return tr("Device");
    case Binding:
        return tr("Binding");

    default:
        return tr("Unknown");
    }
}

/*!
   Does load the color scheme from \p from. And if succesful does set the file source.
 */
bool ColorManager::setSourceFile(const QString &from)
{
    QVersionNumber version(0, 0);

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
            QJsonArray jArr;
            if (jsonDoc.isArray()) {
                jArr = jsonDoc.array();
            } else {
                if (jsonDoc.isObject()) {
                    QJsonObject jobj = jsonDoc.object();
                    if (!jobj.contains("version")) {
                        qWarning() << "JSON content does not have a version";
                        return false;
                    }
                    if (!jobj.contains("colors")) {
                        qWarning() << "JSON content does not have a colors array";
                        return false;
                    }
                    version = QVersionNumber::fromString(jobj["version"].toString());
                    jArr = jobj["colors"].toArray();
                } else {
                    qWarning() << "JSON content does not match";
                    return false;
                }
            }

            for (const QJsonValue &jsonVal : qAsConst(jArr)) {
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
        if (!from.startsWith(":")) {
            m_filePath = from;
            shared::SettingsManager::store<QString>(shared::SettingsManager::Common::ColorSchemeFile, m_filePath);
        }
        Q_EMIT colorsUpdated();
    }

    return loaded;
}

QString ColorManager::sourceFile() const
{
    return m_filePath;
}

bool ColorManager::save(const QString &fileName) const
{
    QJsonObject jobj;
    jobj.insert("version", "1.0");

    QJsonArray ja;
    for (const ColorManager::HandledColors &ct : handledColors()) {
        ColorHandler ch = colorsForItem(ct);
        QJsonObject jObj = ch.toJson();
        jObj["color_type"] = ct;
        ja.append(jObj);
    }
    jobj.insert("colors", ja);

    QFile out(fileName);
    if (out.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        out.write(QJsonDocument(jobj).toJson());
        out.close();
        return true;
    } else {
        return false;
    }
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

    if (shared::copyFile(defaultColorsResourceFile(), jsonFilePath))
        return jsonFilePath;

    qWarning() << "Can't create default colors file" << jsonFilePath;
    return QString();
}

}
