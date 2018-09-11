#ifndef MSCFILE_H
#define MSCFILE_H

#include "mscmodel.h"

#include <QString>

namespace antlr4 {
class ANTLRInputStream;
}

namespace msc {

class MscFile
{
public:
    MscFile();

    void parseFile(const QString &filename);
    void parseText(const QString &text);

    const MscModel &model() const;

private:
    void parse(antlr4::ANTLRInputStream &input);

    MscModel m_model;
};

} // namespace msc

#endif // MSCFILE_H
