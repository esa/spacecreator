#pragma once

#include <QObject>
#include <QString>
#include <QVector>

namespace msc {

/**
   @brief The DataStatement struct
 */
class DataStatement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(msc::DataStatement::StatementType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString variableString READ variableString WRITE setVariableString NOTIFY variableStringChanged)

public:
    enum class StatementType
    {
        Define,
        UnDefine,
        Binding
    };
    Q_ENUM(StatementType)

    explicit DataStatement(QObject *parent = nullptr);

    msc::DataStatement::StatementType type() const;
    void setType(msc::DataStatement::StatementType type);

    QString variableString() const;
    void setVariableString(const QString &variableString);

Q_SIGNALS:
    void typeChanged();
    void variableStringChanged();

private:
    StatementType m_type = StatementType::Define;
    QString m_variableString;
};

}

Q_DECLARE_METATYPE(msc::DataStatement *)
Q_DECLARE_METATYPE(QVector<msc::DataStatement *>)
