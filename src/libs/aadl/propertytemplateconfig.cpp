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

namespace ivm {

PropertyTemplateConfig *PropertyTemplateConfig::m_instance = nullptr;

const static QString kSysAttrsConfigFilePath = QLatin1String(":/defaults/resources/system_attributes.xml");

const static QString kUserAttrsResourceConfigPath = QLatin1String(":/defaults/resources/default_attributes.xml");

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

QList<PropertyTemplate *> PropertyTemplateConfig::systemAttributes()
{
    QFile f(kSysAttrsConfigFilePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Can't open file:" << kSysAttrsConfigFilePath << f.errorString();
        return {};
    }
    QString errorMsg;
    int errorLine = -1;
    int errorColumn = -1;

    QList<PropertyTemplate *> sysAttrs =
            parseAttributesList(QString::fromUtf8(f.readAll()), &errorMsg, &errorLine, &errorColumn);
    if (sysAttrs.isEmpty()) {
        qCritical() << "Can't load system attributes:"
                    << QStringLiteral("%1:%2 => %3").arg(errorLine).arg(errorColumn).arg(errorMsg);
        return {};
    }
    for (auto attr : sysAttrs) {
        attr->setSystem(true);
    }
    return sysAttrs;
}

PropertyTemplateConfig *PropertyTemplateConfig::instance()
{
    if (m_instance == nullptr) {
        m_instance = new PropertyTemplateConfig;
    }
    return m_instance;
}

PropertyTemplateConfig::~PropertyTemplateConfig() {}

void PropertyTemplateConfig::init(const QString &configPath)
{
    QList<PropertyTemplate *> attributes = systemAttributes();

    QList<PropertyTemplate *> userAttributes;
    if (shared::ensureFileExists(configPath, kUserAttrsResourceConfigPath)) {
        d->m_configPath = configPath;
        QFile f(configPath);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Can't open file:" << configPath << f.errorString();
            return;
        }
        userAttributes = parseAttributesList(QString::fromUtf8(f.readAll()));
    }

    // merge configs
    auto inSystemAttributes = [&attributes](const QString &name) -> bool {
        auto it = std::find_if(attributes.begin(), attributes.end(),
                [&name](PropertyTemplate *property) { return property->name() == name; });
        return it != attributes.end();
    };
    auto it = userAttributes.begin();
    while (it != userAttributes.end()) {
        if (!inSystemAttributes((*it)->name())) {
            attributes.append(*it);
            it = userAttributes.erase(it);
        } else {
            ++it;
        }
    }
    qDeleteAll(userAttributes);

    d->init(attributes);
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

QHash<QString, PropertyTemplate *> PropertyTemplateConfig::propertyTemplatesForObject(const ivm::AADLObject *obj)
{
    switch (obj->aadlType()) {
    case ivm::AADLObject::Type::FunctionType:
    case ivm::AADLObject::Type::Function:
        return d->m_function;
    case ivm::AADLObject::Type::RequiredInterface:
        return d->m_reqIface;
    case ivm::AADLObject::Type::ProvidedInterface:
        return d->m_provIface;
    default:
        return {};
    }
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

} // namespace ivm
