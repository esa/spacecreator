/** @file
  * This file is part of the SpaceCreator.
  *
  * @copyright (C) 2021 N7 Space Sp. z o.o.
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Library General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Library General Public License for more details.
  *
  * You should have received a copy of the GNU Library General Public License
  * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
  */

#include <QFileInfo>
#include <QObject>
#include <QtTest>

#include "sedstestbase.h"

#include <converter/import/exceptions.h>
#include <converter/import/options.h>
#include <seds/SedsModel/sedsmodel.h>
#include <seds/SedsXmlImporter/importer.h>

using converter::import::Options;
using seds::importer::SedsXmlImporter;

namespace seds::test {

class tsti_SedsXmlImporter : public SedsTestBase
{
    Q_OBJECT

  public:
    virtual ~tsti_SedsXmlImporter() = default;

  private Q_SLOTS:
    void testValid();
};

void
tsti_SedsXmlImporter::testValid()
{
    Options options;
    options.add(Options::Key::InputFile, "seds.xml");
    options.add(Options::Key::PreprocessedFile, "preprocessed.xml");
    options.add(Options::Key::ExternalRefFile, "external_references.toml");
    options.add(Options::Key::ExternalRef, "integer.name:UnsignedInteger8");

    try {
        const auto model = SedsXmlImporter::import(options);
        const auto sedsModel = dynamic_cast<seds::model::SedsModel*>(model.get());
        const auto& packageFile = std::get<seds::model::PackageFile>(sedsModel->data());
        const auto& dataTypeSet = packageFile.package().dataTypes();

        const auto& unsignedInteger8 = std::get<model::IntegerDataType>(dataTypeSet.at(0));
        QCOMPARE(unsignedInteger8.name().value(), "UnsignedInteger8");
        QCOMPARE(*unsignedInteger8.longDescription(), "A simple 8-bit unsigned integer");

    } catch(const std::exception& ex) {
        QFAIL(ex.what());
    }

    QFileInfo preprocessedFileInfo("preprocessed.xml");
    QVERIFY(!preprocessedFileInfo.exists());
}

} // namespace seds::test

QTEST_MAIN(seds::test::tsti_SedsXmlImporter)

#include "tsti_sedsxmlimporter.moc"
