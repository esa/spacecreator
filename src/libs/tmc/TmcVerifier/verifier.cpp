/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "verifier.h"

#include <QDebug>
#include <QDirIterator>
#include <QMessageBox>
#include <QRegularExpression>
#include <QTimer>
#include <reporting/HtmlReport/htmlreportbuilder.h>
#include <reporting/HtmlReport/htmlreportpresenter.h>
#include <reporting/Report/spinerrorparser.h>

using reporting::HtmlReportBuilder;
using reporting::HtmlReportPresenter;
using tmc::converter::TmcConverter;

namespace tmc::verifier {
TmcVerifier::TmcVerifier(const QString &inputIvFilepath, const QString &outputDirectory)
    : m_inputIvFilepath(inputIvFilepath)
    , m_outputDirectory(outputDirectory)
    , m_explorationMode(ExplorationMode::DepthFirst)
    , m_searchShortestPath(false)
    , m_useFairScheduling(false)
    , m_useBitHashing(false)
    , m_additionalCompilerFlags("-O2")
    , m_executeMode(ExecuteMode::ConvertAndVerify)
    , m_process(new QProcess(this))
    , m_verifierProcess(new QProcess(this))
    , m_traceGeneratorProcess(new QProcess(this))
    , m_timer(new QTimer(this))
    , m_trailCounter(1)
{
    m_converter = std::make_unique<TmcConverter>(m_inputIvFilepath, m_outputDirectory);

    connect(m_converter.get(), SIGNAL(message(QString)), this, SIGNAL(verifierMessage(QString)));
    m_conversionFinishedConnection =
            connect(m_converter.get(), SIGNAL(conversionFinished(bool)), this, SLOT(conversionFinished(bool)));
    connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(processStderrReady()));
    connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStdoutReady()));
    connect(m_process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
    connect(m_verifierProcess, SIGNAL(readyReadStandardError()), this, SLOT(verifierStderrReady()));
    connect(m_verifierProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(verifierStdoutReady()));
    connect(m_verifierProcess, SIGNAL(errorOccurred(QProcess::ProcessError)), this,
            SLOT(processError(QProcess::ProcessError)));

    connect(m_traceGeneratorProcess, SIGNAL(readyReadStandardError()), this, SLOT(traceStderrReady()));
    connect(m_traceGeneratorProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(traceStdoutReady()));
    connect(m_traceGeneratorProcess, SIGNAL(started()), this, SLOT(traceGeneratorStarted()));
    connect(m_traceGeneratorProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(traceGeneratorFinished(int, QProcess::ExitStatus)));
    connect(m_traceGeneratorProcess, SIGNAL(errorOccurred(QProcess::ProcessError)), this,
            SLOT(processError(QProcess::ProcessError)));

    m_processStartedConnection = connect(m_process, SIGNAL(started()), this, SLOT(ccStarted()));

    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

TmcVerifier::~TmcVerifier()
{
    m_converter.reset();
    m_timer->stop();
    if (m_process->state() != QProcess::ProcessState::NotRunning) {
        m_process->kill();
        m_process->waitForFinished();
    }
    if (m_verifierProcess->state() != QProcess::ProcessState::NotRunning) {
        m_verifierProcess->kill();
        m_verifierProcess->waitForFinished();
    }
    if (m_traceGeneratorProcess->state() != QProcess::ProcessState::NotRunning) {
        m_traceGeneratorProcess->kill();
        m_traceGeneratorProcess->waitForFinished();
    }
}

void TmcVerifier::setMscObserverFiles(const QStringList &mscObserverFiles)
{
    m_converter->setMscObserverFiles(mscObserverFiles);
    m_mscObserverFiles = mscObserverFiles;
}

void TmcVerifier::setEnvironmentFunctions(const std::vector<QString> &environmentFunctions)
{
    m_converter->setEnvironmentFunctions(environmentFunctions);
}

void TmcVerifier::setKeepFunctions(const std::vector<QString> &keepFunctions)
{
    m_converter->setKeepFunctions(keepFunctions);
}

void TmcVerifier::setGlobalInputVectorLengthLimit(std::optional<QString> limit)
{
    m_converter->setGlobalInputVectorLengthLimit(std::move(limit));
}

void TmcVerifier::setInterfaceInputVectorLengthLimits(std::unordered_map<QString, QString> limits)
{
    m_converter->setInterfaceInputVectorLengthLimits(std::move(limits));
}

void TmcVerifier::setProcessesBasePriority(std::optional<QString> value)
{
    m_converter->setProcessesBasePriority(std::move(value));
}

void TmcVerifier::setRealTypeEnabled(bool isRealTypeEnabled)
{
    m_converter->setRealTypeEnabled(isRealTypeEnabled);
}

void TmcVerifier::setDelta(std::optional<QString> value)
{
    m_converter->setDelta(std::move(value));
}

void TmcVerifier::setMulticastEnabled(bool isMulticastEnabled)
{
    m_converter->setMulticastEnabled(isMulticastEnabled);
}

void TmcVerifier::setSdl2PromelaTimeout(int timeout)
{
    m_converter->setSdl2PromelaTimeout(timeout);
}

void TmcVerifier::setCCompilerTimeout(int timeout)
{
    m_ccompilerTimeout = timeout;
}

void TmcVerifier::setExternalCommandTimeout(int timeout)
{
    m_externalCommandTimeout = timeout;
}

void TmcVerifier::setAdditionalCompilerFlags(QString compilerFlags)
{
    m_additionalCompilerFlags = std::move(compilerFlags);
}

void TmcVerifier::setSubtypesFilepaths(const std::vector<QString> &filepaths)
{
    m_converter->setSubtypesFilepaths(filepaths);
}

bool TmcVerifier::addStopConditionFiles(const QStringList &files)
{
    return m_converter->addStopConditionFiles(files);
}

bool TmcVerifier::attachObserver(const QString &observerPath, const uint32_t priority)
{
    return m_converter->attachObserver(observerPath, priority);
}

void TmcVerifier::setExplorationMode(ExplorationMode mode)
{
    m_explorationMode = mode;
}

void TmcVerifier::setSearchShortestPath(bool enabled)
{
    m_searchShortestPath = enabled;
}

void TmcVerifier::setUseFairScheduling(bool enabled)
{
    m_useFairScheduling = enabled;
}

void TmcVerifier::setUseBitHashing(bool enabled)
{
    m_useBitHashing = enabled;
}

void TmcVerifier::setNumberOfCores(int value)
{
    m_numberOfCores = value;
}

void TmcVerifier::setTimeLimit(int timeLimit)
{
    m_timeLimitSeconds = timeLimit;
}

void TmcVerifier::setSearchStateLimit(int searchStateLimit)
{
    m_searchStateLimit = searchStateLimit;
}

void TmcVerifier::setErrorLimit(int errorLimit)
{
    m_errorLimit = errorLimit;
}

void TmcVerifier::setMemoryLimit(int memoryLimit)
{
    m_memoryLimit = memoryLimit;
}

void TmcVerifier::setVectorszValue(int vectorValue)
{
    m_vectorszValue = vectorValue;
}

void TmcVerifier::setRawCommandline(QString rawCommandline)
{
    m_rawCommandline = std::move(rawCommandline);
}

void TmcVerifier::setSpinExecutable(QString spinExecutable)
{
    m_spinExecutable = std::move(spinExecutable);
}

bool TmcVerifier::execute(ExecuteMode executeMode)
{
    Q_EMIT verifierMessage(QString("Starting conversion.\n"));
    m_executeMode = executeMode;
    if (m_outputDirectory.isEmpty()) {
        Q_EMIT verifierMessage(QString("Output directory path is not specified. Aborting.\n"));
        Q_EMIT finished(false);
        return false;
    }
    if (m_executeMode == ExecuteMode::ConvertOnly) {
        // run only TmcConverter
        if (!m_converter->prepare()) {
            return false;
        }
        m_converter->convert();
    } else {
        // start from `make observer_dataview`
        // then run TmcConverter and start verification process
        generateObserverDataview();
    }
    return true;
}

void TmcVerifier::generateObserverDataview()
{
    const QFileInfo inputIvInfo(m_inputIvFilepath);
    const QString emptyAsnName = "empty.asn";

    // create empty
    QString emptyAsnFilepath = inputIvInfo.absoluteDir().path() + QDir::separator() + "work" + QDir::separator()
            + "modelchecking" + QDir::separator() + "subtypes" + QDir::separator() + emptyAsnName;

    QFileInfo emptyAsnInfo(emptyAsnFilepath);

    if (!emptyAsnInfo.exists()) {
        QFile emptyAsnFile(emptyAsnFilepath);
        emptyAsnFile.open(QIODevice::WriteOnly | QIODevice::Append);

        emptyAsnFile.close();
    }

    const QString makeExe = "make";
    m_process->setWorkingDirectory(inputIvInfo.absoluteDir().path());

    if (m_processFinishedConnection) {
        disconnect(m_processFinishedConnection);
    }
    m_processFinishedConnection = connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(makeFinished(int, QProcess::ExitStatus)));
    if (m_processStartedConnection) {
        disconnect(m_processStartedConnection);
    }
    m_processStartedConnection = connect(m_process, SIGNAL(started()), this, SLOT(makeStarted()));

    QStringList arguments;
    arguments.append(QString("SUBTYPE=%1").arg(emptyAsnName));
    arguments.append("observer_dataview");

    Q_EMIT verifierMessage(QString("Executing %1 %2\n").arg(makeExe).arg(arguments.join(" ")));
    m_timer->setSingleShot(true);
    m_timer->start(m_startTimeout);
    m_process->start(makeExe, arguments);
}

