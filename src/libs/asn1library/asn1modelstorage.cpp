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

QString ensureAsnFileExists()
{
    const QString asnFileName("taste-types.asn");
    const QString targetDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    const bool ok = shared::ensureDirExists(targetDir);
    if (!ok) {
        qWarning() << "Unable to create directory" << targetDir;
        return {};
    }

    QString asnFilePath = QString("%1/%2").arg(targetDir, asnFileName);

    const QString &rscFilePath = QString(":/asn1resources/%1").arg(asnFileName);
    if (shared::copyResourceFile(rscFilePath, asnFilePath, shared::FileCopyingMode::Overwrite)) {
        return asnFilePath;
    }

    qWarning() << "Can't create default ASN datatypes file" << asnFilePath;
    return QString();
}

/*!
   \brief Asn1ModelStorage::Asn1ModelStorage
 */
Asn1ModelStorage::Asn1ModelStorage()
    : m_asn1Watcher(new QFileSystemWatcher(this))
{
    m_reloadTimer.setSingleShot(true);
    connect(&m_reloadTimer, &QTimer::timeout, this, &Asn1Acn::Asn1ModelStorage::loadChangedFiles);
}

/*!
   \brief Asn1ModelStorage::~Asn1ModelStorage
 */
Asn1ModelStorage::~Asn1ModelStorage() { }

/*!
   Returns the asn types for the given file (full path).
   If the file can't be loaded a default set of types is returned.
 */
QSharedPointer<Asn1Acn::File> Asn1ModelStorage::asn1DataTypes(const QString &fileName) const
{
    if (fileName.isEmpty()) {
        return defaultTypes();
    }

    if (!m_store.contains(fileName)) {
        auto nonConstThis = const_cast<Asn1ModelStorage *>(this);
        nonConstThis->loadFile(fileName);
    }

    if (m_store.contains(fileName)) {
        return m_store[fileName];
    } else {
        return defaultTypes();
    }
}

/*!
   Returns if the file is already loaded in this store
 */
bool Asn1ModelStorage::contains(const QString &fileName) const
{
    return m_store.contains(fileName);
}

/*!
   Clears the whole store
 */
void Asn1ModelStorage::clear()
{
    m_reloadTimer.stop();
    m_filesToReload.clear();
    const QStringList files = m_store.keys();
    if (!files.isEmpty()) {
        m_asn1Watcher->removePaths(m_store.keys());
    }
    m_store.clear();
}

/*!
   Load the data types stored the file \sa fileName
 */
bool Asn1ModelStorage::loadFile(const QString &fileName)
{
    QSharedPointer<Asn1Acn::File> asn1Data = loadData(fileName);
    if (asn1Data.isNull()) {
        return false;
    }

    m_store[fileName] = asn1Data;
    Q_EMIT dataTypesChanged(fileName);

    if (QFileInfo::exists(fileName) && !m_asn1Watcher->files().contains(fileName)) {
        m_asn1Watcher->addPath(fileName);
        connect(m_asn1Watcher, &QFileSystemWatcher::fileChanged, this, [this](const QString &path) {
            m_reloadTimer.stop();
            m_reloadTimer.start(20);
            m_filesToReload.insert(path);
        });
    }
    return true;
}

QSharedPointer<File> Asn1ModelStorage::loadData(const QString &fileName) const
{
    QStringList errorMessages;
    Asn1Acn::Asn1XMLParser parser;
    std::unique_ptr<Asn1Acn::File> asn1Data = parser.parseAsn1File(QFileInfo(fileName), &errorMessages);
    if (!errorMessages.isEmpty()) {
        qWarning() << "Can't read file:" << fileName << errorMessages.join(", ");
        return {};
    }
    return QSharedPointer<Asn1Acn::File>(asn1Data.release());
}

void Asn1ModelStorage::loadChangedFiles()
{
    for (QString fileName : m_filesToReload) {
        loadFile(fileName);
    }
    m_filesToReload.clear();
}

QSharedPointer<File> Asn1ModelStorage::defaultTypes() const
{
    if (m_defaultTypes.isNull()) {
        const QString defaultFile = ensureAsnFileExists();
        m_defaultTypes = loadData(defaultFile);
    }
    return m_defaultTypes;
}

}
