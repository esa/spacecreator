# Shared libraries
add_subdirectory(shared)
add_subdirectory(templating)

# Common tools
add_subdirectory(common)

# Asn.1 editor
add_subdirectory(asn1library)
add_subdirectory(asn1editor)

# iv editor
add_subdirectory(ivcore)
add_subdirectory(libiveditor)

# msc editor
add_subdirectory(libmsceditor)
add_subdirectory(msccore)

# deployment view editor
add_subdirectory(dvcore)
add_subdirectory(libdveditor)

# SDL metamodel and exporter
add_subdirectory(sdl)

# CSV metamodel and importer
add_subdirectory(csv)

# TestDriver generator
add_subdirectory(testgenerator)

# conversion
add_subdirectory(conversion)
add_subdirectory(seds)
add_subdirectory(promela)
add_subdirectory(tmc)

# Total SpaceCreator system
add_subdirectory(spacecreatorsystem)
