/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "common.h"

#include <QList>
#include <QPointer>
#include <QSharedPointer>
#include <QVector>

namespace ivm {
class IVObject;
class IVConnection;
class IVFunction;
}

namespace ive {
class IVEditorCore;
}

namespace msc {
class MSCEditorCore;
class MscMessage;
}

namespace shared {
class UndoCommand;
class VEObject;
}

namespace scs {
class SpaceCreatorProject;

/*!
   Class to perform consistency checks for MSC against a IV model
 */
class MscSystemChecks : public QObject
{
    Q_OBJECT

public:
    MscSystemChecks(QObject *parent = nullptr);

    // Setup functions
    void setStorage(scs::SpaceCreatorProject *storage);

    // MSC functions
    void changeMscInstanceName(const QString &oldName, const QString &name);

    void changeMscMessageName(
            const QString &oldName, const QString &name, const QString &sourceName, const QString &targetName);
    QList<msc::MscMessage *> allMessages(
            const QString &messageName, const QString &sourceName, const QString &targetName);

    // DV functions
    bool dvMessagesExist(const QString &messageName, const QString &sourceName, const QString &targetName,
            shared::MessageEnd msgSide);
    void changeDvFunctionBindingName(const QString &oldName, const QString &name);
    void changeDvMessageBindingName(const QString &oldName, const QString &name, const QString &sourceName,
            const QString &targetName, shared::MessageEnd msgSide);

public Q_SLOTS:
    void onMscEntityNameChanged(QObject *entity, const QString &oldName, shared::UndoCommand *command);
    void onAttributeChanged(shared::VEObject *entity, const QString &attrName, const QVariant &oldValue);

private:
    QPointer<SpaceCreatorProject> m_storage;
    Qt::CaseSensitivity m_caseCheck = Qt::CaseInsensitive;
    bool m_nameUpdateRunning = false;
};

}
