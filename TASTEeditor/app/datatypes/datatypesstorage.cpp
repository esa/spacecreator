/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "datatypesstorage.h"

#include "app/common.h"
#include "baseitems/common/utils.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTextStream>

namespace taste3 {
namespace datatypes {

DataTypesStorage *DataTypesStorage::m_instance = nullptr;

DataTypesStorage::DataTypesStorage(const QMap<QString, BasicDataType *> &dataTypes)
    : m_dataTypes(dataTypes)
{
}

static QString ensureAsnFileExists()
{
    const QString asnFileName("taste-types.asn");
    const QString targetDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    common::ensureDirExists(targetDir);

    QString asnFilePath = QString("%1/%2").arg(targetDir, asnFileName);

    if (QFileInfo::exists(asnFilePath))
        return asnFilePath;

    const QString &rscFilePath = QString(":/defaults/app/resources/%1").arg(asnFileName);
    if (common::copyResourceFile(rscFilePath, asnFilePath))
        return asnFilePath;

    qWarning() << "Can't create default ASN datatypes file" << asnFilePath;
    return QString();
}

static QStringList dataRangeFromString(const QString &data)
{
    if (data.isEmpty())
        return {};

    QString line = data.mid(1, data.length() - 2);
    if (line.contains(".."))
        return line.split("..", QString::SkipEmptyParts);

    return line.split(",", QString::SkipEmptyParts);
}

static BasicDataType *datatypeFromString(const QString &line)
{
    if (line.isEmpty())
        return nullptr;

    const QRegularExpression rx("(.*\\w+)\\s*::=\\s*(\\w+)\\s*(\\(.*\\))?");
    const QStringList &matched = rx.match(line).capturedTexts();
    if (matched.size() >= 3) {
        const QString &dataName = matched[1];
        const QString &dataType = matched[2];
        const QString &dataRangeStr = matched.size() == 4 ? matched[3] : QString();
        if (!dataRangeStr.isEmpty()) {
            const QStringList &dataRange = dataRangeFromString(dataRangeStr);
            if (dataType == "INTEGER") {
                bool containsMinus(false);
                for (const QString &str : dataRange)
                    if (str.contains("-")) {
                        containsMinus = true;
                        break;
                    }
                if (containsMinus) {
                    SignedIntegerDataType *data = new SignedIntegerDataType(dataName);
                    data->setRange({ dataRange.first().toLongLong(), dataRange.last().toLongLong() });
                    return data;
                } else {
                    UnsignedIntegerDataType *data = new UnsignedIntegerDataType(dataName);
                    data->setRange({ dataRange.first().toULongLong(), dataRange.last().toULongLong() });
                    return data;
                }
            }

            if (dataType == "REAL") {
                const QStringList &dataRangeReal = dataRangeFromString(dataRangeStr);
                RealDataType *data = new RealDataType(dataName);
                data->setRange({ dataRangeReal.first().toDouble(), dataRangeReal.last().toDouble() });
                return data;
            }

            if (dataType == "ENUMERATION") {
                const QStringList &dataRangeEnumeration = dataRangeFromString(dataRangeStr);
                return new EnumDataType(dataName, dataRangeEnumeration.toVector());
            }
        }

        if (dataType == "BOOLEAN")
            return new BoolDataType(dataName);

        if (dataType == "STRING")
            return new StringDataType(dataName);
    }
    return nullptr;
}

DataTypesStorage *DataTypesStorage::init()
{
    const QString &asnFilePath = ensureAsnFileExists();
    QFile asnFile(asnFilePath);
    if (asnFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return init(asnFile.readAll());
    else
        qWarning() << "Can't open file:" << asnFilePath << asnFile.errorString();

    return nullptr;
}

DataTypesStorage *DataTypesStorage::init(const QString &from)
{
    QString in(from);
    QTextStream ts(&in);
    QMap<QString, BasicDataType *> datatypes;
    while (!ts.atEnd())
        if (BasicDataType *wrapper = datatypeFromString(ts.readLine().trimmed()))
            datatypes[wrapper->name()] = wrapper;

    return new DataTypesStorage(datatypes);
}

DataTypesStorage *DataTypesStorage::instance()
{
    if (!m_instance)
        m_instance = init();

    return m_instance;
}

QMap<QString, BasicDataType *> DataTypesStorage::dataTypes()
{
    return instance()->m_dataTypes;
}

} // ns datatypes
} // ns taste3