void TmcVerifier::buildVerifier()
{
    Q_EMIT verifierMessage(QString("Building verifier.\n"));

    executeSpin();
}

void TmcVerifier::executeVerifier()
{
    Q_EMIT verifierMessage(QString("Executing verifier.\n"));

    QDir(m_outputDirectory).remove("pan.output");

    const QString exe = "./pan";

    m_verifierProcess->setWorkingDirectory(m_outputDirectory);

    disconnect(m_processFinishedConnection);
    m_processFinishedConnection = connect(m_verifierProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(panFinished(int, QProcess::ExitStatus)));
    disconnect(m_processStartedConnection);
    m_processStartedConnection = connect(m_verifierProcess, SIGNAL(started()), this, SLOT(panStarted()));

    QStringList arguments;
    if (m_rawCommandline.isEmpty()) {
        arguments.append("-E"); // suppress the reporting of invalid endstate errors
        arguments.append("-n"); // no listing of unreached states at the end of the run

        if (m_explorationMode != ExplorationMode::BreadthFirst) {
            arguments.append("-a"); // find acceptance cycles, not available in BFS
        }

        if (m_useFairScheduling) {
            arguments.append("-f"); // add weak fairness
        }

        if (m_errorLimit.has_value()) {
            arguments.append("-e"); // create trails for all errors encountered
            arguments.append(QString("-c%1").arg(m_errorLimit.value())); // stop at Nth error
        }

        if (m_searchStateLimit.has_value()) {
            arguments.append(QString("-m%1").arg(m_searchStateLimit.value())); // set max search depth to N steps
        }

        if (m_searchShortestPath) {
            arguments.append("-i"); // search for shortest path to error
        }

    } else {
        arguments = m_rawCommandline.split(' ');
    }

    Q_EMIT verifierMessage(QString("Executing %1 %2\n").arg(exe).arg(arguments.join(" ")));
    m_timer->setSingleShot(true);
    m_timer->start(m_startTimeout);
    m_verifierProcess->start(exe, arguments);
}

