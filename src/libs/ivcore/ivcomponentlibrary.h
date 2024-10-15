/*
   Copyright (C) 2024 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QObject>
#include <common.h>
#include <ivmodel.h>

namespace ivm {

class IVObject;
class ArchetypeModel;

/*!
 * \brief The IVComponentLibrary class stores and manages all the ivcomponents (IVFunction and IVFunctionTypes)
 */
class IVComponentLibrary : public QObject
{
    Q_OBJECT
public:
    struct Component {
        QStringList asn1Files;
        QList<shared::Id> rootIds;
        QString componentPath;
        std::unique_ptr<IVModel> model;
    };

    IVComponentLibrary(const QString &path, const QString &modelName);
    /*!
     * \brief Returns the libraryPath
     */
    QString libraryPath() const;
    /*!
     * \brief exportComponent exports all the IVObjects that conforms a component including all intermediatefiles
     * (asn1Files,...) \param targetPath \param objects \param projectDir \param asn1FilesPaths \param externAsns \param
     * archetypesModel \return
     */
    bool exportComponent(const QString &targetPath, const QList<IVObject *> objects, const QString &projectDir,
            QStringList asn1FilesPaths, QStringList externAsns, ivm::ArchetypeModel *archetypesModel);
    static bool resetTasteENV(const QString &path);
    /*!
     * \brief removeComponent: Uses the id to obtain a pointer to the component
     *  With that pointer we obtain the path to remove the component in disk and
     *  the list of rootids to remove any other ids pointing to that component in the components structure.
     * \param id
     */
    void removeComponent(const shared::Id &id);
    /*!
     * \brief loadComponent tries to load the component taking into account that at least one of the objects in the
     * model is not of type ivm::IVObject::Type::InterfaceGroup
     * \param path to the interfaceview.xml file in the
     * components library
     * \return return a QSharePointer to a component
     */
    QSharedPointer<ivm::IVComponentLibrary::Component> loadComponent(const QString &path);
    /*!
     * \brief rootObjects: objects that doesn't have a parentObject
     * \param objects: list of IVObjects to be filtered
     * \return
     */
    QVector<IVObject *> rootObjects(QVector<IVObject *> objects);
    /*!
     * \brief component: get a component given an ID
     * \param id
     * \return QSharedPointer<ivm::IVComponentLibrary::Component>
     */
    QSharedPointer<ivm::IVComponentLibrary::Component> component(const shared::Id &id) const;
    /*!
     * \brief asn1Files: returns asn1Files associated with that component
     * \param id
     * \return
     */
    QStringList asn1Files(const shared::Id &id) const;
    /*!
     * \brief componentPath: returns the component's path given his Id
     * \param id
     * \return
     */
    QString componentPath(const shared::Id &id) const;

    QString modelName() const;

    void unWatchComponent(const QString &componentPath);

    QSharedPointer<ivm::IVComponentLibrary::Component> componentFromPath(const QString &path);
Q_SIGNALS:
    void componentUpdated(const shared::Id &id);
    void componentsToBeLoaded(QSet<QString> componentsPaths);
    void componentExported(const QString &filePath, bool ok);

private:
    bool anyLoadableIVObjects(QVector<ivm::IVObject *> objects);
    QList<shared::Id> rootIds(QVector<ivm::IVObject *> objects);

private:
    struct IVComponentLibraryPrivate;
    IVComponentLibraryPrivate *d;
    void addComponent(const QSharedPointer<Component> &component);
};
} // ivm
