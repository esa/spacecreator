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
#include <ivxmlreader.h>

class tst_IVComponentLibrary : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase() { delete library; };
    void test_exportComponent();
    void test_removeComponent();
    void test_renameComponent();

private:
    bool checkRenamedFilesExists();

private:
    ivm::IVComponentLibrary *library;
    QDir componentPath_export;
    QDir componentPath_remove;
    QDir componentPath_rename;
    QString component_newName;
    QDir componentPath_newName;
    QDir projectDir;
};

void tst_IVComponentLibrary::initTestCase()
{
    library = new ivm::IVComponentLibrary(QDir::currentPath(), "test_model");
    componentPath_export = QDir::currentPath();
    componentPath_export = componentPath_export.filePath("test_export");
    componentPath_remove = QDir::currentPath();
    componentPath_remove = componentPath_remove.filePath("test_remove");
    componentPath_rename = QDir::currentPath();
    componentPath_rename = componentPath_rename.filePath("test_rename");
    component_newName = "test_Renamed";
    projectDir = QDir::currentPath();
    componentPath_newName = QDir::currentPath();
    componentPath_newName = componentPath_newName.filePath(component_newName);
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

void tst_IVComponentLibrary::test_renameComponent()
{
    ivm::IVFunction obj;
    QList<ivm::IVObject *> objects;
    objects << &obj;
    ivm::ArchetypeModel *model = new ivm::ArchetypeModel;

    library->exportComponent(componentPath_rename.path(), objects, projectDir.path(), {}, {}, model);
    QVERIFY2(QFile::exists(componentPath_rename.filePath(shared::kDefaultInterfaceViewFileName)) == true,
            tr("Component not exported").toStdString().c_str());
    auto component = library->loadComponent(componentPath_rename.filePath(shared::kDefaultInterfaceViewFileName));
    auto objectId = component->model->ivobjects().values().first()->id();

    library->renameComponent(component_newName, component->rootIds.first());
    auto component_renamed =
            library->loadComponent(componentPath_newName.filePath(shared::kDefaultInterfaceViewFileName));
    if (component_renamed) {
        ivm::IVObject *rootObject = component_renamed->model->ivobjects().values().first();
        if (rootObject) {
            QVERIFY2(rootObject->title() == component_newName,
                    tr("Interfaceview.xml file rootObject not renamed").toStdString().c_str());
            QVERIFY2(rootObject->id() == objectId, tr("Id before and after rename don't match ").toStdString().c_str());
        }
    }
    QVERIFY2(
            checkRenamedFilesExists(), tr("Files inside component's directory were not renamed").toStdString().c_str());
}

bool tst_IVComponentLibrary::checkRenamedFilesExists()
{
    QString filePath = QDir(componentPath_newName).filePath(component_newName + ".pro");
    return (QFileInfo::exists(filePath) && QFileInfo(filePath).isFile());
}

QTEST_MAIN(tst_IVComponentLibrary)

#include "tst_ivcomponentlibrary.moc"
