# Multitech Conduit Module Requirements

## Overview
This document describes the Multitech Conduit module.  

The Conduit module will register the device on start, then  publish telemetry messages received from the Multitech Conduit's LoRa MQTT endpoint or the LoRa Application Up Stream UDP endpoint.

## Reference

[module.h](../../../core/devdoc/module.md)

### Expected Arguments

The argument to this module is a JSON object with the following structure:
```json
{
    "macAddress" : "<mac address in canonical form>",
    "iothubProtocol": "<MQTT|UDP>",
    "deviceId": "<IoT Hub registered device name>",
    "deviceKey": "<IoT Hub primary or secondary key>"
}
```
### Example Arguments
```json
{
    "macAddress" : "01:01:01:01:01:01",
    "iothubProtocol": "MQTT",
    "deviceId": "<IoT Hub registered device name>",
    "deviceKey": "<IoT Hub primary or secondary key>"
}
```

##Exposed API
```c
MODULE_EXPORT const MODULE_API* Module_GetApi(MODULE_API_VERSION gateway_api_version);
```

## Module_GetApi

This is the primary public interface for the module.  It fills out the
provided `MODULE_API` structure containing the implementation functions for this
module.
The `MODULE_API` structure shall have non-`NULL` `Module_Create`, `Module_Start`, `Module_Destroy`, and `Module_Receive` fields.

## MultitechConduit_CreateFromJson
```C
MODULE_HANDLE MultitechConduit_CreateFromJson(BROKER_HANDLE broker, const char* configuration);
```
This function creates the Multitech Conduit module.
