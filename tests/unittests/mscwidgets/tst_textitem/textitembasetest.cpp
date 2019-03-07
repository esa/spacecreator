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

#include "textitembasetest.h"

#include <QApplication>
#include <QClipboard>

TextItemBaseTest::TextItemBaseTest()
    : QObject()
{
}

QString TextItemBaseTest::allowedText() const
{
    return QString();
}

QString TextItemBaseTest::disallowedText() const
{
    return QString();
}

void TextItemBaseTest::initImpl()
{
    m_view = new QGraphicsView();
    m_view->setScene(new QGraphicsScene(m_view));

    m_item = concreteTestItem();
    m_item->setEditable(true);
    m_item->setFramed(true);
    m_item->setPos(m_view->viewport()->rect().center());
    m_view->scene()->addItem(m_item);

    // This could be usefull during local development,
    // but fails the test in CI environment:
    if (isLocalBuild)
        m_view->show();
}

void TextItemBaseTest::cleanupImpl()
{
    m_view->scene()->clear();
    delete m_view;
}

void TextItemBaseTest::setText(const QString &input)
{
    m_item->setPlainText(input);
}

void TextItemBaseTest::typeText(const QString &input)
{
    QTest::mouseMove(m_view->viewport());
    QTest::mouseDClick(m_view->viewport(), Qt::LeftButton, Qt::NoModifier);
    QTest::keyClicks(m_view->viewport(), input);
}

QTextCursor TextItemBaseTest::textCursor() const
{
    QTextCursor c;

    if (m_item) {
        c = m_item->textCursor();
        if (c.isNull())
            c = QTextCursor(m_item->document());
    }

    return c;
}

void TextItemBaseTest::testAllowedInputImpl()
{
    const QString &allowed = allowedText();

    setText(allowed);
    QCOMPARE(m_item->toPlainText(), allowed);

    cleanupImpl();
    initImpl();

    typeText(allowed);
    QCOMPARE(m_item->toPlainText(), allowed);
}

void TextItemBaseTest::testDisallowedInputImpl()
{
    const QString &disallowed = disallowedText();

    setText(disallowed);
    QCOMPARE(m_item->toPlainText(), QString());

    cleanupImpl();
    initImpl();

    typeText(disallowed);
    QCOMPARE(m_item->toPlainText(), QString());
}

void TextItemBaseTest::testMixedInputImpl()
{
    const QString &allowed = allowedText();
    const QString &disallowed = disallowedText();

    QString input;
    for (int i = 0; i < qMin(allowed.length(), disallowed.length()); ++i) {
        input += allowed.at(i);
        input += disallowed.at(i);
    }
    input.prepend(disallowed);
    input.append(disallowed);

    setText(input);
    QCOMPARE(m_item->toPlainText(), allowed);

    cleanupImpl();
    initImpl();

    typeText(input);

    QCOMPARE(m_item->toPlainText(), allowed);
}

void TextItemBaseTest::testSelectAllImpl()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    const QString &allowed = allowedText();

    m_item->setPlainText(allowed + allowed);

    QTest::mouseMove(m_view->viewport());
    QTest::mouseDClick(m_view->viewport(), Qt::LeftButton, Qt::NoModifier);
    QTest::keySequence(m_view, QKeySequence::SelectAll);
    QTest::keyClicks(m_view->viewport(), allowed);

    QCOMPARE(m_item->toPlainText(), allowed);
#else
    QSKIP(qPrintable(QString("Qt version 5.10 or newer required for this test.")));
#endif
}

void TextItemBaseTest::testUndoRedoImpl()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    const QString &allowed = allowedText();

    typeText(allowed);
    QCOMPARE(m_item->toPlainText(), allowed);

    QVERIFY(m_item->document()->isUndoAvailable());
    QTest::keySequence(m_view->viewport(), QKeySequence::Undo);
    QCOMPARE(m_item->toPlainText(), QString());

    QVERIFY(m_item->document()->isRedoAvailable());
    QTest::keySequence(m_view->viewport(), QKeySequence::Redo);
    QCOMPARE(m_item->toPlainText(), allowed);
#else
    QSKIP(qPrintable(QString("Qt version 5.10 or newer required for this test.")));
#endif
}

