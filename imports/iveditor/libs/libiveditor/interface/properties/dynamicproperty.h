#pragma once

#include <QJsonObject>
#include <QObject>
#include <QVariant>
#include <QVector>
#include <memory>

namespace aadlinterface {

class DynamicProperty
{
    Q_GADGET
public:
    enum class Type
    {
        Integer,
        Real,
        Boolean,
        String,
        Enumeration
    };
    Q_ENUM(Type)

    enum class Scope
    {
        None = 0x0,
        FunctionType = 0x1,
        Function = 0x2,
        Interface = 0x4
    };
    Q_ENUM(aadlinterface::DynamicProperty::Scope)
    Q_DECLARE_FLAGS(Scopes, Scope)
    Q_FLAG(aadlinterface::DynamicProperty::Scopes)

    DynamicProperty(const QString &name = "unknown", Type t = Type::Integer, const Scopes &s = Scope::None,
                    const QVector<QVariant> &listVals = QVector<QVariant>());
    ~DynamicProperty();

    QString name() const;
    void setName(const QString &name);

    Type type() const;
    void setType(Type t);

    Scopes scope() const;
    void setScope(const Scopes &s);

    QVector<QVariant> valuesList() const;
    void setValuesList(const QVector<QVariant> &range);

    QJsonObject toJson() const;
    static DynamicProperty *fromJson(const QJsonObject &json);

private:
    struct DynamicPropertyPrivate;
    const std::unique_ptr<DynamicPropertyPrivate> d;
};

}

Q_DECLARE_OPERATORS_FOR_FLAGS(aadlinterface::DynamicProperty::Scopes)
Q_DECLARE_METATYPE(aadlinterface::DynamicProperty::Type)
Q_DECLARE_METATYPE(aadlinterface::DynamicProperty::Scopes)
