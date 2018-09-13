#ifndef MSCPARSERVISITOR_H
#define MSCPARSERVISITOR_H

#include "parser/MscVisitor.h"

namespace msc {
class MscChart;
class MscDocument;
class MscInstance;
class MscModel;
}

class MscParserVisitor : public MscVisitor
{
public:
    void setModel(msc::MscModel *model);

    antlrcpp::Any visitFile(MscParser::FileContext *context) override;
    antlrcpp::Any visitDocument(MscParser::DocumentContext *context) override;
    antlrcpp::Any visitMsc(MscParser::MscContext *context) override;
    antlrcpp::Any visitInstance(MscParser::InstanceContext *context) override;
    antlrcpp::Any visitInstanceHeader(MscParser::InstanceHeaderContext *context) override;
    antlrcpp::Any visitInstancekind(MscParser::InstancekindContext *context) override;
    antlrcpp::Any visitDecomposition(MscParser::DecompositionContext *context) override;
    antlrcpp::Any visitMessage(MscParser::MessageContext *context) override;
    antlrcpp::Any visitInMessage(MscParser::InMessageContext *context) override;
    antlrcpp::Any visitOutMessage(MscParser::OutMessageContext *context) override;
    antlrcpp::Any visitMessageTarget(MscParser::MessageTargetContext *context) override;

private:
    void addInstance(MscParser::InstanceContext *context);

    msc::MscModel *m_model = nullptr;

    msc::MscDocument *m_currentDocument = nullptr;
    msc::MscChart *m_currentChart = nullptr;
    msc::MscInstance *m_currentInstance = nullptr;
};

#endif // MSCPARSERVISITOR_H
