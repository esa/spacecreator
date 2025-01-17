
#include "endtoend/endtoendview.h"
#include "interfacedocument.h"
#include "ivlibrary.h"
#include "scversion.h"
#include "sharedlibrary.h"
#include "templatinglibrary.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char **argv)
{
    templating::initTemplatingLibrary();
    ivm::initIVLibrary();
    shared::initSharedLibrary();

    QApplication app(argc, argv);
    app.setOrganizationName(SC_ORGANISATION);
    app.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    app.setApplicationVersion(spaceCreatorVersionAndGit);
    app.setApplicationName(QObject::tr("e2eimagesaver"));

    QCommandLineParser cmdParser;
    cmdParser.setApplicationDescription("Save end-to-end diagram as an image");
    cmdParser.addHelpOption();
    cmdParser.addVersionOption();
    cmdParser.addPositionalArgument("interfaceview",
            QCoreApplication::translate("main", "Path to the file with diagram which should be used to save as image"));
    cmdParser.addPositionalArgument(
            "msc", QCoreApplication::translate("main", "Path to MSC which is used for End to end view"));
    const QCommandLineOption chartOption(
            "chart", QCoreApplication::translate("main", "Name of the chart to dump a screenshot"), "chartOption");
    cmdParser.addOption(chartOption);
    cmdParser.addPositionalArgument("output", QCoreApplication::translate("main", "Path to Output image file"));
    cmdParser.process(app);
    const QStringList args = cmdParser.positionalArguments();

    if (args.count() != 3) {
        cmdParser.showHelp(1);
        return 0;
    }

    const QString inputIVFile = args.at(0);
    const QString inputMSCFile = args.at(1);
    const QString outputFile = args.at(2);

    qDebug() << "Generating E2E View from\nInterfaceview: " << inputIVFile << "MSC:" << inputMSCFile
             << " storing it to: " << outputFile;

    // ive::IVEditorCore editorCore;
    ive::InterfaceDocument *document = new ive::InterfaceDocument();
    document->loadAvailableComponents();
    if (!document->load(inputIVFile)) {
        qCritical() << "Error: Can't load the interfaceview model:" << inputIVFile;
        return 2;
    }

    auto endToEndView = new ive::EndToEndView(document);
    QString errorString;
    if (!endToEndView->setMscFile(inputMSCFile, &errorString)) {
        qCritical() << "Error: " << errorString;
        return 3;
    }

    if (cmdParser.isSet(chartOption)) {
        const QString chartName = cmdParser.value(chartOption);
        if (!endToEndView->setMscChart(chartName)) {
            qCritical() << QCoreApplication::translate("main", "Error: Unable to select the chartname") << chartName;
            return 4;
        }
    }

    if (!endToEndView->refreshView()) {
        qCritical() << "Error: Can't generate end-to-end view from interfaceview: " << inputIVFile
                    << " with msc file:" << inputMSCFile;
        return 5;
    }

    if (!endToEndView->exportScene(outputFile)) {
        qCritical() << "Error: Can't export End to end view into::" << outputFile;
        return 6;
    }

    return 0;
}
