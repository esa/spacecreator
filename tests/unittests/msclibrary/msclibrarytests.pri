win32:CONFIG(release, debug|release): LIBS += -L$$clean_path($$OUT_PWD/../../../../libs/msclibrary/release/) -lmsclibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$clean_path($$OUT_PWD/../../../../libs/msclibrary/debug/) -lmsclibrary
else:unix: LIBS += -L$$clean_path($$OUT_PWD/../../../../libs/msclibrary/) -lmsclibrary

INCLUDEPATH += $$PWD/../../../libs/msclibrary
DEPENDPATH += $$PWD/../../../libs/msclibrary

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../libs/msclibrary/release/libmsclibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../libs/msclibrary/debug/libmsclibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../libs/msclibrary/release/msclibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../../libs/msclibrary/debug/msclibrary.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../../../libs/msclibrary/libmsclibrary.a

# Add ANTLR runtime
win32: LIBS += -L$$clean_path("$$PWD/../../../3rdparty/antlr/cpp_runtime/lib/vs-2015/x64/Release DLL") -lantlr4-runtime
unix: LIBS += -L$$clean_path($$PWD/../../../3rdparty/antlr/cpp_runtime/dist) -lantlr4-runtime_linux_x64
osx: LIBS += -L$$clean_path($$PWD/../../../3rdparty/antlr/cpp_runtime/lib) -lantlr4-runtime_macos_x64
 
