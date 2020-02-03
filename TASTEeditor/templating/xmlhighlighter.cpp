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
    rule.pattern = QRegExp(QLatin1String(">[^\n]*<"));
    rule.format = xmlValueElementFormat;
    m_highlightingRules.append(rule);

    // keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywords;
    keywords << QLatin1String("\\b?xml\\b") << QLatin1String("/>")
             << QLatin1String(">") << QLatin1String("<");
    foreach (const QString &keyword, keywords) {
        rule.pattern = QRegExp(keyword);
        rule.format = keywordFormat;
        m_highlightingRules.append(rule);
    }

    // <Text> </Text>
    QTextCharFormat xmlElementFormat;
    xmlElementFormat.setForeground(Qt::magenta);
    rule.pattern = QRegExp(QLatin1String("\\b[A-Za-z0-9_]+(?=[\\/>])"));
    rule.format = xmlElementFormat;
    m_highlightingRules.append(rule);

    // < Text= >
    QTextCharFormat xmlAttributeFormat;
    xmlAttributeFormat.setForeground(Qt::red);
    rule.pattern = QRegExp(QLatin1String("\\b[A-Za-z0-9_]+(?=\\=)"));
    rule.format = xmlAttributeFormat;
    m_highlightingRules.append(rule);

    // <!-- Text -->
    QTextCharFormat singleLineCommentFormat;
    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegExp(QLatin1String("<!--[^\n]*-->"));
    rule.format = singleLineCommentFormat;
    m_highlightingRules.append(rule);

    // = "Text"
    QColor valueColor(128, 0, 255);
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
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);

        while (index >= 0) {
            int length = expression.matchedLength();
            this->setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    this->setCurrentBlockState(0);
    int startIndex = 0;

    if (this->previousBlockState() != 1)
        startIndex = m_valueStartExpression.indexIn(text);

    while (startIndex >= 0)
    {
        int endIndex = m_valueEndExpression.indexIn(text, startIndex);
        int commentLength;

        if (endIndex == -1) {
            this->setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else {
            commentLength = endIndex - startIndex + m_valueEndExpression.matchedLength();
        }

        this->setFormat(startIndex, commentLength, m_valueFormat);
        startIndex = m_valueStartExpression.indexIn(text, startIndex + commentLength);
    }
}

} // ns processing
} // ns taste3
