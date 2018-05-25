---
title: Simulate a device with Azure IoT Edge (Linux) | Microsoft Docs
description: How to use Azure IoT Edge on Linux to create a simulated device that sends telemetry through an IoT Edge gateway to an IoT hub.
services: iot-hub
documentationcenter: ''
author: chipalost
manager: timlt
editor: ''

ms.assetid: 11e7bf28-ee3d-48d6-a386-eb506c7a31cf
ms.service: iot-hub
ms.devlang: cpp
ms.topic: article
ms.tgt_pltfrm: na
ms.workload: na
ms.date: 06/09/2017
ms.author: andbuc

---

# Use Azure IoT Edge to send device-to-cloud messages with a simulated device (Linux)

## Install the prerequisites

The steps in this tutorial assume you are running Ubuntu Linux.

To install the prerequisite packages, open a shell and run the following commands:

```bash
sudo apt-get update
sudo apt-get install curl build-essential libcurl4-openssl-dev git cmake pkg-config libssl-dev uuid-dev valgrind libglib2.0-dev libtool autoconf
```

In the shell, run the following command to clone the Azure IoT Edge GitHub repository to your local machine:

```bash
git clone https://github.com/Azure/iot-edge.git
```

## How to build the sample

You can now build the IoT Edge runtime and samples on your local machine:

1. Open a shell.

1. Navigate to the root folder in your local copy of the **iot-edge** repository.

1. Run the build script as follows:

    ```sh
    tools/build.sh --disable-native-remote-modules
    ```

This script uses the **cmake** utility to create a folder called **build** in the root folder of your local copy of the **iot-edge** repository and generate a makefile. The script then builds the solution, skipping unit tests and end to end tests. If you want to build and run the unit tests, add the `--run-unittests` parameter. If you want to build and run the end to end tests, add the `--run-e2e-tests`.

> [!NOTE]
> Every time you run the **build.sh** script, it deletes and then recreates the **build** folder in the root folder of your local copy of the **iot-edge** repository.

## How to run the sample

The **build.sh** script generates its output in the **build** folder in your local copy of the **iot-edge** repository. This output includes the four IoT Edge modules used in this sample.

The build script places the:

* **liblogger.so** in the **build/modules/logger** folder.
* **libiothub.so** in the **build/modules/iothub** folder.
* **lib\_identity\_map.so** in the **build/modules/identitymap** folder.
* **libsimulated\_device.so** in the **build/modules/simulated\_device** folder.

Use these paths for the **module path** values as shown in the following JSON settings file:

The simulated\_device\_cloud\_upload\_sample process takes the path to a JSON configuration file as a command-line argument. The following example JSON file is provided in the SDK repository at **samples\\simulated\_device\_cloud\_upload\_sample\\src\\simulated\_device\_cloud\_upload\_sample\_lin.json**. This configuration file works as is unless you modify the build script to place the IoT Edge modules or sample executables in non-default locations.

> [!NOTE]
> The module paths are relative to the directory from where you run the simulated\_device\_cloud\_upload\_sample executable, not the directory where the executable is located. The sample JSON configuration file defaults to writing to 'deviceCloudUploadGatewaylog.log' in your current working directory.

In a text editor, open the file **samples/simulated\_device\_cloud\_upload\_sample/src/simulated\_device\_cloud\_upload\_lin.json** in your local copy of the **iot-edge** repository. This file configures the IoT Edge modules in the sample gateway:

* The **IoTHub** module connects to your IoT hub. You configure it to send data to your IoT hub. Specifically, set the **IoTHubName** value to the name of your IoT hub and set the **IoTHubSuffix** value to **azure-devices.net**. Set the **Transport** value to one of: **HTTP**, **AMQP**, or **MQTT**. Currently, only **HTTP** shares one TCP connection for all device messages. If you set the value to **AMQP**, or **MQTT**, the gateway maintains a separate TCP connection to IoT Hub for each device.
* The **mapping** module maps the MAC addresses of your simulated devices to your IoT Hub device ids. Make sure that **deviceId** values match the ids of the two devices you added to your IoT hub, and that the **deviceKey** values contain the keys of your two devices.
* The **BLE1** and **BLE2** modules are the simulated devices. Note how their MAC addresses match the addresses in the **mapping** module.
* The **Logger** module logs your gateway activity to a file.
* The **module path** values shown in the example assume that you run the sample from the **build** folder in your local copy of the **iot-edge** repository.
* The **links** array at the bottom of the JSON file connects the **BLE1** and **BLE2** modules to the **mapping** module, and the **mapping** module to the **IoTHub** module. It also ensures that all messages are logged by the **Logger** module.

