cmake_minimum_required(VERSION 3.9.1)
project (bitwyre_exchange)
set(CMAKE_BUILD_TYPE Release)

set (CMAKE_CXX_STANDARD 11)

FIND_PACKAGE( Boost 1.65 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

# Bring the headers
include_directories(include)

# Adding all sources
file(GLOB SOURCES "src/*.cpp")

#Generate the shared library from the sources
#add_library(exchange SHARED ${SOURCES})

add_executable (exchange ${SOURCES})
TARGET_LINK_LIBRARIES(exchange LINK_PUBLIC ${Boost_LIBRARIES})

install(TARGETS exchange DESTINATION /usr/local/bin)
