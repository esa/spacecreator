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

#include "templatehighlighter.h"

namespace taste3 {
namespace templating {

TemplateHighlighter::TemplateHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywords = QStringLiteral("\\s+(include|for|in|endfor|comment|endcomment|if|else|endif)\\s+");
    rule.pattern = QRegularExpression(keywords);
    rule.format = keywordFormat;
    m_highlightingRules.append(rule);

    // brackets
    QTextCharFormat bracketsFormat;
    bracketsFormat.setForeground(Qt::red);
    const QString brackets = QStringLiteral("({{|}}|{%|%})");
    rule.pattern = QRegularExpression(brackets);
    rule.format = bracketsFormat;
    m_highlightingRules.append(rule);

    // include
    QTextCharFormat includeFormat;
    includeFormat.setForeground(Qt::magenta);
    includeFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QLatin1String("(?<=\\binclude\\b)\\s+\".+\""));
    rule.format = includeFormat;
    m_highlightingRules.append(rule);

    // Comment
    m_commentFormat.setForeground(Qt::darkGray);
    m_commentStartExpression.setPattern(QLatin1String("{%\\s*comment\\s*%}"));
    m_commentEndExpression.setPattern(QLatin1String("{%\\s*endcomment\\s*%}"));
}

void TemplateHighlighter::highlightBlock(const QString &text)
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

    if (previousBlockState() != 1) {
        QRegularExpressionMatch startMatch = m_commentStartExpression.match(text);
        startIndex = startMatch.hasMatch() ? startMatch.capturedEnd() : -1;
    }

    while (startIndex >= 0)
    {
        QRegularExpressionMatch endMatch;
        int endIndex = text.indexOf(m_commentEndExpression, startIndex, &endMatch);
        int commentLength;

        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex;
        }

        setFormat(startIndex, commentLength, m_commentFormat);
        QRegularExpressionMatch startMatch = m_commentStartExpression.match(text, startIndex + commentLength);
        startIndex = startMatch.hasMatch() ? startMatch.capturedEnd() : -1;;
    }
}

} // ns processing
} // ns taste3