void TmcVerifier::executeSpin()
{
    const QString inputFile = "system.pml";
    const QString spinExe = m_spinExecutable.isEmpty() ? QString("spin") : m_spinExecutable;

    m_process->setWorkingDirectory(m_outputDirectory);

    if (m_processFinishedConnection) {
        disconnect(m_processFinishedConnection);
    }
    m_processFinishedConnection = connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(spinFinished(int, QProcess::ExitStatus)));
    if (m_processStartedConnection) {
        disconnect(m_processStartedConnection);
    }
    m_processStartedConnection = connect(m_process, SIGNAL(started()), this, SLOT(spinStarted()));

    QStringList arguments;
    arguments.append("-a");
    arguments.append(inputFile);
    Q_EMIT verifierMessage(QString("Executing %1 %2\n").arg(spinExe).arg(arguments.join(" ")));
    m_timer->setSingleShot(true);
    m_timer->start(m_startTimeout);
    m_process->start(spinExe, arguments);
}

void TmcVerifier::startCompilation()
{
    QDir outputDir(m_outputDirectory);

    // find generated C source files
    m_cSourceFiles = outputDir.entryList(QStringList("*.c"),
            QDir::Files | QDir::CaseSensitive | QDir::Readable | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    m_process->setWorkingDirectory(m_outputDirectory);

    disconnect(m_processFinishedConnection);
    m_processFinishedConnection = connect(
            m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(ccFinished(int, QProcess::ExitStatus)));
    disconnect(m_processStartedConnection);
    m_processStartedConnection = connect(m_process, SIGNAL(started()), this, SLOT(ccStarted()));

    executeCC();
}

void TmcVerifier::executeCC()
{
    const QString inputFile = m_cSourceFiles.front();
    m_cSourceFiles.pop_front();

    QStringList arguments = m_additionalCompilerFlags.split(" ");

    if (inputFile == "pan.c") {
        constexpr int VECTORSZ_DEFAULT_VALUE = 65535;
        int vectorszValue = m_vectorszValue.value_or(VECTORSZ_DEFAULT_VALUE);
        arguments.append("-DVECTORSZ=" + QString::number(vectorszValue));
        qDebug() << "VECTORSZ: " << vectorszValue;

        if (m_useBitHashing) {
            arguments.append("-DBITSTATE");
        }

        if (m_explorationMode == ExplorationMode::BreadthFirst) {
            arguments.append("-DBFS");
        }

        if (m_memoryLimit.has_value()) {
            arguments.append(QString("-DMEMLIM=%1").arg(m_memoryLimit.value()));
        }

        if (m_numberOfCores.has_value()) {
            arguments.append(QString("-DNCORE=%1").arg(m_numberOfCores.value()));
        }

        if (m_useFairScheduling) {
            arguments.append(QString("-DNFAIR=%1").arg(m_converter->getNumberOfProctypes()));
        }

        if (m_searchShortestPath) {
            arguments.append("-DREACH");
        }
    }

    arguments.append("-c");
    arguments.append(inputFile);

    runCC(arguments);
}

void TmcVerifier::executeCCLinker()
{
    disconnect(m_processFinishedConnection);
    m_processFinishedConnection = connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(ccLinkerFinished(int, QProcess::ExitStatus)));

    QDir outputDir(m_outputDirectory);
    QStringList arguments = outputDir.entryList(QStringList("*.o"),
            QDir::Files | QDir::CaseSensitive | QDir::Readable | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    const QString outputFile = "pan";
    arguments.append("-o");
    arguments.append(outputFile);

    runCC(arguments);
}

void TmcVerifier::runCC(const QStringList &arguments)
{
    const QString compilerExe = "gcc";

    Q_EMIT verifierMessage(QString("Executing %1 %2\n").arg(compilerExe).arg(arguments.join(" ")));
    m_timer->setSingleShot(true);
    m_timer->start(m_startTimeout);
    m_process->start(compilerExe, arguments);
}

void TmcVerifier::collectErrors()
{
    QString outputFilepath = m_outputDirectory + QDir::separator() + "pan.output";
    QFile file(outputFilepath);
    if (!file.open(QIODevice::ReadOnly)) {
        Q_EMIT verifierMessage(QString("Cannot read file %1\n").arg(outputFilepath));
        Q_EMIT conversionFinished(false);
    }

    QTextStream stream(&file);

    QRegularExpression searchRegExp(R"(errors: (\d+))");

    std::optional<int> errors;

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        QRegularExpressionMatch match = searchRegExp.match(line);
        if (match.hasMatch()) {
            errors = match.captured(1).toInt();
            break;
        }
    }

    file.close();
    if (errors.has_value()) {
        Q_EMIT verifierMessage(QString("Found %1 errors\n").arg(errors.value()));
        generateTraces(errors.value());
    } else {
        Q_EMIT finished(true);
    }
}

