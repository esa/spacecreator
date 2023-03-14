/*
  Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "uiexporter.h"

#include "exportableproperty.h"

#include <veobject.h>

namespace templating {

UIExporter::UIExporter(shared::PropertyTemplateConfig *propertyConfig, QObject *parent)
    : templating::ObjectsExporter { parent }
    , m_propertyConfig(propertyConfig)
{
}

QString UIExporter::defaultTemplatePath() const
{
    /// Split per Editor type
    return QString("%1/aadl_xml/style.%2").arg(templatesPath(), templateFileExtension());
}

QVariant UIExporter::createFrom(const shared::VEObject *object) const
{
    const QList<shared::ExportableProperty> result =
            shared::ExportableProperty::createPropertiesFromObject(m_propertyConfig, object, QLatin1String("UI"));
    return QVariant::fromValue(result);
}

QString UIExporter::groupName(const shared::VEObject *object) const
{
    return QLatin1String("Entities");
}

} // namespace templating
