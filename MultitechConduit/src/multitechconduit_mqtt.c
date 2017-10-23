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
#include <mosquitto.h>

void mqtt_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{

    MESSAGE_CONFIG newMessageCfg;
    MAP_HANDLE newProperties = Map_Create(NULL);
    if (newProperties == NULL)
    {
        LogError("Failed to create message properties");
        (void)printf("Failed to create message properties");
        (void)fflush(stdout);
    }

    else
    {
        if (Map_Add(newProperties, GW_SOURCE_PROPERTY, GW_SOURCE_BLE_TELEMETRY) != MAP_OK)
        {
            LogError("Failed to set source property");
            (void)printf("Failed to set source property");
            (void)fflush(stdout);
        }
        else if (Map_Add(newProperties, GW_MAC_ADDRESS_PROPERTY, mqtt_module_data->fakeMacAddress) != MAP_OK)
        {
            LogError("Failed to set address property"); 
            (void)printf("Failed to set address property");
            (void)fflush(stdout);
        }
        else
        {
            JSON_Value* json = json_parse_string((const char*)message->payload);
            if (json == NULL)
            {
                LogError("unable to json_parse_string");
                (void)printf("unable to json_parse_string");
                (void)fflush(stdout);
            }
            else
            {
                JSON_Object* root = json_value_get_object(json);
                if (root == NULL)
                {
                    LogError("unable to json_value_get_object");
                    (void)printf("unable to json_value_get_object");
                    (void)fflush(stdout);
                }
                else
                {

                    const char* dataPayload = json_object_get_string(root, "data"); // TODO: Requires decrypting 
                    const char* appeui = json_object_get_string(root, "appeui");
                    const char* deveui = json_object_get_string(root, "deveui");
                    const char* timestamp = json_object_get_string(root, "timestamp");

                    (void)printf("Device: %s, Payload: %s, APPEUI: %s, DEVEUI: %s, TimeStamp: %s\r\n",
                        mqtt_module_data->fakeMacAddress,
                        dataPayload,
                        appeui,
                        deveui,
                        timestamp
                        );
                    (void)fflush(stdout);

                    int payloadsize = sizeof(dataPayload) + sizeof(appeui) + sizeof (deveui) + sizeof(timestamp) + 60;

                    char msgText[180]; // payloadsize;

                    (void)sprintf_s(msgText, sizeof(msgText), "{\"data\": \"%s\",\"appeui\": \"%s\",\"deveui\": \"%s\", \"gtyts\": \"%s\"}",
                        dataPayload,
                        appeui, 
                        deveui, 
                        timestamp
                    );
                    (void)printf("msgText: %s\r\n",msgText);
                    (void)fflush(stdout);

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

                        (void)printf("IoT Hub: Message Sent\r\n");
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

void mqtt_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	int i;
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
    MULTITECHCONDUIT_DATA* module_data = (MULTITECHCONDUIT_DATA*)user_data;

    // Need a deep copy here
    mqtt_module_data = (MULTITECHCONDUIT_DATA*)user_data;

	char *host = "localhost"; //tcp://127.0.0.1"; //localhost
	int port = 1883; // or 8883 for SSL
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