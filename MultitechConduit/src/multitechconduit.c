// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>


#include "multitechconduit.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "messageproperties.h"
#include "message.h"
#include "module.h"
#include "broker.h"

#include <parson.h>

#include "multitechconduit_protocol.h"

long count = 0;

void PublishNewMessageToBroker(const char * payload)
{

    // Code issues:
    // 1. Uses global mqtt_module_data because mqtt_message_callback can't obtain module_data otherwise

    MESSAGE_CONFIG newMessageCfg;
    MAP_HANDLE newProperties = Map_Create(NULL);
    if (newProperties == NULL)
    {
        LogError("Failed to create message properties");
    }

    else
    {
        /*Codes_SRS_IDMAP_17_032: [IdentityMap_Receive shall call Map_AddOrUpdate with key of "source" and value of "mapping".]*/
        if (Map_Add(newProperties, GW_SOURCE_PROPERTY, GW_IDMAP_MODULE) != MAP_OK)
        {
            /*Codes_SRS_IDMAP_17_033: [If adding source fails, IdentityMap_Receive shall deallocate all resources and return.]*/
            LogError("Could not attach %s property to message", GW_SOURCE_PROPERTY);
        }
        else if (Map_AddOrUpdate(newProperties, GW_DEVICENAME_PROPERTY, mqtt_module_data->deviceId) != MAP_OK)
        {
            /*Codes_SRS_IDMAP_17_029: [If adding deviceName fails,IdentityMap_Receive shall deallocate all resources and return.]*/
            LogError("Could not attach %s property to message", GW_DEVICENAME_PROPERTY);
        }
        /*Codes_SRS_IDMAP_17_030: [IdentityMap_Receive shall call Map_AddOrUpdate with key of "deviceKey" and value of found deviceKey.]*/
        else if (Map_AddOrUpdate(newProperties, GW_DEVICEKEY_PROPERTY, mqtt_module_data->deviceKey) != MAP_OK)
        {
            /*Codes_SRS_IDMAP_17_031: [If adding deviceKey fails, IdentityMap_Receive shall deallocate all resources and return.]*/
            LogError("Could not attach %s property to message", GW_DEVICEKEY_PROPERTY);
        }
        else
        {
            JSON_Value* json = json_parse_string(payload);

            if (json == NULL)
            {
                LogError("unable to json_parse_string");
            }
            else
            {
                JSON_Object* root = json_value_get_object(json);
                if (root == NULL)
                {
                    LogError("unable to json_value_get_object");
                }
                else
                {

                    const char* dataPayload = json_object_get_string(root, "data");
                    const char* appeui = json_object_get_string(root, "appeui");
                    const char* deveui = json_object_get_string(root, "deveui");
                    const char* timestamp = json_object_get_string(root, "timestamp");

                    char msgText[180]; 
                    
                    (void)sprintf_s(msgText, sizeof(msgText), "{\"data\": \"%s\",\"appeui\": \"%s\",\"deveui\": \"%s\", \"gtyts\": \"%s\"}",
                        dataPayload,
                        appeui, 
                        deveui, 
                        timestamp
                    );

                    newMessageCfg.sourceProperties = newProperties;

                    newMessageCfg.size = strlen(msgText);
                    newMessageCfg.source = (const unsigned char*) msgText;

                    MESSAGE_HANDLE newMessage = Message_Create(&newMessageCfg);

                    if (newMessage == NULL)
                    {
                        LogError("Failed to create new message");
                    }
                    else
                    {
                        if (Broker_Publish(mqtt_module_data->broker, (MODULE_HANDLE)mqtt_module_data, newMessage) != BROKER_OK)
                        {
                            LogError("Failed to publish new message");
                        }

                        count++;
                        (void)printf("Count %d\n\r",count);
                        (void)fflush(stdout);

                        Message_Destroy(newMessage);

                    }
                }
                
            }
        }

        Map_Destroy(newProperties);

    }

    return;
}

