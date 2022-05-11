/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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
#include <QtTest>
#include <asn1library/asn1/asn1model.h>
#include <memory>

using Asn1Acn::Definitions;

namespace tmc::test {

class tst_Asn1ToPromelaTranslator_Env : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testBoolean() const;
    void testInteger() const;
    void testSequence() const;
    void testSequenceEmbeddedType() const;
    void testSequenceOf() const;
    void testSequenceOfVariableSize() const;
    void testEnumerated() const;

private:
    std::unique_ptr<Definitions> createModel() const;
};
}
