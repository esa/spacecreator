#ifndef STRINGTEMPLATE_H
#define STRINGTEMPLATE_H

#include <QObject>

namespace Grantlee {
    class Engine;
}

namespace taste3 {

namespace aadl {
    class AADLObject;
}

namespace templating {

class StringTemplate : public QObject
{
    Q_OBJECT
public:
    explicit StringTemplate(QObject *parent = nullptr);

    QString parseFile(const QHash<QString, QVariantList> &grouppedObjects, const QString &templateFileName);

    QString formatText(const QString &text);

    int autoFormattingIndent() const;

public slots:
    void setAutoFormattingIndent(int autoFormattingIndent);

signals:
    void errorOccurred(const QString &errorString);

private:
    Grantlee::Engine *m_engine;
    int m_autoFormattingIndent;
};

} // ns processing
} // ns taste3

#endif // STRINGTEMPLATE_H
