#include "templatinglibrary.h"

#include <qglobal.h>

static void init_shared_library()
{
    Q_INIT_RESOURCE(style);
    Q_INIT_RESOURCE(template_syntax_help);
}

void templating::initTemplatingLibrary()
{
    init_shared_library();
}
