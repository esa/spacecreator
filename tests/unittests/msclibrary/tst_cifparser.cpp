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

#include "cif/cifparser.h"
#include "exceptions.h"
#include "mscreader.h"

#include <QMetaEnum>
#include <QMetaObject>
#include <QtTest>
#include <mscchart.h>
#include <mscdocument.h>
#include <mscmodel.h>

// add necessary includes here

using namespace msc;
using namespace msc::cif;

class tst_CifParser : public QObject
{
    Q_OBJECT

private:
    MscReader *m_mscReader = nullptr;
    CifParser *m_cifParser = nullptr;
private Q_SLOTS:

    void initTestCase();
    void cleanupTestCase();

    void testCoverage();

    void testParsingCifLineAction();
    void testParsingCifLineCall();
    void testParsingCifLineComment();
    void testParsingCifLineCondition();
    void testParsingCifLineCreate();
    void testParsingCifLineCollapsed();
    void testParsingCifLineEnd();
    void testParsingCifLineHyperLink();
    void testParsingCifLineInstance();
    void testParsingCifLineImport();
    void testParsingCifLineKeep();
    void testParsingCifLineLastModified();
    void testParsingCifLineMessage();
    void testParsingCifLineMscDocument();
    void testParsingCifLineModified();
    void testParsingCifLineMscPageSize();
    void testParsingCifLineNested();
    void testParsingCifLinePosition();
    void testParsingCifLinePreview();
    void testParsingCifLineReset();
    void testParsingCifLineSet();
    void testParsingCifLineStop();
    void testParsingCifLineSubmsc();
    void testParsingCifLineSpecific();
    void testParsingCifLineText();
    void testParsingCifLineTimeout();
    void testParsingCifLineTextMode();
    void testParsingCifLineTextName();

    void testParsingCifBlockAction();
    void testParsingCifBlockCall();
    void testParsingCifBlockComment();
    void testParsingCifBlockCondition();
    void testParsingCifBlockCreate();
    void testParsingCifBlockCollapsed();
    void testParsingCifBlockEnd();
    void testParsingCifBlockHyperLink();
    void testParsingCifBlockInstance();
    void testParsingCifBlockImport();
    void testParsingCifBlockKeep();
    void testParsingCifBlockLastModified();
    void testParsingCifBlockMessage();
    void testParsingCifBlockMscDocument();
    void testParsingCifBlockModified();
    void testParsingCifBlockMscPageSize();
    void testParsingCifBlockNested();
    void testParsingCifBlockPosition();
    void testParsingCifBlockPreview();
    void testParsingCifBlockReset();
    void testParsingCifBlockSet();
    void testParsingCifBlockStop();
    void testParsingCifBlockSubmsc();
    void testParsingCifBlockSpecific();
    void testParsingCifBlockText();
    void testParsingCifBlockTimeout();
    void testParsingCifBlockTextMode();
    void testParsingCifBlockTextName();

    void testParsingCifAtDocumentFront();
    void testIgnoreUnknownCif();

private:
    static QString createMscSource(const QString &cifLine);

    // this has to be void to be able to use QFAIL
    static void createCifLine(CifLine::CifType cif, QString &outLine);

