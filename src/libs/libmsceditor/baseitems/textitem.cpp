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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "textitem.h"

#include "mscreader.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QCompleter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QPainter>
#include <QStringListModel>
#include <QTextCursor>
#include <QTextDocument>

namespace msc {

static const QColor ERROR_BACKGROUND_COLOR(255, 128, 128);

/*!
  \class msc::TextItem
  \brief Text holder with customizable alignment, background and frame.

  \ingroup MscWidgets

*/

TextItem::TextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    setTextAllignment(Qt::AlignCenter);
    setTextInteractionFlags(Qt::NoTextInteraction);
    connect(document(), &QTextDocument::contentsChange, this, &TextItem::onContentsChange);
    setInputValidationPattern(QString());

    connect(this, &TextItem::textChanged, this, &TextItem::updateCompleterText);
}

QBrush TextItem::background() const
{
    if (m_gradientUsed) {
        const QRectF &bounds = boundingRect();
        QLinearGradient gradient(m_gradient);
        gradient.setStart(bounds.topLeft());
        gradient.setFinalStop(bounds.bottomRight());
        return m_gradient;
    }
    return m_bgrColor;
}

void TextItem::setBackgroundColor(const QColor &color)
{
    if (m_bgrColor == color)
        return;

    m_gradientUsed = false;

    m_bgrColor = color;
    update();
}

void TextItem::setBackgroundGradient(const QLinearGradient &gradient)
{
    if (gradient == m_gradient)
        return;

    m_gradientUsed = true;

    m_gradient = gradient;
    update();
}

QColor TextItem::frameColor() const
{
    return m_frameColor;
}

void TextItem::setFrameColor(const QColor &color)
{
    if (color == m_frameColor)
        return;

    m_frameColor = color;
    update();
}

qreal TextItem::frameWidth() const
{
    return m_frameWidth;
}

void TextItem::setFrameWidth(qreal w)
{
    if (qFuzzyCompare(m_frameWidth, w))
        return;

    m_frameWidth = w;
}

Qt::Alignment TextItem::textAlignment() const
{
    return document()->defaultTextOption().alignment();
}

void TextItem::setTextAllignment(Qt::Alignment alignment)
{
    QTextOption txtOpt = document()->defaultTextOption();
    txtOpt.setAlignment(alignment);
    document()->setDefaultTextOption(txtOpt);
}

void TextItem::setTextWrapMode(QTextOption::WrapMode wrapMode)
{
    QTextOption txtOpt = document()->defaultTextOption();

    txtOpt.setWrapMode(wrapMode);
    document()->setDefaultTextOption(txtOpt);
}

bool TextItem::framed() const
{
    return m_showFrame;
}

void TextItem::setFramed(bool to)
{
    if (to == m_showFrame)
        return;

    m_showFrame = to;
    update();
}

void TextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    const QRectF body = boundingRect();
    QBrush backColor = background();
    if (!m_textIsValid) {
        backColor = QBrush(ERROR_BACKGROUND_COLOR);
    }
    painter->fillRect(body, backColor);

    if (framed()) {
        QPen pen(painter->pen());
        pen.setWidthF(frameWidth());
        pen.setColor(frameColor());
        painter->setPen(pen);
        painter->drawRect(body);
    }

    painter->restore();

    QGraphicsTextItem::paint(painter, option, widget);
}

bool TextItem::isEditable() const
{
    return m_editable;
}

bool TextItem::isEditing() const
{
    return isEditable() && textInteractionFlags() != Qt::NoTextInteraction;
}

void TextItem::setEditable(bool editable)
{
    if (editable == m_editable)
        return;

    m_editable = editable;
}

void TextItem::enableEditMode()
{
    if (!m_editable)
        return;

    m_prevText = toPlainText();

    selectText(true);

    setTextInteractionFlags(Qt::TextEditorInteraction | Qt::TextEditable);
    setFocus();
}

void TextItem::disableEditMode()
{
    if (m_disableEditingGuard)
        return;

    m_disableEditingGuard = true;

    if (m_prevText != toPlainText() || !m_textIsValid) {
        if (toPlainText().isEmpty() || !m_textIsValid) {
            setPlainText(m_prevText);
            setTextWidth(idealWidth());
            adjustSize();
            Q_EMIT textChanged();
        } else {
            Q_EMIT edited(toPlainText());
        }
    }
    selectText(false);
    m_prevText.clear();
    setTextInteractionFlags(Qt::NoTextInteraction);

    m_disableEditingGuard = false;
}

void TextItem::focusOutEvent(QFocusEvent *event)
{
    removeCompleter();
    disableEditMode();
    QGraphicsTextItem::focusOutEvent(event);
}

