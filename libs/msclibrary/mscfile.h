#ifndef MSCFILE_H
#define MSCFILE_H

#include <QString>

class MscFile
{
public:
    MscFile();

    void parseFile(const QString &filename);
};

#endif // MSCFILE_H
