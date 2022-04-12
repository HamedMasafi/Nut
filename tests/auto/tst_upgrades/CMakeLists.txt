# Generated from tst_upgrades.pro.

#####################################################################
## tst_upgrades Test:
#####################################################################

if (NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_upgrades LANGUAGES C CXX ASM)
    find_package(Qt6BuildInternals COMPONENTS STANDALONE_TEST)
endif()

qt_internal_add_test(tst_upgrades
    SOURCES
        db1.cpp db1.h
        db2.cpp db2.h
        db3.cpp db3.h
        table1.cpp table1.h
        table2.cpp table2.h
        table3.cpp table3.h
        tst_upgrades.cpp tst_upgrades.h
    DEFINES
        NUT_PATH=\\\"${CMAKE_CURRENT_SOURCE_DIR}/../../\\\"
    INCLUDE_DIRECTORIES
        ../common/../common
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::Sql
        nut
)

#### Keys ignored in scope 1:.:.:tst_upgrades.pro:<TRUE>:
# TEMPLATE = "app"

## Scopes:
#####################################################################

#### Keys ignored in scope 4:.:../common:../common/nut-lib.pri:debug_and_release AND NOT ReleaseBuild AND NOT DebugBuild:
# QMAKE_EXTRA_TARGETS = "runtarget"
# runtarget.CONFIG = "recursive"
# runtarget.recurse_target = "run-tests"
# runtarget.target = "run-tests"

#### Keys ignored in scope 5:.:../common:../common/nut-lib.pri:else:
# QMAKE_EXTRA_TARGETS = "oneshell"
# oneshell.target = ".ONESHELL"

#### Keys ignored in scope 6:.:../common:../common/nut-lib.pri:WIN32 AND NOT win32-g++:
# QMAKE_EXTRA_TARGETS = "runtarget"
# runtarget.commands = "set" "PATH=$$shell_path$$shadowed$$dirname_QMAKE_CONF_/bin;$$shell_path$$[QT_INSTALL_BINS];$(PATH)" "$$escape_expand(\\n\\t)" "set" "QT_PLUGIN_PATH=$$shadowed$$dirname_QMAKE_CONF_/plugins;$$[QT_INSTALL_PLUGINS];$(QT_PLUGIN_PATH)" "$$escape_expand(\\n\\t)" "set" "QML2_IMPORT_PATH=$$shadowed$$dirname_QMAKE_CONF_/qml;$$[QT_INSTALL_QML];$(QML2_IMPORT_PATH)" "$$escape_expand(\\n\\t)" "if" "exist" "$${outdir_helper}\\fail" "del" "$${outdir_helper}\\fail" "$$escape_expand(\\n\\t)" "start" "/w" "call" "$(DESTDIR_TARGET)" "^>" "$${outdir_helper}\\test.log" "^|^|" "echo" "FAIL" "^>" "$${outdir_helper}\\fail" "^&" "exit" "0" "$$escape_expand(\\n\\t)" "type" "$${outdir_helper}\\test.log" "$$escape_expand(\\n\\t)" "if" "exist" "$${outdir_helper}\\fail" "exit" "42"
# runtarget.target = "run-tests"

#### Keys ignored in scope 7:.:../common:../common/nut-lib.pri:(CMAKE_BUILD_TYPE STREQUAL Debug):
# outdir_helper = "debug"

#### Keys ignored in scope 8:.:../common:../common/nut-lib.pri:(CMAKE_BUILD_TYPE STREQUAL Release):
# outdir_helper = "release"

#### Keys ignored in scope 9:.:../common:../common/nut-lib.pri:NOT compat_test:
# runtarget.depends = "$(DESTDIR_TARGET)"

#### Keys ignored in scope 10:.:../common:../common/nut-lib.pri:NOT LOGGING_RULES_ISEMPTY:
# runtarget.commands = "$$escape_expand(\\n\\t)" "set" "\"QT_LOGGING_RULES=$$LOGGING_RULES\""

#### Keys ignored in scope 11:.:../common:../common/nut-lib.pri:else:
# QMAKE_EXTRA_TARGETS = "runtarget"
# runtarget.commands = "export" "PATH=\"$$shell_path$$shadowed$$dirname_QMAKE_CONF_/bin:$$shell_path$$[QT_INSTALL_BINS]:$${LITERAL_DOLLAR}$${LITERAL_DOLLAR}PATH\"" "$$escape_expand(\\n\\t)" "export" "QT_PLUGIN_PATH=\"$$shadowed$$dirname_QMAKE_CONF_/plugins$${QMAKE_DIRLIST_SEP}$$[QT_INSTALL_PLUGINS]$${QMAKE_DIRLIST_SEP}$(QT_PLUGIN_PATH)\"" "$$escape_expand(\\n\\t)" "export" "QML2_IMPORT_PATH=\"$$shadowed$$dirname_QMAKE_CONF_/qml$${QMAKE_DIRLIST_SEP}$$[QT_INSTALL_QML]$${QMAKE_DIRLIST_SEP}$(QML2_IMPORT_PATH)\""
# runtarget.target = "run-tests"

#### Keys ignored in scope 12:.:../common:../common/nut-lib.pri:win32-g++:
# QMAKE_DIRLIST_SEP = ";"

#### Keys ignored in scope 13:.:../common:../common/nut-lib.pri:NOT LOGGING_RULES_ISEMPTY:
# runtarget.commands = "$$escape_expand(\\n\\t)" "export" "QT_LOGGING_RULES=\"$$LOGGING_RULES\""

#### Keys ignored in scope 15:.:../common:../common/nut-lib.pri:LINUX OR win32-g++:
# runtarget.commands = "$$escape_expand(\\n\\t)" "export" "LD_LIBRARY_PATH=\"$$shadowed$$dirname_QMAKE_CONF_/lib$${QMAKE_DIRLIST_SEP}$$[QT_INSTALL_LIBS]$${QMAKE_DIRLIST_SEP}$(LD_LIBRARY_PATH)\"" "$$escape_expand(\\n\\t)" "export" "QT_QPA_PLATFORM=minimal"

#### Keys ignored in scope 17:.:../common:../common/nut-lib.pri:APPLE:
# runtarget.commands = "$$escape_expand(\\n\\t)" "export" "DYLD_LIBRARY_PATH=\"$$shadowed$$dirname_QMAKE_CONF_/lib:$$[QT_INSTALL_LIBS]:$(DYLD_LIBRARY_PATH)\"" "$$escape_expand(\\n\\t)" "export" "DYLD_FRAMEWORK_PATH=\"$$shadowed$$dirname_QMAKE_CONF_/lib:$$[QT_INSTALL_LIBS]:$(DYLD_FRAMEWORK_PATH)\""

#### Keys ignored in scope 19:.:../common:../common/nut-lib.pri:NOT compat_test:
# runtarget.depends = "$(DESTDIR_TARGET)"

#### Keys ignored in scope 20:.:../common:../common/nut-lib.pri:else:
# runtarget.commands = "$$escape_expand(\\n\\t)" "./$(TARGET)"

#### Keys ignored in scope 21:.:../common:../common/nut-lib.pri:NOT compat_test:
# runtarget.depends = "$(TARGET)"