void TextItem::keyPressEvent(QKeyEvent *event)
{
    bool accepted(false);
    if (isEditing())
        switch (event->key()) {
        case Qt::Key_Escape: {
            accepted = true;
            const QString newText = toPlainText();
            if (m_prevText != newText) {
                QSignalBlocker avoidValidation(document());
                setPlainText(m_prevText);
            }
            break;
        }
        case Qt::Key_Return: {
            accepted = event->modifiers() == Qt::NoModifier;
            break;
        }
        case Qt::Key_Enter: {
            accepted = event->modifiers() == Qt::KeypadModifier;
            break;
        }
        default: {
            break;
        }
        }

    if (accepted) {
        event->accept();
        disableEditMode();
        clearFocus();
        return;
    }

    QGraphicsTextItem::keyPressEvent(event);

    if (m_explicitSize.isValid()
            && (QGraphicsTextItem::boundingRect().width() > m_explicitSize.width()
                    || QGraphicsTextItem::boundingRect().height() > m_explicitSize.height())) {
        prepareGeometryChange();
        m_explicitSize = QGraphicsTextItem::boundingRect().size();
    }

    Q_EMIT textChanged();
}

void TextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mousePressEvent(event);
    if (m_sendClickEvent) {
        Q_EMIT clicked();
        event->accept();
    }
}

void TextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseReleaseEvent(event);
}

void TextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    enableEditMode();
    QGraphicsTextItem::mouseDoubleClickEvent(event);
    Q_EMIT doubleClicked();
}

/*!
   \brief TextItem::selectText
   \param select if true, the entire text will be selected. If false, the the selection will be cleared
 */
void TextItem::selectText(bool select)
{
    QTextCursor txtCursor = textCursor();
    if (select)
        txtCursor.select(QTextCursor::Document);
    else if (txtCursor.hasSelection())
        txtCursor.clearSelection();
    setTextCursor(txtCursor);
}

void TextItem::setTextMargin(qreal margin)
{
    document()->setDocumentMargin(margin);
}
qreal TextItem::textMargin() const
{
    return document()->documentMargin();
}

qreal TextItem::idealWidth() const
{
    return document()->idealWidth();
}

/*!
  \brief TextItem::inputValidationPattern
  QRegularExpression pattern used for input validation on the fly. If empty,
  text is not validated during input.
*/
QString TextItem::inputValidationPattern() const
{
    return m_inputValidator.pattern();
}

/*!
  \brief TextItem::setInputValidationPattern
  Sets pattern for QRegularExpression used for input validation on the fly.
  Pass an empty string to disable text validation during input.
*/
void TextItem::setInputValidationPattern(const QString &pattern)
{
    const QString patternForced(pattern.isEmpty() ? ".*" : pattern);
    if (m_inputValidator.pattern() == patternForced)
        return;

    m_inputValidator.setPattern(patternForced);
    Q_EMIT inputValidationPatternChanged(inputValidationPattern());
}

bool TextItem::validateInput(const QString &text) const
{
    if (inputValidationPattern().isEmpty() || text.isEmpty() || !m_inputValidator.isValid())
        return false;

    const QRegularExpressionMatch &matched = m_inputValidator.match(text);
    return matched.hasMatch() && matched.captured() == text;
}

bool TextItem::validateText(const QString &text) const
{
    if (m_mscValidationTest.isEmpty()) {
        return true;
    }

    MscReader reader;
    QStringList errors;
    QString testDoc = m_mscValidationTest.arg(text);
    try {
        reader.parseText(testDoc, &errors);
    } catch (...) {
        return false;
    }
    return errors.isEmpty();
}

QPair<int, int> TextItem::prepareSelectionRange(int desiredFrom, int desiredTo) const
{
    QPair<int, int> res(0, 0);

    auto validatePos = [](int pos, int totalLength) {
        if (pos) {
            if (pos >= totalLength)
                pos = totalLength - 1;
            if (pos < 0)
                pos = 0;
        }
        return pos;
    };

    if (QTextDocument *doc = document()) {
        const int totalChars = doc->characterCount();
        if (totalChars > 0) {
            res.first = validatePos(qMax(0, desiredFrom), totalChars);
            res.second = validatePos(qMin(totalChars - 1, desiredTo), totalChars);
        }
    }

    return res;
}

