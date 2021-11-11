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

#pragma once

#include <QObject>
#include <conversion/registry/registry.h>

namespace Asn1Acn {
class File;
} // namespace Asn1Acn

namespace conversion::asn1::test {

class TestBase : public QObject
{
    Q_OBJECT

protected Q_SLOTS:
    void initTestCase();
    void cleanup();

protected:
    void runConversionTest(QString testFilenameBase, QString asn1PackageName);
    void validateAsn1(const QString &filename);

protected:
    conversion::Registry m_registry;
    QString m_currentTestFilename;
};

} // namespace conversion::asn1::test
