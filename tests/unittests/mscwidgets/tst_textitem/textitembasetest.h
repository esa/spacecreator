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

#pragma once

#include "baseitems/textitem.h"

#include <QGraphicsView>
#include <QObject>
#include <QPointer>
#include <QTextCursor>
#include <QTextDocument>
#include <QtTest>

class TextItemBaseTest : public QObject
{
    Q_OBJECT
public:
protected:
    TextItemBaseTest();

    bool isLocalBuild = false;

    QPointer<QGraphicsView> m_view;
    QPointer<msc::TextItem> m_item;

    virtual msc::TextItem *concreteTestItem() const = 0;
    virtual void initImpl();
    virtual void cleanupImpl();

    virtual QString allowedText() const;
    virtual QString disallowedText() const;

    void setText(const QString &input);
    void typeText(const QString &inputd);

    QTextCursor textCursor() const;

    void testAllowedInputImpl();
    void testDisallowedInputImpl();
    void testMixedInputImpl();

    void testSelectAllImpl();
    void testUndoRedoImpl();
    void testCutPasteImpl();

    void testPasteInStartImpl();
    void testPasteInMiddleImpl();
    void testPasteInEndImpl();
    void testPasteOverSelectionImpl();
};
