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
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat valueFormat;
    QRegExp valueStartExpression;
    QRegExp valueEndExpression;
};

} // ns processing
} // ns taste3

#endif // XMLHIGHLIGHTER_H
