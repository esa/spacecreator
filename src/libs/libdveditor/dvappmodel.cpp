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

#include "dvappmodel.h"

#include "abstractvisualizationmodel.h"
#include "commandsstackbase.h"
#include "dvmodel.h"
#include "dvxmlreader.h"
#include "errorhub.h"

#include <QDebug>
#include <QFileInfo>

namespace dve {

struct DVAppModel::DVAppModelPrivate {
    DVAppModelPrivate()
        : commandsStack(new shared::cmd::CommandsStackBase)
        , objectsModel(new dvm::DVModel)
    {
    }

    std::unique_ptr<shared::cmd::CommandsStackBase> commandsStack;
    QString filePath;
    std::unique_ptr<dvm::DVModel> objectsModel;
};

DVAppModel::DVAppModel(QObject *parent)
    : QObject(parent)
    , d(new DVAppModelPrivate)
{
    connect(d->commandsStack.get(), &shared::cmd::CommandsStackBase::cleanChanged, this,
            [this](bool clean) { Q_EMIT dirtyChanged(!clean); });
}

QUndoStack *DVAppModel::undoStack() const
{
    Q_ASSERT(d->commandsStack);
    return d->commandsStack->undoStack();
}

shared::cmd::CommandsStackBase *DVAppModel::commandsStack() const
{
    return d->commandsStack.get();
}

/*!
   Loads the deployment view data from file \p path
 */
bool DVAppModel::load(const QString &path)
{
    if (path.isEmpty() || !QFileInfo::exists(path)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Invalid path"), path);
        return false;
    }

    if (!d->filePath.isEmpty()) {
        shared::ErrorHub::clearFileErrors(d->filePath);
    }
    const QString oldPath = d->filePath = path;
    setPath(path);

    shared::ErrorHub::setCurrentFile(path);
    dvm::DVXMLReader reader;
    if (!reader.readFile(path)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, reader.errorString(), path);
        setPath(oldPath);
        shared::ErrorHub::clearCurrentFile();
        return false;
    }

    d->objectsModel->initFromObjects(reader.parsedObjects());

    shared::ErrorHub::clearCurrentFile();
    return true;
}

void DVAppModel::close()
{
    d->objectsModel->clear();
    setPath(QString());
    d->commandsStack->clear();
}

QString DVAppModel::path() const
{
    return d->filePath;
}

void DVAppModel::setPath(const QString &path)
{
    if (path != d->filePath) {
        d->filePath = path;
        Q_EMIT titleChanged();
    }
}

/*!
   Returns the data of the deployment view
 */
dvm::DVModel *DVAppModel::objectsModel() const
{
    return d->objectsModel.get();
}

bool DVAppModel::isDirty() const
{
    return d->commandsStack && !d->commandsStack->isClean();
}

QString DVAppModel::supportedFileExtensions() const
{
    return QStringLiteral("*.dv.xml");
}

} // namespace dve
