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

namespace shared {

typedef QUuid Id;

static const Id InvalidId = Id();

static const QString kStringDelemiter = QStringLiteral("::");

static const QString namePatternUI("^[a-zA-Z][\\w ]*(?(?<=_)[a-zA-Z0-9])$");

static const QString kNonCurrentImplementationPath { "implem" };

Id createId();

namespace env {
extern const char *kSharedTypesLibrary;
extern const char *kComponentsLibrary;
extern const char *kInterfaceAttrs;
extern const char *kDeploymentAttrs;
extern const char *kDeploymentLibrary;
}

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

/*!
   Enum describing the two ends of connections/messages
 */
enum MessageEnd
{
    SOURCE = 1, /// Same as "required" or "from"
    TARGET = 2, /// Same as "provided" or "to"
    BOTH = 3, /// Used to handle both ends
};

/*!
   \brief The FileCopyingMode enum
 */
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

/*!
 * Returns path to the HW library directory (containing the boards) set in TASTE_DEPLOYMENT_HW_PATH
 * environment variable otherwise points to default, for ex. ~/.local/share/ESA/DV Editor/HWlibrary
 */
QString hwLibraryPath();

/*!
    Returns ths set of words couldn't be used as entity name
 */
QSet<QString> forbiddenNamesSet();

/*!
   Returns is the given \p name is usable as name in general.
 */
bool isValidName(const QString &name);

}

Q_DECLARE_METATYPE(shared::Id)
