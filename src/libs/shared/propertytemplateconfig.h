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

#pragma once

#include <QVector>
#include <memory>

class QDomElement;

namespace shared {
class VEObject;
class PropertyTemplate;

class PropertyTemplateConfig
{
public:
    virtual ~PropertyTemplateConfig();

    bool hasPropertyTemplateForObject(const VEObject *obj, const QString &name) const;
    PropertyTemplate *propertyTemplateForObject(const VEObject *obj, const QString &name) const;
    QList<PropertyTemplate *> propertyTemplatesForObject(const VEObject *obj) const;

    QString configPath() const;

    QList<PropertyTemplate *> parseAttributesList(const QString &fromData, QString *errorMsg = nullptr,
            int *errorLine = nullptr, int *errorColumn = nullptr) const;
    QList<PropertyTemplate *> systemAttributes() const;
    void init(const QString &configPath);

    virtual QString title() const = 0;
    virtual PropertyTemplate *createPropertyTemplate() const = 0;

protected:
    PropertyTemplateConfig(const QString &sysConfigPath);
    virtual PropertyTemplate *createPropertyTemplate(const QDomElement &element) const = 0;
    virtual QString userAttrsResourceConfigPath() const = 0;

private:
    struct PropertyTemplateConfigPrivate;
    std::unique_ptr<PropertyTemplateConfigPrivate> d;
};

} // namespace shared
