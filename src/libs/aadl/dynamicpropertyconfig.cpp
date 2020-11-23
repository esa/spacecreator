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

#include "dynamicpropertyconfig.h"

#include "aadlobject.h"
#include "common.h"
#include "dynamicproperty.h"

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
        const QHash<QString, DynamicProperty *> &attrs, QHash<DynamicProperty *, DynamicProperty *> &storage)
{
    for (auto attr : attrs) {
        if (!storage.contains(attr)) {
            storage.insert(attr, attr);
        }
    }
};

struct DynamicPropertyConfig::DynamicPropertyConfigPrivate {
    void init(const QList<DynamicProperty *> &attrs)
    {
        QHash<DynamicProperty *, DynamicProperty *> uniqeAttrs;
        collectUniqeAttributes(m_function, uniqeAttrs);
        collectUniqeAttributes(m_reqIface, uniqeAttrs);
        collectUniqeAttributes(m_provIface, uniqeAttrs);

        qDeleteAll(uniqeAttrs);
        m_function.clear();
        m_reqIface.clear();
        m_provIface.clear();

        for (DynamicProperty *attr : attrs) {
            if (attr->scope() == DynamicProperty::Scopes(DynamicProperty::Scope::None))
                continue;

            if (attr->scope().testFlag(DynamicProperty::Scope::Function))
                m_function.insert(attr->name(), attr);

            if (attr->scope().testFlag(DynamicProperty::Scope::Required_Interface))
                m_reqIface.insert(attr->name(), attr);

            if (attr->scope().testFlag(DynamicProperty::Scope::Provided_Interface))
                m_provIface.insert(attr->name(), attr);
        }
    }

    QString m_configPath;
    QHash<QString, DynamicProperty *> m_function;
    QHash<QString, DynamicProperty *> m_reqIface;
    QHash<QString, DynamicProperty *> m_provIface;
};

DynamicPropertyConfig::DynamicPropertyConfig()
    : d(new DynamicPropertyConfigPrivate())
{
}

DynamicPropertyConfig::~DynamicPropertyConfig() { }

QString resourceConfigPath()
{
    return QLatin1String(":/defaults/interface/properties/resources/default_attributes.xml");
}

static bool ensureFileExists(const QString &filePath)
{
    if (!QFileInfo::exists(filePath) && !shared::copyResourceFile(resourceConfigPath(), filePath)) {
        qWarning() << "Can't create default storage for properties/attributes" << filePath
                   << "from:" << resourceConfigPath();
        return false;
    }
    return true;
}

void DynamicPropertyConfig::init(const QString &configPath)
{
    if (ensureFileExists(configPath)) {
        d->m_configPath = configPath;
        QFile f(configPath);
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString errMsg;
            int line;
            int column;
            d->init(parseAttributesList(QString::fromUtf8(f.readAll()), &errMsg, &line, &column));
            return;
        }

        qWarning() << "Can't open file:" << configPath << f.errorString();
    }
}

QList<DynamicProperty *> DynamicPropertyConfig::parseAttributesList(
        const QString &fromData, QString *errorMsg, int *errorLine, int *errorColumn)
{
    QList<DynamicProperty *> attrs;
    QDomDocument doc;
    if (doc.setContent(fromData, false, errorMsg, errorLine, errorColumn)) {
        const QDomElement docElem = doc.documentElement();
        if (docElem.isNull())
            return {};

        QDomElement attributeElement = docElem.firstChildElement(DynamicProperty::tagName());
        while (!attributeElement.isNull()) {
            if (auto dynamicProperty = DynamicProperty::fromXml(attributeElement)) {
                attrs.append(dynamicProperty);
            }
            attributeElement = attributeElement.nextSiblingElement(DynamicProperty::tagName());
        }
    }
    return attrs;
}

QHash<QString, DynamicProperty *> DynamicPropertyConfig::attributesForObject(const aadl::AADLObject *obj)
{
    auto scope = DynamicProperty::Scope::None;
    QList<DynamicProperty *> properties;
    switch (obj->aadlType()) {
    case aadl::AADLObject::Type::FunctionType:
    case aadl::AADLObject::Type::Function:
        scope = DynamicProperty::Scope::Function;
        properties = attributesForFunction();
        break;
    case aadl::AADLObject::Type::RequiredInterface:
        scope = DynamicProperty::Scope::Required_Interface;
        properties = attributesForRequiredInterface();
        break;
    case aadl::AADLObject::Type::ProvidedInterface:
        scope = DynamicProperty::Scope::Provided_Interface;
        properties = attributesForProvidedInterface();
        break;
    default:
        return {};
    }
    QHash<QString, DynamicProperty *> result;
    auto validate = [obj, scope](DynamicProperty *property) {
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
    std::for_each(properties.constBegin(), properties.constEnd(), [&result, validate](DynamicProperty *property) {
        if (validate(property))
            result.insert(property->name(), property);
    });
    return result;
}

QList<DynamicProperty *> DynamicPropertyConfig::attributesForFunction()
{
    return d->m_function.values();
}

QList<DynamicProperty *> DynamicPropertyConfig::attributesForRequiredInterface()
{
    return d->m_reqIface.values();
}

QList<DynamicProperty *> DynamicPropertyConfig::attributesForProvidedInterface()
{
    return d->m_provIface.values();
}

QString DynamicPropertyConfig::configPath() const
{
    return d->m_configPath;
}

} // namespace aadl
