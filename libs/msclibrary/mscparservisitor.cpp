#include "mscparservisitor.h"
#include "mscmodel.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"

#include <QDebug>

#include <string>

using namespace msc;

void MscParserVisitor::setModel(msc::MscModel *model)
{
    m_model = model;
}

antlrcpp::Any MscParserVisitor::visitFile(MscParser::FileContext *context)
{
    Q_ASSERT(m_model != nullptr);
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitDocument(MscParser::DocumentContext *context)
{
    MscDocument *parent = m_currentDocument;

    auto doc = new MscDocument();
    if (m_currentDocument == nullptr) {
        m_model->addDocument(doc);
    } else {
        m_currentDocument->addDocument(doc);
    }
    std::string docName = context->documentHead()->IDENTIFIER()->getText();
    doc->setName(QString::fromStdString(docName));

    m_currentDocument = doc;
    auto ret = visitChildren(context);
    m_currentDocument = parent;
    return ret;
}

antlrcpp::Any MscParserVisitor::visitDocumentHead(MscParser::DocumentHeadContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMsc(MscParser::MscContext *context)
{
    auto chart = new MscChart();
    if (m_currentDocument == nullptr) {
        m_model->addChart(chart);
    } else {
        m_currentDocument->addChart(chart);
    }
    std::string mscName = context->mscHead()->IDENTIFIER()->getText();
    chart->setName(QString::fromStdString(mscName));

    m_currentChart = chart;
    for (auto instanceCtx : context->mscBody()->instance()) {
        addInstance(instanceCtx);
    }
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMscHead(MscParser::MscHeadContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMscBody(MscParser::MscBodyContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitInst(MscParser::InstContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitInstElements(MscParser::InstElementsContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitInstName(MscParser::InstNameContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitInstance(MscParser::InstanceContext *context)
{
    QString name = QString::fromStdString(context->IDENTIFIER()->getText());
    m_currentInstance = m_currentChart->instanceByName(name);
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitInstanceHeader(MscParser::InstanceHeaderContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitInstancekind(MscParser::InstancekindContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitDecomposition(MscParser::DecompositionContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMessageEvent(MscParser::MessageEventContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMessageInput(MscParser::MessageInputContext *context)
{
    QString name = QString::fromStdString(context->IDENTIFIER()->getText());
    QString target = QString::fromStdString(context->inputAddress()->IDENTIFIER()->getText());

    if (m_currentChart->messageByName(name) == nullptr) {
        auto message = new MscMessage(name);
        message->setSourceInstance(m_currentChart->instanceByName(target));
        message->setTargetInstance(m_currentInstance);
        m_currentChart->addMessage(message);
    }
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMessageOutput(MscParser::MessageOutputContext *context)
{
    QString name = QString::fromStdString(context->IDENTIFIER()->getText());
    QString source = QString::fromStdString(context->outputAddress()->IDENTIFIER()->getText());

    if (m_currentChart->messageByName(name) == nullptr) {
        auto message = new MscMessage(name);
        message->setSourceInstance(m_currentInstance);
        message->setTargetInstance(m_currentChart->instanceByName(source));
        m_currentChart->addMessage(message);
    }
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitInputAddress(MscParser::InputAddressContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitOutputAddress(MscParser::OutputAddressContext *context)
{
    return visitChildren(context);
}

void MscParserVisitor::addInstance(MscParser::InstanceContext *context)
{
    Q_ASSERT(m_currentChart != nullptr);
    QString name = QString::fromStdString(context->IDENTIFIER()->getText());
    auto instance = new MscInstance(name);
    if (context->instanceHeader() != nullptr) {
        QString kind = QString::fromStdString(context->instanceHeader()->instancekind()->getText());
        instance->setKind(kind);
        QStringList decomposition;
        for (auto decomp : context->instanceHeader()->decomposition()) {
            decomposition << QString::fromStdString(decomp->getText());
        }
        instance->setDecomposition(decomposition);
    }
    m_currentChart->addInstance(instance);
}
