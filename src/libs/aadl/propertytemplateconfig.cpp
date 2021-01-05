/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "propertytemplateconfig.h"

#include "aadlobject.h"
#include "common.h"
#include "propertytemplate.h"

#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QStandardPaths>

namespace aadl {

/*!
   Adds all attributes from \p attrs that are not already in \a storage to that data
 */
void collectUniqeAttributes(
        const QHash<QString, PropertyTemplate *> &attrs, QHash<PropertyTemplate *, PropertyTemplate *> &storage)
{
    for (auto attr : attrs) {
        if (!storage.contains(attr)) {
            storage.insert(attr, attr);
        }
    }
};

struct PropertyTemplateConfig::PropertyTemplateConfigPrivate {
    void init(const QList<PropertyTemplate *> &attrs)
    {
        QHash<PropertyTemplate *, PropertyTemplate *> uniqeAttrs;
        collectUniqeAttributes(m_function, uniqeAttrs);
        collectUniqeAttributes(m_reqIface, uniqeAttrs);
        collectUniqeAttributes(m_provIface, uniqeAttrs);

        qDeleteAll(uniqeAttrs);
        m_function.clear();
        m_reqIface.clear();
        m_provIface.clear();

        for (PropertyTemplate *attr : attrs) {
            if (attr->scope() == PropertyTemplate::Scopes(PropertyTemplate::Scope::None))
                continue;

            if (attr->scope().testFlag(PropertyTemplate::Scope::Function))
                m_function.insert(attr->name(), attr);

            if (attr->scope().testFlag(PropertyTemplate::Scope::Required_Interface))
                m_reqIface.insert(attr->name(), attr);

            if (attr->scope().testFlag(PropertyTemplate::Scope::Provided_Interface))
                m_provIface.insert(attr->name(), attr);
        }
    }

    QString m_configPath;
    QHash<QString, PropertyTemplate *> m_function;
    QHash<QString, PropertyTemplate *> m_reqIface;
    QHash<QString, PropertyTemplate *> m_provIface;
};

PropertyTemplateConfig::PropertyTemplateConfig()
    : d(new PropertyTemplateConfigPrivate())
{
}

PropertyTemplateConfig::~PropertyTemplateConfig() { }

QString systemResourceConfigPath()
{
    return QLatin1String(":/defaults/interface/properties/resources/system_attributes.xml");
}

QString userResourceConfigPath()
{
    return QLatin1String(":/defaults/interface/properties/resources/default_attributes.xml");
}

static bool ensureFileExists(const QString &filePath)
{
    if (!QFileInfo::exists(filePath) && !shared::copyResourceFile(userResourceConfigPath(), filePath)) {
        qWarning() << "Can't create default storage for properties/attributes" << filePath
                   << "from:" << userResourceConfigPath();
        return false;
    }
    return true;
}

void PropertyTemplateConfig::init(const QString &configPath)
{
    QList<PropertyTemplate *> systemAttributes;
    QFile f(systemResourceConfigPath());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Can't open file:" << configPath << f.errorString();
        return;
    }
    systemAttributes = parseAttributesList(QString::fromUtf8(f.readAll()));

    QList<PropertyTemplate *> userAttributes;
    if (ensureFileExists(configPath)) {
        d->m_configPath = configPath;
        QFile f(configPath);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Can't open file:" << configPath << f.errorString();
            return;
        }
        userAttributes = parseAttributesList(QString::fromUtf8(f.readAll()));
    }

    // merge configs
    auto inSystemAttributes = [&systemAttributes](const QString &name) -> bool {
        auto it = std::find_if(systemAttributes.begin(), systemAttributes.end(),
                [&name](PropertyTemplate *property) { return property->name() == name; });
        return it != systemAttributes.end();
    };
    auto it = userAttributes.begin();
    while (it != userAttributes.end()) {
        if (!inSystemAttributes((*it)->name())) {
            systemAttributes.append(*it);
            it = userAttributes.erase(it);
        } else {
            ++it;
        }
    }
    qDeleteAll(userAttributes);

    d->init(systemAttributes);
}

QList<PropertyTemplate *> PropertyTemplateConfig::parseAttributesList(
        const QString &fromData, QString *errorMsg, int *errorLine, int *errorColumn)
{
    QList<PropertyTemplate *> attrs;
    QDomDocument doc;
    if (doc.setContent(fromData, false, errorMsg, errorLine, errorColumn)) {
        const QDomElement docElem = doc.documentElement();
        if (docElem.isNull())
            return {};

        QDomElement attributeElement = docElem.firstChildElement(PropertyTemplate::tagName());
        while (!attributeElement.isNull()) {
            if (auto PropertyTemplate = PropertyTemplate::fromXml(attributeElement)) {
                attrs.append(PropertyTemplate);
            }
            attributeElement = attributeElement.nextSiblingElement(PropertyTemplate::tagName());
        }
    }
    return attrs;
}

QHash<QString, PropertyTemplate *> PropertyTemplateConfig::propertyTemplatesForObject(const aadl::AADLObject *obj)
{
    auto scope = PropertyTemplate::Scope::None;
    QList<PropertyTemplate *> properties;
    switch (obj->aadlType()) {
    case aadl::AADLObject::Type::FunctionType:
    case aadl::AADLObject::Type::Function:
        scope = PropertyTemplate::Scope::Function;
        properties = attributesForFunction();
        break;
    case aadl::AADLObject::Type::RequiredInterface:
        scope = PropertyTemplate::Scope::Required_Interface;
        properties = attributesForRequiredInterface();
        break;
    case aadl::AADLObject::Type::ProvidedInterface:
        scope = PropertyTemplate::Scope::Provided_Interface;
        properties = attributesForProvidedInterface();
        break;
    default:
        return {};
    }
    QHash<QString, PropertyTemplate *> result;
    auto validate = [obj, scope](PropertyTemplate *property) {
        if (!property->scope().testFlag(scope)) {
            return false;
        }

        const auto scopesValidators = property->attrValidatorPatterns();
        for (auto it = scopesValidators.constBegin(); it != scopesValidators.constEnd(); ++it) {
            if (it.key() != scope) {
                continue;
            }

            ///
            auto checkPattern = [](const QHash<QString, QVariant> &data, const QString &name, const QString &pattern) {
                auto objPropIter = data.constFind(name);
                if (objPropIter != data.constEnd()) {
                    const QRegularExpression rx(pattern);
                    const QRegularExpressionMatch match = rx.match(objPropIter.value().toString());
                    if (!match.hasMatch())
                        return false;
                }
                return true;
            };
            /// TODO: add type into XML storage for AttrValidator (PropValidator)
            /// to lookup in appropriate data set
            /// Add mandatory attribute for combined checks
            if (!checkPattern(obj->props(), it->first, it->second))
                return false;

            if (!checkPattern(obj->attrs(), it->first, it->second))
                return false;
        }

        return true;
    };
    std::for_each(properties.constBegin(), properties.constEnd(), [&result, validate](PropertyTemplate *property) {
        if (validate(property))
            result.insert(property->name(), property);
    });
    return result;
}

QList<PropertyTemplate *> PropertyTemplateConfig::attributesForFunction()
{
    return d->m_function.values();
}

QList<PropertyTemplate *> PropertyTemplateConfig::attributesForRequiredInterface()
{
    return d->m_reqIface.values();
}

QList<PropertyTemplate *> PropertyTemplateConfig::attributesForProvidedInterface()
{
    return d->m_provIface.values();
}

QString PropertyTemplateConfig::configPath() const
{
    return d->m_configPath;
}

} // namespace aadl
