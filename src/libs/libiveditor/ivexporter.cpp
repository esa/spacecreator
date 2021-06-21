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

#include "ivexporter.h"

#include "common.h"
#include "interface/interfacedocument.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "ivobject.h"
#include "stringtemplate.h"
#include "templateeditor.h"
#include "templating/exportableivconnection.h"
#include "templating/exportableivconnectiongroup.h"
#include "templating/exportableivfunction.h"
#include "templating/exportableivinterface.h"
#include "templating/exportableivobject.h"

#include <QBuffer>
#include <QDebug>
#include <QFileDialog>
#include <QMetaEnum>
#include <QObject>
#include <QScopedPointer>
#include <QStandardPaths>

namespace ive {

/*!
 * \class ive::XmlDocExporter
 * \brief The helper that incorporates templating::StringTemplate-related stuff.
 */

QString IVExporter::defaultTemplatePath() const
{
    return QString("%1/aadl_xml/interfaceview.%2").arg(templatesPath(), templateFileExtension());
}

/**
   @brief XmlDocExporter::exportObjects writes the document as xml to the given buffer
   @param objects the set of IV(AADL) entities
   @param outBuffer the buffer that is open and ready to be written to
   @param templatePath the grantlee template to use for the export. If empty, the default one is used
   @return true when the export was successful.
 */
bool IVExporter::exportObjects(
        const QList<shared::VEObject *> &objects, QBuffer *outBuffer, const QString &templatePath)
{
    const QHash<QString, QVariant> ivObjects = collectObjects(objects);
    return exportData(ivObjects, templatePath, outBuffer);
}

bool IVExporter::exportDocSilently(InterfaceDocument *doc, const QString &outPath, const QString &templatePath)
{
    const QHash<QString, QVariant> ivObjects = collectInterfaceObjects(doc);
    return exportData(ivObjects, outPath.isEmpty() ? doc->path() : outPath, templatePath, InteractionPolicy::Silently);
}

bool IVExporter::exportDocInteractively(
        InterfaceDocument *doc, const QString &outPath, const QString &templatePath, QWidget *root)
{
    const QHash<QString, QVariant> ivObjects = collectInterfaceObjects(doc);
    return exportData(
            ivObjects, outPath.isEmpty() ? doc->path() : outPath, templatePath, InteractionPolicy::Interactive, root);
}

IVExporter::IVExporter(QObject *parent)
    : templating::ObjectsExporter(parent)
{
    ensureDefaultTemplatesDeployed(QLatin1String(":/defaults/templating/xml_templates"));
}

QVariant IVExporter::createFrom(const shared::VEObject *object) const
{
    return ExportableIVObject::createFrom(object->as<const ivm::IVObject *>());
}

QString IVExporter::groupName(const shared::VEObject *object) const
{
    if (auto ivObject = object->as<const ivm::IVObject *>()) {
        switch (ivObject->type()) {
        case ivm::IVObject::Type::InterfaceGroup:
            return {};
        case ivm::IVObject::Type::Function:
        case ivm::IVObject::Type::FunctionType:
            return QStringLiteral("Functions");
        case ivm::IVObject::Type::RequiredInterface:
        case ivm::IVObject::Type::ProvidedInterface:
            return QStringLiteral("Interfaces");
        case ivm::IVObject::Type::Comment:
            return QStringLiteral("Comments");
        case ivm::IVObject::Type::Connection:
            return QStringLiteral("Connections");
        case ivm::IVObject::Type::ConnectionGroup:
            return QStringLiteral("ConnectionGroups");
        default:
            break;
        }
    }
    return QString();
}

QHash<QString, QVariant> IVExporter::collectInterfaceObjects(InterfaceDocument *doc)
{
    QHash<QString, QVariant> grouppedObjects = collectObjects(doc->objects().values());
    // Add meta-data
    if (!doc->asn1FileName().isEmpty()) {
        grouppedObjects["Asn1FileName"] = QVariant::fromValue(doc->asn1FileName());
    }
    if (!doc->mscFileName().isEmpty()) {
        grouppedObjects["MscFileName"] = QVariant::fromValue(doc->mscFileName());
    }

    return grouppedObjects;
}
}
