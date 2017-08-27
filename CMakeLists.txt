cmake_minimum_required(VERSION 3.8.0)

project (BITWYRE)

set(CMAKE_BUILD_TYPE Release)
set (CMAKE_CXX_STANDARD 11)

set(Boost_INCLUDE_DIR /usr/local/Cellar/boost/1.65.0/include)
set(Boost_LIBRARY_DIR /usr/local/Cellar/boost/1.65.0/lib)

find_package( Boost 1.65 COMPONENTS system REQUIRED )

include_directories( 
		${Boost_INCLUDE_DIR}
		${BITWYRE_SOURCE_DIR}/include
)

link_directories(
    ${Boost_LIBRARY_DIR}
    ${BITWYRE_SOURCE_DIR}/lib
)

set(
	CMAKE_CXX_LINK_FLAGS 
)

#Generate the shared library from the sources
#add_library(exchange SHARED ${SOURCES})

add_executable(
    ordermatch
    ${BITWYRE_SOURCE_DIR}/src/ordermatch/ordermatch.cpp
    ${BITWYRE_SOURCE_DIR}/src/ordermatch/Market.cpp
    ${BITWYRE_SOURCE_DIR}/src/ordermatch/Application.cpp
)

TARGET_LINK_LIBRARIES(
    ordermatch
    ${Boost_LIBRARY_DIR}
    -lquickfix
)

install(TARGETS ordermatch DESTINATION /usr/local/bin)
