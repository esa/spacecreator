#include "mscparservisitor.h"

antlrcpp::Any MscParserVisitor::visitR(MscParser::RContext *ctx)
{
    return visitChildren(ctx);
}
