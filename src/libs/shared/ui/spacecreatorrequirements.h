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
along with this program. If not, see
<https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once
#include "ui/requirementswidget.h"

namespace requirement {
class RequirementsManager;
class RequirementsModelBase;
}
namespace shared {
namespace ui {
class SpaceCreatorRequirements : public requirement::RequirementsWidget

{
    Q_OBJECT
public:
    SpaceCreatorRequirements(const QString &requirementsUrl, requirement::RequirementsManager *manager,
            requirement::RequirementsModelBase *model, QWidget *parent = nullptr);

public Q_SLOTS:
    void onCredentialsChange(const QUrl &url, const QString& newToken);

private:
    QString tokenKey(const QString &base) const { return QString("%1__token").arg(base); }
    bool loadSavedCredentials();
};
}
}
