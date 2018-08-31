TEMPLATE = app
TARGET = msceditor

include(../esa.pri)

QT += core gui widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

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
