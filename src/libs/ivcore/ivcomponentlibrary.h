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
#include <vemodel.h>

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
        std::unique_ptr<shared::VEModel> model;
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

private:
    struct IVComponentLibraryPrivate;
    IVComponentLibraryPrivate *d;
};
} // ivm
