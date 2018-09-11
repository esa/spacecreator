#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QString>

namespace msc {

class Exception
{
public:
    explicit Exception(const QString &errorMessage);
    const QString &errorMessage() const;

private:
    QString m_errorMessage;
};

class FileNotFoundException : public Exception
{
public:
    FileNotFoundException();
    explicit FileNotFoundException(const QString &errorMessage);
};

class IOException : public Exception
{
public:
    explicit IOException(const QString &errorMessage);
};

class ParserException : public Exception
{
public:
    explicit ParserException(const QString &errorMessage);
};

} // namespace msc

#endif // EXCEPTIONS_H
