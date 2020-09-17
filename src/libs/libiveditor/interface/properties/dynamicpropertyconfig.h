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

#pragma once

#include <QVector>
#include <memory>

namespace aadl {
class AADLObject;
}

namespace aadlinterface {

class DynamicProperty;

class DynamicPropertyConfig
{
public:
    static DynamicPropertyConfig *instance();
    static QList<DynamicProperty *> parseAttributesList(
            const QString &fromData, QString *errorMsg = nullptr, int *errorLine = nullptr, int *errorColumn = nullptr);
    void init();

    static QHash<QString, DynamicProperty *> attributesForObject(const aadl::AADLObject *obj);
    static QList<DynamicProperty *> attributesForFunction();
    static QList<DynamicProperty *> attributesForRequiredInterface();
    static QList<DynamicProperty *> attributesForProvidedInterface();

    static QString defaultConfigPath();
    static QString currentConfigPath();
    static QString resourceConfigPath();

private:
    static DynamicPropertyConfig *m_instance;

    DynamicPropertyConfig();

    QString configPath() const;

    struct DynamicPropertyConfigPrivate;
    std::unique_ptr<DynamicPropertyConfigPrivate> d;
};

} // namespace aadlinterface
