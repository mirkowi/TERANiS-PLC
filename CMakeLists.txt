cmake_minimum_required(VERSION 3.6)
project(TERANiS_PLC)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(avicstd/h)
include_directories(modbus)
include_directories(opcua)

set(SOURCES main.cpp)

add_definitions(-DENVIRONMENT=${env})

if ("${env}" STREQUAL "")
    set(env mraa)
endif ()

message(STATUS Environment=${env})

if ("${env}" MATCHES "mraa")
    set(SOURCES ${SOURCES} PlcIoMraa.cpp)
    add_definitions(-DENV_MRAA)
    add_definitions(-DUA_ARCHITECTURE_POSIX)
    link_libraries(mraa)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif ()

if ("${env}" MATCHES "revpi")
    set(SOURCES ${SOURCES} PlcIoRevPi.cpp)
    add_definitions(-DENV_REVPI)
    add_definitions(-DUA_ARCHITECTURE_POSIX)
endif ()

if ("${env}" MATCHES "windows")
    add_definitions(-DENV_WINDOWS)
    add_definitions(-DUA_ARCHITECTURE_WIN32)
    link_libraries(ws2_32.lib)
    # todo: where to map I/O to?
endif ()

set(SOURCES ${SOURCES} modbus/MModbusTCPServer.cpp)
set(SOURCES ${SOURCES} modbus/MRegExp.cpp)
set(SOURCES ${SOURCES} modbus/MSocket.cpp)
set(SOURCES ${SOURCES} modbus/TCPSocket.cpp)
set(SOURCES ${SOURCES} modbus/TCPSocket.h)
set(SOURCES ${SOURCES} modbus/TCPServer.cpp)
set(SOURCES ${SOURCES} modbus/TCPServer.h)
set(SOURCES ${SOURCES} modbus/TCPClient.cpp)
set(SOURCES ${SOURCES} opcua/OpcuaServer.cpp)
set(SOURCES ${SOURCES} opcua/OpcuaVariable.cpp)
set(SOURCES ${SOURCES} opcua/opcua_configuration.cpp)
set(SOURCES ${SOURCES} opcua/open62541.c)
set(SOURCES ${SOURCES} Plc.cpp)
set(SOURCES ${SOURCES} PlcIo.cpp)
set(SOURCES ${SOURCES} PlcTask.cpp)
set(SOURCES ${SOURCES} PlcMemory.cpp)
set(SOURCES ${SOURCES} TeranisTask.cpp)

add_executable(TERANiS_PLC ${SOURCES})

#force 64 bit mode:
#set_target_properties(TERANiS_PLC PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
