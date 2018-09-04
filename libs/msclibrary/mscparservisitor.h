#ifndef MSCPARSERVISITOR_H
#define MSCPARSERVISITOR_H

#include "parser/MscVisitor.h"

class MscParserVisitor : public MscVisitor
{
public:
    antlrcpp::Any visitR(MscParser::RContext *ctx) override;
};

#endif // MSCPARSERVISITOR_H
