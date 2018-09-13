#include "mscparservisitor.h"
#include "mscmodel.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"

#include <QDebug>

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
    doc->setName(QString::fromStdString(context->IDENTIFIER()->getText()));

    m_currentDocument = doc;
    auto ret = visitChildren(context);
    m_currentDocument = parent;
    return ret;
}

antlrcpp::Any MscParserVisitor::visitMsc(MscParser::MscContext *context)
{
    auto chart = new MscChart();
    if (m_currentDocument == nullptr) {
        m_model->addChart(chart);
    } else {
        m_currentDocument->addChart(chart);
    }
    chart->setName(QString::fromStdString(context->IDENTIFIER()->getText()));

    m_currentChart = chart;
    for (auto instanceCtx : context->instance()) {
        addInstance(instanceCtx);
    }
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

antlrcpp::Any MscParserVisitor::visitMessage(MscParser::MessageContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitInMessage(MscParser::InMessageContext *context)
{
    QString name = QString::fromStdString(context->IDENTIFIER()->getText());
    auto message = new MscMessage(name);

    message->setSourceInstance(m_currentInstance);
    QString target = QString::fromStdString(context->messageTarget()->IDENTIFIER()->getText());
    message->setTargetInstance(m_currentChart->instanceByName(target));

    m_model->addMessage(message);
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitOutMessage(MscParser::OutMessageContext *context)
{
    QString name = QString::fromStdString(context->IDENTIFIER()->getText());
    auto message = new MscMessage(name);

    QString source = QString::fromStdString(context->messageTarget()->IDENTIFIER()->getText());
    message->setSourceInstance(m_currentChart->instanceByName(source));
    message->setTargetInstance(m_currentInstance);

    m_model->addMessage(message);
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMessageTarget(MscParser::MessageTargetContext *context)
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
