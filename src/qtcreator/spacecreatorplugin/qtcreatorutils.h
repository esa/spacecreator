#pragma once

#include <QStringList>
#include <projectexplorer/projectnodes.h>

namespace spctr {

#if QTC_VERSION < 500
QStringList toProjectsFiles(const QStringList &list)
{
    return list;
}
#else
Utils::FilePaths toProjectsFiles(const QStringList &list)
{
    Utils::FilePaths paths;
    for (const QString &path : list) {
        paths.append(Utils::FilePath::fromString(path));
    }
    return paths;
}
#endif

}
