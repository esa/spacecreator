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

class QWidget;

namespace document {
class AbstractTabDocument;
class InterfaceTabDocument;
}

namespace taste3 {
namespace app {

class XmlDocExporter
{
public:
    static QString templatesPath();
    static QString interfaceDefaultTemplate();

    static bool canExportXml(document::AbstractTabDocument *doc);
    static bool exportDocSilently(document::AbstractTabDocument *doc, const QString &outPath = QString(),
            const QString &templatePath = QString());

    static bool exportDocInteractive(document::AbstractTabDocument *doc, QWidget *root,
            const QString &outPath = QString(), const QString &templatePath = QString());

    static QHash<QString, QVariantList> collectInterfaceObjects(document::InterfaceTabDocument *doc);

private:
    XmlDocExporter();

    enum class InteractionPolicy
    {
        Silently,
        Interactive
    };

    enum class RolloutDefaultsPolicy
    {
        Keep = 0,
        Overwrite
    };

    static bool exportDoc(document::AbstractTabDocument *doc, QWidget *root, const QString &outPath,
            const QString &templatePath, InteractionPolicy interaction);

    static void ensureDefaultTemplatesDeployed_interface(RolloutDefaultsPolicy policy = RolloutDefaultsPolicy::Keep);
    static bool exportDocInterface(document::InterfaceTabDocument *doc, QWidget *root, const QString &outPath,
            const QString &templatePath, InteractionPolicy interaction);

    static bool runExportSilently(document::InterfaceTabDocument *doc, const QHash<QString, QVariantList> &content,
            const QString &templateFileName, const QString &outFileName);
    static bool showExportDialog(document::InterfaceTabDocument *doc, QWidget *parentWindow,
            const QHash<QString, QVariantList> &content, const QString &templateFileName, const QString &outFileName);
};

} // ns app
} // ns taste3
