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

#include "asn1/file.h"
#include "asn1reader.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QTextStream>

namespace Asn1Acn {

/*!
   \brief Asn1ModelStorage::Asn1ModelStorage
 */
Asn1ModelStorage::Asn1ModelStorage(QObject *parent)
    : QObject(parent)
    , m_asn1Watcher(new QFileSystemWatcher(this))
{
    // Call invalidateStorage in 100 ms
    m_reloadTimer.setSingleShot(true);
    connect(&m_reloadTimer, &QTimer::timeout, this, &Asn1Acn::Asn1ModelStorage::invalidateStorage);
    connect(m_asn1Watcher, &QFileSystemWatcher::fileChanged, this, [this](const QString &path) {
        m_reloadTimer.stop();
        m_reloadTimer.start(100);
    });
}

/*!
   \brief Asn1ModelStorage::~Asn1ModelStorage
 */
Asn1ModelStorage::~Asn1ModelStorage() {}

/*!
   Returns the asn types for the given file (full path).
   If the file can't be loaded a default set of types is returned.
 */
Asn1Acn::File *Asn1ModelStorage::asn1DataTypes(const QString &fileName)
{
    if (fileName.isEmpty()) {
        return {};
    }

    const QString canonicalFilename = QFileInfo(fileName).canonicalFilePath();

    if (!contains(canonicalFilename)) {
        if (m_asn1Watcher->addPath(canonicalFilename)) {
            invalidateStorage();
        } else {
            return {};
        }
    }
    auto it = m_store.find(canonicalFilename);
    if (m_store.cend() != it) {
        return it->second.get();
    }
    return {};
}

/*!
   Returns if the file is already loaded in this store
 */
bool Asn1ModelStorage::contains(const QString &fileName) const
{
    const QString canonicalFilename = QFileInfo(fileName).canonicalFilePath();
    return m_store.find(canonicalFilename) != m_store.cend();
}

/*!
   Clears the whole store
 */
void Asn1ModelStorage::clear()
{
    m_reloadTimer.stop();
    m_asn1Watcher->removePaths(m_asn1Watcher->files());
    m_store.clear();
}

/*!
   Ensures that the files are watched. So if files on disk are changed, all of them are reloaded then.
 */
void Asn1ModelStorage::watchFiles(const QStringList &fileNames)
{
    if (fileNames == m_asn1Watcher->files()) {
        return;
    }

    QStringList canonicalNames;
    for (const QString &file : fileNames) {
        QString canonicalFile = QFileInfo(file).canonicalFilePath();
        if (QFileInfo::exists(canonicalFile)) {
            canonicalNames.append(canonicalFile);
        }
    }
    canonicalNames.removeDuplicates();

    const QStringList &exludedPaths = m_asn1Watcher->addPaths(canonicalNames);
    if (exludedPaths != canonicalNames) {
        invalidateStorage();
    }
}

void Asn1ModelStorage::invalidateStorage()
{
    const QStringList fileNames = m_asn1Watcher->files();
    QStringList errorMessages;
    Asn1Acn::Asn1Reader parser;

    // convert fileNames to fileInfos
    QVector<QFileInfo> fileInfos;
    std::for_each(fileNames.cbegin(), fileNames.cend(),
            [&fileInfos](const QString &fileName) { fileInfos.append(QFileInfo(fileName)); });

    m_store.clear();
    m_store = parser.parseAsn1Files(fileInfos, &errorMessages);

    if (m_store.empty()) {
        qWarning() << "Can't read file(s) " << fileNames.join(", ") << ":" << errorMessages.join(", ");
        Q_EMIT error(fileNames, errorMessages);
        return;
    }

    Q_EMIT success(fileNames);
}

} // namespace Asn1Acn
