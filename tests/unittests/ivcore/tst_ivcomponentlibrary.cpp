/*
   Copyright (C) 2024 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivcomponentlibrary.h"

#include <QtTest>
#include <archetypes/archetypemodel.h>
#include <ivfunction.h>
#include <ivobject.h>

const QDir libraryPath("./test_components_library");
const QDir componentPath(libraryPath.filePath("test_component"));
const QDir projectDir("./test_projectDir");

class tst_IVComponentLibrary : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase() { library = new ivm::IVComponentLibrary(libraryPath.path(), "test_model"); };
    void cleanupTestCase() { delete library; };
    void test_exportComponent();

private:
    ivm::IVComponentLibrary *library;
};

void tst_IVComponentLibrary::test_exportComponent()
{
    ivm::IVFunction obj;
    QList<ivm::IVObject *> objects;
    objects << &obj;
    ivm::ArchetypeModel *model = new ivm::ArchetypeModel;
    library->exportComponent(componentPath.path(), objects, projectDir.path(), {}, {}, model);
    QVERIFY(QFile::exists(componentPath.filePath(shared::kDefaultInterfaceViewFileName)) == true);
}

QTEST_MAIN(tst_IVComponentLibrary)

#include "tst_ivcomponentlibrary.moc"
