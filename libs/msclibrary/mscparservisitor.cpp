#include "mscparservisitor.h"
#include "mscmodel.h"
#include "mscchart.h"
#include "mscdocument.h"
#include "mscinstance.h"
#include "mscmessage.h"

#include <QDebug>

#include <string>

using namespace msc;

MscParserVisitor::MscParserVisitor()
    : m_model(new MscModel)
{
}

MscParserVisitor::~MscParserVisitor()
{
    delete m_model;
}

msc::MscModel *MscParserVisitor::detachModel()
{
    msc::MscModel *model = m_model;
    m_model = nullptr;
    return model;
}

antlrcpp::Any MscParserVisitor::visitFile(MscParser::FileContext *context)
{
    Q_ASSERT(m_model != nullptr);
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMscDocument(MscParser::MscDocumentContext *context)
{
    MscDocument *parent = m_currentDocument;

    auto doc = new MscDocument();
    if (m_currentDocument == nullptr) {
        m_model->addDocument(doc);
    } else {
        m_currentDocument->addDocument(doc);
    }
    const std::string docName = context->NAME()->getText();
    doc->setName(QString::fromStdString(docName));

    m_currentDocument = doc;
    auto ret = visitChildren(context);
    m_currentDocument = parent;
    return ret;
}

antlrcpp::Any MscParserVisitor::visitDefiningMscReference(MscParser::DefiningMscReferenceContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitVirtuality(MscParser::VirtualityContext *context)
{
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMscDefinition(MscParser::MscDefinitionContext *context)
{
    auto chart = new MscChart();
    if (m_currentDocument == nullptr) {
        m_model->addChart(chart);
    } else {
        m_currentDocument->addChart(chart);
    }
    const std::string mscName = context->NAME()->getText();
    chart->setName(QString::fromStdString(mscName));

    m_currentChart = chart;
    for (auto instanceCtx : context->instance()) {
        addInstance(instanceCtx);
    }
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitInstance(MscParser::InstanceContext *context)
{
    const QString name = QString::fromStdString(context->NAME(0)->getText());
    m_currentInstance = m_currentChart->instanceByName(name);
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitMscevent(MscParser::MsceventContext *context)
{
    const QString name = QString::fromStdString(context->NAME(0)->getText());

    if (m_currentChart->messageByName(name) == nullptr) {
        auto message = new MscMessage(name);
        if (context->a) {
            // is an input event
            if (context->a->NAME()) {
                const QString source = QString::fromStdString(context->a->NAME()->getText());
                message->setSourceInstance(m_currentChart->instanceByName(source));
            }
            message->setTargetInstance(m_currentInstance);
        }
        if (context->c) {
            // is an output event
            if (context->c->NAME()) {
                const QString target = QString::fromStdString(context->c->NAME()->getText());
                message->setTargetInstance(m_currentChart->instanceByName(target));
            }
            message->setSourceInstance(m_currentInstance);
        }
        m_currentChart->addMessage(message);
    }
    return visitChildren(context);
}

antlrcpp::Any MscParserVisitor::visitNameOrEnv(MscParser::NameOrEnvContext *context)
{
    return visitChildren(context);
}

void MscParserVisitor::addInstance(MscParser::InstanceContext *context)
{
    Q_ASSERT(m_currentChart != nullptr);
    if (context->GATE()) {
        return;
    }

    const QString name = QString::fromStdString(context->NAME(0)->getText());
    auto instance = new MscInstance(name);
    //    if (context->instanceHeader() != nullptr) {
    //        QString kind = QString::fromStdString(context->instanceHeader()->instancekind()->getText());
    //        instance->setKind(kind);
    //        QStringList decomposition;
    //        for (auto decomp : context->instanceHeader()->decomposition()) {
    //            decomposition << QString::fromStdString(decomp->getText());
    //        }
    //        instance->setDecomposition(decomposition);
    //    }
    m_currentChart->addInstance(instance);
}
