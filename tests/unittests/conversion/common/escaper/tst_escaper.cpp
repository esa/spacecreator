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
#include <conversion/common/exceptions.h>
#include <unittests/common/verifyexception.h>

using conversion::Escaper;
using conversion::EscaperException;

namespace conversion::iv::test {

class tst_Escaper : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testIvNominals();
    void testIvExceptions();
    void testAsn1Nominals();
    void testAsn1Exceptions();
};

void tst_Escaper::testIvNominals()
{
    QCOMPARE(Escaper::escapeIvName("Something"), "Something");
    QCOMPARE(Escaper::escapeIvName("something"), "something");
    QCOMPARE(Escaper::escapeIvName("someThing"), "someThing");
    QCOMPARE(Escaper::escapeIvName("somethinG"), "somethinG");

    QCOMPARE(Escaper::escapeIvName("some thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName("some   thing"), "some_thing");

    QCOMPARE(Escaper::escapeIvName(" some   thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName(" some thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName("some thing "), "some_thing");
    QCOMPARE(Escaper::escapeIvName("__some thing "), "some_thing");
    QCOMPARE(Escaper::escapeIvName("some thing    "), "some_thing");
    QCOMPARE(Escaper::escapeIvName("some     thing    "), "some_thing");

    QCOMPARE(Escaper::escapeIvName("0nd some thing"), "nd_something");
    QCOMPARE(Escaper::escapeIvName("0 00 some thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName("0 00-some thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName("2345nd some thing"), "nd_something");
    QCOMPARE(Escaper::escapeIvName("2nd some thing"), "nd_something");
    QCOMPARE(Escaper::escapeIvName(" 1st some thing"), "st_something");
    QCOMPARE(Escaper::escapeIvName("some tHing 1"), "some_tHing1");

    QCOMPARE(Escaper::escapeIvName("some-thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName("some -thing"), "some_thing");
    QCOMPARE(Escaper::escapeIvName("some -  thing"), "some_thing");

    QCOMPARE(Escaper::escapeIvName("*some&thing)"), "something");
    QCOMPARE(Escaper::escapeIvName("μsomeμthingμ)"), "something");

    // QCOMPARE(Escaper::escapeIvName("śome thing"), "some_thing");
    // QCOMPARE(Escaper::escapeIvName("ąćśęńółżź"), "acsenolzz");
    // QCOMPARE(Escaper::escapeIvName("udělátko"), "udelatko");
    // QCOMPARE(Escaper::escapeIvName("atriði"), "atridi");
    // QCOMPARE(Escaper::escapeIvName("artículo"), "articulo");
    // QCOMPARE(Escaper::escapeIvName("položka"), "polozka");
    // QCOMPARE(Escaper::escapeIvName("l'objet"), "lobjet");
    // QCOMPARE(Escaper::escapeIvName("$#%^#abcd"), "abcd");
    // QCOMPARE(Escaper::escapeIvName("fdgij$#%@^"), "fdgij");
    // QCOMPARE(Escaper::escapeIvName("Edelweiß"), "Edelweis");
    // QCOMPARE(Escaper::escapeIvName("entkräfter"), "entkrafter");
}

void tst_Escaper::testIvExceptions()
{
    const char *escaperExceptionMsg = "Incorrect name. Name must contain at least one ASCII character.";

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("000000"), EscaperException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Escaper::escapeIvName("12345567675578"), EscaperException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("000 000"), EscaperException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName(" 012 040"), EscaperException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName(" 000-000"), EscaperException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("-000-000"), EscaperException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("-000 000"), EscaperException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName(""), EscaperException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("-----------"), EscaperException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("___"), EscaperException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName(")(*&^%$#@!"), EscaperException, escaperExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Escaper::escapeIvName("313€"), EscaperException, escaperExceptionMsg);
}

void tst_Escaper::testAsn1Nominals()
{
    QCOMPARE(Escaper::escapeAsn1Name("Something"), "Something");
    // TODO
}

void tst_Escaper::testAsn1Exceptions()
{
    // TODO
}

} // namespace conversion::common

QTEST_MAIN(conversion::iv::test::tst_Escaper)

#include "tst_escaper.moc"