    void testParsingCifLine(CifLine::CifType entityType);
    void testParsingCifBlock(const QVector<QVector<CifLine::CifType>> &blocks);
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

void tst_CifParser::createCifLine(CifLine::CifType cif, QString &outLine)
{
    static const QString lineTemplate = "/* CIF %1 */";
    QString cifLine;

    switch (cif) {

    case CifLine::CifType::Action:
        cifLine = lineTemplate.arg(QString("%1 (1, 1), (2, 2)").arg(CifLine::nameForType(CifLine::CifType::Action)));
        break;
    case CifLine::CifType::Call:
        cifLine = lineTemplate.arg(QString("%1 (1, 1), (2, 2)").arg(CifLine::nameForType(CifLine::CifType::Call)));
        break;
    case CifLine::CifType::Comment:
        cifLine = lineTemplate.arg(QString("%1 (1, 1), (2, 2)").arg(CifLine::nameForType(CifLine::CifType::Comment)));
        break;
    case CifLine::CifType::Condition:
        cifLine = lineTemplate.arg(QString("%1 (1, 1), (2, 2)").arg(CifLine::nameForType(CifLine::CifType::Condition)));
        break;
    case CifLine::CifType::Create:
        cifLine = lineTemplate.arg(QString("%1 (1, 1), (2, 2)").arg(CifLine::nameForType(CifLine::CifType::Create)));
        break;
    case CifLine::CifType::Collapsed:
        cifLine = lineTemplate.arg(QString("%1").arg(CifLine::nameForType(CifLine::CifType::Collapsed)));
        break;
    case CifLine::CifType::End:
        cifLine = lineTemplate.arg(QString("%1").arg(CifLine::nameForType(CifLine::CifType::End)));
        break;
    case CifLine::CifType::HyperLink:
        cifLine = lineTemplate.arg(QString("%1").arg(CifLine::nameForType(CifLine::CifType::HyperLink)));
        break;
    case CifLine::CifType::Instance:
        cifLine = lineTemplate.arg(
                QString("%1 (1, 1), (2, 2) (3, 3)").arg(CifLine::nameForType(CifLine::CifType::Instance)));
        break;
    case CifLine::CifType::Import:
        cifLine = lineTemplate.arg(QString("%1").arg(CifLine::nameForType(CifLine::CifType::Import)));
        break;
    case CifLine::CifType::Keep:
        cifLine = lineTemplate.arg(QString("%1").arg(CifLine::nameForType(CifLine::CifType::Keep)));
        break;
    case CifLine::CifType::LastModified:
        cifLine = lineTemplate.arg(QString("%1").arg(CifLine::nameForType(CifLine::CifType::LastModified)));
        break;
    case CifLine::CifType::Message:
        cifLine = lineTemplate.arg(
                QString("%1 (1, 1), (2, 2), (3, 3)").arg(CifLine::nameForType(CifLine::CifType::Message)));
        break;
    case CifLine::CifType::MscDocument:
        cifLine =
                lineTemplate.arg(QString("%1 (1, 1), (2, 2)").arg(CifLine::nameForType(CifLine::CifType::MscDocument)));
        break;
    case CifLine::CifType::Modified:
        cifLine = lineTemplate.arg(QString("%1").arg(CifLine::nameForType(CifLine::CifType::Modified)));
        break;
    case CifLine::CifType::MscPageSize:
        cifLine = lineTemplate.arg(QString("%1 (1, 1)").arg(CifLine::nameForType(CifLine::CifType::MscPageSize)));
        break;
    case CifLine::CifType::Nested:
        cifLine = lineTemplate.arg(QString("%1").arg(CifLine::nameForType(CifLine::CifType::Nested)));
        break;
    case CifLine::CifType::Position:
        cifLine = lineTemplate.arg(QString("%1 (1, 1)").arg(CifLine::nameForType(CifLine::CifType::Position)));
        break;
    case CifLine::CifType::Preview:
        cifLine = lineTemplate.arg(QString("%1").arg(CifLine::nameForType(CifLine::CifType::Preview)));
        break;
    case CifLine::CifType::Reset:
        cifLine = lineTemplate.arg(QString("%1 (1, 1), (2, 2)").arg(CifLine::nameForType(CifLine::CifType::Reset)));
        break;
    case CifLine::CifType::Set:
        cifLine = lineTemplate.arg(QString("%1 (1, 1), (2, 2)").arg(CifLine::nameForType(CifLine::CifType::Set)));
        break;
    case CifLine::CifType::Stop:
        cifLine = lineTemplate.arg(QString("%1 (1, 1), (2, 2)").arg(CifLine::nameForType(CifLine::CifType::Stop)));
        break;
    case CifLine::CifType::Submsc:
        cifLine = lineTemplate.arg(QString("%1").arg(CifLine::nameForType(CifLine::CifType::Submsc)));
        break;
    case CifLine::CifType::Specific:
        cifLine = lineTemplate.arg(QString("%1").arg(CifLine::nameForType(CifLine::CifType::Specific)));
        break;
    case CifLine::CifType::Text:
        cifLine = lineTemplate.arg(QString("%1 (1, 1), (2, 2)").arg(CifLine::nameForType(CifLine::CifType::Text)));
        break;
    case CifLine::CifType::Timeout:
        cifLine = lineTemplate.arg(QString("%1 (1, 1), (2, 2)").arg(CifLine::nameForType(CifLine::CifType::Timeout)));
        break;
    case CifLine::CifType::TextMode:
        cifLine = lineTemplate.arg(QString("%1").arg(CifLine::nameForType(CifLine::CifType::TextMode)));
        break;
    case CifLine::CifType::TextName:
        cifLine = lineTemplate.arg(
                QString("%1 Text used for collapsed comments").arg(CifLine::nameForType(CifLine::CifType::TextName)));
        break;
    case CifLine::CifType::GlobalComment:
        cifLine = QString("/* Text should be placed as global cif comment */");
        break;

    default:
        QFAIL("It seems a new CifLine::CifType has been introduced,\n"
              "but it's not covered here.\n"
              "Please add it to process or ignore explicitly.");
    }

    outLine = cifLine;
}

void tst_CifParser::initTestCase()
{
    m_mscReader = new MscReader;
    m_cifParser = new CifParser;
}

void tst_CifParser::cleanupTestCase()
{
    delete m_cifParser;
    delete m_mscReader;
}

void tst_CifParser::testCoverage()
{
    static const QString testMethodNamePrefixLine("testParsingCifLine");
    static const QString testMethodNamePrefixBlock("testParsingCifBlock");
    const QMetaObject *metaMe = metaObject();
    QStringList testMethodsLine, testMethodsBlock;
    for (int i = metaMe->methodOffset(); i < metaMe->methodCount(); ++i) {
        const QString method = QString::fromLatin1(metaMe->method(i).methodSignature());
        if (method.startsWith(testMethodNamePrefixLine))
            testMethodsLine << method;
        if (method.startsWith(testMethodNamePrefixBlock))
            testMethodsBlock << method;
    }

    QCOMPARE(testMethodsLine.size(), testMethodsBlock.size());

    const QMetaEnum &e = QMetaEnum::fromType<CifLine::CifType>();
    int ignoredTypes(0);
    ++ignoredTypes; // CifLine::CifType::Unknown
    ++ignoredTypes; // CifLine::CifType::GlobalComment, parsed as substring of CIF TEXT block

    QCOMPARE(testMethodsLine.size(), e.keyCount() - ignoredTypes);
}

void tst_CifParser::testParsingCifLine(CifLine::CifType entityType)
{
    QString cifLine;
    createCifLine(entityType, cifLine);
    QString source = createMscSource(cifLine);

    try {
        m_mscReader->parseText(source);
    } catch (...) {
        QFAIL("Unexpected exception!");
    }

    CifLineShared cif = m_cifParser->readCifLine(cifLine);
    QVERIFY(cif != nullptr);

    const QString cifLineHead = QString("/* CIF ") + CifLine::nameForType(entityType);
    QString cifLineInvalid(cifLine);
    cifLineInvalid.replace(cifLineHead, cifLineHead + "_invalid");

    QVERIFY(m_cifParser->readCifLine(cifLineInvalid).isNull());
}

void tst_CifParser::testParsingCifLineAction()
{
    testParsingCifLine(CifLine::CifType::Action);
}
void tst_CifParser::testParsingCifLineCall()
{
    testParsingCifLine(CifLine::CifType::Call);
}
void tst_CifParser::testParsingCifLineComment()
{
    testParsingCifLine(CifLine::CifType::Comment);
}
void tst_CifParser::testParsingCifLineCondition()
{
    testParsingCifLine(CifLine::CifType::Condition);
}
void tst_CifParser::testParsingCifLineCreate()
{
    testParsingCifLine(CifLine::CifType::Create);
}
void tst_CifParser::testParsingCifLineCollapsed()
{
    testParsingCifLine(CifLine::CifType::Collapsed);
}
void tst_CifParser::testParsingCifLineEnd()
{
    testParsingCifLine(CifLine::CifType::End);
}
void tst_CifParser::testParsingCifLineHyperLink()
{
    testParsingCifLine(CifLine::CifType::HyperLink);
}
void tst_CifParser::testParsingCifLineInstance()
{
    testParsingCifLine(CifLine::CifType::Instance);
}
void tst_CifParser::testParsingCifLineImport()
{
    testParsingCifLine(CifLine::CifType::Import);
}
void tst_CifParser::testParsingCifLineKeep()
{
    testParsingCifLine(CifLine::CifType::Keep);
}
void tst_CifParser::testParsingCifLineLastModified()
{
    testParsingCifLine(CifLine::CifType::LastModified);
}
void tst_CifParser::testParsingCifLineMessage()
{
    testParsingCifLine(CifLine::CifType::Message);
}
void tst_CifParser::testParsingCifLineMscDocument()
{
    testParsingCifLine(CifLine::CifType::MscDocument);
}
void tst_CifParser::testParsingCifLineModified()
{
    testParsingCifLine(CifLine::CifType::Modified);
}
void tst_CifParser::testParsingCifLineMscPageSize()
{
    testParsingCifLine(CifLine::CifType::MscPageSize);
}
void tst_CifParser::testParsingCifLineNested()
{
    testParsingCifLine(CifLine::CifType::Nested);
}
void tst_CifParser::testParsingCifLinePosition()
{
    testParsingCifLine(CifLine::CifType::Position);
}
void tst_CifParser::testParsingCifLinePreview()
{
    testParsingCifLine(CifLine::CifType::Preview);
}
void tst_CifParser::testParsingCifLineReset()
{
    testParsingCifLine(CifLine::CifType::Reset);
}
void tst_CifParser::testParsingCifLineSet()
{
    testParsingCifLine(CifLine::CifType::Set);
}
void tst_CifParser::testParsingCifLineStop()
{
    testParsingCifLine(CifLine::CifType::Stop);
}
void tst_CifParser::testParsingCifLineSubmsc()
{
    testParsingCifLine(CifLine::CifType::Submsc);
}
void tst_CifParser::testParsingCifLineSpecific()
{
    testParsingCifLine(CifLine::CifType::Specific);
}
void tst_CifParser::testParsingCifLineText()
{
    testParsingCifLine(CifLine::CifType::Text);
}
void tst_CifParser::testParsingCifLineTimeout()
{
    testParsingCifLine(CifLine::CifType::Timeout);
}
void tst_CifParser::testParsingCifLineTextMode()
{
    testParsingCifLine(CifLine::CifType::TextMode);
}
void tst_CifParser::testParsingCifLineTextName()
{
    testParsingCifLine(CifLine::CifType::TextName);
}

void tst_CifParser::testParsingCifBlock(const QVector<QVector<CifLine::CifType>> &blocks)
{
    QStringList cifLines, cifLinesClean;
    for (const QVector<CifLine::CifType> &block : blocks)
        for (CifLine::CifType t : block) {
            QString line;
            createCifLine(t, line);
            cifLines << line;

            // rm /* and */
            line = line.mid(2);
            line.chop(2);
            line = line.trimmed();
            cifLinesClean << line;
        }

    const QString &source = createMscSource(cifLines.join("\n"));
    try {
        m_mscReader->parseText(source);
    } catch (...) {
        QFAIL("Unexpected exception!");
    }

    const QVector<CifBlockShared> &cifs = m_cifParser->readCifBlocks(cifLinesClean);
    QVERIFY(cifs.size() == blocks.size());
    if (cifs.size()) {
        QVERIFY(cifs.first() != nullptr);
    }
}

void tst_CifParser::testParsingCifBlockAction()
{
    QSKIP(qPrintable(QString("Not implemented yet")));

    /* CIF ACTION (514, 481), (361, 74) */
    /* CIF TextMode 4 */
    /* CIF Modified */

    testParsingCifBlock({ { CifLine::CifType::Action, CifLine::CifType::TextMode, CifLine::CifType::Modified } });
}

void tst_CifParser::testParsingCifBlockCall()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF Keep Specific Geode Generated From 1 1442 */
    /* CIF CALL (5190, 10650), (270, 320) */
    /* CIF TextMode 4 */

