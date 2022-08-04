#include "return.h"

#include <sdl/SdlExporter/visitors/visitor.h>

namespace sdl {

Return::Return(QString content)
    : Action("")
    , m_content(std::move(content))
{
}

const QString &Return::content() const
{
    return m_content;
}

void Return::setContent(QString content)
{
    m_content = std::move(content);
}

void Return::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

} // namespace sdl
