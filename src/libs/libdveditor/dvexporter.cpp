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

#include "dvexporter.h"

#include "dvconnection.h"
#include "dvdevice.h"
#include "dvmodel.h"
#include "dvnode.h"
#include "dvobject.h"
#include "dvpartition.h"
#include "templating/exportabledvbus.h"
#include "templating/exportabledvconnection.h"
#include "templating/exportabledvdevice.h"
#include "templating/exportabledvnode.h"
#include "templating/exportabledvobject.h"
#include "templating/exportabledvpartition.h"

#include <QBuffer>

namespace dve {

QString DVExporter::defaultTemplatePath() const
{
    return QString("%1/aadl_xml/deploymentview.%2").arg(templatesPath(), templateFileExtension());
}

bool DVExporter::exportObjects(
        const QList<shared::VEObject *> &objects, QBuffer *outBuffer, const QString &templatePath)
{
    const QHash<QString, QVariant> dvObjects = collectObjects(objects);
    return exportData(dvObjects, templatePath, outBuffer);
}

bool DVExporter::exportObjectsInteractively(
        const QList<shared::VEObject *> &objects, const QString &outPath, const QString &templatePath, QWidget *root)
{
    const QHash<QString, QVariant> dvObjects = collectObjects(objects);
    return exportData(dvObjects, outPath, templatePath, InteractionPolicy::Interactive, root);
}

bool DVExporter::exportObjectsSilently(
        const QList<shared::VEObject *> &objects, const QString &outPath, const QString &templatePath)
{
    const QHash<QString, QVariant> dvObjects = collectObjects(objects);
    return exportData(dvObjects, outPath, templatePath, InteractionPolicy::Silently);
}

DVExporter::DVExporter(QObject *parent)
    : templating::ObjectsExporter(parent)
{
    ensureDefaultTemplatesDeployed(QLatin1String(":/defaults/templating/xml_templates"));
}

QVariant DVExporter::createFrom(const shared::VEObject *object) const
{
    return ExportableDVObject::createFrom(object->as<const dvm::DVObject *>());
}

QString DVExporter::groupName(const shared::VEObject *object) const
{
    if (auto dvObject = object->as<const dvm::DVObject *>()) {
        switch (dvObject->type()) {
        case dvm::DVObject::Type::Node:
            return QStringLiteral("Nodes");
        case dvm::DVObject::Type::Connection:
            return QStringLiteral("Connections");
        case dvm::DVObject::Type::Bus:
            return QStringLiteral("Buses");
        default:
            return {};
        }
    }
    return {};
}

} // namespace dve
