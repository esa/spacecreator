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

#include "asn1modelstorage.h"

#include "asn1/definitions.h"
#include "asn1/file.h"
#include "asn1/types/builtintypes.h"
#include "asn1xmlparser.h"
#include "common.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QStandardPaths>
#include <QTextStream>

namespace Asn1Acn {

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

Asn1ModelStorage::Asn1ModelStorage()
{
    loadDefault();
    m_reloadTimer.setSingleShot(true);
    connect(&m_reloadTimer, &QTimer::timeout, this, &Asn1Acn::Asn1ModelStorage::loadFile);
}

Asn1ModelStorage::Asn1ModelStorage(QSharedPointer<File> &dataTypes)
    : m_asn1DataTypes(dataTypes)
{
    m_reloadTimer.setSingleShot(true);
    connect(&m_reloadTimer, &QTimer::timeout, this, &Asn1Acn::Asn1ModelStorage::loadFile);
}

Asn1ModelStorage::~Asn1ModelStorage() { }

const QSharedPointer<Asn1Acn::File> &Asn1ModelStorage::asn1DataTypes() const
{
    return m_asn1DataTypes;
}

/*!
   Set the asn1 file for this types storage. \note The data is reloaded by this function
 */
void Asn1ModelStorage::setFileName(const QFileInfo &fileName)
{
    if (m_asn1Watcher != nullptr) {
        // Check if the directory is watched already
        if (!m_asn1Watcher->files().contains(fileName.absoluteFilePath())) {
            disconnect(m_asn1Watcher, nullptr, this, nullptr);
            m_asn1Watcher->deleteLater();
            m_asn1Watcher = nullptr;
        }
    }

    m_fileName = fileName;
    loadFile();

    if (m_asn1Watcher == nullptr && m_fileName.exists()) {
        m_asn1Watcher = new QFileSystemWatcher(this);
        m_asn1Watcher->addPath(m_fileName.absoluteFilePath());
        connect(m_asn1Watcher, &QFileSystemWatcher::fileChanged, this, [this]() {
            m_reloadTimer.stop();
            m_reloadTimer.start(20);
        });
    }
}

/*!
   Returns the full file information of the asn1 file used for this types storage
 */
const QFileInfo &Asn1ModelStorage::fileName() const
{
    return m_fileName;
}

/*!
   Loads the default ASN1 data
 */
void Asn1ModelStorage::loadDefault()
{
    const QString &asnFilePath = ensureAsnFileExists();
    setFileName(asnFilePath);
}

/*!
   Load the data types stored the file \sa fileName
 */
bool Asn1ModelStorage::loadFile()
{
    QStringList errorMessages;
    Asn1Acn::Asn1XMLParser parser;
    std::unique_ptr<Asn1Acn::File> asn1Data = parser.parseAsn1File(m_fileName, &errorMessages);
    if (!errorMessages.isEmpty()) {
        qWarning() << "Can't read file:" << m_fileName << errorMessages.join(", ");
        return false;
    }

    m_asn1DataTypes.reset(asn1Data.release());
    Q_EMIT dataTypesChanged();

    return true;
}

/*!
   Clear the types data and set the filename to empty.
 */
void Asn1ModelStorage::clear()
{
    m_fileName.setFile("");
    m_asn1DataTypes.reset();
    Q_EMIT dataTypesChanged();
}

/*!
   Retruns the first type having the given \p name
 */
const Asn1Acn::Types::Type *Asn1ModelStorage::typeFromName(const QString &name) const
{
    for (const std::unique_ptr<Asn1Acn::Definitions> &definitions : m_asn1DataTypes->definitionsList()) {
        for (const std::unique_ptr<Asn1Acn::TypeAssignment> &assignment : definitions->types()) {
            if (assignment->name() == name) {
                return assignment->type();
            }
        }
    }
    return nullptr;
}

}
