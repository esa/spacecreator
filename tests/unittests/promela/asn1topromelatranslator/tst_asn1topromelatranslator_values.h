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
#include <promela/PromelaModel/inlinedef.h>
#include <promela/PromelaModel/proctypeelement.h>

using Asn1Acn::Definitions;

namespace tmc::test {

class tst_Asn1ToPromelaTranslator_Values : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testSequence() const;
    void testInteger() const;
    void testEnumerated() const;
    void testBoolean() const;

private:
    std::unique_ptr<Definitions> createModel() const;
    const ::promela::model::InlineDef *findInline(
            const std::list<std::unique_ptr<::promela::model::InlineDef>> &list, const QString &name) const;

    template<typename T>
    const T *findProctypeElement(const ::promela::model::Sequence &sequence, size_t index) const;
};
}