void TmcVerifier::generateTraces(int count)
{
    QDirIterator iter(m_outputDirectory, { "*.trail" }, QDir::Files);
    while (iter.hasNext()) {
        iter.next();
        QString file = iter.fileName();
        qDebug() << "Adding " << file;
        m_trailFilesToConvert.append(file);
    }

    if (m_trailFilesToConvert.size() != count) {
        Q_EMIT verifierMessage(QString("Expected number of trail files: %1, but found %2\n")
                                       .arg(count)
                                       .arg(m_trailFilesToConvert.size()));
    }

    generateNextTrace();
}

void TmcVerifier::generateNextTrace()
{
    if (m_trailFilesToConvert.isEmpty()) {
        generateReport();
        return;
    }

    QString trailFile = m_trailFilesToConvert.front();
    m_trailFilesToConvert.pop_front();

    qDebug() << "Write to " << m_currentTraceFile;
    m_currentTraceFile = m_outputDirectory + QDir::separator() + QString("trace_%1.spt").arg(m_trailCounter);
    ++m_trailCounter;
    m_traceGeneratorProcess->setWorkingDirectory(m_outputDirectory);

    const QString exe = "./pan";

    QStringList arguments;
    arguments.append("-S"); // silent replay: only user defined printfs show
    arguments.append("-n"); // no listing of unreached states
    arguments.append("-r"); // read and execute trail in file
    arguments.append(trailFile);
    Q_EMIT verifierMessage(QString("Executing %1 %2\n").arg(exe).arg(arguments.join(" ")));
    m_timer->setSingleShot(true);
    m_timer->start(m_startTimeout);
    m_traceGeneratorProcess->start(exe, arguments);
}

