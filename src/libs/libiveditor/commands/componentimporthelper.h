/*
  Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QObject>
#include <QScopedPointer>

class QTemporaryDir;
class QFileInfo;

namespace ivm {
class IVObject;
}

namespace shared {
class ComponentModel;
}

namespace Asn1Acn {
class Asn1SystemChecks;
}

namespace ive {
namespace cmd {

class ComponentImportHelper : public QObject
{
    Q_OBJECT
public:
    explicit ComponentImportHelper(
            shared::ComponentModel *componentModel, Asn1Acn::Asn1SystemChecks *asn1Checks, QObject *parent = nullptr);
    explicit ComponentImportHelper(
            shared::ComponentModel *componentModel, const QString &projectPath, QObject *parent = nullptr);
    ~ComponentImportHelper() override;

public:
    void setComponentModel(shared::ComponentModel *componentModel);
    void setAsn1SystemChecks(Asn1Acn::Asn1SystemChecks *asn1Checks);
    bool isValid() const;

Q_SIGNALS:
    void asn1FilesImported(const QStringList &files);
    void asn1FilesRemoved(const QStringList &files);

public:
    void redoSourcesCloning(const QList<ivm::IVObject *> &objects);
    void redoSourcesCloning(const ivm::IVObject *object);
    void undoSourcesCloning();
    void redoAsnFilesImport(const ivm::IVObject *object);
    void undoAsnFilesImport();
    QString relativePathForObject(const ivm::IVObject *object) const;

    static QStringList asn1ModuleDuplication(Asn1Acn::Asn1SystemChecks *asn1Checks, QVector<QFileInfo> &asn1FileInfos);

protected:
    shared::ComponentModel *m_model;
    Asn1Acn::Asn1SystemChecks *m_asn1Checks;
    QString m_destPath;
    QScopedPointer<QTemporaryDir> m_tempDir;
    QStringList m_importedAsnFiles;
    QList<const ivm::IVObject *> m_importedSources;
};

} // namespace ive
} // namespace cmd
