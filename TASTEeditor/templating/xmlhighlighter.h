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

#ifndef XMLHIGHLIGHTER_H
#define XMLHIGHLIGHTER_H

#include <QSyntaxHighlighter>

namespace taste3 {
namespace templating {

class XMLHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    XMLHighlighter(QTextDocument *parent);

    // QSyntaxHighlighter interface
protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> m_highlightingRules;

    QTextCharFormat m_valueFormat;
    QRegExp m_valueStartExpression;
    QRegExp m_valueEndExpression;
};

} // ns processing
} // ns taste3

#endif // XMLHIGHLIGHTER_H
