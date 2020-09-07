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

#include "propertytypedelegate.h"

#include "aadlparameter.h"
#include "asn1/definitions.h"
#include "asn1/file.h"
#include "asn1/types/builtintypes.h"

#include <QSharedPointer>

namespace aadlinterface {

static QStringList initNames(const QSharedPointer<Asn1Acn::File> &dataTypes)
{
    QStringList names;

    const QSharedPointer<Asn1Acn::File> &types = dataTypes;
    if (types) {
        for (const std::unique_ptr<Asn1Acn::Definitions> &definitions : types->definitionsList()) {
            for (const std::unique_ptr<Asn1Acn::TypeAssignment> &assignment : definitions->types()) {
                names.append(assignment->name());
            }
        }
    }

    names.append(aadl::BasicParameter::typeName(aadl::BasicParameter::Type::Timer));
    names.append(aadl::BasicParameter::typeName(aadl::BasicParameter::Type::Directive));

    return names;
}

PropertyTypeDelegate::PropertyTypeDelegate(const QSharedPointer<Asn1Acn::File> &dataTypes, QObject *parent)
    : StringListComboDelegate(initNames(dataTypes), parent)
{
}

PropertyTypeDelegate::~PropertyTypeDelegate() { }

}
