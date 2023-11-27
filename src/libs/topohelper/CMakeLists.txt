set(TOPOHELP_LIB_NAME topohelper)

add_library(${TOPOHELP_LIB_NAME} STATIC)

target_sources(${TOPOHELP_LIB_NAME} PUBLIC
    utils.h
    connection.h
)
target_sources(${TOPOHELP_LIB_NAME} PRIVATE
    utils.cpp
    connection.cpp
)

target_include_directories(${TOPOHELP_LIB_NAME} PUBLIC .)

target_link_libraries(${TOPOHELP_LIB_NAME}
    PUBLIC
    ${QT_CORE}
    ${QT_GUI}
)

if(OPTIONS_NO_DEPRECATED)
    target_compile_options(${TOPOHELP_LIB_NAME} PRIVATE -Wno-deprecated -Wno-deprecated-declarations)
endif()
