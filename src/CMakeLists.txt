#########
# Setup #
#########

# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8)

# Message
message(STATUS "Clock Tree Routing")
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Project's name
project(CTRouter)

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 14)

set(PATH_CTR ${CMAKE_CURRENT_SOURCE_DIR})
set(PATH_ALGLIB ${CMAKE_CURRENT_SOURCE_DIR}/../alglib)


###################
# Warnings/Errors #
###################

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror=return-type")

######################
# Check Dependencies #
######################
#find_package(Boost COMPONENTS filesystem program_options REQUIRED)

###############
# Source Code #
###############
file(GLOB_RECURSE SRC_FILES_ALGLIB
    ${PATH_ALGLIB}/src/*.cpp)

file(GLOB_RECURSE SRC_FILES_CTR
    ${PATH_CTR}/*.cpp)

set(SRC_FILES  ${SRC_FILES_CTR} ${SRC_FILES_ALGLIB})

#################
# Library Paths #
#################
# Need to come before target is created.


###########
# Targets #
###########

add_executable(CTRouter ${SRC_FILES})

#######################
# Include Directories #
#######################

include_directories(${PATH_CTR})
include_directories(${PATH_ALGLIB}/src)

#################
# Linker Flags  #
#################

# TODO: set static under release mode only
set_target_properties(CTRouter PROPERTIES LINK_FLAGS "-static -Wl,--whole-archive -rdynamic -lpthread -Wl,--no-whole-archive")
