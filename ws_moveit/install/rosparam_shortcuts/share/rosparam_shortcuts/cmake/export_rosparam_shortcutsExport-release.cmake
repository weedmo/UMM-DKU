#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "rosparam_shortcuts::rosparam_shortcuts" for configuration "Release"
set_property(TARGET rosparam_shortcuts::rosparam_shortcuts APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(rosparam_shortcuts::rosparam_shortcuts PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/librosparam_shortcuts.so"
  IMPORTED_SONAME_RELEASE "librosparam_shortcuts.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS rosparam_shortcuts::rosparam_shortcuts )
list(APPEND _IMPORT_CHECK_FILES_FOR_rosparam_shortcuts::rosparam_shortcuts "${_IMPORT_PREFIX}/lib/librosparam_shortcuts.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
