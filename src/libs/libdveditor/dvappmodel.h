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

#include <QObject>
#include <memory>

class QUndoStack;

namespace dvm {
class DVModel;
}

namespace shared {
namespace cmd {
class CommandsStackBase;
}
}

namespace dve {

/*!
   Main Class of the deployment view editor managing all data
 */
class DVAppModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY titleChanged)

public:
    explicit DVAppModel(QObject *parent = nullptr);

    QUndoStack *undoStack() const;
    shared::cmd::CommandsStackBase *commandsStack() const;

    bool load(const QString &path);
    void close();

    QString path() const;
    void setPath(const QString &path);

    dvm::DVModel *objectsModel() const;

    bool isDirty() const;

    QString supportedFileExtensions() const;

Q_SIGNALS:
    void dirtyChanged(bool dirty);
    void titleChanged();

private:
    struct DVAppModelPrivate;
    DVAppModelPrivate *d;
};

} // namespace dve
