/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QHash>
#include <QVariant>

class QIODevice;

namespace shared {
class VEObject;
}

namespace templating {

class ObjectsExporter : public QObject
{
    Q_OBJECT
public:
    enum class InteractionPolicy
    {
        Silently,
        Interactive
    };

    enum class RolloutDefaultsPolicy
    {
        Keep = 0,
        Overwrite
    };

    virtual ~ObjectsExporter() = default;

Q_SIGNALS:
    void exported(const QString &filePath, bool ok);

protected:
    ObjectsExporter(QObject *parent = nullptr);
    static QString templatesPath();
    static QString templateFileExtension();

    /**
     * @brief ObjectsExporter::createFrom creates appropriate exported class and casts to QVariant
     * @param VEObject exported object
     * @return created exported object as QVariant
     */
    virtual QVariant createFrom(const shared::VEObject *object) const = 0;

    /**
     * @brief groupName is a logical name of a group of objects which given object belongs to.
     * Exportable objects are grouped by the group name and kept in QVariantList.
     * Then the group name is used in the string template to retrieve all grouped objects.
     * @return name of the group
     */
    virtual QString groupName(const shared::VEObject *object) const = 0;

    template<typename T>
    QHash<QString, QVariant> collectObjects(const QList<T *> &objects) const
    {
        QHash<QString, QVariant> grouppedObjects;

        for (const auto &object : objects) {
            if (object->parentObject() != nullptr)
                continue;

            const QVariant &exportedObject = createFrom(object);
            const QString objectGroupName = groupName(object);
            if (!grouppedObjects.contains(objectGroupName)) {
                grouppedObjects[objectGroupName] = QVariant::fromValue(QList<QVariant>());
            }
            QVariantList objects = grouppedObjects[objectGroupName].toList();
            objects << exportedObject;
            grouppedObjects[objectGroupName] = objects;
        }
        return grouppedObjects;
    }

    bool showExportDialog(const QHash<QString, QVariant> &content, const QString &templateFileName,
            const QString &outFileName, QWidget *parentWindow = nullptr);

    void ensureDefaultTemplatesDeployed(
            const QString &defaultsPath, RolloutDefaultsPolicy policy = RolloutDefaultsPolicy::Overwrite);
    virtual QString defaultTemplatePath() const = 0;

    bool exportData(const QHash<QString, QVariant> &data, const QString &templatePath, QIODevice *outDevice);
    bool exportData(const QHash<QString, QVariant> &data, const QString &outPath, const QString &templatePath,
            InteractionPolicy interaction, QWidget *root = nullptr);
};

} // namespace templating
