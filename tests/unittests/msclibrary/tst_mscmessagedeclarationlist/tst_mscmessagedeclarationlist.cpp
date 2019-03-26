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

#include "mscmessagedeclarationlist.h"

#include <QScopedPointer>
#include <QtTest>

using namespace msc;

class tst_MscMessageDeclarationList : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testClone();
    void testSetObjectList();

private:
};

void tst_MscMessageDeclarationList::testClone()
{
    MscMessageDeclarationList list;
    auto decl = new MscMessageDeclaration(&list);
    decl->setNames({ { "AAA" } });
    list.append(decl);

    QScopedPointer<MscMessageDeclarationList> clone(list.clone());

    QVERIFY(clone);
    QCOMPARE(clone->size(), 1);
    QCOMPARE(clone->at(0)->names(), QStringList { { "AAA" } });
    QCOMPARE(clone->at(0)->parent(), clone.data());
}

void tst_MscMessageDeclarationList::testSetObjectList()
{
    auto decl = new MscMessageDeclaration;
    decl->setNames({ { "AAA" } });
    QList<MscMessageDeclaration *> plainList;
    plainList.append(decl);

    MscMessageDeclarationList list;
    list.setObjectList(plainList);

    QCOMPARE(list.size(), 1);
    QCOMPARE(list.at(0)->names(), QStringList { { "AAA" } });
    QCOMPARE(list.at(0)->parent(), &list);
}

QTEST_APPLESS_MAIN(tst_MscMessageDeclarationList)

#include "tst_mscmessagedeclarationlist.moc"
