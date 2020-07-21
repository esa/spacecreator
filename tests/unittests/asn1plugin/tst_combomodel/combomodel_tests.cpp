/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a programme and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "combomodel_tests.h"

#include "modeltest.h"
#include "tree-views/combomodel.h"

#include <QTest>
#include <definitions.h>
#include <file.h>
#include <typeassignment.h>
#include <types/builtintypes.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::TreeViews;
using namespace Asn1Acn::Internal::TreeViews::Tests;

ComboModelTests::ComboModelTests(QObject *parent)
    : QObject(parent)
{
}

ComboModelTests::~ComboModelTests() { }

void ComboModelTests::test_emptyModel()
{
    ModelTest testEmptyModel(new ComboModel({}, this), this);
}

void ComboModelTests::test_modelWithDummyPopulation()
{
    const QString filePath("file.asn1");

    const auto root = std::make_unique<Asn1Acn::File>(filePath);

    auto definitions1 =
            std::make_unique<Asn1Acn::Definitions>("Module1", Asn1Acn::SourceLocation { "file1.asn1", 0, 0 });
    definitions1->addType(std::make_unique<Asn1Acn::TypeAssignment>(
            "Num1", Asn1Acn::SourceLocation { "file1.asn1", 2, 3 }, std::make_unique<Asn1Acn::Types::Integer>()));
    definitions1->addType(std::make_unique<Asn1Acn::TypeAssignment>(
            "Num2", Asn1Acn::SourceLocation { "file1.asn1", 3, 3 }, std::make_unique<Asn1Acn::Types::Integer>()));
    root->add(std::move(definitions1));

    auto definitions2 =
            std::make_unique<Asn1Acn::Definitions>("Module2", Asn1Acn::SourceLocation { "file1.asn1", 5, 0 });
    definitions2->addType(std::make_unique<Asn1Acn::TypeAssignment>(
            "Num3", Asn1Acn::SourceLocation { "file1.asn1", 6, 3 }, std::make_unique<Asn1Acn::Types::Integer>()));
    definitions2->addType(std::make_unique<Asn1Acn::TypeAssignment>(
            "Num4", Asn1Acn::SourceLocation { "file1.asn1", 7, 3 }, std::make_unique<Asn1Acn::Types::Integer>()));
    root->add(std::move(definitions2));

    auto model = new ComboModel(Utils::FileName::fromString(filePath), this);
    model->setRoot(root.get());

    ModelTest testNonEmptyModel(model, this);
}

QTEST_MAIN(ComboModelTests)
