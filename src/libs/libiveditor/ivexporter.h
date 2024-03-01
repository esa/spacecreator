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

#include <QString>
#include <QVariantList>
#include <ivcore/ivxmlwriter.h>
#include <templating/objectsexporter.h>

class QBuffer;
class QWidget;

namespace shared {
class VEObject;
}

namespace ivm {
class ArchetypeModel;
}

namespace templating {
class UIExporter;
}

namespace ive {

class InterfaceDocument;

class IVExporter : public ivm::IVXMLWriter
{
    Q_OBJECT
public:
    explicit IVExporter(QObject *parent = nullptr);
    bool exportDocSilently(InterfaceDocument *doc, const QString &outPath, const QString &templatePath = QString());

    bool exportDocInteractively(InterfaceDocument *doc, const QString &outPath = QString(),
            const QString &templatePath = QString(), QWidget *root = nullptr);

private:
    QHash<QString, QVariant> collectInterfaceObjects(InterfaceDocument *doc);
    void checkArchetypeIntegrity(QList<shared::VEObject *> ivObjects, ivm::ArchetypeModel *archetypesModel);

};

}
