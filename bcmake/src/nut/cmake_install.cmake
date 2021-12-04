# Install script for directory: /doc/dev/qt/Nut/src/nut

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/metatypes" TYPE FILE FILES "/doc/dev/qt/Nut/bcmake/src/nut/meta_types/qt6nut_relwithdebinfo_metatypes.json")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Nut" TYPE FILE FILES
    "/doc/dev/qt/Nut/bcmake/lib/cmake/Qt6Nut/Qt6NutConfig.cmake"
    "/doc/dev/qt/Nut/bcmake/lib/cmake/Qt6Nut/Qt6NutConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQt6Nut.so.6.3.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQt6Nut.so.6"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "\$ORIGIN:/doc/Qt/6.2.0/gcc_64/lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/doc/dev/qt/Nut/bcmake/lib/libQt6Nut.so.6.3.0"
    "/doc/dev/qt/Nut/bcmake/lib/libQt6Nut.so.6"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQt6Nut.so.6.3.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQt6Nut.so.6"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/doc/Qt/6.2.0/gcc_64/lib::::::::"
           NEW_RPATH "\$ORIGIN:/doc/Qt/6.2.0/gcc_64/lib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQt6Nut.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQt6Nut.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQt6Nut.so"
         RPATH "\$ORIGIN:/doc/Qt/6.2.0/gcc_64/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/doc/dev/qt/Nut/bcmake/lib/libQt6Nut.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQt6Nut.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQt6Nut.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQt6Nut.so"
         OLD_RPATH "/doc/Qt/6.2.0/gcc_64/lib::::::::"
         NEW_RPATH "\$ORIGIN:/doc/Qt/6.2.0/gcc_64/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQt6Nut.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/QtNut/6.3.0/QtNut/private" TYPE FILE FILES
    "/doc/dev/qt/Nut/src/nut/core/bulkinserter_p.h"
    "/doc/dev/qt/Nut/src/nut/core/database_p.h"
    "/doc/dev/qt/Nut/src/nut/core/table_p.h"
    "/doc/dev/qt/Nut/src/nut/models/sqlmodel_p.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/QtNut" TYPE FILE FILES
    "/doc/dev/qt/Nut/src/nut/phrase.h"
    "/doc/dev/qt/Nut/src/nut/config/nut_config.h"
    "/doc/dev/qt/Nut/src/nut/config/nut_consts.h"
    "/doc/dev/qt/Nut/src/nut/config/nut_global.h"
    "/doc/dev/qt/Nut/src/nut/config/nut_macros.h"
    "/doc/dev/qt/Nut/src/nut/config/nut_namespace.h"
    "/doc/dev/qt/Nut/src/nut/core/abstracttableset.h"
    "/doc/dev/qt/Nut/src/nut/core/abstracttablesetdata.h"
    "/doc/dev/qt/Nut/src/nut/core/bulkinserter.h"
    "/doc/dev/qt/Nut/src/nut/core/changelogtable.h"
    "/doc/dev/qt/Nut/src/nut/core/database.h"
    "/doc/dev/qt/Nut/src/nut/core/foreigncontainer.h"
    "/doc/dev/qt/Nut/src/nut/core/propertysignalmapper.h"
    "/doc/dev/qt/Nut/src/nut/core/query.h"
    "/doc/dev/qt/Nut/src/nut/core/table.h"
    "/doc/dev/qt/Nut/src/nut/core/tableset.h"
    "/doc/dev/qt/Nut/src/nut/generators/abstractsqlgenerator.h"
    "/doc/dev/qt/Nut/src/nut/generators/mysqlgenerator.h"
    "/doc/dev/qt/Nut/src/nut/generators/postgresqlgenerator.h"
    "/doc/dev/qt/Nut/src/nut/generators/sqlitegenerator.h"
    "/doc/dev/qt/Nut/src/nut/generators/sqlservergenerator.h"
    "/doc/dev/qt/Nut/src/nut/models/databasemodel.h"
    "/doc/dev/qt/Nut/src/nut/models/sqlmodel.h"
    "/doc/dev/qt/Nut/src/nut/models/tablemodel.h"
    "/doc/dev/qt/Nut/src/nut/phrases/abstractfieldphrase.h"
    "/doc/dev/qt/Nut/src/nut/phrases/assignmentphrase.h"
    "/doc/dev/qt/Nut/src/nut/phrases/assignmentphraselist.h"
    "/doc/dev/qt/Nut/src/nut/phrases/conditionalphrase.h"
    "/doc/dev/qt/Nut/src/nut/phrases/fieldphrase.h"
    "/doc/dev/qt/Nut/src/nut/phrases/fieldphrase_bool.h"
    "/doc/dev/qt/Nut/src/nut/phrases/fieldphrase_date.h"
    "/doc/dev/qt/Nut/src/nut/phrases/fieldphrase_qstring.h"
    "/doc/dev/qt/Nut/src/nut/phrases/numericphrase.h"
    "/doc/dev/qt/Nut/src/nut/phrases/phrasedata.h"
    "/doc/dev/qt/Nut/src/nut/phrases/phrasedatalist.h"
    "/doc/dev/qt/Nut/src/nut/phrases/phraselist.h"
    "/doc/dev/qt/Nut/src/nut/types/dbgeography.h"
    "/doc/dev/qt/Nut/src/nut/3rdparty/serializer/src/abstractserializer.h"
    "/doc/dev/qt/Nut/src/nut/3rdparty/serializer/src/binaryserializer.h"
    "/doc/dev/qt/Nut/src/nut/3rdparty/serializer/src/jsonserializer.h"
    "/doc/dev/qt/Nut/src/nut/3rdparty/serializer/src/serializer_global.h"
    "/doc/dev/qt/Nut/src/nut/3rdparty/serializer/src/sqlserializer.h"
    "/doc/dev/qt/Nut/src/nut/3rdparty/serializer/src/stringserializer.h"
    "/doc/dev/qt/Nut/src/nut/3rdparty/serializer/test/containers/foo.h"
    "/doc/dev/qt/Nut/src/nut/3rdparty/serializer/test/enums/foo.h"
    "/doc/dev/qt/Nut/src/nut/3rdparty/serializer/test/object/boo.h"
    "/doc/dev/qt/Nut/src/nut/3rdparty/serializer/test/object/foo.h"
    "/doc/dev/qt/Nut/src/nut/3rdparty/serializer/test/variants/foo.h"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/NutGlobal"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/AbstractTableSet"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/AbstractTableSetData"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/BulkInserter"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/Database"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/ForeignContainer"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/PropertySignalMapper"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/Query"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/Table"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/TableSet"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/AbstractSqlGenerator"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/DatabaseModel"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/SqlModel"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/FieldModel"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/RelationModel"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/TableModel"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/AbstractFieldPhrase"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/AssignmentPhrase"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/AssignmentPhraseList"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/ConditionalPhrase"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/FieldPhrase"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/NumericFieldPhrase"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/PhraseList"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/AbstractSerializer"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/BinarySerializer"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/JsonSerializer"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/SqlSerializer"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/StringSerializer"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/Foo"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/Boo"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/qtnutversion.h"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/QtNutVersion"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/QtNut"
    "/doc/dev/qt/Nut/bcmake/include/QtNut/QtNutDepends"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Nut/Qt6NutTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Nut/Qt6NutTargets.cmake"
         "/doc/dev/qt/Nut/bcmake/src/nut/CMakeFiles/Export/lib/cmake/Qt6Nut/Qt6NutTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Nut/Qt6NutTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Nut/Qt6NutTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Nut" TYPE FILE FILES "/doc/dev/qt/Nut/bcmake/src/nut/CMakeFiles/Export/lib/cmake/Qt6Nut/Qt6NutTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Nut" TYPE FILE FILES "/doc/dev/qt/Nut/bcmake/src/nut/CMakeFiles/Export/lib/cmake/Qt6Nut/Qt6NutTargets-relwithdebinfo.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Nut/Qt6NutVersionlessTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Nut/Qt6NutVersionlessTargets.cmake"
         "/doc/dev/qt/Nut/bcmake/src/nut/CMakeFiles/Export/lib/cmake/Qt6Nut/Qt6NutVersionlessTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Nut/Qt6NutVersionlessTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Nut/Qt6NutVersionlessTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Nut" TYPE FILE FILES "/doc/dev/qt/Nut/bcmake/src/nut/CMakeFiles/Export/lib/cmake/Qt6Nut/Qt6NutVersionlessTargets.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/doc/dev/qt/Nut/bcmake/lib/Qt6Nut.debug")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/./modules" TYPE FILE FILES "/doc/dev/qt/Nut/bcmake/./modules/Nut.json")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Qt6Nut" TYPE FILE FILES "/doc/dev/qt/Nut/bcmake/lib/cmake/Qt6Nut/Qt6NutAdditionalTargetInfo.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/mkspecs/modules" TYPE FILE FILES
    "/doc/dev/qt/Nut/bcmake/mkspecs/modules/qt_lib_nut.pri"
    "/doc/dev/qt/Nut/bcmake/mkspecs/modules/qt_lib_nut_private.pri"
    )
endif()

