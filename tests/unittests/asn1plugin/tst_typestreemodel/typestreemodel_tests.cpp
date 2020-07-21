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
#include "typestreemodel_tests.h"

#include "modeltest.h"
#include "tree-views/typestreemodel.h"

#include <QTest>
#include <asn1/definitions.h>
#include <asn1/file.h>
#include <asn1/project.h>
#include <asn1/root.h>
#include <asn1/typeassignment.h>
#include <asn1/types/builtintypes.h>

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::TreeViews;
using namespace Asn1Acn::Internal::TreeViews::Tests;

TypesTreeModelTests::TypesTreeModelTests(QObject *parent)
    : QObject(parent)
{
}

TypesTreeModelTests::~TypesTreeModelTests() { }

void TypesTreeModelTests::test_emptyModel()
{
    ModelTest testEmptyModel(new TypesTreeModel(this), this);
}

void TypesTreeModelTests::test_modelWithDummyPopulation()
{
    auto file1 = std::make_unique<Asn1Acn::File>("file1.asn1");
    {
        auto definitions1 =
                std::make_unique<Asn1Acn::Definitions>("Module1", Asn1Acn::SourceLocation { "file1.asn1", 0, 0 });
        definitions1->addType(std::make_unique<Asn1Acn::TypeAssignment>(
                "Num1", Asn1Acn::SourceLocation { "file1.asn1", 2, 3 }, std::make_unique<Asn1Acn::Types::Integer>()));
        definitions1->addType(std::make_unique<Asn1Acn::TypeAssignment>(
                "Num2", Asn1Acn::SourceLocation { "file1.asn1", 3, 3 }, std::make_unique<Asn1Acn::Types::Integer>()));
        file1->add(std::move(definitions1));

        auto definitions2 =
                std::make_unique<Asn1Acn::Definitions>("Module2", Asn1Acn::SourceLocation { "file1.asn1", 5, 0 });
        definitions2->addType(std::make_unique<Asn1Acn::TypeAssignment>(
                "Num3", Asn1Acn::SourceLocation { "file1.asn1", 6, 3 }, std::make_unique<Asn1Acn::Types::Integer>()));
        definitions2->addType(std::make_unique<Asn1Acn::TypeAssignment>(
                "Num4", Asn1Acn::SourceLocation { "file1.asn1", 7, 3 }, std::make_unique<Asn1Acn::Types::Integer>()));
        file1->add(std::move(definitions2));
    }

    auto file2 = std::make_unique<Asn1Acn::File>("file2.asn1");
    {
        auto definitions1 =
                std::make_unique<Asn1Acn::Definitions>("Module10", Asn1Acn::SourceLocation { "file2.asn1", 0, 0 });
        definitions1->addType(std::make_unique<Asn1Acn::TypeAssignment>(
                "Num10", Asn1Acn::SourceLocation { "file2.asn1", 2, 3 }, std::make_unique<Asn1Acn::Types::Integer>()));
        file2->add(std::move(definitions1));
    }

    auto project1 = std::make_unique<Asn1Acn::Project>("Project1");
    project1->add(std::move(file1));
    project1->add(std::move(file2));

    auto file3 = std::make_unique<Asn1Acn::File>("file3.asn1");
    {
        auto definitions1 =
                std::make_unique<Asn1Acn::Definitions>("Module20", Asn1Acn::SourceLocation { "file3.asn1", 0, 0 });
        definitions1->addType(std::make_unique<Asn1Acn::TypeAssignment>(
                "Num20", Asn1Acn::SourceLocation { "file3.asn1", 2, 3 }, std::make_unique<Asn1Acn::Types::Integer>()));
        file3->add(std::move(definitions1));
    }

    auto project2 = std::make_unique<Asn1Acn::Project>("Project2");
    project2->add(std::move(file3));

    auto root = std::make_unique<Asn1Acn::Root>();
    root->add(std::move(project1));
    root->add(std::move(project2));

    auto model = new TypesTreeModel(this);
    model->setRoot(root.get());

    ModelTest testNonEmptyModel(model, this);
}

QTEST_MAIN(TypesTreeModelTests)
