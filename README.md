Backend of Dark Pool
============

## Goals
* Facilitate rapid development and deployment of high-performance, scalable exchanges.
* Provide FAST1.2, FIX5.0SP2/FIXT1.1, and JSON-RPC reference gateways.

## Build Dependencies
* Clang 3.2+ or gcc 7.1 with a C++11 compliant libc++
* [CMake](http://www.cmake.org) 3.8.0

## Supported Platforms

Development is done on MacOS X Sierra 10.12 with FreeBSD 10 as the primary deployment target.

## Build instructions

    git clone git@github.com:dendisuhubdy/dark_pool.git
    mkdir build
    cd build
    cmake ..
    make -j n
    
where n is the number of preferred threads

## 3rd party components

* [libuv](https://github.com/joyent/libuv)
* [disruptor--](https://github.com/fsaintjacques/disruptor--)
* [Liquibook](https://github.com/objectcomputing/liquibook): an incredibly fast order matching engine.
* [QuickFIX](http://www.quickfixengine.org/): a FIX message parser. 
* [QuickFAST](https://code.google.com/p/quickfast/): a FAST message parser.