```json
{
    "modules": [
        {
            "name": "IotHub",
          "loader": {
            "name": "native",
            "entrypoint": {
              "module.path": "./modules/iothub/libiothub.so"
            }
            },
            "args": {
              "IoTHubName": "<<insert here IoTHubName>>",
              "IoTHubSuffix": "<<insert here IoTHubSuffix>>",
              "Transport": "HTTP"
            }
          },
        {
            "name": "mapping",
          "loader": {
            "name": "native",
            "entrypoint": {
              "module.path": "./modules/identitymap/libidentity_map.so"
            }
            },
            "args": [
              {
                "macAddress": "01:01:01:01:01:01",
                "deviceId": "<<insert here deviceId>>",
                "deviceKey": "<<insert here deviceKey>>"
              },
              {
                "macAddress": "02:02:02:02:02:02",
                "deviceId": "<<insert here deviceId>>",
                "deviceKey": "<<insert here deviceKey>>"
              }
            ]
          },
        {
            "name": "BLE1",
          "loader": {
            "name": "native",
            "entrypoint": {
              "module.path": "./modules/simulated_device/libsimulated_device.so"
            }
            },
            "args": {
              "macAddress": "01:01:01:01:01:01"
            }
          },
        {
            "name": "BLE2",
          "loader": {
            "name": "native",
            "entrypoint": {
              "module.path": "./modules/simulated_device/libsimulated_device.so"
            }
            },
            "args": {
              "macAddress": "02:02:02:02:02:02"
            }
          },
        {
            "name": "Logger",
          "loader": {
            "name": "native",
            "entrypoint": {
              "module.path": "./modules/logger/liblogger.so"
            }
            },
            "args": {
              "filename": "deviceCloudUploadGatewaylog.log"
            }
          }
    ],
    "links": [
        {
            "source": "*",
            "sink": "Logger"
        },
        {
            "source": "BLE1",
            "sink": "mapping"
        },
        {
            "source": "BLE2",
            "sink": "mapping"
        },
        {
            "source": "mapping",
            "sink": "IotHub"
        }
    ]
}
```

Save the changes you made to the configuration file.

To run the sample:

1. In your shell, navigate to the **iot-edge/build** folder.
2. Run the following command:
   
    ```sh
    ./samples/simulated_device_cloud_upload/simulated_device_cloud_upload_sample ../samples/simulated_device_cloud_upload/src/simulated_device_cloud_upload_lin.json
    ```
3. You can use the [device explorer][lnk-device-explorer] or [iothub-explorer][lnk-iothub-explorer] tool to monitor the messages that IoT hub receives from the gateway. For example, using iothub-explorer you can monitor device-to-cloud messages using the following command:

    ```sh
    iothub-explorer monitor-events --login "HostName={Your iot hub name}.azure-devices.net;SharedAccessKeyName=iothubowner;SharedAccessKey={Your IoT Hub key}"
    ```

## Next steps

To gain a more advanced understanding of Azure IoT Edge and experiment with some code examples, visit the following developer tutorials and resources:

* [Send device-to-cloud messages from a physical device with Azure IoT Edge][lnk-physical-device]

<!-- Links -->
[lnk-physical-device]: ../ble_gateway/iot-hub-iot-edge-physical-device.md
[lnk-device-explorer]: https://github.com/Azure/azure-iot-sdk-csharp/tree/master/tools/DeviceExplorer
[lnk-iothub-explorer]: https://github.com/Azure/iothub-explorer/blob/master/readme.md
