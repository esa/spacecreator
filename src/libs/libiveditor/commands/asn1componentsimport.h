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

namespace ivm {
class IVObject;
}

namespace Asn1Acn {
class Asn1SystemChecks;
}

namespace ive {
namespace cmd {

class ASN1ComponentsImport : public QObject
{
    Q_OBJECT
public:
    explicit ASN1ComponentsImport(
            Asn1Acn::Asn1SystemChecks *asn1Checks, const QString &srcPath, const QString &destPath);
    ~ASN1ComponentsImport() override;

public:
    void setAsn1SystemChecks(Asn1Acn::Asn1SystemChecks *asn1Checks);

Q_SIGNALS:
    void asn1FilesImported(const QStringList &files);
    void asn1FilesRemoved(const QStringList &files);

public:
    void redoSourceCloning(const ivm::IVObject *object);
    void undoSourceCloning(const ivm::IVObject *object);
    void redoAsnFileImport(const ivm::IVObject *object);
    void undoAsnFileImport();
    QString relativePathForObject(const ivm::IVObject *object) const;

protected:
    Asn1Acn::Asn1SystemChecks *m_asn1Checks;
    QString m_srcPath;
    QString m_destPath;
    QScopedPointer<QTemporaryDir> m_tempDir;
    QStringList m_importedAsnFiles;
};

} // namespace ive
} // namespace cmd
