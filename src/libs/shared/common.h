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

#include <QMetaEnum>
#include <QMetaType>
#include <QMimeData>
#include <QObject>
#include <QUuid>

namespace Asn1Acn {
class File;
}

namespace shared {

typedef QUuid Id;

static const Id InvalidId = Id();

static const QString kStringDelemiter = QStringLiteral("::");

Id createId();

class DropData : public QMimeData
{
    Q_OBJECT
public:
    enum class Type
    {
        None,
        ImportableType,
        InstantiatableType,
    };
    Q_ENUM(Type);

    Type dropType = { Type::ImportableType };
    shared::Id entityId;
};

enum FileCopyingMode
{
    Keep,
    Overwrite
};

void copyDir(const QString &source, const QString &dest, FileCopyingMode replaceMode = FileCopyingMode::Keep);

bool copyFile(const QString &source, const QString &target, FileCopyingMode replaceMode = FileCopyingMode::Keep);

void setWidgetFontColor(QWidget *widget, const QColor &color);

bool ensureDirExists(const QString &path);

bool ensureFileExists(const QString &filePath, const QString &defaultFilePath);

template<typename T>
bool isAncestorOf(const T *const parent, T *object)
{
    if (!parent || !object)
        return false;

    if (parent == object)
        return true;

    T *obj { object };
    while (auto objParent = qobject_cast<T *>(obj->parent())) {
        if (objParent == parent)
            return true;

        obj = objParent;
    }

    return false;
}

template<typename E>
QString typeName(const E &e)
{
    static const QMetaEnum &me = QMetaEnum::fromType<E>();
    return QString::fromLatin1(me.valueToKey(static_cast<int>(e))).section(kStringDelemiter, -1);
}

template<typename E>
E typeFromName(const QString &name)
{
    static const QMetaEnum &me = QMetaEnum::fromType<E>();
    return static_cast<E>(me.keyToValue(name.toLatin1()));
}

//! Default filename for Interface View diagram
static const QString kDefaultInterfaceViewFileName { QLatin1String("interfaceview.xml") };

/*!
 * Returns path to exported components library set in TASTE_COMPONENTS_LIBRARY
 * environment variable otherwise points to default, for ex. ~/.local/share/ESA/IV Editor/components_library
 */
QString componentsLibraryPath();

/*!
 * Returns path to shared function types set in TASTE_SHARED_TYPES
 * environment variable otherwise points to default, for ex. ~/.local/share/ESA/IV Editor/shared_types
 */
QString sharedTypesPath();

/*!
 * Returns path to user defined attributes set in TASTE_DEFAULT_ATTRIBUTES_PATH
 * environment variable otherwise points to default, for ex. ~/.local/share/ESA/IV Editor/default_attributes.xml
 */
QString interfaceCustomAttributesFilePath();

/*!
 * Returns path to user defined attributes set in TASTE_DEPLOYMENT_ATTRIBUTES_PATH
 * environment variable otherwise points to default, for ex. ~/.local/share/ESA/DV Editor/default_attributes.xml
 */
QString deploymentCustomAttributesFilePath();

QStringList asn1Names(const Asn1Acn::File *dataTypes);

}

Q_DECLARE_METATYPE(shared::Id)
