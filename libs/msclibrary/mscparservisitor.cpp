#include "mscparservisitor.h"
#include "mscmodel.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscinstance.h"

#include <QDebug>

using namespace msc;

void MscParserVisitor::setModel(msc::MscModel *model)
{
    m_model = model;
}

antlrcpp::Any MscParserVisitor::visitFile(MscParser::FileContext *ctx)
{
    Q_ASSERT(m_model != nullptr);
    return visitChildren(ctx);
}

antlrcpp::Any MscParserVisitor::visitDocument(MscParser::DocumentContext *ctx)
{
    MscDocument *parent = m_currentDocument;

    auto doc = new MscDocument();
    if (m_currentDocument == nullptr) {
        m_model->addDocument(doc);
    } else {
        m_currentDocument->addDocument(doc);
    }
    doc->setName(QString::fromStdString(ctx->IDENTIFIER()->getText()));

    m_currentDocument = doc;
    auto ret = visitChildren(ctx);
    m_currentDocument = parent;
    return ret;
}

antlrcpp::Any MscParserVisitor::visitMsc(MscParser::MscContext *ctx)
{
    auto chart = new MscChart();
    if (m_currentDocument == nullptr) {
        m_model->addChart(chart);
    } else {
        m_currentDocument->addChart(chart);
    }
    chart->setName(QString::fromStdString(ctx->IDENTIFIER()->getText()));

    m_currentChart = chart;
    return visitChildren(ctx);
}

antlrcpp::Any MscParserVisitor::visitInstance(MscParser::InstanceContext *ctx)
{
    Q_ASSERT(m_currentChart != nullptr);
    QString name = QString::fromStdString(ctx->IDENTIFIER()->getText());
    auto instance = new MscInstance(name);
    m_currentChart->addInstance(instance);
    return visitChildren(ctx);
}
