/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QList>
#include <QMarginsF>
#include <QSizeF>

class QLineF;
class QPointF;
class QGraphicsScene;
class QGraphicsItem;

namespace Asn1Acn {
class File;
}

namespace ive {

/*!
  \brief ive::ZOrder

  This static anonymous struct holds values for items Z ordering:
    \var Topmost
        Maximum available Z level.
    \var Selected
        The Z level for a selected item; When selected, the item is raised to be almost-the-topmost (Topmost-1),
        so a Connection overlapped by a Comment is visible.
    \var Preview
        The alias for Topmost Z level. Used for preview an item during its creation, so it does not go under
  existent/selected items.

    \var Function
        Z level of IVFunctionGraphicsItem and IVFunctionTypeGraphicsItem.
    \var Connection
        Z level of IVConnectionGraphicsItem.
    \var Interface
        Z level of IVInterfaceGraphicsItem.
    \var Comment
        Z level of IVCommentGraphicsItem.
    \var GripPoint
        Z level of GripPoint item — alias for Selected (almost-the-topmost).
 */

static const struct {
    const qreal Topmost = 10.;
    const qreal Selected = Topmost - 1.;
    const qreal Preview = Topmost;

    const qreal Connection = 0.;
    const qreal Function = 1.;
    const qreal InternalConnection = 1.5;
    const qreal Interface = 2.;
    const qreal Comment = 3.;
    const qreal GripPoint = Selected;
} ZOrder {};

//! Default filename for Interface View diagram
static const QString kDefaultInterfaceViewFileName { QLatin1String("interfaceview.xml") };

/*!
 * Returns path to exported components library set in TASTE_COMPONENTS_LIBRARY
 * environment variable otherwise points to default, for ex. ~/.local/share/ESA/IV Editor/components_library
 */
QString componentsLibraryPath();

/*!
 * Returns path to shared function types set in TASTE_SHARED_TYPES
 * environment variable otherwise points to default, for ex. ~/.local/share/ESA/IV Editor/shared_types
 */
QString sharedTypesPath();

/*!
 * Returns path to user defined attributes set in TASTE_DEFAULT_ATTRIBUTES_PATH
 * environment variable otherwise points to default, for ex. ~/.local/share/ESA/IV Editor/default_attributes.xml
 */
QString dynamicPropertiesFilePath();

QStringList asn1Names(const Asn1Acn::File *dataTypes);

} // namespace ive
