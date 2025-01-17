# Write file "scversion.h" in the build directory

set(VERSION_FILE_HEADER "// autogenerated by WriteVersionFiles.cmake")
set(VERSION_FILE_CONTENTS "${VERSION_FILE_HEADER}\n\n")

set(VERSION_FILE_CONTENTS "${VERSION_FILE_CONTENTS}#include <QString>\n\n")

# Add plain version string
set(VERSION_FILE_CONTENTS
    "${VERSION_FILE_CONTENTS}static const QString spaceCreatorVersion(\"${CMAKE_PROJECT_VERSION}\");\n"
)

# Add git information
set(VERSION_FILE_CONTENTS "${VERSION_FILE_CONTENTS}static const QString spaceCreatorGitHash(\"${SC_GIT_HASH}\");\n")

# Add version and git information in one string
set(VERSION_FILE_CONTENTS
    "${VERSION_FILE_CONTENTS}static const QString spaceCreatorVersionAndGit(\"${CMAKE_PROJECT_VERSION} - githash: #${SC_GIT_HASH}\");\n"
)

# Write file
set(VERSION_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/scversion.h")
message(STATUS "Writing ${VERSION_FILE_NAME}")
file(WRITE ${VERSION_FILE_NAME} "${VERSION_FILE_CONTENTS}")

# Add include dir, so the file can be included
include_directories(${CMAKE_CURRENT_BINARY_DIR})
