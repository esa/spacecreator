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

#include <QObject>
#include <QtTest>

#include "sedstestbase.h"

#include <converter/import/exceptions.h>
#include <seds/XmlValidator/exceptions.h>
#include <seds/XmlValidator/validator.h>

using converter::import::FileNotFound;
using seds::validator::XmlValidator;
using seds::validator::XmlValidatorException;

namespace seds::test {

class tsti_XmlValidator : public SedsTestBase
{
    Q_OBJECT

  public:
    virtual ~tsti_XmlValidator() = default;

  private Q_SLOTS:
    void testValid();
    void testInvalid();
    void testIncludesInvalid();
};

void
tsti_XmlValidator::testValid()
{
    try {
        XmlValidator::validate("Valid.xml", "seds.xsd");
    } catch(const std::exception& ex) {
        QFAIL(ex.what());
    }
}

void
tsti_XmlValidator::testInvalid()
{
    QVERIFY_EXCEPTION_THROWN(XmlValidator::validate("seds.xsd", "Invalid.xml"), XmlValidatorException);
}

void
tsti_XmlValidator::testIncludesInvalid()
{
    QVERIFY_EXCEPTION_THROWN(XmlValidator::validate("seds.xsd", "IncludesInvalid.xml"), XmlValidatorException);
}

} // namespace seds::test

QTEST_MAIN(seds::test::tsti_XmlValidator)

#include "tsti_xmlvalidator.moc"
