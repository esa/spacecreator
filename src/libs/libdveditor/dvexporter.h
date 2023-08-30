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

class QBuffer;

namespace dvm {
class DVObject;
}

namespace templating {
class UIExporter;
}

namespace dve {

class DVExporter : public templating::ObjectsExporter
{
    Q_OBJECT
public:
    explicit DVExporter(QObject *parent = nullptr);
    QString defaultTemplatePath() const override;

    static QString templatePath(const QString &templateName);

    bool exportObjects(const QList<shared::VEObject *> &objects, QIODevice *outBuffer,
            const QString &pathToTemplate = templatePath(QLatin1String("deploymentview.ui")));

    bool exportObjectsInteractively(const QList<shared::VEObject *> &objects, const QString &creatorGitHash, const QString &outPath = QString(),
            const QString &pathToTemplate = QString(), QWidget *root = nullptr);

    bool exportObjectsSilently(const QList<shared::VEObject *> &objects, const QString &creatorGitHash, const QString &outPath,
            const QString &pathToTemplate = {}, const QString &uiFile = {});

private:
    /**
     * @brief DVExporter::createFrom creates appropriate exported class and casts to QVariant
     * @param object exported object
     * @return created exported object as QVariant
     */
    QVariant createFrom(const shared::VEObject *object) const override;
    QString groupName(const shared::VEObject *object) const override;

    templating::UIExporter *m_uiExporter;
};

} // namespace dve