void TextItem::onContentsChange(int position, int charsRemoved, int charsAdded)
{
    Q_UNUSED(charsRemoved);

    if (0 == charsAdded) {
        checkTextValidity();
        return;
    }

    // QTextDocument automatically appends the 'PARAGRAPH SEPARATOR' (U+2029) char
    // which is not actually an input (and does not affect the view)
    auto isAutoParagraphSeparator = [](const QChar &c, int pos, int length) {
        const bool paragraph = c == QChar(0x2029);
        const bool last = pos == length - 1;
        return paragraph && last;
    };

    QString inputString, inputStringValid;
    const int inputLength = position + charsAdded;
    for (int i = position; i < inputLength; ++i) {
        const QChar &newChar = document()->characterAt(i);
        if (isAutoParagraphSeparator(newChar, i, inputLength)) {
            break;
        }

        inputString.append(newChar);
        if (!validateInput(newChar) && m_filterInvalidText) {
            const QString wrnMsg("Invalid characted '%1' [%2] at #%3 filtered out in '%4'");
            qWarning() << wrnMsg.arg(newChar).arg(newChar.unicode()).arg(i + 1).arg(inputString);
        } else {
            inputStringValid.append(newChar);
        }
    }

    if (inputStringValid == inputString) {
        checkTextValidity();
        return;
    }

    QTextCursor currCursor = textCursor();
    if (currCursor.isNull()) {
        // By default, if the item's text has not been set, this property contains a null text cursor;
        // otherwise it contains a text cursor placed at the start of the item's document.
        currCursor = QTextCursor(document());
    }

    if (currCursor.hasSelection())
        currCursor.clearSelection();
    const QPair<int, int> &selectionRange = prepareSelectionRange(position, inputLength);
    currCursor.setPosition(selectionRange.first, QTextCursor::MoveAnchor);
    currCursor.setPosition(selectionRange.second, QTextCursor::KeepAnchor);
    currCursor.insertText(inputStringValid);

    QSignalBlocker suppressContentsChange(document());
    setTextCursor(currCursor);
    checkTextValidity();
}

void TextItem::updateCompleterText()
{
    if (!m_completer) {
        return;
    }

    m_completer->setCompletionPrefix(toPlainText());
    QAbstractItemView *popup = m_completer->popup();
    if (m_completer->completionModel()->rowCount() > 0) {
        popup->show();
    } else {
        popup->hide();
    }
}

void TextItem::setExplicitSize(const QSizeF &size)
{
    if (m_explicitSize != size) {
        m_explicitSize = size;
        if (!m_explicitSize.isEmpty()) {
            setTextMargin(textMargin());
            setTextWidth(m_explicitSize.width());
        }
        update();
    }
}

QRectF TextItem::boundingRect() const
{
    return m_explicitSize.isEmpty() ? QGraphicsTextItem::boundingRect() : QRectF(QPointF(0., 0.), m_explicitSize);
}

/*!
   If set to true, emits the clicked() signal when clicked. But consumes the mouse event.
   So other items don't receive the event.
 */
void TextItem::setSendClickEvent(bool send)
{
    m_sendClickEvent = send;
}

bool TextItem::textIsValid() const
{
    return m_textIsValid;
}

/*!
   Sets the text to check if the user input is valid. The text is a MSC text with \em %1 as a placeholder for the text.
   For the validation the MSC parser is used.
   Example:
   \code
    QString("msc c1;instance i1 comment '%1';endinstance;endmsc;")
   \endcode
 */
void TextItem::setMscValidationTest(const QString &text)
{
    m_mscValidationTest = text;
}

/*!
   Updates and show the name completer with all function names, if there are (matching) items
 */
void TextItem::updateCompleter(const QStringList &completionList)
{
    if (completionList.isEmpty()) {
        removeCompleter();
    }

    if (m_completer == nullptr) {
        m_completer = new QCompleter(completionList, this);
        m_completer->setCompletionMode(QCompleter::PopupCompletion);
        m_completer->setFilterMode(Qt::MatchContains);
        m_completer->setCaseSensitivity(Qt::CaseInsensitive);
        connect(m_completer, qOverload<const QString &>(&QCompleter::highlighted), this, &msc::TextItem::setPlainText);
    }

    if (auto stringModel = qobject_cast<QStringListModel *>(m_completer->model())) {
        if (stringModel->stringList() != completionList) {
            stringModel->setStringList(completionList);
        }
    }

    m_completer->setCompletionPrefix(toPlainText());

    QPoint textPos;
    QList<QGraphicsView *> views = scene()->views();
    if (!views.isEmpty()) {
        QGraphicsView *view = views[0];
        m_completer->setWidget(view);
        QPointF sceneP = mapToScene(boundingRect().bottomLeft());
        QPoint viewP = view->mapFromScene(sceneP);
        textPos = view->viewport()->mapToGlobal(viewP);
    }
    QAbstractItemView *popup = m_completer->popup();
    QRect popRect = popup->geometry();
    popRect.moveTopLeft(textPos);
    popup->setGeometry(popRect);
    if (m_completer->completionModel()->rowCount() > 0) {
        popup->show();
    } else {
        popup->hide();
    }
}

void TextItem::removeCompleter()
{
    if (m_completer) {
        m_completer->deleteLater();
        m_completer = nullptr;
    }
}

void TextItem::checkTextValidity()
{
    bool valid = validateText(toPlainText());
    if (valid == m_textIsValid) {
        return;
    }

    m_textIsValid = valid;
    Q_EMIT textIsValidChanged();
}

} // namespace msc
