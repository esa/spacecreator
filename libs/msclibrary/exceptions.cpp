#include "exceptions.h"

#include <QObject>

/*!
  Exception for reading/writing MSC files
*/

namespace msc {

Exception::Exception(const QString &errorMessage)
    : m_errorMessage(errorMessage)
{
}

const QString &Exception::errorMessage() const
{
    return m_errorMessage;
}

FileNotFoundException::FileNotFoundException()
    : Exception(QObject::tr("File not found"))
{
}

FileNotFoundException::FileNotFoundException(const QString &errorMessage)
    : Exception(errorMessage)
{
}

IOException::IOException(const QString &errorMessage)
    : Exception(errorMessage)
{
}

ParserException::ParserException(const QString &errorMessage)
    : Exception(errorMessage)
{
}

} // namespace msc
