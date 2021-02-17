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

#pragma once

#include <memory>

#include <QByteArray>
#include <QHash>
#include <QString>
#include <QThread>

#include "parseddocumentbuilder.h"

namespace Asn1Acn {
namespace Internal {

class Asn1ReadWorker;

/*!
   Build ASN data structure from ASN files/content
 */
class Asn1SccParsedDocumentBuilder : public ParsedDocumentBuilder
{
    Q_OBJECT

public:
    static ParsedDocumentBuilder *create(const QHash<QString, QString> &documents);

    Asn1SccParsedDocumentBuilder(const QHash<QString, QString> &documents);
    ~Asn1SccParsedDocumentBuilder();
    void run() override;

    std::vector<std::unique_ptr<Asn1Acn::File>> takeDocuments() override;
    const std::vector<Asn1Acn::ErrorMessage> &errorMessages() const override
    {
        return m_errorMessages;
    }

private Q_SLOTS:
    void handleResults();

private:
    const QHash<QString, QString> m_documentSources;

    std::vector<std::unique_ptr<Asn1Acn::File>> m_parsedDocuments;
    std::vector<Asn1Acn::ErrorMessage> m_errorMessages;

    QThread m_workerThread;
    Asn1ReadWorker *m_worker = nullptr;

    friend class Asn1ReadWorker;
};

/*!
   Worker class for parsing asn.1 files in a thread
 */
class Asn1ReadWorker : public QObject
{
    Q_OBJECT
public:
    Asn1ReadWorker(Asn1SccParsedDocumentBuilder *builder);
    bool hadError() const;

public Q_SLOTS:
    void doWork();

Q_SIGNALS:
    void resultReady();

private:
    Asn1SccParsedDocumentBuilder *m_builder = nullptr;
};

} /* namespace Internal */
} /* namespace Asn1Acn */