void mqtt_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    PublishNewMessageToBroker((const char*)message->payload);
}

void mqtt_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	if(!result){
		/* Subscribe to broker information topics on successful connect. */
		mosquitto_subscribe(mosq, NULL, "lora/+/up", 2); // $SYS/# is topic specifier. "lora/+/up" is Conduit lorawan message topic
	}else{
		fprintf(stderr, "Connect failed\n");
	}
}

void mqtt_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
	int i;

	printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
	for(i=1; i<qos_count; i++){
		printf(", %d", granted_qos[i]);
	}
	printf("\n");
}

void mqtt_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	/* Pring all log messages regardless of level. */
	printf("%s\n", str);
}

static int mqtt_worker(void * user_data)
{
    // Code issues:
    // 1. Global mqtt_module_data because mqtt_message_callback can't obtain module_data otherwise
    // 4. Hardcoded keepalive

    mqtt_module_data = (MULTITECHCONDUIT_DATA*)user_data;

	char *host = (char *)mqtt_module_data->portHost; //"localhost"; //tcp://127.0.0.1"; //localhost
	int port = mqtt_module_data->port; //1883; // or 8883 for SSL
	int keepalive = 60;
	bool clean_session = true;
	struct mosquitto *mosq = NULL;

	mosquitto_lib_init();
	mosq = mosquitto_new(NULL, clean_session, NULL);
	if(!mosq){
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}
	mosquitto_log_callback_set(mosq, mqtt_log_callback);
	mosquitto_connect_callback_set(mosq, mqtt_connect_callback);
	mosquitto_message_callback_set(mosq, mqtt_message_callback);
	mosquitto_subscribe_callback_set(mosq, mqtt_subscribe_callback);

	if(mosquitto_connect(mosq, host, port, keepalive)){
		fprintf(stderr, "Unable to connect.\n");
		return 1;
	}

	mosquitto_loop_forever(mosq, -1, 1);

	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
    return 0;
}

