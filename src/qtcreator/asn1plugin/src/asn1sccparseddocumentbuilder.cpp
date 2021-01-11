/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a program and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "asn1sccparseddocumentbuilder.h"

#include <extensionsystem/pluginmanager.h>

#include "messages/messagemanager.h"
#include <messages/messageutils.h>

#include "asn1/definitions.h"
#include "asn1/file.h"

#include "asn1reader.h"
#include "astxmlparser.h"
#include "errormessageparser.h"

namespace Asn1Acn {
namespace Internal {

/*!
  Factory method to create a ParsedDocumentBuilder object
 */
ParsedDocumentBuilder *Asn1SccParsedDocumentBuilder::create(const QHash<QString, QString> &documents)
{
    auto serviceProvider = ExtensionSystem::PluginManager::getObject<ParsingServiceProvider>();
    return new Asn1SccParsedDocumentBuilder(serviceProvider, documents);
}

Asn1SccParsedDocumentBuilder::Asn1SccParsedDocumentBuilder(ParsingServiceProvider *serviceProvider,
                                                           const QHash<QString, QString> &documents)
    : m_serviceProvider(serviceProvider)
    , m_documentSources(documents)
{}

Asn1SccParsedDocumentBuilder::~Asn1SccParsedDocumentBuilder()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void Asn1SccParsedDocumentBuilder::run()
{
    m_worker = new Asn1ReadWorker(this);
    m_worker->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(m_worker,
            &Asn1ReadWorker::resultReady,
            this,
            &Asn1SccParsedDocumentBuilder::handleResults);
    m_workerThread.start();
    QMetaObject::invokeMethod(m_worker, "doWork");
}

void Asn1SccParsedDocumentBuilder::handleResults()
{
    Q_ASSERT(m_worker != nullptr);
    if (m_worker->hadError()) {
        Q_EMIT errored();
    }
    Q_EMIT finished();
}

std::vector<std::unique_ptr<Asn1Acn::File>> Asn1SccParsedDocumentBuilder::takeDocuments()
{
    return std::move(m_parsedDocuments);
}

/*!
   \brief Asn1ReadWorker::Asn1ReadWorker
   \param builder
 */
Asn1ReadWorker::Asn1ReadWorker(Asn1SccParsedDocumentBuilder *builder)
    : QObject()
    , m_builder(builder)
{}

/*!
   Does the parsing for all the files
 */
void Asn1ReadWorker::doWork()
{
    Asn1Reader reader;
    for (auto it = m_builder->m_documentSources.begin(); it != m_builder->m_documentSources.end();
         ++it) {
        const QString &fileName = it.key();
        const QString &asnContent = it.value();
        QStringList errorMessages;
        std::unique_ptr<Asn1Acn::File> data = reader.parseAsn1File(fileName,
                                                                   &errorMessages,
                                                                   asnContent);
        if (!errorMessages.isEmpty() || data.get() == nullptr) {
            if (errorMessages.isEmpty()) {
                m_builder->m_errorMessages.push_back(
                    {{}, QString("Error reading file %1").arg(fileName)});
            } else {
                for (const QString &error : qAsConst(errorMessages)) {
                    m_builder->m_errorMessages.push_back({SourceLocation(fileName, 1, 1), error});
                }
            }
        } else {
            m_builder->m_parsedDocuments.push_back(std::move(data));
        }
    }
    Q_EMIT resultReady();
}

} // namespace Internal
} // namespace Asn1Acn
