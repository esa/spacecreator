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

#include <QDir>
#include <QObject>
#include <QtTest>

#include "sedstestbase.h"

#include <converter/import/exceptions.h>
#include <seds/SymbolDefinitionReader/exceptions.h>
#include <seds/SymbolDefinitionReader/symboldefinitionreader.h>

using converter::import::FileNotFound;
using seds::symbolreader::SymbolDefinitionReader;
using seds::symbolreader::SymbolDefinitionReaderException;

namespace seds::test {

class tsti_SymbolDefinitionReader : public SedsTestBase
{
    Q_OBJECT

  public:
    virtual ~tsti_SymbolDefinitionReader() = default;

  private Q_SLOTS:
    void readValidFile();
    void readNonexistingFile();
    void readInvalidFile();
};

void
tsti_SymbolDefinitionReader::readValidFile()
{
    SymbolDefinitionReader::ExternalReferencesMap externalReferences;

    try {
        externalReferences = SymbolDefinitionReader::readSymbols("Valid.toml");
    } catch(const std::exception& ex) {
        QFAIL(ex.what());
    }

    QCOMPARE(externalReferences.at("long_description"), "This is a long description");
    QCOMPARE(externalReferences.at("integer.name"), "DataItem");
}

void
tsti_SymbolDefinitionReader::readNonexistingFile()
{
    QVERIFY_EXCEPTION_THROWN(SymbolDefinitionReader::readSymbols("Nonexisting.toml"), FileNotFound);
}

void
tsti_SymbolDefinitionReader::readInvalidFile()
{
    QVERIFY_EXCEPTION_THROWN(SymbolDefinitionReader::readSymbols("Invalid.toml"), SymbolDefinitionReaderException);
}

} // namespace seds::test

QTEST_MAIN(seds::test::tsti_SymbolDefinitionReader)

#include "tsti_symboldefinitionreader.moc"
