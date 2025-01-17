/*
    Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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
#include <QStandardItemModel>
#include <ivcomponentlibrary.h>
#include <ivobject.h>

namespace ive {

class IVComponentModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum Type
    {
        ComponentLibrary,
        SharedTypesLibrary,
    };

    explicit IVComponentModel(Type type, const QString &modelName, QObject *parent = nullptr);

    ivm::IVObject *getObject(const shared::Id &id);
    void removeComponent(const shared::Id &id);
    QString componentPath(const shared::Id &id);
    QStringList asn1Files(const shared::Id &id) const;
    QString libraryPath() const;
    void loadAvailableComponents();
    bool exportComponent(const QString &targetPath, const QList<ivm::IVObject *> objects, const QString &projectDir,
            QStringList asn1FilesPaths, QStringList externAsns, ivm::ArchetypeModel *archetypesModel);

    void unWatchComponentPath(const QString &componentPath);
    QSharedPointer<ivm::IVComponentLibrary::Component> component(const shared::Id &id) const;
    QList<shared::Id> componentIDs() const;

protected:
    QStandardItem *loadComponent(const QString &path);
    QStandardItem *itemFromComponent(QSharedPointer<ivm::IVComponentLibrary::Component> component);
    QStandardItem *itemById(const shared::Id &id);
    void reloadComponent(const shared::Id &id);
    void removeComponentFromModel(const shared::Id &id);

private:
    QStandardItem *processObject(ivm::IVObject *ivObject);

private:
    std::unique_ptr<ivm::IVComponentLibrary> m_compLibrary;
    Type m_type;
};

} // namespace ive
