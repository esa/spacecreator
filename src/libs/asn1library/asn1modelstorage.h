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

#pragma once

#include <QFileInfo>
#include <QHash>
#include <QMap>
#include <QObject>
#include <QSet>
#include <QSharedPointer>
#include <QString>
#include <QTimer>
#include <memory>

class QFileSystemWatcher;

namespace Asn1Acn {
class File;

namespace Types {
class Type;
}

/*!
   Stores shared pointers to all asn1 file objects. If needed the file i loaded (lazy loading).
   A signal is emitted when a file loaded already is changed \sa dataTypesChanged
 */
class Asn1ModelStorage : public QObject
{
    Q_OBJECT
public:
    Asn1ModelStorage(QObject *parent = nullptr);
    ~Asn1ModelStorage();

    QSharedPointer<File> asn1DataTypes(const QString &fileName) const;
    bool contains(const QString &fileName) const;

    void clear();

    void watchFile(const QString &fileName);

Q_SIGNALS:
    void dataTypesChanged(const QString &fileName);
    void success(const QString &fileName);
    void error(const QString &fileName, const QStringList &errors);

private:
    bool loadFile(const QString &fileName);
    QSharedPointer<Asn1Acn::File> loadData(const QString &fileName);
    Q_SLOT void loadChangedFiles();

    QHash<QString, QSharedPointer<Asn1Acn::File>> m_store;
    QFileSystemWatcher *m_asn1Watcher = nullptr;
    QTimer m_reloadTimer;
    QSet<QString> m_filesToReload;
};

}
