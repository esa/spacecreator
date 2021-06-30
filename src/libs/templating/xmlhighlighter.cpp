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

#include "xmlhighlighter.h"

#include <QDebug>

namespace templating {

/**
 * @brief XMLHighlighter::XMLHighlighter ctor
 * @param parent
 */
XMLHighlighter::XMLHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // >text<
    QTextCharFormat xmlValueElementFormat;
    xmlValueElementFormat.setForeground(Qt::black);
    xmlValueElementFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral(">[^\n]*</"), QRegularExpression::InvertedGreedinessOption);
    rule.format = xmlValueElementFormat;
    m_highlightingRules.append(rule);

    // keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::red);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywords = QStringLiteral("(<\\?xml\\b|\\?>|</?|/?>)");
    rule.pattern = QRegularExpression(keywords);
    rule.format = keywordFormat;
    m_highlightingRules.append(rule);

    // <Text> </Text>
    QTextCharFormat xmlElementFormat;
    xmlElementFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("(?<=[<\\/])\\b[A-Za-z0-9_]+"));
    rule.format = xmlElementFormat;
    m_highlightingRules.append(rule);

    // < Text= >
    QTextCharFormat xmlAttributeFormat;
    xmlAttributeFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\=)"));
    rule.format = xmlAttributeFormat;
    m_highlightingRules.append(rule);

    // <!-- Text -->
    QTextCharFormat singleLineCommentFormat;
    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegularExpression(QStringLiteral("<!--[^\n]*-->"));
    rule.format = singleLineCommentFormat;
    m_highlightingRules.append(rule);

    // = "Text"
    QTextCharFormat valueFormat;
    valueFormat.setForeground(Qt::magenta);
    valueFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("(?<=[=>])\".*\""), QRegularExpression::InvertedGreedinessOption);
    rule.format = valueFormat;
    m_highlightingRules.append(rule);
}

/**
 * @brief XMLHighlighter::highlightBlock highlights text
 * @param text
 */
void XMLHighlighter::highlightBlock(const QString &text)
{
    // for every pattern
    for (const HighlightingRule &rule : m_highlightingRules) {
        QRegularExpression expression(rule.pattern);
        if (!expression.isValid()) {
            qWarning() << Q_FUNC_INFO << expression.errorString() << expression.pattern();
            continue;
        }
        QRegularExpressionMatchIterator it = expression.globalMatch(text);

        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            int index = match.capturedStart();
            int length = match.capturedLength();
            this->setFormat(index, length, rule.format);
        }
    }
}

}
