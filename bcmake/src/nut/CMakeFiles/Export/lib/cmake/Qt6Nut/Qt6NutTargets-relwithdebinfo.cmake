#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::Nut" for configuration "RelWithDebInfo"
set_property(TARGET Qt6::Nut APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(Qt6::Nut PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libQt6Nut.so.6.3.0"
  IMPORTED_SONAME_RELWITHDEBINFO "libQt6Nut.so.6"
  )

list(APPEND _IMPORT_CHECK_TARGETS Qt6::Nut )
list(APPEND _IMPORT_CHECK_FILES_FOR_Qt6::Nut "${_IMPORT_PREFIX}/lib/libQt6Nut.so.6.3.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
