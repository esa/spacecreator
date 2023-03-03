#ifndef INCONSISTENT_MODEL_EXCEPTION
#define INCONSISTENT_MODEL_EXCEPTION

#include <exception>
#include <QString>

/**
 * @brief Inconsistent model exception class
 */
class InconsistentModelException : public std::exception
{
public:
    /**
     * @brief   Constructor
     *
     * @param   message     Error message
     */
    explicit InconsistentModelException(const QString& message = "Inconsistent Model Exception")
        : m_message(message)
    {}

    /**
     * @brief   Returns error message as const char*
     *
     * @returns Error message
     */
    const char * what() const noexcept
    {
    	return m_message.toStdString().c_str();
    }

private:
    /** @brief  Error message */
    QString m_message;
};

#endif
