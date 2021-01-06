
# Make an explicit list of all source files in Notima_INC. This is important
# because CMake is not a build system: it is a build system generator. Suppose
# you add a file foo.cpp to src/ after running cmake .. . If you set
# Notima_INC with `file(GLOB ... )`, this is not passed to the makefile; it
# doesn't know that foo.cpp exists and will not re-run cmake. Your
# collaborator's builds will fail and it will be unclear why. Whether you use
# file(GLOB ...) or not, you will need to re-run cmake, but with an explicit
# file list, you know beforehand why your code isn't compiling. 
set(Notima_INC
    notima/bit_array.hpp
    notima/codec8.hpp
    notima/integer_array.hpp
    notima/poppy.hpp
    notima/radix_array.hpp
    notima/sparse_array.hpp
    notima/spookyv2.hpp
    notima/stats.hpp
    notima/subword_array.hpp
    notima/wordy.hpp
    notima/internal/stats.hpp
    fastq.hpp
    files.hpp
    gzipf.hpp
    topgm.hpp
    )

# Form the full path to the source files...
PREPEND(Notima_INC)
# ... and pass the variable to the parent scope.
set(Notima_INC ${Notima_INC}  PARENT_SCOPE)
