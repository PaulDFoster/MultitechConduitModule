# Install script for directory: /mnt/c/LinuxPlace/iot-edge/core

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
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/libgateway.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/libgateway.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/libgateway.so"
         RPATH "/mnt/c/LinuxPlace/iot-edge/install-deps/lib:/mnt/c/LinuxPlace/iot-edge/build/proxy/modules/native_module_host/:/mnt/c/LinuxPlace/iot-edge/build/core/")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7" TYPE SHARED_LIBRARY FILES "/mnt/c/LinuxPlace/iot-edge/build/core/libgateway.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/libgateway.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/libgateway.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/libgateway.so"
         OLD_RPATH "/mnt/c/LinuxPlace/iot-edge/install-deps/lib::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/mnt/c/LinuxPlace/iot-edge/install-deps/lib:/mnt/c/LinuxPlace/iot-edge/build/proxy/modules/native_module_host/:/mnt/c/LinuxPlace/iot-edge/build/core/")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/opt/mlinux/3.3.6/sysroots/x86_64-mlinux-linux/usr/bin/arm-mlinux-linux-gnueabi/arm-mlinux-linux-gnueabi-strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/libgateway.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7" TYPE STATIC_LIBRARY FILES "/mnt/c/LinuxPlace/iot-edge/build/core/libgateway_static.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7" TYPE STATIC_LIBRARY FILES "/mnt/c/LinuxPlace/iot-edge/build/core/libmodule_host_static.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/azure_iot_gateway_sdk-1.0.7" TYPE FILE FILES
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/message.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/module.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/module_access.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/module_loader.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/dynamic_library.h"
    "/mnt/c/LinuxPlace/iot-edge/core/../deps/parson/parson.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/experimental/event_system.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/gateway.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/gateway_export.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/gateway_version.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./src/gateway_internal.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/message_queue.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/broker.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/module_loaders/dynamic_loader.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/message.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/module.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/module_access.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/module_loader.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/dynamic_library.h"
    "/mnt/c/LinuxPlace/iot-edge/core/../deps/parson/parson.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/experimental/event_system.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/gateway.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/gateway_export.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/gateway_version.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./src/gateway_internal.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/message_queue.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/broker.h"
    "/mnt/c/LinuxPlace/iot-edge/core/./inc/module_loaders/dynamic_loader.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/cmake/azure_iot_gateway_sdkTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/cmake/azure_iot_gateway_sdkTargets.cmake"
         "/mnt/c/LinuxPlace/iot-edge/build/core/CMakeFiles/Export/lib/azure_iot_gateway_sdk-1.0.7/cmake/azure_iot_gateway_sdkTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/cmake/azure_iot_gateway_sdkTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/cmake/azure_iot_gateway_sdkTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/cmake" TYPE FILE FILES "/mnt/c/LinuxPlace/iot-edge/build/core/CMakeFiles/Export/lib/azure_iot_gateway_sdk-1.0.7/cmake/azure_iot_gateway_sdkTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/cmake" TYPE FILE FILES "/mnt/c/LinuxPlace/iot-edge/build/core/CMakeFiles/Export/lib/azure_iot_gateway_sdk-1.0.7/cmake/azure_iot_gateway_sdkTargets-debug.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/azure_iot_gateway_sdk-1.0.7/cmake" TYPE FILE FILES
    "/mnt/c/LinuxPlace/iot-edge/core/azure_iot_gateway_sdkConfig.cmake"
    "/mnt/c/LinuxPlace/iot-edge/build/core/azure_iot_gateway_sdk/azure_iot_gateway_sdkConfigVersion.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/mnt/c/LinuxPlace/iot-edge/build/core/deps/cmake_install.cmake")

endif()

