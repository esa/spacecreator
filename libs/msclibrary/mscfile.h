#ifndef MSCFILE_H
#define MSCFILE_H

#include <QString>

namespace antlr4 {
class ANTLRInputStream;
}

namespace msc {
class MscModel;

class MscFile
{
public:
    MscFile();

    MscModel *parseFile(const QString &filename);
    MscModel *parseText(const QString &text);

private:
    MscModel *parse(antlr4::ANTLRInputStream &input);
};

} // namespace msc

#endif // MSCFILE_H
