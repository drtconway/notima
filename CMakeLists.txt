# It's important to specify the minimum CMake version upfront required by
# CMakeLists.txt. This is so that a user can clearly understand the reason the 
# build will fail before the build actually occurs, instead of searching for the
# CMake function that was used that is causing the failure.
cmake_minimum_required(VERSION 3.1)

# Set the global package-wide C++ standard. This will be inherited by all
# targets specified in the project. One can also specify the C++ standard in a
# target-specific manner, using:
#   set_target_properties(foo PROPERTIES CXX_STANDARD 11)
#   target_compile_features(foo PUBLIC cxx_std_14)
# together.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the project name and version number. This allows for a user of your
# library or tool to specify a particular version when they include it, as in 
#   find_package(Notima 1.0 REQUIRED)
project(Notima VERSION 1.0)
set(Notima_VERSION 1.0)

# enable unit testing via "make test" once the code has been compiled.
include(CTest)

# Function to prepend the subdirectory to source files in subdirectories
FUNCTION(PREPEND var )
   SET(listVar "")
   FOREACH(f ${${var}})
       LIST(APPEND listVar "${CMAKE_CURRENT_SOURCE_DIR}/${f}")
   ENDFOREACH(f)
   SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)

# After a normal build, we can specify the location of various outputs of the
# build. We put executables and static libraries outside the build directory in
# bin/ and lib/, respectively.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

find_package(ZLIB REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(docopt COMPONENTS CXX REQUIRED)

add_subdirectory(src)

add_subdirectory(include)

include_directories(${DOCOPT_INCLUDE_DIRS})
include_directories(include)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_executable(notima ${Notima_SRC} ${Notima_INC})

target_link_libraries(
    notima
    ${HTSLIB_LIBRARIES}
    ${ZLIB_LIBRARIES}
    nlohmann_json::nlohmann_json)