    testParsingCifBlock({ { CifLine::CifType::Keep, CifLine::CifType::Call, CifLine::CifType::TextMode } });
}

void tst_CifParser::testParsingCifBlockComment()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF COMMENT (1147, 1449), (127, 159) */
    /* CIF TextMode 3 */
    /* CIF Modified */

    testParsingCifBlock({ { CifLine::CifType::Comment, CifLine::CifType::TextMode, CifLine::CifType::Modified } });
}

void tst_CifParser::testParsingCifBlockCondition()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF CONDITION (372, 463), (250, 150) */
    /* CIF TextMode 4 */
    testParsingCifBlock({ { CifLine::CifType::Condition, CifLine::CifType::TextMode } });
}

void tst_CifParser::testParsingCifBlockCreate()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF TextMode 3 */
    /* CIF CREATE (925, 10330) (5200, 10560) */
    /* CIF POSITION (3065, 10409) */
    testParsingCifBlock({ { CifLine::CifType::TextMode, CifLine::CifType::Create, CifLine::CifType::Position } });
}

void tst_CifParser::testParsingCifBlockCollapsed()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF TextMode 4 */
    /* CIF TEXT (715, 40), (469, 82) */
    /* CIF Collapsed */
    /* CIF TextName Nominal transaction         */
    /* This scenario describes the nominal
    scenario associated with a transaction*/
    /* CIF End Text */
    testParsingCifBlock({ { CifLine::CifType::TextMode, CifLine::CifType::Text, CifLine::CifType::Collapsed,
            CifLine::CifType::TextName, CifLine::CifType::End } });
}

