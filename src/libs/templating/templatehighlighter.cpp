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

namespace templating {

/**
 * @brief TemplateHighlighter::TemplateHighlighter ctor
 * @param parent
 */
TemplateHighlighter::TemplateHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    // tag rule
    HighlightingRule tagRule;
    tagRule.format.setForeground(Qt::blue);
    tagRule.format.setFontWeight(QFont::Bold);
    const QString tags = QStringLiteral(
            "\\b(include|extends|for|in|endfor|comment|endcomment|if|elif|else|endif|firstof|block|endblock|"
            "autoescape\\s+(on|off)|endautoescape|cycle|as|silent|filter|endfilter|debug|ifchanged|endifchanged|"
            "ifequal|endifequal|ifnotequal|endifnotequal|load|from|media_finder|now|range|endrange|by|regroup|"
            "spaceless|endspaceless|templatetag|widthratio|with|endwith)\\b");
    tagRule.pattern = QRegularExpression(tags, QRegularExpression::InvertedGreedinessOption);

    // block.super tag rule
    HighlightingRule blockSuperTagRule;
    blockSuperTagRule.format = tagRule.format;
    blockSuperTagRule.pattern =
            QRegularExpression(QStringLiteral("\\bblock.super\\b"), QRegularExpression::InvertedGreedinessOption);

    // variable rule
    HighlightingRule variableRule;
    variableRule.format.setForeground(Qt::darkGreen);
    variableRule.format.setFontWeight(QFont::Bold);
    variableRule.pattern = QRegularExpression(QStringLiteral("(?<!\\|)\\b[a-zA-Z_]\\w*(\\.\\w+)*\\b"));

    // filter rule
    HighlightingRule filterRule;
    filterRule.format.setForeground(Qt::darkRed);
    filterRule.format.setFontWeight(QFont::Bold);
    const QString filters =
            QStringLiteral("(?<=\\||filter\\s)\\s*\\b(add|addslashes|capfirst|center|cut|date|default|"
                           "default_if_none|dictsort|divisibleby|escape|escapejs|first|floatformat|force_escape|"
                           "get_digit|join|last|length|length_is|linebreaks|linebreaksbr|linenumbers|ljust|"
                           "lower|make_list|random|rjust|safe|safeseq|slice|slugify|stringformat|striptags|time|"
                           "timesince|timeuntil|title|truncatewords|unordered_list|upper|wordcount|wordwrap|yesno)\\b");
    filterRule.pattern = QRegularExpression(filters, QRegularExpression::InvertedGreedinessOption);

    // filter delimiter rule
    HighlightingRule filterDelimiterRule;
    filterDelimiterRule.format = filterRule.format;
    filterDelimiterRule.format.setFontWeight(QFont::Bold);
    filterDelimiterRule.pattern = QRegularExpression(QStringLiteral("(?<=\\w)\\s*(\\||:)\\s*"));

    // string rule
    HighlightingRule stringRule;
    stringRule.format.setForeground(Qt::magenta);
    stringRule.format.setFontWeight(QFont::Bold);
    stringRule.pattern =
            QRegularExpression(QStringLiteral("(\".*\")|(\'.*\')"), QRegularExpression::InvertedGreedinessOption);

    // brackets highlighting
    Highlighting bracketHighlighting;
    bracketHighlighting.baseFormat.setForeground(Qt::red);
    bracketHighlighting.basePattern = QRegularExpression(QStringLiteral("({{|}}|{%|%}|{#|#})"));
    m_highlightings.append(bracketHighlighting);

    // tags highlighting
    Highlighting tagHighlighting;
    tagHighlighting.basePattern =
            QRegularExpression(QStringLiteral("(?<={%).*(?=%})"), QRegularExpression::InvertedGreedinessOption);
    tagHighlighting.highlightingRules.append(variableRule);
    tagHighlighting.highlightingRules.append(filterRule);
    tagHighlighting.highlightingRules.append(filterDelimiterRule);
    tagHighlighting.highlightingRules.append(tagRule);
    tagHighlighting.highlightingRules.append(stringRule);
    m_highlightings.append(tagHighlighting);

    // variables highlighting
    Highlighting variableHighlighting;
    variableHighlighting.basePattern =
            QRegularExpression(QStringLiteral("(?<={{).*(?=}})"), QRegularExpression::InvertedGreedinessOption);
    variableHighlighting.highlightingRules.append(variableRule);
    variableHighlighting.highlightingRules.append(blockSuperTagRule);
    variableHighlighting.highlightingRules.append(filterRule);
    variableHighlighting.highlightingRules.append(filterDelimiterRule);
    variableHighlighting.highlightingRules.append(stringRule);
    m_highlightings.append(variableHighlighting);

    // comments highlighting
    m_commentFormat.setForeground(Qt::darkGray);
    m_commentStartExpression.setPattern(QStringLiteral("{%\\s*comment\\s*%}"));
    m_commentEndExpression.setPattern(QStringLiteral("{%\\s*endcomment\\s*%}"));

    Highlighting commentHighlighting;
    commentHighlighting.basePattern =
            QRegularExpression(QStringLiteral("(?<={#).+(?=#})"), QRegularExpression::InvertedGreedinessOption);
    commentHighlighting.baseFormat = m_commentFormat;
    m_highlightings.append(commentHighlighting);
}

/**
 * @brief TemplateHighlighter::highlightBlock highlights text
 * @param text
 */
void TemplateHighlighter::highlightBlock(const QString &text)
{
    for (const Highlighting &highlighting : m_highlightings) {
        QRegularExpression expression = highlighting.basePattern;
        if (!expression.isValid()) {
            qWarning() << Q_FUNC_INFO << expression.errorString() << expression.pattern();
            continue;
        }

        QRegularExpressionMatchIterator it = expression.globalMatch(text);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            int startIndex = match.capturedStart();
            if (highlighting.highlightingRules.isEmpty()) {
                int length = match.capturedLength();
                setFormat(startIndex, length, highlighting.baseFormat);
                continue;
            }

            const QString &captured = match.captured();
            for (const HighlightingRule &rule : highlighting.highlightingRules) {
                QRegularExpression pattern = rule.pattern;
                if (!pattern.isValid()) {
                    qWarning() << Q_FUNC_INFO << pattern.errorString() << pattern.pattern();
                    continue;
                }
                QRegularExpressionMatchIterator i = pattern.globalMatch(captured);
                while (i.hasNext()) {
                    QRegularExpressionMatch m = i.next();
                    int index = m.capturedStart();
                    int length = m.capturedLength();
                    setFormat(startIndex + index, length, rule.format);
                }
            }
        }
    }

    setCurrentBlockState(0);
    int startIndex = 0;

    if (previousBlockState() != 1) {
        QRegularExpressionMatch startMatch = m_commentStartExpression.match(text);
        startIndex = startMatch.hasMatch() ? startMatch.capturedEnd() : -1;
    }

    while (startIndex >= 0) {
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
        startIndex = startMatch.hasMatch() ? startMatch.capturedEnd() : -1;
        ;
    }
}

}
