/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#pragma once

#include "aadlobject.h"

#include <QMap>

struct XmlFileMock {

    XmlFileMock(const QString &content = QString(), int expectedErrors = 0, bool canBeParsed = true);

    const QString m_xmlContent;
    int m_expectedErrorCount { 0 };
    bool m_canBeParsed { true };
    QMap<aadl::AADLObject::Type, int> m_objectCountByType;

    int expectedObjectCount(aadl::AADLObject::Type t = aadl::AADLObject::Type::Unknown) const;
    void setExpectedObjectCount(aadl::AADLObject::Type t, int count);

    static XmlFileMock createEmptyFile();
    static XmlFileMock createEmptyDoc();
    static XmlFileMock createSingleFunction();
    static XmlFileMock createSingleFunctionType();
    static XmlFileMock createSingleComment();
    static XmlFileMock createSingleIfaceInvalid();
    static XmlFileMock createSingleIfaceValid();
    static XmlFileMock createParametrizedIfaces();
    static XmlFileMock createSingleConnectionOrphan();
    static XmlFileMock createSingleConnectionValid();
    static XmlFileMock createSingleConnectionValidMultipoint();
    static XmlFileMock createAllItems();
};

struct XmlHelper {
private:
    XmlHelper();
    static XmlHelper *m_instance;

public:
    static XmlHelper *instance();

    const XmlFileMock EmptyFile;
    const XmlFileMock EmptyDoc;
    const XmlFileMock SingleFunction;
    const XmlFileMock SingleFunctionType;
    const XmlFileMock SingleComment;
    const XmlFileMock SingleIfaceInvalid;
    const XmlFileMock SingleIfaceValid;
    const XmlFileMock ParametrizedIfaces;
    const XmlFileMock SingleConnectionOrphan;
    const XmlFileMock SingleConnectionValid;
    const XmlFileMock SingleConnectionValidMultipoint;
    const XmlFileMock AllItems;
};
