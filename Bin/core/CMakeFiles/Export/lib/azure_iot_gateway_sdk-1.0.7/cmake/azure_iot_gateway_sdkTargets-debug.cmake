#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "parson" for configuration "Debug"
set_property(TARGET parson APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(parson PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libparson.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS parson )
list(APPEND _IMPORT_CHECK_FILES_FOR_parson "${_IMPORT_PREFIX}/lib/libparson.a" )

# Import target "gateway" for configuration "Debug"
set_property(TARGET gateway APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(gateway PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/libgateway.so"
  IMPORTED_SONAME_DEBUG "libgateway.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS gateway )
list(APPEND _IMPORT_CHECK_FILES_FOR_gateway "${_IMPORT_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/libgateway.so" )

# Import target "gateway_static" for configuration "Debug"
set_property(TARGET gateway_static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(gateway_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/libgateway_static.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS gateway_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_gateway_static "${_IMPORT_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/libgateway_static.a" )

# Import target "module_host_static" for configuration "Debug"
set_property(TARGET module_host_static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(module_host_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/libmodule_host_static.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS module_host_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_module_host_static "${_IMPORT_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/libmodule_host_static.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
