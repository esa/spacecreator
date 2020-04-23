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

#include "app/common.h"
#include "baseitems/common/utils.h"
#include "dynamicproperty.h"
#include "settings/appoptions.h"
#include "tab_aadl/aadlobject.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

namespace taste3 {
namespace aadl {

struct DynamicPropertyConfigPrivate {
    DynamicPropertyConfigPrivate() {}
    ~DynamicPropertyConfigPrivate() {}
    void init(const QVector<DynamicProperty *> &attrs)
    {
        QHash<DynamicProperty *, DynamicProperty *> uniqeAttrs;
        auto collectUniqeAttributes = [](const QHash<QString, DynamicProperty *> &attrs,
                                         QHash<DynamicProperty *, DynamicProperty *> &storage) {
            for (auto attr : attrs)
                if (!storage.contains(attr))
                    storage.insert(attr, attr);
        };

        collectUniqeAttributes(m_functionType, uniqeAttrs);
        collectUniqeAttributes(m_function, uniqeAttrs);
        collectUniqeAttributes(m_iface, uniqeAttrs);

        qDeleteAll(uniqeAttrs);
        m_functionType.clear();
        m_function.clear();
        m_iface.clear();

        for (DynamicProperty *attr : attrs) {
            if (attr->scope() == DynamicProperty::Scopes(DynamicProperty::Scope::None))
                continue;

            if (attr->scope().testFlag(DynamicProperty::Scope::FunctionType))
                m_functionType.insert(attr->name(), attr);

            if (attr->scope().testFlag(DynamicProperty::Scope::Function))
                m_function.insert(attr->name(), attr);

            if (attr->scope().testFlag(DynamicProperty::Scope::Interface))
                m_iface.insert(attr->name(), attr);
        }
    }

