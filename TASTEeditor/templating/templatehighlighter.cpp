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

#include <QDebug>

namespace taste3 {
namespace templating {

TemplateHighlighter::TemplateHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // variables
    QTextCharFormat variableFormat;
    variableFormat.setForeground(Qt::darkGreen);
    variableFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("(?<={{)\\s*[a-zA-Z_]\\w*(\\.\\w+)*(\\|[a-zA-Z_]\\w*)*(:\".+\")?\\s*(?=}})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // for variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\sfor)\\s+[a-zA-Z_]\\w*\\s+(?:in)\\s+[a-zA-Z_]\\w*.?[a-zA-Z_]\\w*\\s+(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // if variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\sif|elif)\\s+[a-zA-Z_]\\w*.?[a-zA-Z_]\\w*\\s+(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // block variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\sblock)\\s+[a-zA-Z_]\\w*\\s+(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // cycle variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\scycle)\\s+(\'?[a-zA-Z_]\\w*\'?\\s+)+(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // filter variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\sfilter)\\s+[a-zA-Z_]\\w*(\\.\\w+)*(\\|[a-zA-Z_]\\w*)*(:\".+\")?\\s*(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // firstof variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\sfirstof)\\s+(([a-zA-Z_]\\w*|(\'|\").*(\'|\"))(\\|[a-zA-Z_]\\w*)?\\s+)+(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // ifchanged variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\sifchanged)\\s+([a-zA-Z_]\\w*(\\.\\w+)*\\s+)*(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // ifequal and ifnotequal variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\sifequal|ifnotequal)\\s+([a-zA-Z_]\\w*(\\.\\w+)*\\s+){2,2}(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // load variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\sload)\\s+([a-zA-Z_]\\w*(\\.[a-zA-Z_]\\w+)*\\s+)+(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // now variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\snow)\\s+(\".*\"\\s+)?(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // range variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\srange)\\s+([a-zA-Z_]\\w*\\s+)+(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // regroup variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\sregroup)\\s+([a-zA-Z_]\\w*\\s+)+(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // templatetag variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\stemplatetag)\\s+(openblock|closeblock|openvariable|closevariable|openbrace|closebrace|opencomment|closecomment)\\s+(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // widthratio variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\swidthratio)\\s+([a-zA-Z_]\\w*\\s+)+(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // with variables
    rule.pattern = QRegularExpression(QStringLiteral("(?<={%\\swith)\\s+[a-zA-Z_]\\w*=[a-zA-Z_]\\w*(.[a-zA-Z_]\\w*)*\\s+(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = variableFormat;
    m_highlightingRules.append(rule);

    // keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::blue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywords = QStringLiteral("\\s+(include|extends|for|in|endfor|comment|endcomment|if|elif|else|endif|firstof|block(.super)?|endblock|"
                                            "autoescape\\s+(on|off)|endautoescape|cycle|as|silent|filter|endfilter|debug|ifchanged|endifchanged|"
                                            "ifequal|endifequal|ifnotequal|endifnotequal|load|from|media_finder|now|range|endrange|by|regroup|"
                                            "spaceless|endspaceless|templatetag|widthratio|with|endwith)\\s+");
    rule.pattern = QRegularExpression(keywords, QRegularExpression::InvertedGreedinessOption);
    rule.format = keywordFormat;
    m_highlightingRules.append(rule);

    // brackets
    QTextCharFormat bracketsFormat;
    bracketsFormat.setForeground(Qt::red);
    const QString brackets = QStringLiteral("({{|}}|{%|%}|{#|#})");
    rule.pattern = QRegularExpression(brackets);
    rule.format = bracketsFormat;
    m_highlightingRules.append(rule);

    // include
    QTextCharFormat includeFormat;
    includeFormat.setForeground(Qt::magenta);
    includeFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("(?<=\\b(include|extends)\\b)\\s+\"[a-zA-Z_].*\"\\s+(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = includeFormat;
    m_highlightingRules.append(rule);

    // media_finder
    rule.pattern = QRegularExpression(QStringLiteral("(?<=\\bmedia_finder\\b)\\s+\"[a-zA-Z_].*\"\\s+(?=%})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = includeFormat;
    m_highlightingRules.append(rule);

    // Comment
    m_commentFormat.setForeground(Qt::darkGray);
    rule.pattern = QRegularExpression(QStringLiteral("(?<={#).+(?=#})"),
                                      QRegularExpression::InvertedGreedinessOption);
    rule.format = m_commentFormat;
    m_highlightingRules.append(rule);

    m_commentStartExpression.setPattern(QStringLiteral("{%\\s*comment\\s*%}"));
    m_commentEndExpression.setPattern(QStringLiteral("{%\\s*endcomment\\s*%}"));
}

void TemplateHighlighter::highlightBlock(const QString &text)
{
    //for every pattern
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
            setFormat(index, length, rule.format);
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
