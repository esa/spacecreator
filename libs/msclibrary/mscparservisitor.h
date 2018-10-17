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
    antlrcpp::Any visitMscDocument(MscParser::MscDocumentContext *context) override;
    antlrcpp::Any visitDefiningMscReference(MscParser::DefiningMscReferenceContext *context) override;
    antlrcpp::Any visitVirtuality(MscParser::VirtualityContext* context) override;
    antlrcpp::Any visitMscDefinition(MscParser::MscDefinitionContext *context) override;
    antlrcpp::Any visitInstance(MscParser::InstanceContext *context) override;
    antlrcpp::Any visitMscevent(MscParser::MsceventContext *context) override;
    antlrcpp::Any visitNameOrEnv(MscParser::NameOrEnvContext *context) override;

private:
    void addInstance(MscParser::InstanceContext *context);

    msc::MscModel *m_model = nullptr;

    msc::MscDocument *m_currentDocument = nullptr;
    msc::MscChart *m_currentChart = nullptr;
    msc::MscInstance *m_currentInstance = nullptr;
};

#endif // MSCPARSERVISITOR_H
