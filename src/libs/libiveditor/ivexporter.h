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

#pragma once

#include "objectsexporter.h"

#include <QString>
#include <QVariantList>

class QBuffer;
class QWidget;
namespace ivm {
class IVObject;
}

namespace ive {

class InterfaceDocument;

class IVExporter : public templating::ObjectsExporter
{
public:
    explicit IVExporter(QObject *parent = nullptr);
    QString defaultTemplatePath() const override;

    bool exportObjects(
            const QList<ivm::IVObject *> &objects, QBuffer *outBuffer, const QString &templatePath = QString());

    bool exportDocSilently(
            InterfaceDocument *doc, const QString &outPath = QString(), const QString &templatePath = QString());

    bool exportDocInteractively(InterfaceDocument *doc, const QString &outPath = QString(),
            const QString &templatePath = QString(), QWidget *root = nullptr);

private:
    /**
     * @brief IVExporter::createFrom creates appropriate exported class and casts to QVariant
     * @param object exported object
     * @return created exported object as QVariant
     */
    QVariant createFrom(const shared::VEObject *object) const override;
    QString groupName(const shared::VEObject *object) const override;

    QHash<QString, QVariant> collectInterfaceObjects(InterfaceDocument *doc);
};

}
