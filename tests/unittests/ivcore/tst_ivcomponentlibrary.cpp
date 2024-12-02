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

class tst_IVComponentLibrary : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase() { delete library; };
    void test_exportComponent();
    void test_removeComponent();

private:
    ivm::IVComponentLibrary *library;
    QDir componentPath_export;
    QDir componentPath_remove;
    QDir projectDir;
};

void tst_IVComponentLibrary::initTestCase()
{
    library = new ivm::IVComponentLibrary(QDir::currentPath(), "test_model");
    componentPath_export = QDir::currentPath();
    componentPath_export = componentPath_export.filePath("test_export");
    componentPath_remove = QDir::currentPath();
    componentPath_remove = componentPath_remove.filePath("test_remove");
    projectDir = QDir::currentPath();
    projectDir = projectDir.filePath("./test_projectDir");
}

void tst_IVComponentLibrary::test_exportComponent()
{
    ivm::IVFunction obj;
    QList<ivm::IVObject *> objects;
    objects << &obj;
    ivm::ArchetypeModel *model = new ivm::ArchetypeModel;
    library->exportComponent(componentPath_export.path(), objects, projectDir.path(), {}, {}, model);
    QVERIFY(QFile::exists(componentPath_export.filePath(shared::kDefaultInterfaceViewFileName)) == true);
}

void tst_IVComponentLibrary::test_removeComponent()
{
    ivm::IVFunction obj;
    QList<ivm::IVObject *> objects;
    objects << &obj;
    ivm::ArchetypeModel *model = new ivm::ArchetypeModel;
    library->exportComponent(componentPath_remove.path(), objects, projectDir.path(), {}, {}, model);
    QVERIFY(QFile::exists(componentPath_remove.filePath(shared::kDefaultInterfaceViewFileName)) == true);
    auto component = library->loadComponent(componentPath_remove.filePath(shared::kDefaultInterfaceViewFileName));
    library->removeComponent(component->rootIds.first());
    QVERIFY(QFile::exists(componentPath_remove.filePath(shared::kDefaultInterfaceViewFileName)) == false);
}

QTEST_MAIN(tst_IVComponentLibrary)

#include "tst_ivcomponentlibrary.moc"
