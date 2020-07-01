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
class tst_TextItem : public TextItemBaseTest
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

msc::TextItem *tst_TextItem::concreteTestItem() const
{
    msc::TextItem *item = new TextItem();
    return item;
}

QString tst_TextItem::allowedText() const
{
    static const QString allowedSymbols("1234567890-=QWERTYUIOP{}|asdfghjkl;'zxcvbnm,./~!@#$%^&*()");
    return allowedSymbols;
}

QString tst_TextItem::disallowedText() const
{
    return QString();
}

void tst_TextItem::init()
{
    vstest::saveMousePosition();
    TextItemBaseTest::initImpl();
}

void tst_TextItem::cleanup()
{
    TextItemBaseTest::cleanupImpl();
    vstest::restoreMousePosition();
}

void tst_TextItem::testAllowedInput()
{
    testAllowedInputImpl();
}

void tst_TextItem::testDisallowedInput()
{
    testDisallowedInputImpl();
}

void tst_TextItem::testMixedInput()
{
    testDisallowedInputImpl();
}

void tst_TextItem::testSelectAll()
{
    testSelectAllImpl();
}

void tst_TextItem::testUndoRedo()
{
    testUndoRedoImpl();
}

void tst_TextItem::testCutPaste()
{
    testCutPasteImpl();
}

void tst_TextItem::testPasteInStart()
{
    testPasteInStartImpl();
}

void tst_TextItem::testPasteInMiddle()
{
    testPasteInMiddleImpl();
}

void tst_TextItem::testPasteInEnd()
{
    testPasteInEndImpl();
}

void tst_TextItem::testPasteOverSelection()
{
    testPasteOverSelectionImpl();
}

QTEST_MAIN(tst_TextItem)

#include "tst_textitem.moc"
