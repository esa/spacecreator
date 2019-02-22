/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cif/cifparser.h"
#include "exceptions.h"
#include "mscfile.h"

#include <QMetaEnum>
#include <QMetaObject>
#include <QtTest>

// add necessary includes here

using namespace msc;
using namespace msc::cif;

class tst_CifParser : public QObject
{
    Q_OBJECT

private:
    MscFile *m_mscFile = nullptr;
    CifParser *m_cifParser = nullptr;
private slots:

    void initTestCase();
    void cleanupTestCase();

    void testCoverage();

    void testParsingCifEntityAction();
    void testParsingCifEntityCall();
    void testParsingCifEntityComment();
    void testParsingCifEntityCondition();
    void testParsingCifEntityCreate();
    void testParsingCifEntityCollapsed();
    void testParsingCifEntityEnd();
    void testParsingCifEntityHyperLink();
    void testParsingCifEntityInstance();
    void testParsingCifEntityImport();
    void testParsingCifEntityKeep();
    void testParsingCifEntityLastModified();
    void testParsingCifEntityMessage();
    void testParsingCifEntityMscDocument();
    void testParsingCifEntityModified();
    void testParsingCifEntityMscPageSize();
    void testParsingCifEntityNested();
    void testParsingCifEntityPosition();
    void testParsingCifEntityPreview();
    void testParsingCifEntityReset();
    void testParsingCifEntitySet();
    void testParsingCifEntityStop();
    void testParsingCifEntitySubmsc();
    void testParsingCifEntitySpecific();
    void testParsingCifEntityText();
    void testParsingCifEntityTimeout();
    void testParsingCifEntityTextMode();
    void testParsingCifEntityTextName();

private:
    static QString createMscSource(const QString &cifLine);

    // this has to be void to be able to use QFAIL
    static void createCifLine(CifEntity::CifType cif, QString &outLine);

    void testParsingCifEntity(CifEntity::CifType entityType);
};

QString tst_CifParser::createMscSource(const QString &cifLine)
{
    static const QString mscTemplate = "MSCDOCUMENT doc_name;"
                                       "MSC msc_name;"
                                       "%1"
                                       "INSTANCE instance_name;"
                                       "ENDINSTANCE;"
                                       "ENDMSC;"
                                       "ENDMSCDOCUMENT;";

    return mscTemplate.arg(cifLine);
}

void tst_CifParser::createCifLine(CifEntity::CifType cif, QString &outLine)
{
    static const QString lineTemplate = "/* CIF %1 */";
    QString cifLine;

    switch (cif) {

    case CifEntity::CifType::Action:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Action)));
        break;
    case CifEntity::CifType::Call:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Call)));
        break;
    case CifEntity::CifType::Comment:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Comment)));
        break;
    case CifEntity::CifType::Condition:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Condition)));
        break;
    case CifEntity::CifType::Create:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Create)));
        break;
    case CifEntity::CifType::Collapsed:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Collapsed)));
        break;
    case CifEntity::CifType::End:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::End)));
        break;
    case CifEntity::CifType::HyperLink:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::HyperLink)));
        break;
    case CifEntity::CifType::Instance:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Instance)));
        break;
    case CifEntity::CifType::Import:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Import)));
        break;
    case CifEntity::CifType::Keep:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Keep)));
        break;
    case CifEntity::CifType::LastModified:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::LastModified)));
        break;
    case CifEntity::CifType::Message:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Message)));
        break;
    case CifEntity::CifType::MscDocument:
        cifLine = lineTemplate.arg(
                QString("%1 (20, 20), (1500, 1090)").arg(CifEntity::nameForType(CifEntity::CifType::MscDocument)));
        break;
    case CifEntity::CifType::Modified:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Modified)));
        break;
    case CifEntity::CifType::MscPageSize:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::MscPageSize)));
        break;
    case CifEntity::CifType::Nested:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Nested)));
        break;
    case CifEntity::CifType::Position:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Position)));
        break;
    case CifEntity::CifType::Preview:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Preview)));
        break;
    case CifEntity::CifType::Reset:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Reset)));
        break;
    case CifEntity::CifType::Set:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Set)));
        break;
    case CifEntity::CifType::Stop:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Stop)));
        break;
    case CifEntity::CifType::Submsc:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Submsc)));
        break;
    case CifEntity::CifType::Specific:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Specific)));
        break;
    case CifEntity::CifType::Text:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Text)));
        break;
    case CifEntity::CifType::Timeout:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::Timeout)));
        break;
    case CifEntity::CifType::TextMode:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::TextMode)));
        break;
    case CifEntity::CifType::TextName:
        cifLine = lineTemplate.arg(QString("%1").arg(CifEntity::nameForType(CifEntity::CifType::TextName)));
        break;

    default:
        QFAIL("It seems a new CifEntity::CifType has been introduced,\n"
              "but it's not covered here.\n"
              "Please add it to process or ignore explicitly.");
    }

    outLine = cifLine;
}

void tst_CifParser::initTestCase()
{
    m_mscFile = new MscFile;
    m_cifParser = new CifParser;
}

void tst_CifParser::cleanupTestCase()
{
    delete m_cifParser;
    delete m_mscFile;
}

