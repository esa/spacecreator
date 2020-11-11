/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "file.h"
#include "types/builtintypes.h"

#include <QScopedPointer>
#include <QtTest>

class tst_File : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init();
    void testAsn1Compliance();

private:
    void addAsn1Types();

    QScopedPointer<Asn1Acn::File> m_asn1Data;
};

void tst_File::addAsn1Types()
{
    Asn1Acn::SourceLocation location;
    auto type = std::make_unique<Asn1Acn::Types::Integer>();
    type->setParameters({ { "min", 0 }, { "max", 255 } });
    auto assignment = std::make_unique<Asn1Acn::TypeAssignment>("MyInt", location, std::move(type));

    auto asn1Definitions = std::make_unique<Asn1Acn::Definitions>("TestDef", location);
    asn1Definitions->addType(std::move(assignment));
    m_asn1Data->add(std::move(asn1Definitions));
}

void tst_File::init()
{
    m_asn1Data.reset(new Asn1Acn::File("/dummy/path"));
}

void tst_File::testAsn1Compliance()
{
    addAsn1Types();

    bool ok = m_asn1Data->checkAsn1Compliance("5", "MyInt");
    QCOMPARE(ok, true);

    ok = m_asn1Data->checkAsn1Compliance("false", "MyInt");
    QCOMPARE(ok, false);
}

QTEST_APPLESS_MAIN(tst_File)

#include "tst_file.moc"