void TmcVerifier::generateReport()
{
    Q_EMIT verifierMessage(QString("Generating report\n"));
    HtmlReportBuilder builder;

    QString outputFilepath = m_outputDirectory + QDir::separator() + "pan.output";

    QStringList spinMessages;
    spinMessages.append(outputFilepath);

    QStringList sclFiles = m_converter->getStopConditionFiles();

    QList<reporting::RawErrorItem> rawErrorItems;
    const int traceCount = m_spinTraceFiles.size();
    for (int i = 0; i < traceCount; ++i) {
        reporting::RawErrorItem rawErrorItem;
        rawErrorItem.spinTraceFile = m_spinTraceFiles[i];
        rawErrorItem.scenarioFile = m_traceFiles[i];
        rawErrorItem.possibleCycleSource = std::make_pair(QString(), 0);
        rawErrorItems.append(rawErrorItem);
    };

    QString report = builder.parseAndBuildHtmlReport(spinMessages, sclFiles, rawErrorItems, m_mscObserverFiles);
    QFileInfo reportFilepath = QFileInfo(m_outputDirectory + QDir::separator() + "report.html");

    saveReport(reportFilepath, report);
    if (m_executeMode == ExecuteMode::ConvertAndVerify) {
        presentReport(reportFilepath);
    }

    Q_EMIT finished(true);
}

void TmcVerifier::presentReport(const QFileInfo &reportFilepath)
{
    HtmlReportPresenter::present(reportFilepath.absoluteFilePath());
}

void TmcVerifier::saveReport(const QFileInfo &reportFilepath, const QString &data)
{
    Q_EMIT verifierMessage(QString("Generated %1 chars\n").arg(data.length()));

    QFile file(reportFilepath.absoluteFilePath());
    file.open(QIODevice::WriteOnly);
    if (!file.isOpen()) {
        Q_EMIT verifierMessage(QString("Cannot open %1\n").arg(file.errorString()));
    }
    QByteArray buffer = data.toUtf8();
    qint64 bytesWritten = file.write(buffer);
    if (bytesWritten != data.size()) {
        Q_EMIT verifierMessage(QString("write error %1\n").arg(file.errorString()));
    }
    file.close();

    Q_EMIT verifierMessage(QString("Report saved to %1\n").arg(reportFilepath.absoluteFilePath()));
}

void TmcVerifier::processStderrReady()
{
    QByteArray buffer = m_process->readAllStandardError();
    QString text = QString(buffer);
    Q_EMIT verifierMessage(text);
}

