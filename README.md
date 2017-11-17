# MultitechConduitModule
An Azure Iot Edge SDK V1 module that integrates with the Multitech Conduit's built in Lora server so that data can be processed locally and sent to Azure IoT Hub

<b>Using the module</b>

A bin directory has been provided which contains the basic required modules for testing the module. Two JSON configuration scripts are supplied that use the modules.

To successful run the IoT Edge solution you must have access to an Azure IoT Hub and have registered the Multitech Conduit device with it. Edit the JSON scripts to enter your IoT Hub name, registerd device name, a device primary key.

The conduit_test_MQTT.json file is configured to use the MQTT endpoint from the LoRa Server.

The conduit_test_udp.json file is configured to use the UDP endpoint from the LoRa Server.

Copy the contents of the bin directory to the Multitech Conduit. This can be easily achieved by directly copying to a MicroSD card to be placed in the Conduit or by using the SCP command to network copy the files to the Conduit.

<i>Hint: its easier to copy everything via the microSD card first, and only use SCP to upload subsequent individual file changes.</i>

Run the IoT Edge solution by executing the iotedge_device_cloud_upload_sample with a parameter specifying the name of the JSON configuration file you want to use.

This will start the solution as a console application where logged information will be displayed.
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