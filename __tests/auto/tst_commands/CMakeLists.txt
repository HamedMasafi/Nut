# Generated from tst_commands.pro.

#####################################################################
## tst_nut Test:
#####################################################################

if (NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_nut LANGUAGES C CXX ASM)
    find_package(Qt6BuildInternals COMPONENTS STANDALONE_TEST)
endif()

qt_internal_add_test(tst_nut
    QMLTEST
    QML_IMPORTPATH
        "${CMAKE_CURRENT_BINARY_DIR}/../src/imports"
    SOURCES
        ../common/comment.cpp ../common/comment.h
        ../common/post.cpp ../common/post.h
        ../common/user.cpp ../common/user.h
        ../common/weblogdatabase.cpp ../common/weblogdatabase.h
        tst_commands.cpp tst_commands.h
    INCLUDE_DIRECTORIES
        ../../src
        ../common
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::Qml
        Qt::Quick
        Qt::Sql
)

#### Keys ignored in scope 1:.:.:tst_commands.pro:<TRUE>:
# IMPORTPATH = "$$OUT_PWD/../src/imports"
# TEMPLATE = "app"
