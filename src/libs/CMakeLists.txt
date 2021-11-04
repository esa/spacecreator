# Shared libraries
add_subdirectory(shared)
add_subdirectory(templating)

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

# conversion
add_subdirectory(conversion)
add_subdirectory(seds)
add_subdirectory(promela)

# Total SpaceCreator system
add_subdirectory(spacecreatorsystem)
