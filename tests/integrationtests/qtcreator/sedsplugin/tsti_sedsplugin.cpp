/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "common.h"
#include "ivmodel.h"
#include "modelloader.h"
#include "sedspluginwindow.h"

#include <QObject>
#include <QTest>
#include <QtTest/qtest.h>
#include <QtTest/qtestkeyboard.h>
#include <QtTest/qtestsystem.h>
#include <qdir.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <sedsplugin.h>
#include <shared/sharedlibrary.h>

namespace sedsplugin {

class SedsPlugin : public QObject
{
    Q_OBJECT

private slots:
    void testImportInterfaceView();
    void initTestCase();
    void init();

private:
    QString m_mainDir;
    QString m_ivConfigFile;
};

static const QString resourcesDir = "resources";
static const QString testProjectDir = "testProject";
static const QString inputEdsDir = "input-eds";
static const QString separator = QDir::separator();

void SedsPlugin::initTestCase()
{
    shared::initSharedLibrary();

    m_mainDir = QDir::currentPath();
    m_ivConfigFile = shared::interfaceCustomAttributesFilePath();
}

void SedsPlugin::init()
{
    QDir::setCurrent(m_mainDir);
}

static QString makeIvPath(const QString &ivFilename)
{
    return QString("%1%2%3%4%5%6%7")
            .arg("..")
            .arg(separator)
            .arg(resourcesDir)
            .arg(separator)
            .arg(inputEdsDir)
            .arg(separator)
            .arg(ivFilename);
}

static unsigned int countFunctionsInIvModel(ivm::IVModel *const model)
{
    if (model == nullptr) {
        return 0;
    }

    unsigned int functionsCount = 0;
    for (auto &srcIvObject : model->visibleObjects()) {
        auto *const srcFunction = dynamic_cast<ivm::IVFunction *>(srcIvObject);
        if (srcFunction == nullptr) {
            continue;
        } else {
            functionsCount++;
        }
    }
    return functionsCount;
}

void SedsPlugin::testImportInterfaceView()
{
    QDir::setCurrent(resourcesDir);
    QDir::setCurrent(testProjectDir);

    const auto ivModelRaw = spctr::ModelLoader::loadIvModel(m_ivConfigFile, "interfaceview.xml");
    const auto ivModel = dynamic_cast<ivm::IVModel *>(ivModelRaw.get());
    const auto functionsBefore = countFunctionsInIvModel(ivModel);

    spctr::SedsPlugin plugin;
    plugin.importInterfaceView("test_provided_interfaces.xml");

    const auto functionsAfter = countFunctionsInIvModel(ivModel);

    qDebug() << "before: " << functionsBefore;
    qDebug() << "after: " << functionsAfter;
}

} // namespace sedsplugin

QTEST_MAIN(sedsplugin::SedsPlugin)
#include "tsti_sedsplugin.moc"
