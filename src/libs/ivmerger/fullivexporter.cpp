/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2023 N7 Space Sp. z o.o.
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

#include "fullivexporter.h"

#include <QDir>
#include <QFileInfo>
#include <QHash>
#include <QSaveFile>
#include <QVariant>

namespace ivmerger {
FullIVExporter::FullIVExporter(ivm::IVModel *modelToExport)
    : m_modelToExport(modelToExport)
{
}

bool FullIVExporter::doExport(QString outputIvFile)
{
    QString uiFile = "interfaceview.ui.xml";
    QList<shared::VEObject *> objects = m_modelToExport->objects().values();
    QHash<QString, QVariant> ivObjects = collectObjects(objects);

    ivObjects[QLatin1String("UiFile")] = QVariant::fromValue(uiFile);

    bool result = exportObjectsSilently(m_modelToExport->objects().values(), outputIvFile);
    if (!result) {
        return false;
    }

    QSaveFile saveFile(outputIvFile);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        return false;
    }
    result = exportData(ivObjects, QString(), &saveFile);

    if (!result) {
        return false;
    }

    if (!saveFile.commit()) {
        return false;
    }

    saveFile.QIODevice::close();

    const QHash<QString, QVariant> uiObjects = m_uiExporter->collectObjects(objects);

    saveFile.setFileName(QFileInfo(outputIvFile).absolutePath() + QDir::separator() + uiFile);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        return false;
    }
    result = m_uiExporter->exportData(uiObjects, m_uiExporter->defaultTemplatePath(), &saveFile);
    if (!result) {
        return false;
    }
    result = saveFile.commit();
    if (!result) {
        return false;
    }
    saveFile.QIODevice::close();
    return true;
}
}
