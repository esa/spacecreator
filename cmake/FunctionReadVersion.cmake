# function for parsing version variables that are set in file VERSION
# the version identifiers there are defined as follows:
# SC_MAJOR_VERSION = 3
# SC_MINOR_VERSION = 4
# SC_MINOR is set by cmake variable BUILD_PATCH_NUMBER, or is set to 0

function(read_version vFile outMajor outMinor outPatch)
    file(STRINGS ${vFile} _vFileContents REGEX "^export SC_.+")
    set(_regex "export SC_MAJOR_VERSION=+([0-9]+);export SC_MINOR_VERSION=+([0-9]+)")
     # note quotes around _regex, they are needed because the variable contains semicolons
    string(REGEX MATCH "${_regex}" _tmp "${_vFileContents}")
    if (NOT _tmp)
        message(FATAL_ERROR "Could not detect project version number from ${vFile}")
    endif()

    set(${outMajor} ${CMAKE_MATCH_1} PARENT_SCOPE)
    set(${outMinor} ${CMAKE_MATCH_2} PARENT_SCOPE)

    if(DEFINED BUILD_PATCH_NUMBER)
        set(${outPatch} ${BUILD_PATCH_NUMBER} PARENT_SCOPE)
    else()
        set(${outPatch} 0 PARENT_SCOPE)
    endif()
endfunction()
