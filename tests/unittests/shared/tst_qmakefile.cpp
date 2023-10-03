/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "qmakefile.h"

#include <QFile>
#include <QFileInfo>
#include <QtTest>

class tst_QMakeFile : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testReadFilesList();
    void testRenameFileBasename();
    void testRenameDirectory();

private:
    QString copyFile(const QString &filePath) const;
};

void tst_QMakeFile::testReadFilesList()
{
    const QStringList filePaths =
            shared::QMakeFile::readFilesList(QFINDTESTDATA("asn1files.pro"), { "acn", "asn", "asn1" });
    QCOMPARE(filePaths.size(), 12);
    QCOMPARE(filePaths.filter("_multiline.").size(), 4);
    QCOMPARE(filePaths.filter("_singleline.").size(), 4);
    QVERIFY(filePaths.filter("$$PWD").isEmpty());
    QVERIFY(filePaths.filter("interfaceview").isEmpty());
    QCOMPARE(filePaths.filter(".acn").size(), 5);
    QCOMPARE(filePaths.filter(".asn1").size(), 3);
    QCOMPARE(filePaths.filter(".xml").size(), 0);
}

void tst_QMakeFile::testRenameFileBasename()
{
    QString testFile = copyFile(QFINDTESTDATA("batterymngt.pro"));

    shared::QMakeFile::renameFileName(testFile, "batterymngt.cc", "battery_management.cc");
    shared::QMakeFile::renameFileName(testFile, "batterymngt.h", "battery_management.h");

    QFile file(testFile);
    file.open(QIODevice::ReadOnly);
    QVERIFY(file.isOpen());
    QList<QByteArray> lines = file.readAll().split('\n');
    QCOMPARE(lines[0], "SOURCES += work/batterymngt/CPP/src/battery_management.cc");
    QCOMPARE(lines[1], "HEADERS += work/batterymngt/CPP/src/battery_management.h");
    QCOMPARE(lines[2], "SOURCES += work/batterymngt/CPP/src/batterymngt_state.h");

    file.close();
    file.remove();
}

void tst_QMakeFile::testRenameDirectory()
{
    QString testFile = copyFile(QFINDTESTDATA("taste.pro"));

    shared::QMakeFile::renameDirectory(testFile, "batterymngt", "battery_management");

    QFile file(testFile);
    file.open(QIODevice::ReadOnly);
    QVERIFY(file.isOpen());
    QList<QByteArray> lines = file.readAll().split('\n');
    QCOMPARE(lines[0], "INCLUDEPATH += work/dataview/C");
    QCOMPARE(lines[1], "HEADERS += work/dataview/C/dataview-uniq.h");
    QCOMPARE(lines[2], "HEADERS += work/dataview/Ada/src/*.ads");
    QCOMPARE(lines[3], "include(batteryctrl/batteryctrl.pro)");
    QCOMPARE(lines[4], "include(battery_management/batterymngt.pro)");
    QCOMPARE(lines[5], "include(batterymngt_a/batterymngt_a.pro)");
    QCOMPARE(lines[6], "include(b_batterymngt/b_batterymngt.pro)");

    file.close();
    file.remove();
}

QString tst_QMakeFile::copyFile(const QString &filePath) const
{
    QFileInfo fi(filePath);
    QString testFile = fi.path() + "test.pro";
    if (QFile::exists(testFile)) {
        QFile::remove(testFile);
    }
    bool ok = QFile::copy(filePath, testFile);
    Q_ASSERT(ok);
    return testFile;
}

QTEST_MAIN(tst_QMakeFile)

#include "tst_qmakefile.moc"