    QHash<QString, DynamicProperty *> m_functionType;
    QHash<QString, DynamicProperty *> m_function;
    QHash<QString, DynamicProperty *> m_iface;
};

DynamicPropertyConfig *DynamicPropertyConfig::m_instance = nullptr;

DynamicPropertyConfig::DynamicPropertyConfig()
    : d(new DynamicPropertyConfigPrivate())
{
}

QString DynamicPropertyConfig::defaultConfigPath()
{
    const QString &path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    common::ensureDirExists(path);
    return path + "/aadl_properties.json";
}

QString DynamicPropertyConfig::currentConfigPath()
{
    return instance()->configPath();
}

QString DynamicPropertyConfig::configPath() const
{
    QString storedFilePath = AppOptions::Aadl.CustomPropertiesConfig.read().toString();
    if (storedFilePath.isEmpty()) {
        storedFilePath = defaultConfigPath();
        AppOptions::Aadl.CustomPropertiesConfig.write(storedFilePath);
    }
    return storedFilePath;
}

DynamicPropertyConfig *DynamicPropertyConfig::instance()
{
    if (!m_instance) {
        m_instance = new DynamicPropertyConfig();
        m_instance->init();
    }

    return m_instance;
}

QString ensureFileExists()
{
    QString storedFilePath(AppOptions::Aadl.CustomPropertiesConfig.read().toString());
    if (storedFilePath.isEmpty() || !QFileInfo::exists(storedFilePath)) {
        if (storedFilePath.isEmpty())
            storedFilePath = DynamicPropertyConfig::defaultConfigPath();

        const QString rscFilePath(":/defaults/tab_interface/properties/resources/aadl_properties.json");
        if (!common::copyResourceFile(rscFilePath, storedFilePath)) {
            qWarning() << "Can't create default ASN datatypes file" << storedFilePath;
            return QString();
        }
    }
    return storedFilePath;
}

void DynamicPropertyConfig::init()
{
    const QString &filePath = ensureFileExists();
    if (!filePath.isEmpty()) {
        QFile f(filePath);
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            d->init(readAttrs(f.readAll()));
            return;
        }

        qWarning() << "Can't open file:" << filePath << f.errorString();
    }
}

void DynamicPropertyConfig::generateSampleFile()
{
    QVector<DynamicProperty *> attrs;
    attrs << new DynamicProperty("CustomIntForAll", DynamicProperty::Type::Integer,
                                 DynamicProperty::Scope::FunctionType | DynamicProperty::Scope::Function
                                         | DynamicProperty::Scope::Interface);
    attrs << new DynamicProperty("CustomRealForTwo", DynamicProperty::Type::Real,
                                 DynamicProperty::Scope::FunctionType | DynamicProperty::Scope::Function);
    attrs << new DynamicProperty("CustomBoolForOne", DynamicProperty::Type::Boolean, DynamicProperty::Scope::Interface);
    attrs << new DynamicProperty("CustomString", DynamicProperty::Type::String, DynamicProperty::Scope::Interface);
    attrs << new DynamicProperty("CustomStringList", DynamicProperty::Type::String, DynamicProperty::Scope::Interface,
                                 { "./file.a", "../file.b", "/file.c" });
    attrs << new DynamicProperty("CustomIntList", DynamicProperty::Type::Integer, DynamicProperty::Scope::Interface,
                                 { -1, 0, 1 });
    attrs << new DynamicProperty("CustomRealList", DynamicProperty::Type::Integer, DynamicProperty::Scope::Interface,
                                 { -1.5, 0., 1.5 });
    attrs << new DynamicProperty("CustomEnum", DynamicProperty::Type::Enumeration, DynamicProperty::Scope::Interface,
                                 { "First", "Next", "Last" });

    QJsonArray attrsHolder;
    for (auto attr : attrs)
        attrsHolder.append(attr->toJson());

    QFile tmp("./aadl_properties.json");
    tmp.open(QIODevice::WriteOnly);
    QJsonDocument doc(attrsHolder);
    const QByteArray &j = doc.toJson();
    tmp.write(j);
}

QVector<DynamicProperty *> DynamicPropertyConfig::parseAttributesList(const QByteArray &fromData)
{
    QVector<DynamicProperty *> attrs;
    QJsonParseError errorHandler;
    QJsonDocument doc = QJsonDocument::fromJson(fromData, &errorHandler);
    if (errorHandler.error != QJsonParseError::NoError) {
        qWarning() << errorHandler.errorString();
        return attrs;
    }

    QJsonArray jsonRoot = doc.array();
    if (jsonRoot.isEmpty()) {
        qWarning() << "No configured properties were found";
        return attrs;
    }

    for (int i = 0; i < jsonRoot.size(); ++i)
        if (DynamicProperty *attr = DynamicProperty::fromJson(jsonRoot.at(i).toObject()))
            attrs << attr;

    return attrs;
}

QVector<DynamicProperty *> DynamicPropertyConfig::readAttrs(const QByteArray &data) const
{
    const QVector<DynamicProperty *> &attrs = parseAttributesList(data);
    if (attrs.isEmpty())
        qWarning() << "It seems no custom properties has been defined";

    return attrs;
}

QVector<DynamicProperty *> DynamicPropertyConfig::attributesForObject(AADLObject *obj)
{
    switch (obj->aadlType()) {
    case AADLObject::Type::FunctionType:
        return attributesForFunctionType();
    case AADLObject::Type::Function:
        return attributesForFunction();
    case AADLObject::Type::RequiredInterface:
    case AADLObject::Type::ProvidedInterface:
        return attributesForIface();
    default:
        return {};
    }
}

QVector<DynamicProperty *> DynamicPropertyConfig::attributesForFunctionType()
{
    return instance()->d->m_functionType.values().toVector();
}

QVector<DynamicProperty *> DynamicPropertyConfig::attributesForFunction()
{
    return instance()->d->m_function.values().toVector();
}

QVector<DynamicProperty *> DynamicPropertyConfig::attributesForIface()
{
    return instance()->d->m_iface.values().toVector();
}

} // ns aadl
} // ns taste3
