/*
   Copyright (C) 2018 - 2019 European Space Agency - <maxime.perrotin@esa.int>

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
#include <QPointer>
#include <coreplugin/editormanager/ieditorfactory.h>
#include <memory>

namespace spctr {
class ActionHandler;
class SpaceCreatorProjectManager;

#if QTC_VERSION >= 1200
class MscEditorFactory : public QObject, public Core::IEditorFactory
#else
class MscEditorFactory : public Core::IEditorFactory
#endif
{
    Q_OBJECT

public:
    MscEditorFactory(SpaceCreatorProjectManager *projectManager, QObject *parent);

    Core::IEditor *createMSCEditor();

private:
    std::unique_ptr<ActionHandler> m_actionHandler;
    QPointer<SpaceCreatorProjectManager> m_projectManager;
};
}
