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
#include <QtTest/qtestcase.h>
#include <QtTest>
#include <conversion/common/escaper/escaper.h>
#include <conversion/common/escaper/exceptions.h>
#include <conversion/common/exceptions.h>
#include <qnamespace.h>
#include <unittests/common/verifyexception.h>

using conversion::EmptyNameException;
using conversion::Escaper;

namespace conversion::iv::test {

class tst_Escaper : public QObject
{
    Q_OBJECT

private:
    const char *escaperExceptionMsg = "Incorrect name. Name must contain at least one ASCII character.";

private Q_SLOTS:
    void testIvNominals();
    void testIvExceptions();
    void testAsn1TypeNominals();
    void testAsn1TypeExceptions();
    void testAsn1FieldNominals();
    void testAsn1FieldExceptions();
};

void tst_Escaper::testIvNominals()
{
    QCOMPARE(Escaper::escapeIvName("Something"), "Something");
    QCOMPARE(Escaper::escapeIvName("something"), "something");
    QCOMPARE(Escaper::escapeIvName("someThing"), "someThing");
    QCOMPARE(Escaper::escapeIvName("somethinG"), "somethinG");

    QCOMPARE(Escaper::escapeIvName("some thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName("some   thing"), "some_thing");

    QCOMPARE(Escaper::escapeIvName("the-some-thing"), "the_some_thing");
    QCOMPARE(Escaper::escapeIvName("the - some - thing"), "the_some_thing");
    QCOMPARE(Escaper::escapeIvName("the --- some - thing"), "the_some_thing");
    QCOMPARE(Escaper::escapeIvName("the --- some - - -- thing"), "the_some_thing");
    QCOMPARE(Escaper::escapeIvName("the --   - some - -       -- thing"), "the_some_thing");

    QCOMPARE(Escaper::escapeIvName("the some thing"), "the_some_thing");
    QCOMPARE(Escaper::escapeIvName("the some   thing"), "the_some_thing");
    QCOMPARE(Escaper::escapeIvName(" the some   thing"), "the_some_thing");

    QCOMPARE(Escaper::escapeIvName(" some   thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName(" some thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName("some thing "), "some_thing");
    QCOMPARE(Escaper::escapeIvName("__some thing "), "some_thing");
    QCOMPARE(Escaper::escapeIvName("some thing    "), "some_thing");
    QCOMPARE(Escaper::escapeIvName("some     thing    "), "some_thing");

    QCOMPARE(Escaper::escapeIvName("0nd some thing"), "nd_some_thing");
    QCOMPARE(Escaper::escapeIvName("0 00 some thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName("0 00-some thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName("2345nd some thing"), "nd_some_thing");
    QCOMPARE(Escaper::escapeIvName("2nd some thing"), "nd_some_thing");
    QCOMPARE(Escaper::escapeIvName(" 1st some thing"), "st_some_thing");
    QCOMPARE(Escaper::escapeIvName("some tHing 1"), "some_tHing_1");

    QCOMPARE(Escaper::escapeIvName("some-thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName("some -thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName("some -  thing"), "some_thing");

    QCOMPARE(Escaper::escapeIvName("*some&thing)"), "something");
    QCOMPARE(Escaper::escapeIvName("μsomeμthingμ)"), "something");
}

void tst_Escaper::testIvExceptions()
{
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("000000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeIvName("12345567675578"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("000 000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName(" 012 040"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName(" 000-000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("-000-000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("-000 000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName(""), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("-----------"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("___"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName(")(*&^%$#@!"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("313€"), EmptyNameException, escaperExceptionMsg);
}

void tst_Escaper::testAsn1TypeNominals()
{
    QCOMPARE(Escaper::escapeAsn1TypeName("Something"), "Something");
    QCOMPARE(Escaper::escapeAsn1TypeName("something"), "Something");
    QCOMPARE(Escaper::escapeAsn1TypeName("someThing"), "SomeThing");
    QCOMPARE(Escaper::escapeAsn1TypeName("somethinG"), "SomethinG");

    QCOMPARE(Escaper::escapeAsn1TypeName("some thing"), "Some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("some   thing"), "Some-thing");

    QCOMPARE(Escaper::escapeAsn1TypeName(" some   thing"), "Some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("the some thing"), "The-some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("the some   thing"), "The-some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName(" the some   thing"), "The-some-thing");

    QCOMPARE(Escaper::escapeAsn1TypeName(" some thing"), "Some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("some thing "), "Some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("__some thing "), "Some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("some thing    "), "Some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("some     thing    "), "Some-thing");

    QCOMPARE(Escaper::escapeAsn1TypeName("0nd some thing"), "Nd-some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("0 00 some thing"), "Some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("0 00-some thing"), "Some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("2345nd some thing"), "Nd-some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("2nd some thing"), "Nd-some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName(" 1st some thing"), "St-some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("some tHing 1"), "Some-tHing-1");
    QCOMPARE(Escaper::escapeAsn1TypeName("something-)"), "Something-");

    QCOMPARE(Escaper::escapeAsn1TypeName("some-thing"), "Some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("some -thing"), "Some-thing");
    QCOMPARE(Escaper::escapeAsn1TypeName("some -  thing"), "Some-thing");

    QCOMPARE(Escaper::escapeAsn1TypeName("*some&thing)"), "Something");
    QCOMPARE(Escaper::escapeAsn1TypeName("μsomeμthingμ)"), "Something");
}

void tst_Escaper::testAsn1TypeExceptions()
{
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1TypeName("000000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1TypeName("12345567675578"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1TypeName("000 000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1TypeName(" 012 040"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1TypeName(" 000-000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1TypeName("-000-000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1TypeName("-000 000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeAsn1TypeName(""), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1TypeName("-----------"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeAsn1TypeName("___"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1TypeName(")(*&^%$#@!"), EmptyNameException, escaperExceptionMsg);
}

void tst_Escaper::testAsn1FieldNominals()
{
    QCOMPARE(Escaper::escapeAsn1FieldName("Something"), "something");
    QCOMPARE(Escaper::escapeAsn1FieldName("something"), "something");
    QCOMPARE(Escaper::escapeAsn1FieldName("someThing"), "someThing");
    QCOMPARE(Escaper::escapeAsn1FieldName("somethinG"), "somethinG");

    QCOMPARE(Escaper::escapeAsn1FieldName("some thing"), "some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("some   thing"), "some-thing");

    QCOMPARE(Escaper::escapeAsn1FieldName(" some   thing"), "some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("the some thing"), "the-some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("the some   thing"), "the-some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName(" the some   thing"), "the-some-thing");

    QCOMPARE(Escaper::escapeAsn1FieldName(" some   thing"), "some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName(" some thing"), "some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("some thing "), "some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("__some thing "), "some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("some thing    "), "some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("some     thing    "), "some-thing");

    QCOMPARE(Escaper::escapeAsn1FieldName("0nd some thing"), "nd-some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("0 00 some thing"), "some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("0 00-some thing"), "some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("2345nd some thing"), "nd-some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("2nd some thing"), "nd-some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName(" 1st some thing"), "st-some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("some tHing 1"), "some-tHing-1");

    QCOMPARE(Escaper::escapeAsn1FieldName("some-thing"), "some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("some -thing"), "some-thing");
    QCOMPARE(Escaper::escapeAsn1FieldName("some -  thing"), "some-thing");

    QCOMPARE(Escaper::escapeAsn1FieldName("*some&thing)"), "something");
    QCOMPARE(Escaper::escapeAsn1FieldName("μsomeμthingμ)"), "something");
}

void tst_Escaper::testAsn1FieldExceptions()
{
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1FieldName("000000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1FieldName("12345567675578"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1FieldName("000 000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1FieldName(" 012 040"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1FieldName(" 000-000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1FieldName("-000-000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1FieldName("-000 000"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeAsn1FieldName(""), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1FieldName("-----------"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeAsn1FieldName("___"), EmptyNameException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeAsn1FieldName(")(*&^%$#@!"), EmptyNameException, escaperExceptionMsg);
}

} // namespace conversion::common

QTEST_MAIN(conversion::iv::test::tst_Escaper)

#include "tst_escaper.moc"