void tst_CifParser::testParsingCifBlockEnd()
{
    // TODO: to be used in pair with CifLine::CifType::Text
    QSKIP(qPrintable(QString("Unsupported yet")));
}

void tst_CifParser::testParsingCifBlockHyperLink()
{
    QSKIP(qPrintable(QString("Unsupported yet")));
}

void tst_CifParser::testParsingCifBlockInstance()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF TextMode 4 */
    /* CIF Modified */
    /* CIF INSTANCE (874, 100), (312, 150), (800, 463) */
    testParsingCifBlock({ { CifLine::CifType::TextMode, CifLine::CifType::Modified, CifLine::CifType::Instance } });
}

void tst_CifParser::testParsingCifBlockImport()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF Specific Geode Version V4.0.0 beta.1 (3) geodedit */
    /* CIF Import 1 ATM.cd */
    /* CIF LastModified Wed Dec 10 15:54:39 1997 */ /* MSC REPEAT */;
    testParsingCifBlock({ { CifLine::CifType::Specific, CifLine::CifType::Import, CifLine::CifType::LastModified } });
}

void tst_CifParser::testParsingCifBlockKeep()
{
    /* CIF Keep Specific Geode SDL 1 ATM.pr */
    /* CIF Keep Specific Geode SDL 2 Netware.pr */
    QSKIP(qPrintable(QString("Unsupported yet")));
}

