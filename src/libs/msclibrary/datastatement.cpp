#include "datastatement.h"

namespace msc {

DataStatement::DataStatement(QObject *parent)
    : QObject(parent)
{
}

msc::DataStatement::StatementType DataStatement::type() const
{
    return m_type;
}

void DataStatement::setType(StatementType type)
{
    if (m_type == type) {
        return;
    }

    m_type = type;
    Q_EMIT typeChanged();
}

QString DataStatement::variableString() const
{
    return m_variableString;
}

void msc::DataStatement::setVariableString(const QString &variableString)
{
    if (m_variableString == variableString) {
        return;
    }

    m_variableString = variableString;
    Q_EMIT variableStringChanged();
}

}
