install(DIRECTORY files DESTINATION ${IDE_DATA_PATH}/templates/wizards)
install(DIRECTORY projects DESTINATION ${IDE_DATA_PATH}/templates/wizards)

# Add all files from the wizards to the QtC tree
file(GLOB_RECURSE file_wizards files/*)
file(GLOB_RECURSE project_wizards projects/*)
add_custom_target(FileWizards SOURCES ${file_wizards})
add_custom_target(ProjectWizards SOURCES ${project_wizards})
