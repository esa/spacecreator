#Suppress warning from ANTLR
win32 {

    QMAKE_CXXFLAGS += /wd4251 /wd4996
    QMAKE_LFLAGS += /ignore:4221 /ignore:4217
    QMAKE_LIBFLAGS += /ignore:4221
}
