/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivpropertytemplateconfig.h"

#include "ivpropertytemplate.h"
#include "ivcommonprops.h"

namespace ivm {

const static QString kSysAttrsConfigFilePath = QLatin1String(":/defaults/resources/iv_system_attributes.xml");
const static QString kUserAttrsResourceConfigPath = QLatin1String(":/defaults/resources/iv_custom_attributes.xml");

IVPropertyTemplateConfig *IVPropertyTemplateConfig::m_instance = nullptr;

IVPropertyTemplateConfig *IVPropertyTemplateConfig::instance()
{
    if (m_instance == nullptr) {
        m_instance = new IVPropertyTemplateConfig;
    }
    return m_instance;
}

void IVPropertyTemplateConfig::cleanup()
{
    delete m_instance;
    m_instance = nullptr;
}

QString IVPropertyTemplateConfig::title() const
{
    return QObject::tr("InterfaceView");
}

shared::PropertyTemplate *IVPropertyTemplateConfig::createPropertyTemplate() const
{
    return new IVPropertyTemplate();
}

/**
 * Returns the name for the implementation of a programming language
 * An example is "Simulink" -> "SIMULINK"
 * @param language The programming name
 */
QString IVPropertyTemplateConfig::languageDirectory(const QString &language) const
{
    const QString languageKey = meta::Props::token(meta::Props::Token::language);
    const shared::PropertyTemplate *property = propertyTemplateForObject(nullptr, languageKey);
    if (property) {
        QMap<QString, QString> data = property->enumData(language);
        if (data.isEmpty()) {
            qWarning() << QString("No language '%1' configured. Using the language name.").arg(language);
            return language;
        }
        const QString folderKey = "folder_name";
        if (data.contains(folderKey)) {
            return data[folderKey];
        } else {
            qWarning() << QString("No folder name set for language '%1'. Using the language name .").arg(language);
        }
        return language;
    }

    qWarning() << QString("No property 'language' in attributes. Using the language name '%1'").arg(language);
    return language;
}

IVPropertyTemplateConfig::IVPropertyTemplateConfig()
    : shared::PropertyTemplateConfig(kSysAttrsConfigFilePath)
{
}

shared::PropertyTemplate *IVPropertyTemplateConfig::createPropertyTemplate(const QDomElement &element) const
{
    return shared::PropertyTemplate::createFromXml<IVPropertyTemplate>(element);
}

QString IVPropertyTemplateConfig::userAttrsResourceConfigPath() const
{
    return kUserAttrsResourceConfigPath;
}

} // namespace ivm
