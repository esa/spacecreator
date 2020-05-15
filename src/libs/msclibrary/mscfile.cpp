/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscfile.h"

#include "MscBaseVisitor.h"
#include "MscLexer.h"
#include "MscParser.h"
#include "asn1xmlparser.h"
#include "exceptions.h"
#include "mscdocument.h"
#include "mscerrorlistener.h"
#include "mscmodel.h"
#include "mscparservisitor.h"

#include <QFileInfo>
#include <QObject>
#include <antlr4-runtime.h>
#include <fstream>

/*!
  \class MscFile
  \inmodule MscLibrary

  The class to load MSC files
*/

using namespace antlr4;

namespace msc {
/*!
  \brief MscFile::MscFile
*/
MscFile::MscFile() { }

/*!
  \fn MscFile::parseFile(const QString &filename)

  Loads the file \a filename
*/
MscModel *MscFile::parseFile(const QString &filename, QStringList *errorMessages)
{
    if (!QFileInfo::exists(filename)) {
        throw FileNotFoundException();
    }

    std::ifstream stream;
    stream.open(filename.toStdString());
    if (!stream) {
        throw IOException(QObject::tr("Error opening the file"));
    }

    ANTLRInputStream input(stream);
    MscModel *mscModel = parse(input, errorMessages);

    if (!mscModel->dataDefinitionString().isEmpty()) {
        // parse ASN.1
        asn1::Asn1XMLParser xmlParser;
        mscModel->setAsn1TypesData(xmlParser.parseAsn1File(
                QFileInfo(filename).absolutePath(), mscModel->dataDefinitionString(), errorMessages));
    }

    return mscModel;
}

MscModel *MscFile::parseText(const QString &text, QStringList *errorMessages)
{
    ANTLRInputStream input(text.toStdString());
    return parse(input, errorMessages);
}

MscModel *MscFile::parse(ANTLRInputStream &input, QStringList *errorMessages)
{
    MscErrorListener errorListener;

    MscLexer lexer(&input);
    lexer.removeErrorListeners();
    lexer.addErrorListener(&errorListener);

    CommonTokenStream tokens(&lexer);
    tokens.fill();

    MscParser parser(&tokens);
    parser.removeErrorListeners();
    parser.addErrorListener(&errorListener);

    MscParserVisitor visitor(&tokens);
    visitor.visit(parser.file());

    if (errorMessages != nullptr) {
        *errorMessages = errorListener.getErrorMessages();
    }

    if (lexer.getNumberOfSyntaxErrors() > 0) {
        throw ParserException(QObject::tr("Lexer syntax error"));
    }
    if (parser.getNumberOfSyntaxErrors() > 0) {
        throw ParserException(QObject::tr("Parser syntax error"));
    }

    msc::MscModel *model = visitor.detachModel();
    if (model) {
        for (MscDocument *doc : model->documents()) {
            checkDocumentHierarchy(doc);
        }
    }

    return model;
}

void MscFile::checkDocumentHierarchy(MscDocument *doc)
{
    Q_ASSERT(doc);
    if (doc->hierarchyType() == MscDocument::HierarchyUnkown) {
        if (doc->documents().empty()) {
            doc->setHierarchyType(MscDocument::HierarchyLeaf);
        } else {
            doc->setHierarchyType(MscDocument::HierarchyAnd);
        }
    }

    for (MscDocument *childDoc : doc->documents()) {
        checkDocumentHierarchy(childDoc);
    }
}

} // namespace msc
