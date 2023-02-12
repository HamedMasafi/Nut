# Generated from nut.pro.

cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)

include(.cmake.conf)
project(QtNut
    VERSION "${QT_REPO_MODULE_VERSION}"
    DESCRIPTION "Qt orm library"
    HOMEPAGE_URL "https://github.com/HamedMasafi/Nut"
    LANGUAGES CXX C
)

set(PROJECT_VERSION_MAJOR, 0)
set(PROJECT_VERSION_MINOR, 7)
set(PROJECT_VERSION_PATCH, 0)

find_package(Qt6 CONFIG REQUIRED COMPONENTS BuildInternals Core Gui Sql)

qt_build_repo()
