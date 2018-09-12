TEMPLATE = app
TARGET = msceditor

include(../esa.pri)

QT += core gui widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainmodel.cpp

HEADERS += \
    mainwindow.h \
    mainmodel.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libs/mscwidgets/release/ -lmscwidgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libs/mscwidgets/debug/ -lmscwidgets
else:unix: LIBS += -L$$OUT_PWD/../libs/mscwidgets/ -lmscwidgets

INCLUDEPATH += $$PWD/../libs/mscwidgets
DEPENDPATH += $$PWD/../libs/mscwidgets

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/mscwidgets/release/libmscwidgets.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/mscwidgets/debug/libmscwidgets.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/mscwidgets/release/mscwidgets.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/mscwidgets/debug/mscwidgets.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libs/mscwidgets/libmscwidgets.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libs/msclibrary/release/ -lmsclibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libs/msclibrary/debug/ -lmsclibrary
else:unix: LIBS += -L$$OUT_PWD/../libs/msclibrary/ -lmsclibrary

INCLUDEPATH += $$PWD/../libs/msclibrary
DEPENDPATH += $$PWD/../libs/msclibrary

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/msclibrary/release/libmsclibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/msclibrary/debug/libmsclibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/msclibrary/release/msclibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/msclibrary/debug/msclibrary.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libs/msclibrary/libmsclibrary.a

# Add ANTLR runtime
win32: LIBS += -L$$clean_path("$$PWD/../3rdparty/antlr/cpp_runtime/lib/vs-2015/x64/Release DLL") -lantlr4-runtime
unix: LIBS += -L$$clean_path($$PWD/../3rdparty/antlr/cpp_runtime/dist) -lantlr4-runtime_linux_x64
osx: LIBS += -L$$clean_path($$PWD/../3rdparty/antlr/cpp_runtime/lib) -lantlr4-runtime_macos_x64
