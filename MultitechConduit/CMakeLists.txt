#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

cmake_minimum_required(VERSION 2.8.12)

set(multitechconduit_sources
    ./src/multitechconduit.c
    ./src/base64Decode.c
    ./src/serialoutput.c
)

set(multitechconduit_headers
    ./inc/mosquitto.h
    ./inc/multitechconduit_protocol.h
    ./inc/multitechconduit.h
    ./inc/base64Decode.h
)

include_directories(./inc)
include_directories(${GW_INC})

#this builds the multitechconduit dynamic library
add_library(multitechconduit MODULE ${multitechconduit_sources}  ${multitechconduit_headers})
target_link_libraries(multitechconduit gateway libmosquitto.so.1)

#this builds the multitechconduit static library
add_library(multitechconduit_static STATIC ${multitechconduit_sources} ${multitechconduit_headers})
target_compile_definitions(multitechconduit_static PRIVATE BUILD_MODULE_TYPE_STATIC)
target_link_libraries(multitechconduit_static gateway libmosquitto)

linkSharedUtil(multitechconduit)
linkSharedUtil(multitechconduit_static)

add_module_to_solution(multitechconduit)

if(install_modules)
    install(TARGETS multitechconduit LIBRARY DESTINATION "${LIB_INSTALL_DIR}/modules") 
endif()

