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

#include "syntheticinteraction.h"
#include "textitembasetest.h"

#include <QtTest>

using namespace msc;
class tst_NameItem : public TextItemBaseTest
{
    Q_OBJECT

protected:
    msc::TextItem *concreteTestItem() const override;
    QString allowedText() const override;
    QString disallowedText() const override;

private Q_SLOTS:
    void init();
    void cleanup();

    void testAllowedInput();
    void testDisallowedInput();
    void testMixedInput();

    void testSelectAll();
    void testUndoRedo();
    void testCutPaste();

    void testPasteInStart();
    void testPasteInMiddle();
    void testPasteInEnd();
    void testPasteOverSelection();
};

msc::TextItem *tst_NameItem::concreteTestItem() const
{
    msc::TextItem *item = new NameItem();
    return item;
}

QString tst_NameItem::allowedText() const
{
    static const QString allowedSymbols("ABCdef0123_.-`/");
    return allowedSymbols;
}

QString tst_NameItem::disallowedText() const
{
    static const QString disallowedSymbols("~!@#$%^&*()+ ={}[]:;'\",<>|\\");
    return disallowedSymbols;
}

void tst_NameItem::init()
{
    vstest::saveMousePosition();
    TextItemBaseTest::initImpl();
}

void tst_NameItem::cleanup()
{
    TextItemBaseTest::cleanupImpl();
    vstest::restoreMousePosition();
}

void tst_NameItem::testAllowedInput()
{
    testAllowedInputImpl();
}

void tst_NameItem::testDisallowedInput()
{
    testDisallowedInputImpl();
}

void tst_NameItem::testMixedInput()
{
    testDisallowedInputImpl();
}

void tst_NameItem::testSelectAll()
{
    testSelectAllImpl();
}

void tst_NameItem::testUndoRedo()
{
    testUndoRedoImpl();
}

void tst_NameItem::testCutPaste()
{
    testCutPasteImpl();
}

void tst_NameItem::testPasteInStart()
{
    testPasteInStartImpl();
}

void tst_NameItem::testPasteInMiddle()
{
    testPasteInMiddleImpl();
}

void tst_NameItem::testPasteInEnd()
{
    testPasteInEndImpl();
}

void tst_NameItem::testPasteOverSelection()
{
    testPasteOverSelectionImpl();
}

QTEST_MAIN(tst_NameItem)

#include "tst_nameitem.moc"