void TmcVerifier::processStdoutReady()
{
    QByteArray buffer = m_process->readAllStandardOutput();
    QString text = QString(buffer);
    Q_EMIT verifierMessage(text);
}

void TmcVerifier::verifierStderrReady()
{
    QByteArray buffer = m_verifierProcess->readAllStandardError();
    QString text = QString(buffer);
    Q_EMIT verifierMessage(text);

    QString outputFilepath = m_outputDirectory + QDir::separator() + "pan.output";
    QFile file(outputFilepath);
    file.open(QIODevice::Append);
    QTextStream stream(&file);
    stream << buffer;
    file.close();
}

void TmcVerifier::verifierStdoutReady()
{
    QByteArray buffer = m_verifierProcess->readAllStandardOutput();
    QString text = QString(buffer);
    Q_EMIT verifierMessage(text);

    QString outputFilepath = m_outputDirectory + QDir::separator() + "pan.output";
    QFile file(outputFilepath);
    file.open(QIODevice::Append);
    QTextStream stream(&file);
    stream << buffer;
    file.close();
}

void TmcVerifier::traceStderrReady()
{
    QByteArray buffer = m_traceGeneratorProcess->readAllStandardError();
    QFile file(m_currentTraceFile);
    file.open(QIODevice::Append);
    QTextStream stream(&file);
    stream << buffer;
    file.close();
}

void TmcVerifier::traceStdoutReady()
{
    QByteArray buffer = m_traceGeneratorProcess->readAllStandardOutput();
    QFile file(m_currentTraceFile);
    file.open(QIODevice::Append);
    QTextStream stream(&file);
    stream << buffer;
    file.close();
}

void TmcVerifier::conversionFinished(bool success)
{
    if (success) {
        if (m_executeMode == ExecuteMode::ConvertOnly) {
            Q_EMIT(finished(true));
        } else {
            buildVerifier();
        }
    } else {
        Q_EMIT finished(false);
    }
}

void TmcVerifier::makeStarted()
{
    m_timer->stop();
    m_timer->setSingleShot(true);
    int timeoutValue = m_externalCommandTimeout.has_value() ? m_externalCommandTimeout.value() * 1000
                                                            : m_defaultExternalCommandTimeout;
    m_timer->start(timeoutValue);
}

void TmcVerifier::makeFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);
    m_timer->stop();
    m_process->terminate();
    if (exitCode != EXIT_SUCCESS) {
        auto message = QString("Make finished with code: %1\n").arg(exitCode);
        Q_EMIT verifierMessage(message);
        Q_EMIT finished(false);
        return;
    }

    if (!m_converter->prepare()) {
        Q_EMIT finished(false);
        return;
    }
    m_converter->convert();
}

void TmcVerifier::spinStarted()
{
    m_timer->stop();
    m_timer->setSingleShot(true);
    int timeoutValue = m_externalCommandTimeout.has_value() ? m_externalCommandTimeout.value() * 1000
                                                            : m_defaultExternalCommandTimeout;
    m_timer->start(timeoutValue);
}

void TmcVerifier::spinFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);
    m_timer->stop();
    m_process->terminate();
    if (exitCode != EXIT_SUCCESS) {
        auto message = QString("Spin finished with code: %1\n").arg(exitCode);
        Q_EMIT verifierMessage(message);
        Q_EMIT finished(false);
        return;
    }
    startCompilation();
}

void TmcVerifier::ccStarted()
{
    m_timer->stop();
    m_timer->setSingleShot(true);
    int timeoutValue =
            m_ccompilerTimeout.has_value() ? m_ccompilerTimeout.value() * 1000 : m_defaultExternalCommandTimeout;
    m_timer->start(timeoutValue);
}

void TmcVerifier::ccFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);
    m_timer->stop();
    m_process->terminate();
    if (exitCode != EXIT_SUCCESS) {
        auto message = QString("CC finished with code: %1\n").arg(exitCode);
        Q_EMIT verifierMessage(message);
        Q_EMIT finished(false);
        return;
    }
    if (m_cSourceFiles.empty()) {
        executeCCLinker();
    } else {
        executeCC();
    }
}

