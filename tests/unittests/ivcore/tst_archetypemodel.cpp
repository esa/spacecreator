/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "archetypes/archetypelibrary.h"
#include "archetypes/archetypemodel.h"
#include "archetypes/functionarchetype.h"
#include "ivtestutils.h"

#include <QPointer>
#include <QStandardPaths>
#include <QTest>

class tst_ArchtypeModel : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void tst_getLibrariesNames();
    void tst_getFunctionsNamesByLibraryName();
};

void tst_ArchtypeModel::tst_getLibrariesNames()
{
    QPointer<ivm::ArchetypeModel> archetypeModel = new ivm::ArchetypeModel();
    QVector<ivm::ArchetypeObject *> archetypeObjects;
    archetypeObjects.append(new ivm::ArchetypeLibrary(QString("TestLibrary1"), archetypeModel));
    archetypeObjects.append(new ivm::ArchetypeLibrary("TestLibrary2", archetypeModel));
    archetypeModel->addObjects(archetypeObjects);

    QStringList librariesNames = archetypeModel->getLibrariesNames();

    QCOMPARE(librariesNames.size(), 2);
    QVERIFY(librariesNames.contains("TestLibrary1"));
    QVERIFY(librariesNames.contains("TestLibrary2"));
}

void tst_ArchtypeModel::tst_getFunctionsNamesByLibraryName()
{
    QPointer<ivm::ArchetypeModel> archetypeModel = new ivm::ArchetypeModel();
    QVector<ivm::ArchetypeObject *> archetypeObjects;
    ivm::ArchetypeLibrary *archetypeLibrary = new ivm::ArchetypeLibrary("TestLibrary1", archetypeModel);
    archetypeLibrary->addFunction(new ivm::FunctionArchetype("TestFunction1", archetypeLibrary));
    archetypeLibrary->addFunction(new ivm::FunctionArchetype("TestFunction2", archetypeLibrary));

    archetypeObjects.append(archetypeLibrary);
    archetypeModel->addObjects(archetypeObjects);

    QStringList functionsNames = archetypeModel->getFunctionsNamesByLibraryName("TestLibrary1");

    QCOMPARE(functionsNames.size(), 2);
    QVERIFY(functionsNames.contains("TestFunction1"));
    QVERIFY(functionsNames.contains("TestFunction2"));
}

QTEST_APPLESS_MAIN(tst_ArchtypeModel)

#include "tst_archetypemodel.moc"
