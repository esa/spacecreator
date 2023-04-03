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

#include "adarefactor.h"

namespace spctr {

QString AdaRefactor::language() const
{
    return tr("Ada");
}

QString AdaRefactor::languageDir() const
{
    return "Ada";
}

QString AdaRefactor::filename(const QString &funcName) const
{
    return QString("/src/%1.adb").arg(funcName.toLower());
}

} // namespace spctr