void TmcVerifier::ccLinkerFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);
    m_timer->stop();
    m_process->terminate();
    if (exitCode != EXIT_SUCCESS) {
        auto message = QString("CC finished with code: %1\n").arg(exitCode);
        Q_EMIT verifierMessage(message);
        Q_EMIT finished(false);
        return;
    }
    executeVerifier();
}

void TmcVerifier::panStarted()
{
    m_timer->stop();
    m_timer->setSingleShot(true);
    if (m_timeLimitSeconds.has_value()) {
        m_timer->start(m_timeLimitSeconds.value() * 1000);
    } else {
        m_timer->start(m_verifierDefaultTimeout);
    }
}

void TmcVerifier::panFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);
    m_timer->stop();
    m_verifierProcess->terminate();
    if (exitCode != EXIT_SUCCESS) {
        auto message = QString("Verifier finished with code: %1\n").arg(exitCode);
        Q_EMIT verifierMessage(message);
        Q_EMIT finished(false);
        return;
    }
    collectErrors();
}

void TmcVerifier::traceGeneratorStarted()
{
    m_timer->stop();
    m_timer->setSingleShot(true);
    int timeoutValue = m_externalCommandTimeout.has_value() ? m_externalCommandTimeout.value() * 1000
                                                            : m_defaultExternalCommandTimeout;
    m_timer->start(timeoutValue);
}

void TmcVerifier::traceGeneratorFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);
    m_timer->stop();
    m_traceGeneratorProcess->terminate();
    if (exitCode != EXIT_SUCCESS) {
        auto message = QString("Spin finished with code : %1\n").arg(exitCode);
        Q_EMIT verifierMessage(message);
        Q_EMIT finished(false);
        return;
    }

    QFileInfo info(m_currentTraceFile);
    QString outputFile = m_outputDirectory + QDir::separator() + info.baseName() + ".sim";

    if (m_conversionFinishedConnection) {
        disconnect(m_conversionFinishedConnection);
    }
    Q_EMIT verifierMessage(QString("Converting %1 to %2\n").arg(info.fileName()).arg(info.baseName() + ".sim"));
    m_converter->convertTrace(m_currentTraceFile, outputFile);

    m_spinTraceFiles.append(m_currentTraceFile);
    m_traceFiles.append(outputFile);

    generateNextTrace();
}

void TmcVerifier::processError(QProcess::ProcessError error)
{
    m_timer->stop();
    if (m_process->state() != QProcess::ProcessState::NotRunning) {
        m_process->terminate();
    }
    if (m_verifierProcess->state() != QProcess::ProcessState::NotRunning) {
        m_verifierProcess->terminate();
    }
    if (m_traceGeneratorProcess->state() != QProcess::ProcessState::NotRunning) {
        m_traceGeneratorProcess->terminate();
    }
    switch (error) {
    case QProcess::ProcessError::FailedToStart:
        Q_EMIT verifierMessage("External process failed to start.");
        Q_EMIT conversionFinished(false);
        break;
    case QProcess::ProcessError::Crashed:
        Q_EMIT verifierMessage("External process crashed.");
        Q_EMIT conversionFinished(false);
        break;
    case QProcess::ProcessError::Timedout:
        Q_EMIT verifierMessage("External process timeout.");
        Q_EMIT conversionFinished(false);
        break;
    case QProcess::ProcessError::WriteError:
        Q_EMIT verifierMessage("External process write error.");
        Q_EMIT conversionFinished(false);
        break;
    case QProcess::ProcessError::ReadError:
        Q_EMIT verifierMessage("External process read error.");
        Q_EMIT conversionFinished(false);
        break;
    case QProcess::ProcessError::UnknownError:
        Q_EMIT verifierMessage("External process unknown error.");
        Q_EMIT conversionFinished(false);
        break;
    }
}

void TmcVerifier::timeout()
{
    Q_EMIT verifierMessage(QString("Timeout.\n"));
    if (m_process->state() != QProcess::ProcessState::NotRunning) {
        m_process->terminate();
    }
    if (m_verifierProcess->state() != QProcess::ProcessState::NotRunning) {
        m_verifierProcess->terminate();
    }
    Q_EMIT finished(false);
}
}
