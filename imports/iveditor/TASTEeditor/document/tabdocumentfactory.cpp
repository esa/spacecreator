/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "tabdocumentfactory.h"

#include "abstracttabdocument.h"
#include "app/aadltabdocument.h"
#include "tab_concurrency/concurrencytabdocument.h"
#include "tab_data/datatabdocument.h"
#include "tab_deployment/deploymenttabdocument.h"
#include "tab_interface/interfacetabdocument.h"
#include "tab_msc/msctabdocument.h"

namespace document {

/*!
\class taste3::document::TabDocumentFactory
\brief taste3::document::TabDocumentFactory is a factory to create particular data format documents.

\sa taste3::document::AbstractTabDocument, taste3::document::DocumentsManager, taste3::document::InterfaceTabDocument
*/

AbstractTabDocument *TabDocumentFactory::initDoc(AbstractTabDocument *doc)
{
    if (doc)
        doc->init();

    return doc;
}

AbstractTabDocument *TabDocumentFactory::createDataTabDocument(QObject *parent)
{
    return initDoc(new DataTabDocument(parent));
}

AbstractTabDocument *TabDocumentFactory::createInterfaceTabDocument(QObject *parent)
{
    return initDoc(new InterfaceTabDocument(parent));
}

AbstractTabDocument *TabDocumentFactory::createDeploymentTabDocument(QObject *parent)
{
    return initDoc(new DeploymentTabDocument(parent));
}

AbstractTabDocument *TabDocumentFactory::createConcurrencyTabDocument(QObject *parent)
{
    return initDoc(new ConcurrencyTabDocument(parent));
}

AbstractTabDocument *TabDocumentFactory::createAADLTabDocument(QObject *parent)
{
    return initDoc(new AADLTabDocument(parent));
}

AbstractTabDocument *TabDocumentFactory::createMSCTabDocument(QObject *parent)
{
    return initDoc(new MSCTabDocument(parent));
}

}