void TextItemBaseTest::testCutPasteImpl()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    const QString &allowed = allowedText();

    typeText(allowed);
    QCOMPARE(m_item->toPlainText(), allowed);

    QTest::keySequence(m_view->viewport(), QKeySequence::SelectAll);
    QTest::keySequence(m_view->viewport(), QKeySequence::Cut);
    QCOMPARE(m_item->toPlainText(), QString());

    QTest::keySequence(m_view->viewport(), QKeySequence::Paste);
    QCOMPARE(m_item->toPlainText(), allowed);
#else
    QSKIP(qPrintable(QString("Qt version 5.10 or newer required for this test.")));
#endif
}

void TextItemBaseTest::testPasteInStartImpl()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    QTest::mouseMove(m_view->viewport());
    QTest::mouseDClick(m_view->viewport(), Qt::LeftButton, Qt::NoModifier);

    const QString &allowed = allowedText();
    setText(allowed);
    QCOMPARE(m_item->toPlainText(), allowed);

    QApplication::clipboard()->setText(allowed);

    QTextCursor c = textCursor();
    c.setPosition(0, QTextCursor::MoveAnchor);
    m_item->setTextCursor(c);

    QTest::keySequence(m_view, QKeySequence::Paste);

    QCOMPARE(m_item->toPlainText(), allowed + allowed);

#else
    QSKIP(qPrintable(QString("Qt version 5.10 or newer required for this test.")));
#endif
}

void TextItemBaseTest::testPasteInMiddleImpl()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    QTest::mouseMove(m_view->viewport());
    QTest::mouseDClick(m_view->viewport(), Qt::LeftButton, Qt::NoModifier);

    const QString &allowed = allowedText();
    const int allowedMiddle = allowed.length() / 2;
    setText(allowed);
    QCOMPARE(m_item->toPlainText(), allowed);

    QApplication::clipboard()->setText(allowed);

    QTextCursor c = textCursor();
    c.setPosition(allowedMiddle, QTextCursor::MoveAnchor);
    m_item->setTextCursor(c);

    QTest::keySequence(m_view, QKeySequence::Paste);

    QCOMPARE(m_item->toPlainText(),
             allowed.left(allowedMiddle) + allowed + allowed.right(allowed.length() - allowedMiddle));

#else
    QSKIP(qPrintable(QString("Qt version 5.10 or newer required for this test.")));
#endif
}

void TextItemBaseTest::testPasteInEndImpl()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    QTest::mouseMove(m_view->viewport());
    QTest::mouseDClick(m_view->viewport(), Qt::LeftButton, Qt::NoModifier);

    const QString &allowed = allowedText();
    const int allowedEnd = allowed.length();
    setText(allowed);
    QCOMPARE(m_item->toPlainText(), allowed);

    QApplication::clipboard()->setText(allowed);

    QTextCursor c = textCursor();
    c.setPosition(allowedEnd, QTextCursor::MoveAnchor);
    m_item->setTextCursor(c);

    QTest::keySequence(m_view, QKeySequence::Paste);

    QCOMPARE(m_item->toPlainText(), allowed + allowed);

#else
    QSKIP(qPrintable(QString("Qt version 5.10 or newer required for this test.")));
#endif
}

void TextItemBaseTest::testPasteOverSelectionImpl()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    QTest::mouseMove(m_view->viewport());
    QTest::mouseDClick(m_view->viewport(), Qt::LeftButton, Qt::NoModifier);

    const QString &allowed = allowedText();
    const int allowedEnd = allowed.length() - 1;
    setText(allowed);
    QCOMPARE(m_item->toPlainText(), allowed);

    QApplication::clipboard()->setText(allowed);

    QTextCursor c = textCursor();
    c.setPosition(allowedEnd - 1, QTextCursor::MoveAnchor);
    c.setPosition(allowedEnd, QTextCursor::KeepAnchor);
    m_item->setTextCursor(c);

    QTest::keySequence(m_view, QKeySequence::Paste);

    QCOMPARE(m_item->toPlainText(), allowed.left(allowedEnd - 1) + allowed + allowed.right(1));

#else
    QSKIP(qPrintable(QString("Qt version 5.10 or newer required for this test.")));
#endif
}
