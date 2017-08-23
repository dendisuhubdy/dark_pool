cmake_minimum_required(VERSION 3.8.0)
project (bitwyre_exchange)
set(CMAKE_BUILD_TYPE Release)

set (CMAKE_CXX_STANDARD 11)

set(Boost_INCLUDE_DIR /usr/local/Cellar/boost/1.65.0/include)
set(Boost_LIBRARY_DIR /usr/local/Cellar/boost/1.65.0/lib)
find_package( Boost 1.65 COMPONENTS system REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )
link_directories(${Boost_LIBRARY_DIR})

# Bring the headers
include_directories(include)

# Adding all sources
file(GLOB SOURCES "src/*.cpp")

#Generate the shared library from the sources
#add_library(exchange SHARED ${SOURCES})

add_executable (exchange ${SOURCES})
TARGET_LINK_LIBRARIES(exchange LINK_PUBLIC ${Boost_LIBRARIES})

install(TARGETS exchange DESTINATION /usr/local/bin)
