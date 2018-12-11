/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

namespace msc {
namespace chart {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
Q_NAMESPACE
#endif // Qt >= 5.8.0

enum class Element {
    Chart = 0,
    Document,
    Instance,
    Message,
    Gate,

    ElementsCount
};

#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
Q_ENUM_NS(Element)
#endif // Qt >= 5.8.0

} // ns chart
} // ns msc
