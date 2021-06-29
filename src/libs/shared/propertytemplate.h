#pragma once

#include <QDomElement>
#include <QMetaEnum>
#include <QObject>
#include <QVariant>
#include <QVector>
#include <memory>

class QValidator;
class QDomDocument;

namespace shared {
class VEObject;

class PropertyTemplate
{
    Q_GADGET
public:
    static const QString kAttributeTagName;
    static const QString kScopesTagName;

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

    PropertyTemplate();
    virtual ~PropertyTemplate();

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

    int scopes() const;
    void setScopes(int s);

    bool isVisible() const;
    void setVisible(bool value);

    QVariant value() const;
    void setValue(const QVariant &value);

    QVariant defaultValue() const;
    void setDefaultValue(const QVariant &value);

    QString valueValidatorPattern() const;
    void setValueValidatorPattern(const QString &pattern);

    QMap<int, QPair<QString, QString>> attrValidatorPatterns() const;
    void setAttrValidatorPattern(const QMap<int, QPair<QString, QString>> &pattern);

    QDomElement toXml(QDomDocument *domDoc) const;

    bool validate(const VEObject *object) const;

    static QVariant convertData(const QVariant &value, PropertyTemplate::Type type);

    virtual QMetaEnum scopeMetaEnum() const = 0;
    virtual int objectScope(const VEObject *obj) const = 0;

    template<typename T, typename = typename std::enable_if<std::is_base_of<PropertyTemplate, T>::value, T>::type>
    static PropertyTemplate *createFromXml(const QDomElement &element)
    {
        if (element.isNull() || element.tagName() != T::kAttributeTagName
                || !element.hasAttribute(QLatin1String("name")) || !element.hasChildNodes()) {
            return nullptr;
        }

        PropertyTemplate *t = new T;
        t->initFromXml(element);
        return t;
    }

protected:
    void initFromXml(const QDomElement &element);

private:
    struct PropertyTemplatePrivate;
    const std::unique_ptr<PropertyTemplatePrivate> d;
};

} // namespace shared