void tst_CifParser::testParsingCifBlockLastModified()
{
    /* CIF LastModified Tue Nov 23 15:19:52 1999 */;
    QSKIP(qPrintable(QString("Unsupported yet")));
}

void tst_CifParser::testParsingCifBlockMessage()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF TextMode 4 */
    /* CIF Modified */
    /* CIF MESSAGE (20, 239) (758, 239) */
    /* CIF POSITION (209, 193) */
    testParsingCifBlock({ { CifLine::CifType::TextMode, CifLine::CifType::Modified, CifLine::CifType::Message,
            CifLine::CifType::Position } });
}

void tst_CifParser::testParsingCifBlockMscDocument()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF MSCDOCUMENT (20, 20), (1619, 2043) */
    /* CIF TextMode 0 */
    /* CIF Modified */
    testParsingCifBlock({ { CifLine::CifType::MscDocument, CifLine::CifType::TextMode, CifLine::CifType::Modified } });
}

void tst_CifParser::testParsingCifBlockModified()
{
    /* CIF Modified */
    QSKIP(qPrintable(QString("Unsupported yet")));
}

void tst_CifParser::testParsingCifBlockMscPageSize()
{
    // TODO: to be used in pair with CifLine::CifType::MscDocument
    QSKIP(qPrintable(QString("Unsupported yet")));
}

void tst_CifParser::testParsingCifBlockNested()
{
    // TODO: to be used in pair with CifLine::CifType::SUBMSC
    QSKIP(qPrintable(QString("Unsupported yet")));
}

void tst_CifParser::testParsingCifBlockPosition()
{
    // TODO: to be used in pair with CifLine::CifType::Message, etc
    QSKIP(qPrintable(QString("Unsupported yet")));
}

