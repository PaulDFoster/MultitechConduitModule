// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef MULTITECHCONDUIT_H
#define MULTITECHCONDUIT_H

#include "module.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/crt_abstractions.h"

typedef struct MULTITECHCONDUIT_DATA_TAG
{
    BROKER_HANDLE       broker;
    THREAD_HANDLE       listenerThread;
    const char *        fakeMacAddress;
    const char *        iothubProtocol;
    const char *        deviceId;
    const char *        deviceKey;
    const char *        portHost;
    int                 port;
    unsigned int        listenerRunning : 1;
} MULTITECHCONDUIT_DATA;

typedef struct MULTITECHCONDUIT_CONFIG_TAG
{
    char *              macAddress;
    char *              iothubProtocol;
    char *              deviceId;
    char *              deviceKey;
    char *              portHost;
    int                 port;
} MULTITECHCONDUIT_CONFIG;

//Bad
// A global copy of module data to allow the mqtt callback to access the module data and broker directly
MULTITECHCONDUIT_DATA * mqtt_module_data; // = malloc(size_t(MULTITECHCONDUIT_DATA))

#ifdef __cplusplus
extern "C"
{
#endif

MODULE_EXPORT const MODULE_API* MODULE_STATIC_GETAPI(MULTITECHCONDUIT_MODULE)(MODULE_API_VERSION gateway_api_version);

#ifdef __cplusplus
}
#endif

#endif /*MULTITECHCONDUIT_H*/
