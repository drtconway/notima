
# Make an explicit list of all source files in `Notima_SRC`. This is important
# because CMake is not a build system: it is a build system generator. Suppose
# you add a file foo.cpp to src/ after running cmake .. . If you set
# `Notima_SRC` with `file(GLOB ... )`, this is not passed to the makefile;
# the makefile doesn't know that foo.cpp exists and will not re-run cmake. Your
# collaborator's builds will fail and it will be unclear why. Whether you use
# file(GLOB ...) or not, you will need to re-run cmake, but with an explicit
# file list, you know beforehand why your code isn't compiling. 
set(Notima_SRC
    main.cpp
    )

# Form the full path to the source files...
PREPEND(Notima_SRC)
# ... and pass the variable to the parent scope.
set(Notima_SRC ${Notima_SRC}  PARENT_SCOPE)
