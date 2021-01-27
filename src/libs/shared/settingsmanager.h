/*
   Copyright (C) 2018-2021 European Space Agency - <maxime.perrotin@esa.int>

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
#include <QObject>
#include <QSettings>

namespace shared {

class SettingsManager : public QObject
{
    Q_OBJECT

    SettingsManager();

public:
    enum class Common
    {
        Geometry,
        State,
        ColorSchemeFile,
        ShowMinimap,
    };
    Q_ENUM(Common)

    enum class MSC
    {
        DocViewMode,
        RecentFiles,
    };
    Q_ENUM(MSC)

    enum class IVE
    {
        RecentFiles,
    };
    Q_ENUM(IVE)

    enum class BugReport
    {
        Host,
        ProjectID,
        AccessToken,
    };
    Q_ENUM(BugReport)

    QSettings *storage();
    static SettingsManager *instance();

    /*!
     * \brief perform storing \a value with key \a type. If the key already exists, the previous value is
     * overwritten. Internal QSettings is synced after setting the \a value
     * \a type - enum key, name of the enum is used as group name
     * \a value - value wrapped into QVariant. Therefore type of value should be declared for Qt meta type system.
     * \code{.cpp}
     *    store<QString>(Common::ColorSchemeFile, "~/.local/share/ESA/IV_Editor/colors/default_colors.json");
     * \endcode
     */
    template<typename V, typename T, typename std::enable_if<std::is_enum<T>::value, void *>::type = nullptr>
    static void store(const T &type, const V &value)
    {
        const QMetaEnum typeMetaEnum = QMetaEnum::fromType<T>();
        const QString key = QString::fromLatin1(typeMetaEnum.name()) + QLatin1Char('/')
                + QString::fromLatin1(typeMetaEnum.valueToKey(static_cast<int>(type)));
        instance()->storage()->setValue(key, QVariant::fromValue<V>(value));
        instance()->storage()->sync();
    }

    /*!
     * \brief perform loading \a value according to key \a type. If the key doesn't exist, the \a defaultValue is used
     * \a type - enum key
     * \a defaultValue - value wrapped into QVariant. Therefore type of value should be declared for Qt meta type system
     * and convertible to type V.
     * \code{.cpp}
     *    load<QString>(Common::ColorSchemeFile, "~/.local/share/ESA/IV_Editor/colors/default_colors.json");
     * \endcode
     */
    template<typename V, typename T, typename std::enable_if<std::is_enum<T>::value, void *>::type = nullptr>
    static V load(const T &type, const V &defaultValue = V())
    {
        const QMetaEnum typeMetaEnum = QMetaEnum::fromType<T>();
        const QString key = QString::fromLatin1(typeMetaEnum.name()) + QLatin1Char('/')
                + QString::fromLatin1(typeMetaEnum.valueToKey(static_cast<int>(type)));
        const QVariant value = instance()->storage()->value(key);
        return value.isValid() ? value.value<V>() : defaultValue;
    }

private:
    QSettings *m_settings = nullptr;
};

}
