cmake_minimum_required(VERSION 2.8)

project(uv)

include_directories(
  include
  include/uv-private
  src)

set(
  UV_SOURCES_COMMON
  src/fs-poll.c
  src/inet.c
  src/uv-common.c)

if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
  set(
    UV_SOURCES_PLATFORM_SPECIFIC
    src/unix/freebsd.c
    src/unix/kqueue.c
    src/unix/async.c
    src/unix/core.c
    src/unix/dl.c
    src/unix/error.c
    src/unix/fs.c
    src/unix/getaddrinfo.c
    src/unix/internal.h
    src/unix/loop.c
    src/unix/loop-watcher.c
    src/unix/pipe.c
    src/unix/poll.c
    src/unix/process.c
    src/unix/signal.c
    src/unix/stream.c
    src/unix/tcp.c
    src/unix/thread.c
    src/unix/threadpool.c
    src/unix/timer.c
    src/unix/tty.c
    src/unix/udp.c)
    add_definitions(-g -pedantic -Wall -Wextra -Wno-unused-parameter -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(
    UV_SOURCES_PLATFORM_SPECIFIC
    src/unix/linux/linux-core.c
    src/unix/linux/inotify.c
    src/unix/linux/syscalls.c
    src/unix/async.c
    src/unix/core.c
    src/unix/dl.c
    src/unix/error.c
    src/unix/fs.c
    src/unix/getaddrinfo.c
    src/unix/internal.h
    src/unix/loop.c
    src/unix/loop-watcher.c
    src/unix/pipe.c
    src/unix/poll.c
    src/unix/process.c
    src/unix/signal.c
    src/unix/stream.c
    src/unix/tcp.c
    src/unix/thread.c
    src/unix/threadpool.c
    src/unix/timer.c
    src/unix/tty.c
    src/unix/udp.c)
    add_definitions(-g -pedantic -Wall -Wextra -Wno-unused-parameter -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  set(
    UV_SOURCES_PLATFORM_SPECIFIC
    src/unix/kqueue.c
    src/unix/fsevents.c
    src/unix/darwin.c
    src/unix/async.c
    src/unix/core.c
    src/unix/dl.c
    src/unix/error.c
    src/unix/fs.c
    src/unix/getaddrinfo.c
    src/unix/internal.h
    src/unix/loop.c
    src/unix/loop-watcher.c
    src/unix/pipe.c
    src/unix/poll.c
    src/unix/process.c
    src/unix/signal.c
    src/unix/stream.c
    src/unix/tcp.c
    src/unix/thread.c
    src/unix/threadpool.c
    src/unix/timer.c
    src/unix/tty.c
    src/unix/udp.c)
    add_definitions(-g -pedantic -Wall -Wextra -Wno-unused-parameter -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64)
    set(CMAKE_SHARED_LINKER_FLAGS "-framework CoreServices -framework CoreFoundation -dynamiclib")
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    add_library(
        uv
        SHARED
        ${UV_SOURCES_COMMON} ${UV_SOURCES_PLATFORM_SPECIFIC}
    )
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_library(
        uv
        ${UV_SOURCES_COMMON} ${UV_SOURCES_PLATFORM_SPECIFIC}
    )
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD") 
    target_link_libraries(
        uv
        kvm
        pthread
    )
endif()

