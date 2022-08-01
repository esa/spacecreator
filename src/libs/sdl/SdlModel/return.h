#pragma once

#include "action.h"

#include <QString>

namespace sdl {

class Return final : public Action
{
public:
    /**
     * @brief   Default constructor
     */
    Return() = default;
    /**
     * @brief   Constructor
     *
     * @param   content   content of the return
     */
    Return(QString content);
    /**
     * @brief   Deleted copy constructor
     */
    Return(const Return &) = delete;
    /**
     * @brief   Default move constructor
     */
    Return(Return &&) = default;

    /**
     * @brief   Deleted copy assignment operator
     */
    Return &operator=(const Return &) = delete;
    /**
     * @brief   Default move assignment operator
     */
    Return &operator=(Return &&) = default;

public:
    /**
     * @brief   Getter for the return content
     *
     * @return  return content
     */
    auto content() const -> const QString &;
    /**
     * @brief   Setter for the return content
     *
     * @param   content return content
     */
    auto setContent(QString content) -> void;

public:
    /**
     * @brief  visitor acceptor (calls visit method of the given visitor)
     */
    virtual auto accept(Visitor &visitor) const -> void override;

private:
    QString m_content;
};

} // namespace sdl
