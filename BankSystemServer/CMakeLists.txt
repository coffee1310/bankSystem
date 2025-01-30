cmake_minimum_required(VERSION 3.28.1)
project(BankSystemServer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_SOURCE_FILE_ENCODING "UTF-8")

add_executable(BankSystemServer
    BankSystemServer/BankSystemServer.cpp
    BankSystemServer/sqlite3.c
    packages/sqlite3_c_plus_plus.1.0.3/build/native/include/sqlite3.h
    packages/sqlite3_c_plus_plus.1.0.3/build/native/include/sqlite3ext.h)

find_package(OpenSSL REQUIRED)

target_link_libraries(BankSystemServer wsock32 ws2_32 OpenSSL::Crypto OpenSSL::SSL)
