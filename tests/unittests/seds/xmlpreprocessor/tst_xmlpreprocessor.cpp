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
#include <QDomDocument>
#include <QDomElement>
#include <QObject>
#include <QtTest>
#include <conversion/common/exceptions.h>
#include <seds/SymbolDefinitionReader/symboldefinitionreader.h>
#include <seds/XmlPreprocessor/exceptions.h>
#include <seds/XmlPreprocessor/preprocessor.h>

using conversion::FileNotFoundException;
using seds::preprocessor::UndefinedExternalReference;
using seds::preprocessor::XmlPreprocessor;
using seds::preprocessor::XmlPreprocessorException;
using seds::symbolreader::SymbolDefinitionReader;

namespace seds::test {

class tst_XmlPreprocessor : public QObject
{
    Q_OBJECT

public:
    virtual ~tst_XmlPreprocessor() = default;

private Q_SLOTS:
    void testValid();
    void testUndefinedExternalReference();
    void testXIncludedNonexisting();
};

void tst_XmlPreprocessor::testValid()
{
    SymbolDefinitionReader::ExternalReferencesMap externalReferences;
    externalReferences.insert({ "long_description", "This is a long description" });
    externalReferences.insert({ "integer.name", "DataItem" });

    try {
        XmlPreprocessor::preprocess("IncludesValid.xml", "IncludesValidPreprocessed.xml", externalReferences);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QFile file("IncludesValidPreprocessed.xml");
    if (!file.open(QIODevice::ReadOnly)) {
        QFAIL("Unable to open file");
    }

    QDomDocument xmlDoc;
    if (!xmlDoc.setContent(&file)) {
        QFAIL("Unable to load DOM");
    }

    QDomElement integerDataTypeElement = xmlDoc.elementsByTagName("IntegerDataType").at(0).toElement();
    QCOMPARE(integerDataTypeElement.attribute("name"), "DataItem");

    QDomText longDescriptionElement = xmlDoc.elementsByTagName("LongDescription").at(0).firstChild().toText();
    QCOMPARE(longDescriptionElement.nodeValue(), "This is a long description");
}

void tst_XmlPreprocessor::testUndefinedExternalReference()
{
    SymbolDefinitionReader::ExternalReferencesMap externalReferences;

    QVERIFY_EXCEPTION_THROWN(XmlPreprocessor::preprocess("UndefinedExternalReference.xml",
                                     "UnfedinedExternalReference.xml", externalReferences),
            UndefinedExternalReference);
}

void tst_XmlPreprocessor ::testXIncludedNonexisting()
{
    SymbolDefinitionReader::ExternalReferencesMap externalReferences;
    externalReferences.insert({ "long_description", "This is a long description" });
    externalReferences.insert({ "integer.name", "DataItem" });

    QVERIFY_EXCEPTION_THROWN(XmlPreprocessor::preprocess("IncludesNonexisting.xml",
                                     "IncludesNonexistingPreprocessed.xml", externalReferences),
            FileNotFoundException);
}

} // namespace seds::test

QTEST_MAIN(seds::test::tst_XmlPreprocessor)

#include "tst_xmlpreprocessor.moc"
