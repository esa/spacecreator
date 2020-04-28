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

#include <QStringList>
#include <QVector>
#include <memory>

namespace aadl {
class AADLObject;
class AADLObjectTemplate;
class AADLObjectFunction;
class AADLObjectInterface;
}

namespace aadlinterface {

class DynamicProperty;

class DynamicPropertyConfig
{
public:
    static DynamicPropertyConfig *instance();
    static QVector<DynamicProperty *> parseAttributesList(const QByteArray &fromData);
    void init();

    static QVector<DynamicProperty *> attributesForObject(aadl::AADLObject *obj);
    static QVector<DynamicProperty *> attributesForFunctionType();
    static QVector<DynamicProperty *> attributesForFunction();
    static QVector<DynamicProperty *> attributesForIface();

    static QString defaultConfigPath();
    static QString currentConfigPath();

private:
    static DynamicPropertyConfig *m_instance;

    DynamicPropertyConfig();

    static void generateSampleFile();
    QString configPath() const;
    QVector<DynamicProperty *> readAttrs(const QByteArray &data) const;

    struct DynamicPropertyConfigPrivate;
    std::unique_ptr<DynamicPropertyConfigPrivate> d;
};

}
