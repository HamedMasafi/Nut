# Generated from nut.pro.

cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)

include(.cmake.conf)
project(Nut
    VERSION "1.0"
    DESCRIPTION "Nut"
    HOMEPAGE_URL "https://github.com/HamedMasafi/Nut"
    LANGUAGES CXX C
)

find_package(Qt6 CONFIG REQUIRED COMPONENTS BuildInternals Core Gui Sql)

qt_build_repo()
