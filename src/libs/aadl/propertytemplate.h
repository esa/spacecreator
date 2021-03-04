#pragma once

#include <QObject>
#include <QVariant>
#include <QVector>
#include <memory>

class QValidator;
class QDomDocument;
class QDomElement;

namespace ivm {

class PropertyTemplate
{
    Q_GADGET
public:
    enum class Info
    {
        Property,
        Attribute
    };
    Q_ENUM(Info)

    enum class Type
    {
        Unknown,
        Boolean,
        Integer,
        Real,
        String,
        Enumeration
    };
    Q_ENUM(Type)

    enum class Scope
    {
        None = 0x0,
        Function = 0x1,
        Required_Interface = 0x2,
        Provided_Interface = 0x4,
        Comment = 0x8,
        Connection = 0x10,
        All = Function | Required_Interface | Provided_Interface | Comment | Connection
    };

    Q_ENUM(Scope)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
    Q_DECLARE_FLAGS(Scopes, Scope)
#else
    Q_DECLARE_FLAGS(Scopes, ivm::PropertyTemplate::Scope)
#endif
    Q_FLAG(Scopes)

    PropertyTemplate();
    ~PropertyTemplate();

    QString name() const;
    void setName(const QString &name);

    QString label() const;
    void setLabel(const QString &name);

    Type type() const;
    void setType(Type t);

    Info info() const;
    void setInfo(Info info);

    bool isSystem() const;
    void setSystem(bool value);

    Scopes scope() const;
    void setScope(const Scopes &s);

    bool isVisible() const;
    void setVisible(bool value);

    QList<QVariant> valuesList() const;
    void setValuesList(const QList<QVariant> &range);

    QVariant defaultValue() const;
    void setDefaultValue(const QVariant &value);

    QString valueValidatorPattern() const;
    void setValueValidatorPattern(const QString &pattern);

    QMap<PropertyTemplate::Scope, QPair<QString, QString>> attrValidatorPatterns() const;
    void setAttrValidatorPattern(const QMap<PropertyTemplate::Scope, QPair<QString, QString>> &pattern);

    QDomElement toXml(QDomDocument *domDoc) const;
    static PropertyTemplate *fromXml(const QDomElement &element);
    static QString tagName();
    static QVariant convertData(const QVariant &value, PropertyTemplate::Type type);

private:
    struct PropertyTemplatePrivate;
    const std::unique_ptr<PropertyTemplatePrivate> d;
};

} // namespace ivm

Q_DECLARE_OPERATORS_FOR_FLAGS(ivm::PropertyTemplate::Scopes)
Q_DECLARE_METATYPE(ivm::PropertyTemplate::Type)
Q_DECLARE_METATYPE(ivm::PropertyTemplate::Scopes)
