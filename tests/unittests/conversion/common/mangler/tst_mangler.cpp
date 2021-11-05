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
#include <conversion/common/exceptions.h>
#include <conversion/common/mangler/mangler.h>
#include <unittests/common/verifyexception.h>

using conversion::Mangler;
using conversion::ManglerException;

namespace conversion::iv::test {

class tst_Mangler : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testIvNominals();
    void testIvExceptions();
};

void tst_Mangler::testIvNominals()
{
    QCOMPARE(Mangler::mangleIvName("Something"), "Something");
    QCOMPARE(Mangler::mangleIvName("something"), "something");
    QCOMPARE(Mangler::mangleIvName("some thing"), "some_thing");
    QCOMPARE(Mangler::mangleIvName("some   thing"), "some_thing");
    QCOMPARE(Mangler::mangleIvName(" some   thing"), "some_thing");
    QCOMPARE(Mangler::mangleIvName(" some thing"), "some_thing");
    QCOMPARE(Mangler::mangleIvName(" 1st some thing"), "st_something");
    QCOMPARE(Mangler::mangleIvName("some tHing 1"), "something1");
    QCOMPARE(Mangler::mangleIvName("some-thing"), "some_thing");
    QCOMPARE(Mangler::mangleIvName("śome thing"), "some_thing");

    QCOMPARE(Mangler::mangleIvName("śome thing"), "some_thing");
    QCOMPARE(Mangler::mangleIvName("ąćśęńółżź"), "acsenolzz");
    QCOMPARE(Mangler::mangleIvName("udělátko"), "udelatko");
    QCOMPARE(Mangler::mangleIvName("atriði"), "atridi");
    QCOMPARE(Mangler::mangleIvName("artículo"), "articulo");
    QCOMPARE(Mangler::mangleIvName("položka"), "polozka");
    QCOMPARE(Mangler::mangleIvName("l'objet"), "lobjet");
    QCOMPARE(Mangler::mangleIvName("$#%^#abcd"), "abcd");
    QCOMPARE(Mangler::mangleIvName("fdgij$#%@^"), "fdgij");
    QCOMPARE(Mangler::mangleIvName("Edelweiß"), "Edelweis");
    QCOMPARE(Mangler::mangleIvName("entkräfter"), "entkrafter");
}

void tst_Mangler::testIvExceptions()
{
    const char *manglerExceptionMsg = "Incorrect name. Name must contain at least one ASCII character.";

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Mangler::mangleIvName("000000"), ManglerException, manglerExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            Mangler::mangleIvName("12345567675578"), ManglerException, manglerExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Mangler::mangleIvName("000 000"), ManglerException, manglerExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Mangler::mangleIvName(" 012 040"), ManglerException, manglerExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Mangler::mangleIvName(" 000-000"), ManglerException, manglerExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Mangler::mangleIvName("-000-000"), ManglerException, manglerExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Mangler::mangleIvName("-000 000"), ManglerException, manglerExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Mangler::mangleIvName(""), ManglerException, manglerExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Mangler::mangleIvName("-----------"), ManglerException, manglerExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Mangler::mangleIvName("___"), ManglerException, manglerExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Mangler::mangleIvName(")(*&^%$#@!"), ManglerException, manglerExceptionMsg);
    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(Mangler::mangleIvName("313€"), ManglerException, manglerExceptionMsg);
}

} // namespace conversion::common

QTEST_MAIN(conversion::iv::test::tst_Mangler)

#include "tst_mangler.moc"
