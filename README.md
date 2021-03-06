# MultitechConduitModule
An Azure Iot Edge SDK V1 module that integrates with the Multitech Conduit's built in Lora server so that data can be processed locally and sent to Azure IoT Hub

<b>Using the module</b>

A bin directory has been provided which contains the basic required modules for testing the module. Two JSON configuration scripts are supplied that use the modules.

To successful run the IoT Edge solution you must have access to an Azure IoT Hub and have registered the Multitech Conduit device with it. Edit the JSON scripts to enter your IoT Hub name, registerd device name, a device primary key.

The conduit_test_MQTT.json file is configured to use the MQTT endpoint from the LoRa Server.

The conduit_test_udp.json file is configured to use the UDP endpoint from the LoRa Server.

Copy the contents of the bin directory to the Multitech Conduit. This can be easily achieved by directly copying to a MicroSD card to be placed in the Conduit or by using the SCP command to network copy the files to the Conduit.

<i>Hint: its easier to copy everything via the microSD card first, and only use SCP to upload subsequent individual file changes.</i>

Run the IoT Edge solution by executing the iotedge_device_cloud_upload_sample sample from the IoT Edge V1 SDK, or use the bridge executable in the bin directory, with a parameter specifying the name of the JSON configuration file you want to use.

This will start the solution as a console application where logged information will be displayed. A modified simulated_device_cloud_upload code drop is now included in this repo. It removes the 'wait for user input' to stop the application, and thereby allows the application to be run as a background process with the addition of an & suffix on the command line. The code provided is a very simple solution and not intended for production.

The solution is stopped by pressing a key or crtl-c.

<b>Adding the module to the IoT Edge SDK V1 build script</b>

The module source can be compiled using the Multitech published toolchain.

See the geting started page on Azure IoT Suite catalog for the Multitech Conduit.

https://catalog.azureiotsuite.com/details?title=MultiConnect-Conduit&source=partner-page

A <i>source</i> command is required to specify the development environment:

<i>For example:

source /opt/mlinux/3.3.6/environment-setup-arm926ejste-mlinux-linux-gnueabi
</i>

In the IoT Edge V1 SDK Modules directory, edit the CMakeLists.txt file and add the line:

<i>add_subdirectory(MultitechConduit)
</i>

Now running the top level build script for IoT Edge with the toolchain parameter specifying the Multitech toolchain file, will compile the IoT Edge SDK and the Multitech Module. For example:

<i>./iot-edge/tools/build.sh --toolchain-file toolchain.cmake --disable-native-remote-modules</i>

<b>Multitech Conduit LoRa Setup</b>

Follow the Multitech instructions for setting up LoRa on your Conduit device.

http://www.multitech.net/developer/software/lora/getting-started-with-lora-conduit-aep/

Ensure the sensor node whitelist is correctly setup via the lora-query -a commmand.

Each sensor node needs to be specified unless the named network and passphase security options have been used and published to every connecting device.

<b>Multitech Conduit Firmware 1.4.11 issue</b>

The Multitech Conduit firmware 1.4.11 produces invalid JSON on the stream end points, with duplicate keys. A JSON pre-processor has been implemented to strip duplicate keys from the JSON. First key wins.

The firmware is also not producing a timestamp element. Therefore, gateway timestamp element output is null.

<b>IoT Hub module modification</b>

An extended IoT Hub module has been added to this repository to provide support for Direct Methods and Device TWIN. 

Direct Methods are implemented by setting up the callbacks required in the IoT Hub module. On receipt of a Direct Method callback the module creates a new broker message addressed to the Multitech Conduit module with a METHODNAME property containing the method called. The Multitech Conduit Module receives the message and determines which subroutine it will execute, retrieving the method call parameters from the payload of the broker message.
IoT Hub module sends a success response to Azure IoT Hub as soon as it has created the broker message and submitted it. 

Device TWIN. The Device TWIN callbacks have been registered as demonstrated by the IoT Hub C Client SDK sample for device TWIN support. However, this does not work correctly at thie time. Notification of device TWIN state is received by the module callbacks, but the reporting of device state back to IoT Hub fails. The product team have determined a bug and are investigating.