void tst_CifParser::testParsingCifBlockPreview()
{
    // TODO: to be used in pair with CifLine::CifType::MscDocument
    QSKIP(qPrintable(QString("Unsupported yet")));
}

void tst_CifParser::testParsingCifBlockReset()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF RESET (1271, 529), (-139, 116) */
    /* CIF TextMode 3 */
    /* CIF Modified */
    testParsingCifBlock({ { CifLine::CifType::Reset, CifLine::CifType::TextMode, CifLine::CifType::Modified } });
}

void tst_CifParser::testParsingCifBlockSet()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF SET (695, 1700), (87, 58) */
    /* CIF TextMode 3 */
    /* CIF Modified */
    testParsingCifBlock({ { CifLine::CifType::Set, CifLine::CifType::TextMode, CifLine::CifType::Modified } });
}

void tst_CifParser::testParsingCifBlockStop()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF STOP (553, 1380), (250, 150) */
    /* CIF TextMode 4 */
    /* CIF Modified */
    testParsingCifBlock({ { CifLine::CifType::Stop, CifLine::CifType::TextMode, CifLine::CifType::Modified } });
}

void tst_CifParser::testParsingCifBlockSubmsc()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF SUBMSC (99, 79), (250, 110) */
    /* CIF TextMode 4 */
    /* CIF Modified */
    /* CIF NESTED */
    QSKIP(qPrintable(QString("Unsupported yet")));
}
void tst_CifParser::testParsingCifBlockSpecific()
{
    QSKIP(qPrintable(QString("Unsupported yet")));
}

void tst_CifParser::testParsingCifBlockText()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF TextMode 4 */
    /* CIF TEXT (160, 990), (405, 114) */
    /* The inserted card
    is authorized by the
    consortium */
    /* CIF End Text */
    testParsingCifBlock({ { CifLine::CifType::TextMode, CifLine::CifType::Text, CifLine::CifType::End } });
}

void tst_CifParser::testParsingCifBlockTimeout()
{
    QSKIP(qPrintable(QString("Not implemented yet")));
    /* CIF TIMEOUT (489, 279), (222, 93) */
    /* CIF TextMode 3 */
    /* CIF Modified */
    testParsingCifBlock({ { CifLine::CifType::Timeout, CifLine::CifType::TextMode, CifLine::CifType::Modified } });
}

void tst_CifParser::testParsingCifBlockTextMode()
{
    // used within others?
    QSKIP(qPrintable(QString("Unsupported yet")));
}

void tst_CifParser::testParsingCifBlockTextName()
{
    // used within text?
    QSKIP(qPrintable(QString("Unsupported yet")));
}

void tst_CifParser::testParsingCifAtDocumentFront()
{
    const QString source = "\
            /* CIF MSCDOCUMENT (20, 20), (1521, 1552) */\
            /* CIF MscPageSize (2100, 2970) */ \
            MSCDOCUMENT doc;\
            ENDMSCDOCUMENT;";
    QScopedPointer<msc::MscModel> model;
    try {
        model.reset(m_mscReader->parseText(source));
    } catch (...) {
        QFAIL("Unexpected exception!");
    }

    QCOMPARE(model->documents().size(), 1);
    msc::MscDocument *doc = model->documents().at(0);
    QVector<cif::CifBlockShared> cifs = doc->cifs();
    // MSDOCUMENT comment only is used, as MscPageSize is ignored in CifBlockFactory::createBlock
    QCOMPARE(cifs.size(), 1);
}

void tst_CifParser::testIgnoreUnknownCif()
{
    const QString source = "\
            /* CIF Font  '-microsoft windows-arial-bold-r-*-*-12-*-*-*-*-*-*-*' */\
            MSCDOCUMENT doc;\
            ENDMSCDOCUMENT;";
    QScopedPointer<msc::MscModel> model;
    try {
        model.reset(m_mscReader->parseText(source));
    } catch (...) {
        QFAIL("Unexpected exception!");
    }

    QCOMPARE(model->documents().size(), 1);
    msc::MscDocument *doc = model->documents().at(0);
    QVector<cif::CifBlockShared> cifs = doc->cifs();
    QCOMPARE(cifs.size(), 0);
}

QTEST_APPLESS_MAIN(tst_CifParser)

#include "tst_cifparser.moc"
