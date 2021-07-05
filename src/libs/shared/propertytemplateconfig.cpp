/*
  Copyright (C) 2019-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "common.h"
#include "errorhub.h"
#include "propertytemplate.h"
#include "veobject.h"

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

namespace shared {

struct PropertyTemplateConfig::PropertyTemplateConfigPrivate {
    QString m_configPath;
    QString m_sysConfigPath;
    QList<PropertyTemplate *> m_attrTemplates;
};

PropertyTemplateConfig::PropertyTemplateConfig(const QString &sysConfigPath)
    : d(new PropertyTemplateConfigPrivate())
{
    d->m_sysConfigPath = sysConfigPath;
}

QList<PropertyTemplate *> PropertyTemplateConfig::systemAttributes() const
{
    QFile f(d->m_sysConfigPath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ErrorHub::addError(ErrorItem::Warning,
                QStringLiteral("Can't open file: %1 - %2").arg(d->m_sysConfigPath, f.errorString()));
        return {};
    }
    QString errorMsg;
    int errorLine = -1;
    int errorColumn = -1;

    QList<PropertyTemplate *> sysAttrs =
            parseAttributesList(QString::fromUtf8(f.readAll()), &errorMsg, &errorLine, &errorColumn);
    if (sysAttrs.isEmpty()) {
        ErrorHub::addError(ErrorItem::Error,
                QStringLiteral("Can't load system attributes: %1:%2 => %3")
                        .arg(errorLine)
                        .arg(errorColumn)
                        .arg(errorMsg));
        return {};
    }
    for (auto attr : qAsConst(sysAttrs)) {
        attr->setSystem(true);
    }
    return sysAttrs;
}

PropertyTemplateConfig::~PropertyTemplateConfig() { }

void PropertyTemplateConfig::init(const QString &configPath)
{
    QList<PropertyTemplate *> attributes = systemAttributes();

    QList<PropertyTemplate *> userAttributes;
    if (ensureFileExists(configPath, userAttrsResourceConfigPath())) {
        d->m_configPath = configPath;
        QFile f(configPath);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            ErrorHub::addError(
                    ErrorItem::Warning, QStringLiteral("Can't open file: %1 - %2").arg(configPath, f.errorString()));
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

    qDeleteAll(d->m_attrTemplates);
    d->m_attrTemplates = attributes;
}

bool PropertyTemplateConfig::hasPropertyTemplateForObject(const VEObject *obj, const QString &name) const
{
    return propertyTemplateForObject(obj, name) != nullptr;
}

PropertyTemplate *PropertyTemplateConfig::propertyTemplateForObject(const VEObject *obj, const QString &name) const
{
    const QList<PropertyTemplate *> templates = propertyTemplatesForObject(obj);
    auto it = std::find_if(templates.constBegin(), templates.constEnd(),
            [name](PropertyTemplate *propTemplate) { return propTemplate->name() == name; });
    return it == templates.constEnd() ? nullptr : *it;
}

QList<PropertyTemplate *> PropertyTemplateConfig::parseAttributesList(
        const QString &fromData, QString *errorMsg, int *errorLine, int *errorColumn) const
{
    QList<PropertyTemplate *> attrs;
    QDomDocument doc;
    if (doc.setContent(fromData, false, errorMsg, errorLine, errorColumn)) {
        const QDomElement docElem = doc.documentElement();
        if (docElem.isNull())
            return {};

        QDomElement attributeElement = docElem.firstChildElement(PropertyTemplate::kAttributeTagName);
        while (!attributeElement.isNull()) {
            if (PropertyTemplate *propTemplate = createPropertyTemplate(attributeElement)) {
                attrs.append(propTemplate);
            }
            attributeElement = attributeElement.nextSiblingElement(PropertyTemplate::kAttributeTagName);
        }
    }
    return attrs;
}

QList<PropertyTemplate *> PropertyTemplateConfig::propertyTemplatesForObject(const VEObject *obj) const
{
    QList<PropertyTemplate *> attrs;
    std::copy_if(d->m_attrTemplates.cbegin(), d->m_attrTemplates.cend(), std::back_inserter(attrs),
            [obj](PropertyTemplate *attrTemplate) {
                return !attrTemplate->scopes() || (attrTemplate->scopes() & attrTemplate->objectScope(obj));
            });
    return attrs;
}

QString PropertyTemplateConfig::configPath() const
{
    return d->m_configPath;
}

} // namespace shared
