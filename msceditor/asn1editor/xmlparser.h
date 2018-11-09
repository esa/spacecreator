#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QVariant>
#include <QDomElement>

class XMLParser : public QObject
{
    Q_OBJECT

public:
    XMLParser(const QString &file, QObject *parent = nullptr);

    QVariantMap parseAsn1Xml();

Q_SIGNALS:
    void parseError(const QString &error);

private:
    QVariantMap parseType(const QDomElement &type, const QString &name = QString());
    void parseSequenceType(const QDomElement &type, QVariantMap &result);
    void parseEnumeratedType(const QDomElement &type, QVariantMap &result);
    void parseChoiceType(const QDomElement &type, QVariantMap &result);

private:
    QString m_fileName;
};

#endif // XMLPARSER_H
