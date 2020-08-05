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

#include "asn1/definitions.h"
#include "asn1/file.h"
#include "asn1/types/builtintypes.h"
#include "asn1xmlparser.h"
#include "common.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextStream>

namespace aadl {

DataTypesStorage *DataTypesStorage::m_instance = nullptr;

static QString ensureAsnFileExists()
{
    const QString asnFileName("taste-types.asn");
    const QString targetDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    const bool ok = shared::ensureDirExists(targetDir);
    if (!ok) {
        qWarning() << "Unable to create directory" << targetDir;
        return {};
    }

    QString asnFilePath = QString("%1/%2").arg(targetDir, asnFileName);

    const QString &rscFilePath = QString(":/defaults/resources/%1").arg(asnFileName);
    if (shared::copyResourceFile(rscFilePath, asnFilePath, shared::FileCopyingMode::Overwrite)) {
        return asnFilePath;
    }

    qWarning() << "Can't create default ASN datatypes file" << asnFilePath;
    return QString();
}

DataTypesStorage::DataTypesStorage(std::unique_ptr<Asn1Acn::File> &dataTypes)
    : m_asn1DataTypes(std::move(dataTypes))
{
}

DataTypesStorage::~DataTypesStorage() { }

void DataTypesStorage::init()
{
    if (m_instance) {
        return;
    }

    const QString &asnFilePath = ensureAsnFileExists();
    QFileInfo fileInfo(asnFilePath);
    QStringList errorMessages;
    asn1::Asn1XMLParser parser;
    std::unique_ptr<Asn1Acn::File> asn1Data = parser.parseAsn1File(fileInfo, &errorMessages);
    if (!errorMessages.isEmpty()) {
        qWarning() << "Can't read file:" << asnFilePath << errorMessages.join(", ");
        return;
    }

    m_instance = new DataTypesStorage(asn1Data);
}

DataTypesStorage *DataTypesStorage::instance()
{
    init();
    return m_instance;
}

const std::unique_ptr<Asn1Acn::File> &DataTypesStorage::asn1DataTypes() const
{
    return m_asn1DataTypes;
}

}
