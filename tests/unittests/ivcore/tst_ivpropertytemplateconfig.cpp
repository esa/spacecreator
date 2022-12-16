/*
   Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivpropertytemplateconfig.h"

#include <QDomDocument>
#include <QTest>

class tst_IVPropertyTemplateConfig : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test_languageDirectory();
};

void tst_IVPropertyTemplateConfig::test_languageDirectory()
{
    QString xmlContent = R"(<Attrs>
    <Attr label="Language" name="language" visible="false">
        <Type>
            <Enumeration defaultValue="SDL">
                <Entry value="Ada" folder_name="Ada"/>
                <Entry value="C++" dummy="foo"/>
                <Entry value="Simulink" folder_name="SIMULINK"/>
            </Enumeration>
        </Type>
    </Attr></Attrs>)";
    auto config = ivm::IVPropertyTemplateConfig::instance();
    config->initFromData(xmlContent);

    QString dir = config->languageDirectory("Ada");
    QCOMPARE(dir, "Ada");
    dir = config->languageDirectory("C++");
    QCOMPARE(dir, "C++");
    dir = config->languageDirectory("Simulink");
    QCOMPARE(dir, "SIMULINK");
    dir = config->languageDirectory("Carbon");
    QCOMPARE(dir, "Carbon");

    config->cleanup();
}

QTEST_APPLESS_MAIN(tst_IVPropertyTemplateConfig)

#include "tst_ivpropertytemplateconfig.moc"
