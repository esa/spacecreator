/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#pragma once

#include "archetypes/archetypemodel.h"

#include <QList>
#include <QString>
#include <shared/veobject.h>
#include <templating/objectsexporter.h>
#include <templating/uiexporter.h>

namespace ivm {

class IVXMLWriter : public templating::ObjectsExporter
{
    Q_OBJECT
public:
    IVXMLWriter(QObject *parent = nullptr);
    static QString templatePath(const QString &templateName);
    QString defaultTemplatePath() const override;

    bool exportObjects(const QList<shared::VEObject *> &objects, QIODevice *outBuffer,
            ivm::ArchetypeModel *archetypesModel = nullptr,
            const QString &pathToTemplate = templatePath(QLatin1String("interfaceview.ui")));

    bool exportObjectsSilently(const QList<shared::VEObject *> &objects, const QString &outPath,
            ivm::ArchetypeModel *archetypesModel = nullptr, const QString &templatePath = QString());

protected:
    templating::UIExporter *m_uiExporter;

private:
    /**
     * @brief IVXMLWriter::createFrom creates appropriate exported class and casts to QVariant
     * @param object exported object
     * @return created exported object as QVariant
     */
    QVariant createFrom(const shared::VEObject *object) const override;
    QString groupName(const shared::VEObject *object) const override;

    void checkArchetypeIntegrity(QList<shared::VEObject *> ivObjects, ivm::ArchetypeModel *archetypesModel);
};
}
