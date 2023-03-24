set(LIB_NAME spacecreatorsystem)

add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    dvrefactor.cpp
    dvrefactor.h
    dvsystemqueries.cpp
    dvsystemqueries.h
    refactorbase.cpp
    refactorbase.h
    ivrefactorhandler.cpp
    ivrefactorhandler.h
    ivsystemchecks.cpp
    ivsystemchecks.h
    ivsystemqueries.cpp
    ivsystemqueries.h
    mscrefactor.cpp
    mscrefactor.h
    mscrefactorhandler.cpp
    mscrefactorhandler.h
    spacecreatorproject.cpp
    spacecreatorproject.h
)

target_include_directories(${LIB_NAME}
    PUBLIC .
)

target_link_libraries(${LIB_NAME}
    asn1library
    libmsceditor
    libdveditor
    libiveditor
    ${QT_CORE}
    ${QT_GUI}
    ${QT_WIDGETS}
)
