#pragma once

#include <QStringList>
#include <projectexplorer/projectnodes.h>

namespace spctr {

Utils::FilePaths toProjectsFiles(const QStringList &list)
{
    Utils::FilePaths paths;
    for (const QString &path : list) {
        paths.append(Utils::FilePath::fromString(path));
    }
    return paths;
}

}
