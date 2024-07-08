/*
   Copyright (C) 2018-2024 European Space Agency - <maxime.perrotin@esa.int>

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

#include "asn1scc.h"

#include <QFileInfo>
#include <QProcess>
#include <QSettings>
#include <shared/externalprocess.h>
#include <shared/standardpaths.h>

namespace Asn1Acn {

static const QStringList defaultParameter { "--field-prefix", "AUTO", "--xml-ast" };

int Asn1Scc::getCompilerTimeout() const
{
    QSettings settings;
    QVariant asn1CompilerTimeoutSetting = settings.value("SpaceCreator/asn1compilertimeout").toInt();
    int asn1CompilerTimeout = 60000;
    if (asn1CompilerTimeoutSetting.isValid()) {
        bool convOk = false;
        int value = asn1CompilerTimeoutSetting.toInt(&convOk);
        if (convOk && value != 0) {
            asn1CompilerTimeout = value * 1000;
        }
    }

    return asn1CompilerTimeout;
}

QStringList Asn1Scc::defaultParameter() const
{
    return Asn1Acn::defaultParameter;
}

QString Asn1Scc::getAsn1SccCommand() const
{
    QSettings settings;
    QString asn1Compiler = settings.value("SpaceCreator/asn1compiler").toString();

    if (asn1Compiler.isEmpty() || !QFile::exists(asn1Compiler)) {
        asn1Compiler = checkforCompiler();
    }

    return asn1Compiler;
}

/*!
   Does search in the system path for asn1scc and returns the full file path.
   An empty string is returned if asn1scc was not found.
 */
QString Asn1Scc::checkforCompiler() const
{
#ifdef Q_OS_WIN
    QProcess process;
    process.start(QString("where asn1"));
    process.waitForFinished();
    QString asn1Exec = process.readAll();
    asn1Exec.remove("\r\n");
    asn1Exec.remove("asn1scc.exe");
    if (asn1Exec.isEmpty()) {
        qWarning() << tr("Unable to find the asn1scc compiler");
        return {};
    }
    return asn1Exec;
#else
    std::unique_ptr<QProcess> process = shared::ExternalProcess::create();
    process->start(QString("which"), { QString("asn1scc") });
    process->waitForFinished();
    QString asn1Exec = process->readAll();
    asn1Exec.remove('\n');
    if (asn1Exec.isEmpty()) {
        asn1Exec = shared::StandardPaths::writableLocation(QStandardPaths::HomeLocation)
                + QLatin1String("/tool-inst/share/asn1scc/asn1scc");
        if (!QFileInfo::exists(asn1Exec)) {
            qWarning() << tr("Unable to find the asn1scc compiler");
            return {};
        }
    }
    return asn1Exec;
#endif
}

QPair<QString, QStringList> Asn1Scc::asn1CompilerCommand() const
{
    QSettings settings;
    QString asn1Compiler = settings.value("SpaceCreator/asn1compiler").toString();
    QString parameter = settings.value("SpaceCreator/asn1compilerparameter").toString();

    // Compiler. Use compiler set by user or see if it is installed on the system
    if (asn1Compiler.isEmpty() || !QFile::exists(asn1Compiler)) {
        asn1Compiler = checkforCompiler();
    }
    if (asn1Compiler.isEmpty()) {
        qWarning() << "No asn1scc compiler found";
        return { {}, {} };
    }

    // Parameters to compiler. Use parameters set by user or use the default ones
    QStringList paramsList;
    if (parameter.isEmpty()) {
        paramsList = defaultParameter();
    } else {
        QStringList paramsList = parameter.split(' ', Qt::SkipEmptyParts);
    }

    // replace %1 in users parameters with path to the asn1 compiler.
    QFileInfo asnFile(asn1Compiler);
    if (paramsList.contains("%1")) {
        int pos = paramsList.indexOf("%1");
        paramsList[pos] = asnFile.path();
    }

    return { asn1Compiler, paramsList };
}

}
