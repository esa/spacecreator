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

#include "ivfunction.h"
#include "ivinterface.h"
#include "ivlibrary.h"

#include <QCryptographicHash>
#include <QDirIterator>
#include <QStandardPaths>
#include <QTest>

class tst_AADLUtils : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();

    void tst_copyResourceFile();
    void tst_ensureDirExists();
    void tst_ensureFileExists();
    void tst_isAncestorOf();
};

void tst_AADLUtils::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    ivm::initIVLibrary();
}

static inline QByteArray fileHash(const QString &filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        return {};
    }
    return QCryptographicHash::hash(file.readAll(), QCryptographicHash::Sha1);
}

void tst_AADLUtils::tst_copyResourceFile()
{
    const QString path = QDir::tempPath();
    const QString destFilePath = path + QDir::separator() + QLatin1String("test.file");
    QFile::remove(destFilePath);

    QDirIterator it(QLatin1String(":/"), QDir::Files, QDirIterator::Subdirectories);
    QString filePath;
    while (it.hasNext()) {
        filePath = it.next();
        if (it.fileInfo().isFile() && it.fileInfo().size()) {
            break;
        }
    }
    if (!QFile::exists(filePath)) {
        return;
    }

    QVERIFY(shared::copyResourceFile(filePath, destFilePath));
    const auto hash = fileHash(destFilePath);

    while (it.hasNext()) {
        filePath = it.next();
        if (it.fileInfo().isFile() && it.fileInfo().size()) {
            break;
        }
    }
    if (!QFile::exists(filePath)) {
        return;
    }
    QVERIFY(!shared::copyResourceFile(filePath, destFilePath, shared::FileCopyingMode::Keep));
    QVERIFY(hash == fileHash(destFilePath));

    while (it.hasNext()) {
        filePath = it.next();
        if (it.fileInfo().isFile() && it.fileInfo().size()) {
            break;
        }
    }
    if (!QFile::exists(filePath)) {
        return;
    }
    QVERIFY(shared::copyResourceFile(filePath, destFilePath, shared::FileCopyingMode::Overwrite));
    QVERIFY(hash != fileHash(destFilePath));

    QFile::remove(destFilePath);
}

void tst_AADLUtils::tst_ensureDirExists()
{
    const QString path = QDir::tempPath() + QDir::separator() + QLatin1String("testFolder");
    QDir dir { path };
    QVERIFY(shared::ensureDirExists(path) == dir.exists());
    QVERIFY(dir.removeRecursively());
}

void tst_AADLUtils::tst_ensureFileExists()
{
    const QString destFilePath = QDir::tempPath() + QDir::separator() + QLatin1String("test.file");
    if (QFile::exists(destFilePath) && !QFile::remove(destFilePath)) {
        return;
    }
    const QString appName = QTest::currentAppName();
    QVERIFY(shared::ensureFileExists(destFilePath, appName));
    QVERIFY(fileHash(destFilePath) == fileHash(appName));

    QFile::remove(destFilePath);
}

void tst_AADLUtils::tst_isAncestorOf()
{
    ivm::IVFunction fn(QLatin1String("fn"));
    ivm::IVFunction sibling(QLatin1String("sibling"));
    QVERIFY(!shared::isAncestorOf(&fn, &sibling));
    QVERIFY(!shared::isAncestorOf(&sibling, &fn));

    ivm::IVFunction nestedFn(QLatin1String("nestedFn"), &fn);
    QVERIFY(shared::isAncestorOf(&fn, &nestedFn));
    QVERIFY(!shared::isAncestorOf(&nestedFn, &fn));

    ivm::IVInterface::CreationInfo ci;

    ci.type = ivm::IVInterface::InterfaceType::Required;
    ci.name = QLatin1String("reqIface");
    ci.function = &nestedFn;
    ivm::IVInterfaceRequired reqIface(ci);
    QVERIFY(shared::isAncestorOf<ivm::IVObject>(&fn, &reqIface));
    QVERIFY(shared::isAncestorOf<ivm::IVObject>(&nestedFn, &reqIface));
    QVERIFY(!shared::isAncestorOf<ivm::IVObject>(&reqIface, &fn));
    QVERIFY(!shared::isAncestorOf<ivm::IVObject>(&reqIface, &nestedFn));

    ci.type = ivm::IVInterface::InterfaceType::Provided;
    ci.name = QLatin1String("provIface");
    ci.function = &nestedFn;
    ivm::IVInterfaceProvided provIface(ci);
    QVERIFY(shared::isAncestorOf<ivm::IVObject>(&fn, &provIface));
    QVERIFY(shared::isAncestorOf<ivm::IVObject>(&nestedFn, &provIface));
    QVERIFY(!shared::isAncestorOf<ivm::IVObject>(&provIface, &fn));
    QVERIFY(!shared::isAncestorOf<ivm::IVObject>(&provIface, &nestedFn));
}

QTEST_APPLESS_MAIN(tst_AADLUtils)

#include "tst_aadlutils.moc"
