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

#pragma once

#include <QGraphicsTextItem>
#include <QLinearGradient>
#include <QRegularExpression>
#include <QTextOption>

namespace taste3 {

class TextGraphicsItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    explicit TextGraphicsItem(QGraphicsItem *parent = nullptr);

    QBrush background() const;
    void setBackgroundColor(const QColor &color);
    void setBackgroundGradient(const QLinearGradient &color);

    QColor frameColor() const;
    void setFrameColor(const QColor &color);

    qreal frameWidth() const;
    void setFrameWidth(qreal w);

    Qt::Alignment textAlignment() const;
    void setTextAlignment(Qt::Alignment alignment);

    void setTextWrapMode(QTextOption::WrapMode wrapMode);

    bool framed() const;
    void setFramed(bool to);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool isEditing() const;
    bool isEditable() const;
    void setEditable(bool editable);

    void enableEditMode();
    void disableEditMode();

    void setTextMargin(qreal margin);
    qreal textMargin() const;

    void selectText(bool select);

    qreal idealWidth() const;

    QString inputValidationPattern() const;
    void setInputValidationPattern(const QString &pattern);

    void setExplicitSize(const QSizeF &r);

    QRectF boundingRect() const override;

    void setSendClickEvent(bool send);

Q_SIGNALS:
    void edited(const QString &newText);
    void textChanged();
    void inputValidationPatternChanged(const QString &);
    /*!
       Send when setSendClickEvent() is set true, and the item was clicked.
     */
    void clicked();

protected Q_SLOTS:
    virtual void onContentsChange(int position, int charsRemoved, int charsAdded);

protected:
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    virtual bool validateInput(const QString &text) const;

    QPair<int, int> prepareSelectionRange(int desiredFrom, int desiredTo) const;

protected:
    QColor m_bgrColor = Qt::white;
    QLinearGradient m_gradient;
    QColor m_frameColor = Qt::black;
    qreal m_frameWidth = 0.5;
    bool m_showFrame = false;
    bool m_gradientUsed = false;
    bool m_editable = false;
    QString m_prevText;
    QRegularExpression m_inputValidator;
    QSizeF m_explicitSize;
    bool m_sendClickEvent = false;
    bool m_disableEditingGuard = false;
};

class NameItem : public TextGraphicsItem
{
    // This could be done on the caller side, but currently it's the only used
    // concretization, so let's wrapp it here to manage in a centralized manner.
    // And since it's just a single string customization I don't want to place
    // it into a separate file (at least, for now).

    Q_OBJECT
public:
    NameItem(QGraphicsItem *parent = nullptr)
        : TextGraphicsItem(parent)
    {
        // see the MSC spec or Msc.g4
        static const QLatin1String LETTER("A-Za-z");
        static const QLatin1String DECIMALDIGIT("\\d");
        static const QLatin1String UNDERLINE("_");
        static const QLatin1String FULLSTOP(".");
        static const QLatin1String MINUS("\\-");
        static const QLatin1String BACKQUOTE("`");
        static const QLatin1String SLASH("/");

        static const QString pattern =
                // NAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | MINUS | '`' | '/' )+ ;
                "[" + LETTER + DECIMALDIGIT + UNDERLINE + FULLSTOP + MINUS + BACKQUOTE + SLASH + "]";
        setInputValidationPattern(pattern);
    }
};

} // ns taste3
