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

#include "propertytemplate.h"

#include "veobject.h"

#include <QRegularExpressionMatch>

namespace shared {

const QString PropertyTemplate::kAttributeTagName = QLatin1String("Attr");
const QString PropertyTemplate::kScopesTagName = QLatin1String("Scopes");

struct PropertyTemplate::PropertyTemplatePrivate {
    QString m_name;
    QString m_label;
    PropertyTemplate::Info m_info;
    PropertyTemplate::Type m_type;
    QString m_exportGroupName;
    QVariant m_value;
    QVariant m_defaultValue;
    QString m_rxValueValidatorPattern;
    QMultiMap<int, QPair<QString, QString>> m_rxAttrValidatorPattern;
    int m_scopes = 0;
    bool m_isVisible = true;
    bool m_isEditable = true;
    bool m_isSystem = false;
    bool m_isOptional = false;
    QMap<QString, QMap<QString, QString>> m_enumData;
};

PropertyTemplate::PropertyTemplate()
    : d(new PropertyTemplatePrivate)
{
}

PropertyTemplate::~PropertyTemplate() { }

QString PropertyTemplate::name() const
{
    return d->m_name;
}

void PropertyTemplate::setName(const QString &name)
{
    d->m_name = name;
}

QString PropertyTemplate::label() const
{
    return d->m_label;
}

void PropertyTemplate::setLabel(const QString &label)
{
    d->m_label = label;
}

PropertyTemplate::Info PropertyTemplate::info() const
{
    return d->m_info;
}

void PropertyTemplate::setInfo(PropertyTemplate::Info info)
{
    d->m_info = info;
}

bool PropertyTemplate::isSystem() const
{
    return d->m_isSystem;
}

void PropertyTemplate::setSystem(bool value)
{
    d->m_isSystem = value;
}

PropertyTemplate::Type PropertyTemplate::type() const
{
    return d->m_type;
}

void PropertyTemplate::setType(PropertyTemplate::Type t)
{
    d->m_type = t;
}

int PropertyTemplate::scopes() const
{
    return d->m_scopes;
}

void PropertyTemplate::setScopes(int s)
{
    d->m_scopes = s;
}

bool PropertyTemplate::isVisible() const
{
    return d->m_isVisible;
}

void PropertyTemplate::setVisible(bool value)
{
    d->m_isVisible = value;
}

bool PropertyTemplate::isEditable() const
{
    return d->m_isEditable;
}

void PropertyTemplate::setEditable(bool value)
{
    d->m_isEditable = value;
}

bool PropertyTemplate::isOptional() const
{
    return d->m_isOptional;
}

void PropertyTemplate::setOptional(bool value)
{
    d->m_isOptional = value;
}

QString PropertyTemplate::exportGroupName() const
{
    return d->m_exportGroupName;
}

void PropertyTemplate::setExportGroupName(const QString &expGroupName)
{
    d->m_exportGroupName = expGroupName;
}

QVariant PropertyTemplate::value() const
{
    return d->m_value;
}

void PropertyTemplate::setValue(const QVariant &value)
{
    d->m_value = value;
}

QVariant PropertyTemplate::defaultValue() const
{
    return d->m_defaultValue;
}

void PropertyTemplate::setDefaultValue(const QVariant &value)
{
    d->m_defaultValue = value;
}

QString PropertyTemplate::valueValidatorPattern() const
{
    return d->m_rxValueValidatorPattern;
}

void PropertyTemplate::setValueValidatorPattern(const QString &pattern)
{
    d->m_rxValueValidatorPattern = pattern;
}

QMultiMap<int, QPair<QString, QString>> PropertyTemplate::attrValidatorPatterns() const
{
    return d->m_rxAttrValidatorPattern;
}

void PropertyTemplate::setAttrValidatorPattern(const QMultiMap<int, QPair<QString, QString>> &pattern)
{
    d->m_rxAttrValidatorPattern = pattern;
}

QMap<QString, QString> PropertyTemplate::enumData(const QString &enumValue) const
{
    if (!d->m_enumData.contains(enumValue)) {
        return {};
    }
    return d->m_enumData.value(enumValue);
}

QDomElement PropertyTemplate::toXml(QDomDocument *doc) const
{
    QDomElement attrElement = doc->createElement(kAttributeTagName);
    attrElement.setAttribute(QLatin1String("name"), d->m_name);
    if (!d->m_label.isEmpty()) {
        attrElement.setAttribute(QLatin1String("label"), d->m_label);
    }
    if (!d->m_isVisible) {
        attrElement.setAttribute(QLatin1String("visible"), QLatin1String("false"));
    }
    if (d->m_isOptional) {
        attrElement.setAttribute(QLatin1String("optional"), QLatin1String("true"));
    }

    static const QMetaEnum &infoMeta = QMetaEnum::fromType<PropertyTemplate::Info>();
    if (d->m_info == PropertyTemplate::Info::Property) {
        const QString infoStr = QString::fromLatin1(infoMeta.valueToKey(static_cast<int>(d->m_info)));
        attrElement.setAttribute(QLatin1String("type"), infoStr.toLower());
    }

    static const QMetaEnum &typeMeta = QMetaEnum::fromType<PropertyTemplate::Type>();
    QDomElement typeElement = doc->createElement(typeMeta.name());
    const QString typeString = typeMeta.valueToKey(static_cast<int>(d->m_type));
    QDomElement typeSubElement = doc->createElement(typeString);
    if (!d->m_rxValueValidatorPattern.isEmpty()) {
        typeSubElement.setAttribute(QLatin1String("validator"), d->m_rxValueValidatorPattern);
    }
    if (d->m_type == PropertyTemplate::Type::Enumeration) {
        for (const auto &entry : d->m_value.toList()) {
            QDomElement entryElement = doc->createElement(QLatin1String("Entry"));
            entryElement.setAttribute(QLatin1String("value"), entry.toString());
            typeSubElement.appendChild(entryElement);
        }
    }
    typeElement.appendChild(typeSubElement);
    attrElement.appendChild(typeElement);

    QDomElement scopeElement = doc->createElement(kScopesTagName);
    auto scopesToStrings = [scopeMeta = scopeMetaEnum()](int s) -> QStringList {
        QStringList result;
        for (int idx = 0; idx < scopeMeta.keyCount(); ++idx) {
            if (s == 0 || s & scopeMeta.value(idx)) {
                result.append(QString::fromLatin1(scopeMeta.key(idx)));
            }
        }
        return result;
    };

    for (const QString &scope : scopesToStrings(d->m_scopes)) {
        QDomElement scopeSubElement = doc->createElement(scope);
        for (auto it = d->m_rxAttrValidatorPattern.constBegin(); it != d->m_rxAttrValidatorPattern.constEnd(); ++it) {
            QDomElement attrValidatorElement = doc->createElement(QLatin1String("AttrValidator"));
            attrValidatorElement.setAttribute(QLatin1String("name"), it.value().first);
            attrValidatorElement.setAttribute(QLatin1String("value"), it.value().second);
            scopeSubElement.appendChild(attrValidatorElement);
        }
        scopeElement.appendChild(scopeSubElement);
    }
    attrElement.appendChild(scopeElement);

    return attrElement;
}

void PropertyTemplate::initFromXml(const QDomElement &element)
{
    bool ok;

    const QString attrName = element.attribute(QLatin1String("name"));
    const QString attrLabel = element.attribute(QLatin1String("label"));
    const QString attrType = element.attribute(QLatin1String("type"), QLatin1String("Attribute"));
    const QString expGroupName = element.attribute(QLatin1String("export") /*, QLatin1String("model")*/);
    const bool isEditable =
            QString::compare(element.attribute(QLatin1String("editable")), QLatin1String("false"), Qt::CaseInsensitive)
            != 0;
    const bool isVisible =
            QString::compare(element.attribute(QLatin1String("visible")), QLatin1String("false"), Qt::CaseInsensitive)
            != 0;
    const bool isOptional =
            QString::compare(element.attribute(QLatin1String("optional")), QLatin1String("true"), Qt::CaseInsensitive)
            == 0;
    static const QMetaEnum &infoMeta = QMetaEnum::fromType<PropertyTemplate::Info>();
    const int infoInt = infoMeta.keyToValue(attrType.toUtf8().data(), &ok);
    const PropertyTemplate::Info i =
            ok ? static_cast<PropertyTemplate::Info>(infoInt) : PropertyTemplate::Info::Property;

    static const QMetaEnum &typeMeta = QMetaEnum::fromType<PropertyTemplate::Type>();
    const QDomElement typeElement = element.firstChildElement(typeMeta.name());
    PropertyTemplate::Type t = PropertyTemplate::Type::Unknown;

    QVariant defaultValue;
    QVariant value;
    QString typeValidator;
    if (!typeElement.isNull()) {
        const QDomElement typeSubElement = typeElement.firstChildElement();
        if (!typeSubElement.isNull()) {
            typeValidator = typeSubElement.attribute(QLatin1String("validator"));
            const int typeInt = typeMeta.keyToValue(typeSubElement.tagName().toUtf8().data(), &ok);
            if (ok && typeInt != -1) {
                t = static_cast<PropertyTemplate::Type>(typeInt);
            }
            if (t == PropertyTemplate::Type::Enumeration) {
                defaultValue = typeSubElement.attribute(QLatin1String("defaultValue"));
                QDomElement typeEntryElement = typeSubElement.firstChildElement(QLatin1String("Entry"));
                QList<QVariant> values;
                while (!typeEntryElement.isNull()) {
                    const QVariant enumValue = typeEntryElement.attribute(QLatin1String("value"));
                    values.append(enumValue);
                    addEnumData(typeEntryElement);
                    typeEntryElement = typeEntryElement.nextSiblingElement(typeEntryElement.tagName());
                }
                value = values;
            } else {
                defaultValue = convertData(typeSubElement.attribute(QLatin1String("defaultValue")), t);
            }
        }
    }

    const QDomElement scopeElement = element.firstChildElement(kScopesTagName);
    const QMetaEnum scopeMeta = scopeMetaEnum();
    int s = 0;
    QMultiMap<int, QPair<QString, QString>> attrValidators;
    if (!scopeElement.isNull() && scopeElement.hasChildNodes()) {
        QDomElement scopeSubElement = scopeElement.firstChildElement();
        while (!scopeSubElement.isNull()) {
            const QString scopeName = scopeSubElement.tagName();
            const int scopeInt = scopeMeta.keyToValue(scopeName.toUtf8().data(), &ok);
            if (ok && scopeInt != -1) {
                s |= scopeInt;
                QDomElement attrValidatorElement = scopeSubElement.firstChildElement(QLatin1String("AttrValidator"));
                while (!attrValidatorElement.isNull()) {
                    const QString attrValidatorName = attrValidatorElement.attribute(QLatin1String("name"));
                    const QString attrValidatorPattern = attrValidatorElement.attribute(QLatin1String("value"));
                    if (!attrValidatorName.isEmpty() && !attrValidatorPattern.isNull()) {
                        attrValidators.insert(scopeInt, qMakePair(attrValidatorName, attrValidatorPattern));
                    }
                    attrValidatorElement = attrValidatorElement.nextSiblingElement(attrValidatorElement.tagName());
                }
            }
            scopeSubElement = scopeSubElement.nextSiblingElement();
        }
    }

    setName(attrName);
    setLabel(attrLabel);
    setInfo(i);
    setType(t);
    setScopes(s);
    setValue(value);
    setDefaultValue(defaultValue);
    setAttrValidatorPattern(attrValidators);
    setValueValidatorPattern(typeValidator);
    setVisible(isVisible);
    setEditable(isEditable);
    setOptional(isOptional);
    setExportGroupName(expGroupName);
}

void PropertyTemplate::addEnumData(const QDomElement &typeEntryElement)
{
    const QString valueKey { "value" };
    const QString enumValue = typeEntryElement.attribute(valueKey);
    QMap<QString, QString> data;
    QDomNamedNodeMap attributes = typeEntryElement.attributes();
    for (int i = 0; i < attributes.count(); ++i) {
        QDomAttr attr = attributes.item(i).toAttr();
        if (attr.name() != valueKey) {
            data[attr.name()] = attr.value();
        }
    }

    d->m_enumData[enumValue] = data;
};

QVariant PropertyTemplate::convertData(const QVariant &value, PropertyTemplate::Type type)
{
    QVariant typedValue;
    switch (type) {
    case PropertyTemplate::Type::Boolean: {
        const bool falseValue = QString::compare(value.toString(), QLatin1String("false"), Qt::CaseInsensitive) == 0;
        const bool trueValue = QString::compare(value.toString(), QLatin1String("true"), Qt::CaseInsensitive) == 0;
        if (falseValue) {
            typedValue = false;
        } else if (trueValue) {
            typedValue = true;
        }
    } break;
    case PropertyTemplate::Type::Integer: {
        bool ok;
        typedValue = value.toString().toInt(&ok);
        if (!ok)
            return QVariant();
    } break;
    case PropertyTemplate::Type::Real: {
        bool ok;
        typedValue = value.toString().toDouble(&ok);
        if (!ok)
            return QVariant();
    } break;
    case PropertyTemplate::Type::String: {
        if (value.isValid()) {
            QString stringValue = value.toString();
            if (!stringValue.isEmpty()) {
                typedValue = stringValue;
            }
        }
    } break;
    case PropertyTemplate::Type::Enumeration: {
        if (value.isValid()) {
            QStringList typedList;
            for (const auto &dataItem : value.toList()) {
                typedList.append(dataItem.toString());
            }
            if (!typedList.isEmpty()) {
                typedValue = typedList;
            }
        }
    } break;
    default:
        break;
    }
    return typedValue;
}

/*!
 * \brief PropertyTemplate::validate
 * \param object which attributes to be checked
 * \return
 */
bool PropertyTemplate::validate(const VEObject *object) const
{
    const int objScope = objectScope(object);
    if (d->m_scopes != 0 && !(d->m_scopes & objScope)) {
        return false;
    }

    if (d->m_rxAttrValidatorPattern.isEmpty()) {
        return true;
    }

    auto it = d->m_rxAttrValidatorPattern.lowerBound(objScope);
    const auto upperBound = d->m_rxAttrValidatorPattern.upperBound(objScope);
    while (it != upperBound) {
        auto checkPattern = [](const EntityAttributes &data, const QString &name, const QString &pattern) {
            auto objAttrIter = data.constFind(name);
            if (objAttrIter != data.constEnd()) {
                const QRegularExpression rx(pattern);
                const QString value = objAttrIter.value().value<QString>();
                const QRegularExpressionMatch match = rx.match(value);
                return match.capturedLength() == value.length();
            }
            return true;
        };
        if (checkPattern(object->entityAttributes(), it->first, it->second))
            return true;
        ++it;
    }
    return false;
}

} // namespace shared