void tst_CifParser::testCoverage()
{
    const QMetaObject *metaMe = metaObject();
    QStringList testMethods;
    for (int i = metaMe->methodOffset(); i < metaMe->methodCount(); ++i) {
        const QString method = QString::fromLatin1(metaMe->method(i).methodSignature());
        if (method.startsWith("testParsingCifEntity"))
            testMethods << method;
    }

    const QMetaEnum &e = QMetaEnum::fromType<CifEntity::CifType>();
    int ignoredTypes(0);
    ++ignoredTypes; // CifEntity::CifType::Unknown

    QCOMPARE(testMethods.size(), e.keyCount() - ignoredTypes);
}

void tst_CifParser::testParsingCifEntity(CifEntity::CifType entityType)
{
    QString cifLine;
    createCifLine(entityType, cifLine);
    QString source = createMscSource(cifLine);

    try {
        m_mscFile->parseText(source);
    } catch (...) {
        QFAIL("Unexpected exception!");
    }

    CifEntityShared cif = m_cifParser->readCIF(cifLine);
    QVERIFY(cif != nullptr);

    const QString cifLineHead = QString("/* CIF ") + CifEntity::nameForType(entityType);
    QString cifLineInvalid(cifLine);
    cifLineInvalid.replace(cifLineHead, cifLineHead + "_invalid");

    QVERIFY_EXCEPTION_THROWN(m_cifParser->readCIF(cifLineInvalid), ParserException);
}

void tst_CifParser::testParsingCifEntityAction()
{
    QEXPECT_FAIL("", "CifType::Action support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Action);
}
void tst_CifParser::testParsingCifEntityCall()
{
    QEXPECT_FAIL("", "CifType::Call support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Call);
}
void tst_CifParser::testParsingCifEntityComment()
{
    QEXPECT_FAIL("", "CifType::Comment support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Comment);
}
void tst_CifParser::testParsingCifEntityCondition()
{
    QEXPECT_FAIL("", "CifType::Condition support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Condition);
}
void tst_CifParser::testParsingCifEntityCreate()
{
    QEXPECT_FAIL("", "CifType::Create support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Create);
}
void tst_CifParser::testParsingCifEntityCollapsed()
{
    QEXPECT_FAIL("", "CifType::Collapsed support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Collapsed);
}
void tst_CifParser::testParsingCifEntityEnd()
{
    QEXPECT_FAIL("", "CifType::End support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::End);
}
void tst_CifParser::testParsingCifEntityHyperLink()
{
    QEXPECT_FAIL("", "CifType::HyperLink support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::HyperLink);
}
void tst_CifParser::testParsingCifEntityInstance()
{
    QEXPECT_FAIL("", "CifType::Instance support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Instance);
}
void tst_CifParser::testParsingCifEntityImport()
{
    QEXPECT_FAIL("", "CifType::Import support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Import);
}
void tst_CifParser::testParsingCifEntityKeep()
{
    QEXPECT_FAIL("", "CifType::Keep support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Keep);
}
void tst_CifParser::testParsingCifEntityLastModified()
{
    QEXPECT_FAIL("", "CifType::LastModified support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::LastModified);
}
void tst_CifParser::testParsingCifEntityMessage()
{
    QEXPECT_FAIL("", "CifType::Message support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Message);
}
void tst_CifParser::testParsingCifEntityMscDocument()
{
    QEXPECT_FAIL("", "CifType::MscDocument support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::MscDocument);
}
void tst_CifParser::testParsingCifEntityModified()
{
    QEXPECT_FAIL("", "CifType::Modified support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Modified);
}
void tst_CifParser::testParsingCifEntityMscPageSize()
{
    QEXPECT_FAIL("", "CifType::MscPageSize support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::MscPageSize);
}
void tst_CifParser::testParsingCifEntityNested()
{
    QEXPECT_FAIL("", "CifType::Nested support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Nested);
}
void tst_CifParser::testParsingCifEntityPosition()
{
    QEXPECT_FAIL("", "CifType::Position support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Position);
}
void tst_CifParser::testParsingCifEntityPreview()
{
    QEXPECT_FAIL("", "CifType::Preview support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Preview);
}
void tst_CifParser::testParsingCifEntityReset()
{
    QEXPECT_FAIL("", "CifType::Reset support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Reset);
}
void tst_CifParser::testParsingCifEntitySet()
{
    QEXPECT_FAIL("", "CifType::Set support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Set);
}
void tst_CifParser::testParsingCifEntityStop()
{
    QEXPECT_FAIL("", "CifType::Stop support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Stop);
}
void tst_CifParser::testParsingCifEntitySubmsc()
{
    QEXPECT_FAIL("", "CifType::Submsc support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Submsc);
}
void tst_CifParser::testParsingCifEntitySpecific()
{
    QEXPECT_FAIL("", "CifType::Specific support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Specific);
}
void tst_CifParser::testParsingCifEntityText()
{
    QEXPECT_FAIL("", "CifType::Text support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Text);
}
void tst_CifParser::testParsingCifEntityTimeout()
{
    QEXPECT_FAIL("", "CifType::Timeout support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::Timeout);
}
void tst_CifParser::testParsingCifEntityTextMode()
{
    QEXPECT_FAIL("", "CifType::TextMode support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::TextMode);
}
void tst_CifParser::testParsingCifEntityTextName()
{
    QEXPECT_FAIL("", "CifType::TextName support is not implemented yet", Continue);
    testParsingCifEntity(CifEntity::CifType::TextName);
}

QTEST_APPLESS_MAIN(tst_CifParser)

#include "tst_cifparser.moc"
