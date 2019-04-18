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

#include "commandlineparser.h"
#include "mainwindow.h"

#include <QApplication>
#include <QMetaEnum>
#include <QPlainTextEdit>
#include <QPointer>

// Get the default Qt message handler.
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);

struct LogHandler {
    static LogHandler *instance()
    {
        if (!m_instance)
            m_instance = new LogHandler();
        return m_instance;
    }

    void setupOutputView(QPlainTextEdit *view) { instance()->m_view = view; }

    void handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        if (m_view) {
            QString htmlPre, htmlPost, marker;
            switch (type) {
            case QtDebugMsg:
                marker = "D";
                break;
            case QtInfoMsg:
                marker = "I";
                break;
            case QtWarningMsg:
                marker = "W";
                htmlPre = "<i style='color:red;'>";
                htmlPost = "</i>";
                break;
            case QtCriticalMsg:
            case QtFatalMsg:
                marker = "E";
                htmlPre = "<i><b style='color:red;'>";
                htmlPost = "</b></i>";
                break;
            }

#ifdef QT_DEBUG
            m_view->appendHtml(QString("%1: %2%3%4").arg(marker, htmlPre, msg, htmlPost));
#else
            if (type != QtDebugMsg && type != QtInfoMsg)
                m_view->appendHtml(QString("%1: %2%3%4").arg(marker, htmlPre, msg, htmlPost));
#endif
        }
        (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
    }

private:
    QPointer<QPlainTextEdit> m_view = nullptr;
    LogHandler() {}

    static LogHandler *m_instance;
};

LogHandler *LogHandler::m_instance = nullptr;

void globalLogMessageWrapper(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    LogHandler::instance()->handleMessage(type, context, msg);
}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(asn1_resources);
    Q_INIT_RESOURCE(resources);

    QApplication a(argc, argv);
    a.setApplicationName(QObject::tr("MSC Editor"));
    a.setApplicationVersion("0.0.1");

    MainWindow w;
    LogHandler::instance()->setupOutputView(w.textOutputPane());

    CommandLineParser cmdParser;
    cmdParser.process(a.arguments());
    if (!cmdParser.isSet(CommandLineParser::Positional::StartRemoteControl))
        qInstallMessageHandler(globalLogMessageWrapper);

    QVector<CommandLineParser::Positional> args;
    const QMetaEnum &e = QMetaEnum::fromType<CommandLineParser::Positional>();
    for (int i = 0; i < e.keyCount(); ++i) {
        const CommandLineParser::Positional posArgType(static_cast<CommandLineParser::Positional>(e.value(i)));
        if (CommandLineParser::Positional::Unknown != posArgType)
            if (cmdParser.isSet(posArgType)) {
                if (posArgType == CommandLineParser::Positional::OpenFileAsn
                    || posArgType == CommandLineParser::Positional::OpenFileMsc)
                    args.prepend(posArgType);
                else
                    args.append(posArgType);
            }
    }
    QVector<CommandLineParser::Positional>::const_reverse_iterator it = args.crbegin();
    while (it != args.crend()) {
        const CommandLineParser::Positional arg = *it;
        w.processCommandLineArg(arg, cmdParser.value(arg));
        ++it;
    }

    w.show();

    return a.exec();
}
