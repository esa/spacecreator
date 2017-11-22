/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.pl/Space
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a programme and funded by
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
#include "usagesfinder.h"

#include <algorithm>

#include <QDir>
#include <QFutureInterface>

#include <coreplugin/find/searchresultwindow.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/progressmanager/futureprogress.h>
#include <coreplugin/progressmanager/progressmanager.h>
#include <utils/runextensions.h>

#include <data/typereference.h>
#include <data/root.h>
#include <data/project.h>
#include <data/file.h>

#include "asn1acnconstants.h"
#include "parseddatastorage.h"

using namespace Asn1Acn::Internal;
using namespace Core;
using Data::TypeReference;

UsagesFinder::UsagesFinder(QObject *parent)
    : QObject(parent)
{
}

void UsagesFinder::findUsages(const QString &module, const QString &type)
{
    SearchResult *search
            = SearchResultWindow::instance()->startNewSearch(tr("ASN.1 Usages:"),
                                                             QString(),
                                                             module + "::" + type,
                                                             SearchResultWindow::SearchOnly,
                                                             SearchResultWindow::PreserveCaseDisabled,
                                                             QLatin1String("AsnEditor"));
     search->setSearchAgainSupported(true);
     connect(search, &SearchResult::searchAgainRequested, this, &UsagesFinder::searchAgain);

     UsagesFinderParameters params { module, type };
     search->setUserData(qVariantFromValue(params));

     findAll(search, params);
}

void UsagesFinder::searchAgain()
{
    SearchResult *search = qobject_cast<SearchResult *>(sender());
    search->restart();
    const auto params = search->userData().value<UsagesFinderParameters>();
    findAll(search, params);
}

static int countFiles()
{
    const auto &projects = ParsedDataStorage::instance()->root()->projects();
    auto fileCount = std::accumulate(std::begin(projects),
                                     std::end(projects),
                                     0,
                                     [](int s, const std::unique_ptr<Data::Project> &p) {
        return s + p->files().size();
    });
    return static_cast<int>(fileCount);
}

static void performSearch(QFutureInterface<TypeReference> &future,
                          const UsagesFinderParameters &params)
{
    future.setProgressRange(0, countFiles());

    for (const auto &project : ParsedDataStorage::instance()->root()->projects())
        for (const auto &file : project->files())
        {
            for (const auto &ref : file->references())
                if (ref->module() == params.module
                        && ref->name() == params.type)
                    future.reportResult(*ref);
            future.setProgressValue(future.progressValue() + 1);
        }

    // TODO search only "current" project
}

static void displayResults(SearchResult *search,
                           QFutureWatcher<TypeReference> *watcher,
                           int first, int last)
{
    for (int index = first; index != last; ++index) {
        auto result = watcher->future().resultAt(index);
        search->addResult(result.location().path(),
                          result.location().line(),
                          "TODO: result.lineText",
                          result.location().column(),
                          result.name().length());
    }
}

void UsagesFinder::createWatcher(const QFuture<TypeReference> &future,
                                 SearchResult *search)
{
    auto watcher = new QFutureWatcher<TypeReference>();

    connect(watcher, &QFutureWatcherBase::finished, watcher, [search, watcher]() {
        search->finishSearch(watcher->isCanceled());
        watcher->deleteLater();
    });

    connect(watcher, &QFutureWatcherBase::resultsReadyAt, search,
            [search, watcher](int first, int last) {
                displayResults(search, watcher, first, last);
            });
    connect(watcher, &QFutureWatcherBase::finished, search, [search, watcher]() {
        search->finishSearch(watcher->isCanceled());
    });
    connect(search, &SearchResult::cancelled, watcher, [watcher]() { watcher->cancel(); });
    connect(search, &SearchResult::paused, watcher, [watcher](bool paused) {
        if (!paused || watcher->isRunning()) // guard against pausing when the search is finished
            watcher->setPaused(paused);
    });
    watcher->setPendingResultsLimit(1);
    watcher->setFuture(future);
}

void UsagesFinder::findAll(SearchResult *search, const UsagesFinderParameters &params)
{
    connect(search, &SearchResult::activated,
            this, &UsagesFinder::openEditor);

    SearchResultWindow::instance()->popup(IOutputPane::ModeSwitch | IOutputPane::WithFocus);
    auto result = Utils::runAsync(performSearch, params);
    createWatcher(result, search);

    auto progress = ProgressManager::addTask(result, tr("Searching for Usages"),
                                             Constants::TASK_SEARCH);

    connect(progress, &FutureProgress::clicked, search, &SearchResult::popup);
}

void UsagesFinder::openEditor(const SearchResultItem &item)
{
    if (item.path.size() > 0) {
        EditorManager::openEditorAt(QDir::fromNativeSeparators(item.path.first()),
                                    item.mainRange.begin.line,
                                    item.mainRange.begin.column);
    } else {
        EditorManager::openEditor(QDir::fromNativeSeparators(item.text));
    }
}

/*
 *
 * TODO
 * void CppEditorPlugin::onTaskStarted(Id type)
{
    if (type == CppTools::Constants::TASK_INDEX) {
        m_renameSymbolUnderCursorAction->setEnabled(false);
        m_findUsagesAction->setEnabled(false);
        m_reparseExternallyChangedFiles->setEnabled(false);
        m_openTypeHierarchyAction->setEnabled(false);
        m_openIncludeHierarchyAction->setEnabled(false);
    }
}

void CppEditorPlugin::onAllTasksFinished(Id type)
{
    if (type == CppTools::Constants::TASK_INDEX) {
        m_renameSymbolUnderCursorAction->setEnabled(true);
        m_findUsagesAction->setEnabled(true);
        m_reparseExternallyChangedFiles->setEnabled(true);
        m_openTypeHierarchyAction->setEnabled(true);
        m_openIncludeHierarchyAction->setEnabled(true);
    }
}

 * */
