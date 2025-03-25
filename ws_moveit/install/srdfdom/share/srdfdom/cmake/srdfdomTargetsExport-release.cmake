#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "srdfdom::srdfdom" for configuration "Release"
set_property(TARGET srdfdom::srdfdom APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(srdfdom::srdfdom PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsrdfdom.so.2.0.7"
  IMPORTED_SONAME_RELEASE "libsrdfdom.so.2.0.7"
  )

list(APPEND _IMPORT_CHECK_TARGETS srdfdom::srdfdom )
list(APPEND _IMPORT_CHECK_FILES_FOR_srdfdom::srdfdom "${_IMPORT_PREFIX}/lib/libsrdfdom.so.2.0.7" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