static int udp_worker(void * user_data)
{
    //Code Issues:
    // 1. Global mqtt_module_data because mqtt_message_callback can't obtain module_data otherwise
    // 4. Hardcoded buffer length (512)
    // 5. No means to restart UDP socket if failure
    // 6. close not predefined in a header, causes compiler warning

    // Setting global to be compatible with mqtt callback which has no means of obtaining the module_data
    mqtt_module_data = (MULTITECHCONDUIT_DATA*)user_data;
    
	MULTITECHCONDUIT_DATA* module_data = (MULTITECHCONDUIT_DATA*)user_data;

	//int port = 1784; // Up stream application port
	struct sockaddr_in si_other,si_me;

    int socketInstance, buflen = 512;
    socklen_t slen = sizeof(si_other);

    char buf[buflen];
    if ((socketInstance = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		LogError("Failed to create UDP socket");
		return 1;
	}

    memset((char*)&si_me, 0, sizeof(si_me));
    
	si_me.sin_family = AF_INET;
    si_me.sin_port = htons(module_data->port);
    
    struct in_addr address4;
    
    // getaddrinfo would be a better solution. Some Conduit's have many eth interfaces
    inet_pton(AF_INET, module_data->portHost, &address4);

    si_me.sin_addr = address4;//htonl();//INADDR_ANY);

    if(bind(socketInstance, (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
    {
        LogError("Failed to bind socket");
        return 1;
    }

	while (module_data->listenerRunning)
	{
		memset(buf, '\0', buflen);
		if (recvfrom(socketInstance, buf, buflen, 0, (struct sockaddr *)&si_other, &slen) == -1)
		{
			LogError("Failed to receive message");
        }

        char * trimmedBuffer;

        trimmedBuffer = strstr(buf,"/up");
        if(trimmedBuffer!=NULL)
        {
            trimmedBuffer = strchr(buf,'{');
            if(trimmedBuffer!=NULL)
            {
                int startposition = trimmedBuffer - buf;
                char * payload;

                if (mallocAndStrcpy_s(&(payload), &buf[startposition]) != 0)
                {
                    LogError("failed to copy payload");
                }
                else
                {
                    PublishNewMessageToBroker(payload);
                }

            }
        }
        else
        {
            (void)printf("Message received not data payload\n\r");
            (void)fflush(stdout);
        }

    }
	close(socketInstance);

	return 0;
}

static void MultitechConduit_Receive(MODULE_HANDLE moduleHandle, MESSAGE_HANDLE messageHandle)
{
    // Print the properties & content of the received message
    CONSTMAP_HANDLE properties = Message_GetProperties(messageHandle);
    if (properties != NULL)
    {
        const char* addr = ((MULTITECHCONDUIT_DATA*)moduleHandle)->fakeMacAddress;

        // We're only interested in cloud-to-device (C2D) messages addressed to
        // this device
        if (ConstMap_ContainsKey(properties, GW_MAC_ADDRESS_PROPERTY) == true &&
            strcmp(addr, ConstMap_GetValue(properties, GW_MAC_ADDRESS_PROPERTY)) == 0)
        {
            const char* const * keys;
            const char* const * values;
            size_t count;

            if (ConstMap_GetInternals(properties, &keys, &values, &count) == CONSTMAP_OK)
            {
                const CONSTBUFFER* content = Message_GetContent(messageHandle);
                if (content != NULL)
                {
                    (void)printf(
                        "Received a message\r\n"
                        "Properties:\r\n"
                        );

                    for (size_t i = 0; i < count; ++i)
                    {
                        (void)printf("  %s = %s\r\n", keys[i], values[i]);
                    }

                    (void)printf("Content:\r\n");
                    (void)printf("  %.*s\r\n", (int)content->size, content->buffer);
                    (void)fflush(stdout);
                }
            }
        }

        ConstMap_Destroy(properties);
    }

    return;
}

static void MultitechConduit_Destroy(MODULE_HANDLE moduleHandle)
{
    if (moduleHandle == NULL)
    {
        LogError("Attempt to destroy NULL module");
    }
    else
    {
        MULTITECHCONDUIT_DATA* module_data = (MULTITECHCONDUIT_DATA*)moduleHandle;
        int result;

        /* Tell thread to stop */
        module_data->listenerRunning = 0;
        /* join the thread */
        ThreadAPI_Join(module_data->listenerThread, &result);
        /* free module data */
        free((void*)module_data->fakeMacAddress);
        free(module_data);
    }
}

static void MultitechConduit_Start(MODULE_HANDLE moduleHandle)
{
    if (moduleHandle == NULL)
    {
        LogError("Attempt to start NULL module");
    }
    else
    {
        MESSAGE_CONFIG newMessageCfg;
        MAP_HANDLE newProperties = Map_Create(NULL);
        if (newProperties == NULL)
        {
            LogError("Failed to create message properties");
        }
        else
        {
            MULTITECHCONDUIT_DATA* module_data = (MULTITECHCONDUIT_DATA*)moduleHandle;

            /*Codes_SRS_IDMAP_17_032: [IdentityMap_Receive shall call Map_AddOrUpdate with key of "source" and value of "mapping".]*/
            if (Map_Add(newProperties, GW_SOURCE_PROPERTY, GW_IDMAP_MODULE) != MAP_OK)
            {
                /*Codes_SRS_IDMAP_17_033: [If adding source fails, IdentityMap_Receive shall deallocate all resources and return.]*/
                LogError("Could not attach %s property to message", GW_SOURCE_PROPERTY);
            }
            else if (Map_Add(newProperties, GW_DEVICENAME_PROPERTY, module_data->deviceId) != MAP_OK)
            {
                /*Codes_SRS_IDMAP_17_029: [If adding deviceName fails,IdentityMap_Receive shall deallocate all resources and return.]*/
                LogError("Could not attach %s property to message", GW_DEVICENAME_PROPERTY);
            }
            /*Codes_SRS_IDMAP_17_030: [IdentityMap_Receive shall call Map_AddOrUpdate with key of "deviceKey" and value of found deviceKey.]*/
            else if (Map_Add(newProperties, GW_DEVICEKEY_PROPERTY, module_data->deviceKey) != MAP_OK)
            {
                /*Codes_SRS_IDMAP_17_031: [If adding deviceKey fails, IdentityMap_Receive shall deallocate all resources and return.]*/
                LogError("Could not attach %s property to message", GW_DEVICEKEY_PROPERTY);
            }
            else if (Map_Add(newProperties, "deviceFunction", "register") != MAP_OK)
            {
                LogError("Failed to set deviceFunction property");
            }
            
            else
            {
                    newMessageCfg.size = 0;
                    newMessageCfg.source = NULL;
                    newMessageCfg.sourceProperties = newProperties;

                    MESSAGE_HANDLE newMessage = Message_Create(&newMessageCfg);
                    if (newMessage == NULL)
                    {
                        LogError("Failed to create register message");
                    }
                    else
                    {
                        if (Broker_Publish(module_data->broker, (MODULE_HANDLE)module_data, newMessage) != BROKER_OK)
                        {
                            LogError("Failed to publish register message");
                        }

                        Message_Destroy(newMessage);
                    }
                }
            }
            Map_Destroy(newProperties);

            MULTITECHCONDUIT_DATA* module_data = (MULTITECHCONDUIT_DATA*)moduleHandle;
            /* OK to start */

            if((strcmp(module_data->iothubProtocol,"MQTT")==0) || (strcmp(module_data->iothubProtocol,"mqtt")==0))
            {
                /* Create a MQTT listener thread.  */
                
                if (ThreadAPI_Create(
                    &(module_data->listenerThread),
                    mqtt_worker,
                    (void*)module_data) != THREADAPI_OK)
                {
                    LogError("ThreadAPI_Create failed");
                    module_data->listenerThread = NULL;
                }
            }
            else if ((strcmp(module_data->iothubProtocol,"UDP")==0) || (strcmp(module_data->iothubProtocol,"udp")==0))
            {
                /* Create a UDP listener thread.  */
                
                if (ThreadAPI_Create(
                    &(module_data->listenerThread),
                    udp_worker,
                    (void*)module_data) != THREADAPI_OK)
                {
                    LogError("ThreadAPI_Create failed");
                    module_data->listenerThread = NULL;
                }
                
            }
            else
            {
                LogError("Failed to create protocol listener: Unknown protocol specified");  
            }
            
    }
}

static MODULE_HANDLE MultitechConduit_Create(BROKER_HANDLE broker, const void* configuration)
{
    MULTITECHCONDUIT_DATA * result;
    MULTITECHCONDUIT_CONFIG * config = (MULTITECHCONDUIT_CONFIG *) configuration;
    if (broker == NULL || config == NULL)
    {
        LogError("Invalid Multitech Conduit module args.");
        result = NULL;
    }
    else
    {
        /* allocate module data struct */
        result = (MULTITECHCONDUIT_DATA*)malloc(sizeof(MULTITECHCONDUIT_DATA));
        if (result == NULL)
        {
            LogError("Couldn't allocate memory for Multitech Conduit Module");
        }
        else
        {
            /* save the message broker */
            result->broker = broker;
            /* set module is running to true */
            result->listenerRunning = 1;
            /* save fake MacAddress */
            char * newFakeAddress;
            int status = mallocAndStrcpy_s(&newFakeAddress, config -> macAddress);

            if (status != 0)
            {
                LogError("MacAddress did not copy");
            }
            else
            {
                result->fakeMacAddress = newFakeAddress;
                result -> iothubProtocol = config -> iothubProtocol;
                result->deviceId = config->deviceId;
                result->deviceKey = config->deviceKey;
                result->portHost = config->portHost;
                result->port = config->port;
                result->listenerThread = NULL;

            }

        }
    }
    return result;
}

static void * MultitechConduit_ParseConfigurationFromJson(const char* configuration)
{
    MULTITECHCONDUIT_CONFIG * result;
    if (configuration == NULL)
    {
        LogError("invalid module args.");
        result = NULL;
    }
    else
    {
        JSON_Value* json = json_parse_string((const char*)configuration);
        if (json == NULL)
        {
            LogError("unable to json_parse_string");
            result = NULL;
        }
        else
        {
            JSON_Object* root = json_value_get_object(json);
            if (root == NULL)
            {
                LogError("unable to json_value_get_object");
                result = NULL;
            }
            else
            {
                MULTITECHCONDUIT_CONFIG config;
                const char* macAddress = json_object_get_string(root, "macAddress");
                if (macAddress == NULL)
                {
                    LogError("unable to json_object_get_string");
                    result = NULL;
                }
                else
                {
                    const char* protocol = json_object_get_string(root, "iothubProtocol");
                    const char* deviceId = json_object_get_string(root, "deviceId");
                    const char* deviceKey = json_object_get_string(root, "deviceKey");
                    const char* portHost = json_object_get_string(root, "portHost");
                    int port = (int) json_object_get_number(root, "port");
                    if (mallocAndStrcpy_s(&(config.iothubProtocol), protocol) != 0)
                    {
                        LogError("Invalid IOT Hub protocol specified");
                        result = NULL;
                    }
                    else
                    {
                        if (mallocAndStrcpy_s(&(config.macAddress), macAddress) != 0)
                        {
                            LogError("Invalid mac address specified");
                            result = NULL;
                        }
                        else
                            if (mallocAndStrcpy_s(&(config.deviceId), deviceId) != 0)
                            {
                                LogError("Invalid device id specified");
                                result = NULL;
                            }
                            else
                                if (mallocAndStrcpy_s(&(config.deviceKey), deviceKey) != 0)
                                {
                                    LogError("Invalid device key specified");
                                    result = NULL;
                                }
                                else
                                    if (mallocAndStrcpy_s(&(config.portHost), portHost) != 0)
                                    {
                                        LogError("Invalid port host specified");
                                        result = NULL;
                                    }
                                    else
                                        if (port <= 0)
                                        {
                                            LogError("Invalid port specified");
                                            result = NULL;
                                        }
                                        else
                                        {
                                            config.port = port;
                                            result = malloc(sizeof(MULTITECHCONDUIT_CONFIG));
                                            if (result == NULL) {
                                                free(config.macAddress);
                                                free(config.iothubProtocol);
                                                free(config.deviceId);
                                                free(config.deviceKey);
                                                free(config.portHost);
                                                LogError("allocation of configuration failed");
                                            }
                                            else
                                            {
                                                *result = config;
                                            }
                                        }
                    }
                }
            }
            json_value_free(json);
        }
    }
    return result;
}

void MultitechConduit_FreeConfiguration(void * configuration)
{
    if (configuration != NULL)
    {
        MULTITECHCONDUIT_CONFIG * config = (MULTITECHCONDUIT_CONFIG *)configuration;
        free(config->macAddress);
        free(config);
    }
}

/*
 *    Required for all modules:  the public API and the designated implementation functions.
 */
static const MODULE_API_1 MultitechConduit_APIS_all =
{
    {MODULE_API_VERSION_1},

    MultitechConduit_ParseConfigurationFromJson,
    MultitechConduit_FreeConfiguration,
    MultitechConduit_Create,
    MultitechConduit_Destroy,
    MultitechConduit_Receive,
    MultitechConduit_Start
};

#ifdef BUILD_MODULE_TYPE_STATIC
MODULE_EXPORT const MODULE_API* MODULE_STATIC_GETAPI(MULTITECHCONDUIT_MODULE)(MODULE_API_VERSION gateway_api_version)
#else
MODULE_EXPORT const MODULE_API* Module_GetApi(MODULE_API_VERSION gateway_api_version)
#endif
{
    (void)gateway_api_version;
    return (const MODULE_API *)&MultitechConduit_APIS_all;
}
