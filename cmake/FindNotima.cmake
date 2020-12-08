# - Try to find the Notima library
# Once done this will define
#
#  Notima_FOUND - system has Notima
#  Notima_INCLUDE_DIR - Notima include directory
#  Notima_LIB - Notima library directory
#  Notima_LIBRARIES - Notima libraries to link

if(Notima_FOUND)
    return()
endif()

# We prioritize libraries installed in /usr/local with the prefix .../Notima-*, 
# so we make a list of them here
file(GLOB lib_glob "/usr/local/lib/Notima-*")
file(GLOB inc_glob "/usr/local/include/Notima-*")

# Find the library with the name "Notima" on the system. Store the final path
# in the variable Notima_LIB
find_library(Notima_LIB 
    # The library is named "Notima", but can have various library forms, like
    # libNotima.a, libNotima.so, libNotima.so.1.x, etc. This should
    # search for any of these.
    NAMES Notima
    # Provide a list of places to look based on prior knowledge about the system.
    # We want the user to override /usr/local with environment variables, so
    # this is included here.
    HINTS
        ${Notima_DIR}
        ${CMAKEDEMO_DIR}
        $ENV{Notima_DIR}
        $ENV{CMAKEDEMO_DIR}
        ENV CMAKEDEMO_DIR
    # Provide a list of places to look as defaults. /usr/local shows up because
    # that's the default install location for most libs. The globbed paths also
    # are placed here as well.
    PATHS
        /usr
        /usr/local
        /usr/local/lib
        ${lib_glob}
    # Constrain the end of the full path to the detected library, not including
    # the name of library itself.
    PATH_SUFFIXES 
        lib
)

# Find the path to the file "source_file.hpp" on the system. Store the final
# path in the variables Notima_INCLUDE_DIR. The HINTS, PATHS, and
# PATH_SUFFIXES, arguments have the same meaning as in find_library().
find_path(Notima_INCLUDE_DIR source_file.hpp
    HINTS
        ${Notima_DIR}
        ${CMAKEDEMO_DIR}
        $ENV{Notima_DIR}
        $ENV{CMAKEDEMO_DIR}
        ENV CMAKEDEMO_DIR
    PATHS
        /usr
        /usr/local
        /usr/local/include
        ${inc_glob}
    PATH_SUFFIXES 
        include
)


# Check that both the paths to the include and library directory were found.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Notima
    "\nNotima not found --- You can download it using:\n\tgit clone 
    https://github.com/mmorse1217/cmake-project-template\n and setting the CMAKEDEMO_DIR environment variable accordingly"
    Notima_LIB Notima_INCLUDE_DIR)

# These variables don't show up in the GUI version of CMake. Not required but
# people seem to do this...
mark_as_advanced(Notima_INCLUDE_DIR Notima_LIB)

# Finish defining the variables specified above. Variables names here follow
# CMake convention.
set(Notima_INCLUDE_DIRS ${Notima_INCLUDE_DIR})
set(Notima_LIBRARIES ${Notima_LIB})

# If the above CMake code was successful and we found the library, and there is
# no target defined, lets make one.
if(Notima_FOUND AND NOT TARGET Notima::Notima)
    add_library(Notima::Notima UNKNOWN IMPORTED)
    # Set location of interface include directory, i.e., the directory
    # containing the header files for the installed library
    set_target_properties(Notima::Notima PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Notima_INCLUDE_DIRS}"
        )

    # Set location of the installed library
    set_target_properties(Notima::Notima PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
        IMPORTED_LOCATION "${Notima_LIBRARIES}"
        )
endif()
