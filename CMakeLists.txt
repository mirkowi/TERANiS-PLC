cmake_minimum_required(VERSION 3.6)
project(TERANiS_PLC)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(avicstd/h)
include_directories(modbus)

add_executable(TERANiS_PLC
        main.cpp
        modbus/MModbusTCPServer.cpp
        modbus/MRegExp.cpp
        modbus/MRegExp.h
        modbus/MSocket.cpp
        modbus/MSocket.h
        modbus/platform.h
        modbus/TCPSocket.cpp
        modbus/TCPSocket.h
        teranis.cpp
        teranis.h
        modbus/TCPServer.cpp
        modbus/TCPServer.h
        modbus/TCPClient.cpp
        modbus/TCPClient.h 
	plc.cpp 
	plc.h 
	PlcTask.cpp 
	PlcTask.h PlcIo.cpp PlcIo.h PlcIoRevPi.cpp PlcIoRevPi.h)
