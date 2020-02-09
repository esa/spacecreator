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

namespace taste3 {
namespace templating {

XMLHighlighter::XMLHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // >text<
    QTextCharFormat xmlValueElementFormat;
    xmlValueElementFormat.setForeground(Qt::black);
    xmlValueElementFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QLatin1String(">[^\n]*<"));
    rule.format = xmlValueElementFormat;
    m_highlightingRules.append(rule);

    // keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::red);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywords;
    keywords << QLatin1String("\\b?xml\\b") << QLatin1String("/>")
             << QLatin1String("</") << QLatin1String(">") << QLatin1String("<");
    foreach (const QString &keyword, keywords) {
        rule.pattern = QRegularExpression(keyword);
        rule.format = keywordFormat;
        m_highlightingRules.append(rule);
    }

    // <Text> </Text>
    QTextCharFormat xmlElementFormat;
    xmlElementFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QLatin1String("(?<=[<\\/])\\b[A-Za-z0-9_]+"));
    rule.format = xmlElementFormat;
    m_highlightingRules.append(rule);

    // < Text= >
    QTextCharFormat xmlAttributeFormat;
    xmlAttributeFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QLatin1String("\\b[A-Za-z0-9_]+(?=\\=)"));
    rule.format = xmlAttributeFormat;
    m_highlightingRules.append(rule);

    // <!-- Text -->
    QTextCharFormat singleLineCommentFormat;
    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegularExpression(QLatin1String("<!--[^\n]*-->"));
    rule.format = singleLineCommentFormat;
    m_highlightingRules.append(rule);

    // = "Text"
    QColor valueColor(Qt::magenta);
    m_valueFormat.setForeground(valueColor);
    m_valueFormat.setFontWeight(QFont::Bold);
    m_valueStartExpression.setPattern(QLatin1String("\""));
    m_valueEndExpression.setPattern(QLatin1String("\"(?=[\\s></])"));
}

void taste3::templating::XMLHighlighter::highlightBlock(const QString &text)
{
    //for every pattern
    foreach (const HighlightingRule &rule, m_highlightingRules)
    {
        QRegularExpression expression(rule.pattern);
        QRegularExpressionMatchIterator it = expression.globalMatch(text);

        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            int index = match.capturedStart();
            int length = match.capturedLength();
            this->setFormat(index, length, rule.format);
        }
    }

    setCurrentBlockState(0);
    int startIndex = 0;

    if (previousBlockState() != 1)
        startIndex = text.indexOf(m_valueStartExpression);

    while (startIndex >= 0)
    {
        QRegularExpressionMatch endMatch;
        int endIndex = text.indexOf(m_valueEndExpression, startIndex, &endMatch);
        int commentLength;

        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + endMatch.capturedLength();
        }

        setFormat(startIndex, commentLength, m_valueFormat);
        startIndex = text.indexOf(m_valueStartExpression, startIndex + commentLength);
    }
}

} // ns processing
} // ns taste3
