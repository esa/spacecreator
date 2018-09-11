#ifndef MSCPARSERVISITOR_H
#define MSCPARSERVISITOR_H

#include "parser/MscVisitor.h"

namespace msc {
class MscChart;
class MscDocument;
class MscModel;
}

class MscParserVisitor : public MscVisitor
{
public:
    void setModel(msc::MscModel *model);

    antlrcpp::Any visitFile(MscParser::FileContext *ctx) override;
    antlrcpp::Any visitDocument(MscParser::DocumentContext *ctx) override;
    antlrcpp::Any visitMsc(MscParser::MscContext *ctx) override;
    antlrcpp::Any visitInstance(MscParser::InstanceContext *ctx) override;

private:
    msc::MscModel *m_model = nullptr;

    msc::MscDocument *m_currentDocument = nullptr;
    msc::MscChart *m_currentChart = nullptr;
};

#endif // MSCPARSERVISITOR_H
