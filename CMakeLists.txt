cmake_minimum_required(VERSION 3.6)
project(TERANiS_PLC)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(avicstd/h)
include_directories(modbus)

set(SOURCES main.cpp)

add_compile_definitions(ENVIRONMENT=${env})

if ("${env}" STREQUAL "")
    set(env windows)
endif ()

message(STATUS Environment=${env})

if ("${env}" MATCHES "mraa")
    set(SOURCES ${SOURCES} PlcIoMraa.cpp)
    add_compile_definitions(ENV_MRAA)
    link_libraries(mraa)
endif ()

if ("${env}" MATCHES "revpi")
    set(SOURCES ${SOURCES} PlcIoRevPi.cpp)
    add_compile_definitions(ENV_REVPI)
endif ()

if ("${env}" MATCHES "windows")
    add_compile_definitions(ENV_WINDOWS)
    # todo: where to map I/O to?
endif ()

link_libraries(ws2_32.lib)

set(SOURCES ${SOURCES} modbus/MModbusTCPServer.cpp)
set(SOURCES ${SOURCES} modbus/MRegExp.cpp)
set(SOURCES ${SOURCES} modbus/MSocket.cpp)
set(SOURCES ${SOURCES} modbus/TCPSocket.cpp)
set(SOURCES ${SOURCES} modbus/TCPSocket.h)
set(SOURCES ${SOURCES} modbus/TCPServer.cpp)
set(SOURCES ${SOURCES} modbus/TCPServer.h)
set(SOURCES ${SOURCES} modbus/TCPClient.cpp)
set(SOURCES ${SOURCES} Plc.cpp)
set(SOURCES ${SOURCES} PlcIo.cpp)
set(SOURCES ${SOURCES} PlcTask.cpp)
set(SOURCES ${SOURCES} PlcMemory.cpp)
set(SOURCES ${SOURCES} TeranisTask.cpp)

add_executable(TERANiS_PLC ${SOURCES})

#force 64 bit mode:
#set_target_properties(TERANiS_PLC PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
