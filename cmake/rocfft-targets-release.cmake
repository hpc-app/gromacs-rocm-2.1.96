#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "roc::rocfft-device" for configuration "Release"
set_property(TARGET roc::rocfft-device APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(roc::rocfft-device PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/rocfft/lib/librocfft-device.so.0.8.6.0"
  IMPORTED_SONAME_RELEASE "librocfft-device.so.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS roc::rocfft-device )
list(APPEND _IMPORT_CHECK_FILES_FOR_roc::rocfft-device "${_IMPORT_PREFIX}/rocfft/lib/librocfft-device.so.0.8.6.0" )

# Import target "roc::rocfft" for configuration "Release"
set_property(TARGET roc::rocfft APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(roc::rocfft PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/rocfft/lib/librocfft.so.0.8.6.0"
  IMPORTED_SONAME_RELEASE "librocfft.so.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS roc::rocfft )
list(APPEND _IMPORT_CHECK_FILES_FOR_roc::rocfft "${_IMPORT_PREFIX}/rocfft/lib/librocfft.so.0.8.6.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
