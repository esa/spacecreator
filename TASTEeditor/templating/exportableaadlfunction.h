/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "exportableaadlobject.h"

namespace taste3 {

namespace aadl {
    class AADLObjectFunctionType;
}

namespace templating {

/**
 * @brief The ExportableAADLFunction is a class to export AADLObjectFunctionType and AADLObjectFunction
 */
class ExportableAADLFunction : public ExportableAADLObject
{
    Q_GADGET
    Q_PROPERTY(QVariantList interfaces READ interfaces)
    Q_PROPERTY(QVariantList functions READ functions)
    Q_PROPERTY(QVariantList comments READ comments)
    Q_PROPERTY(QVariantList connections READ connections)

public:
    explicit ExportableAADLFunction(const aadl::AADLObjectFunctionType *function = nullptr);

    QVariantList interfaces() const;
    QVariantList functions() const;
    QVariantList comments() const;
    QVariantList connections() const;
};

} // ns templating
} // ns taste3

DECLARE_EXPORTED_TYPE(ExportableAADLFunction)
