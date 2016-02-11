# About

This library provides a class called i2cDevices to manage generic control of any i2c slaves connected to the master.

### Developer Information
---
NCD has been designing and manufacturing computer control products since 1995. We have specialized in hardware design and manufacturing of Relay controllers for 20 years. We pride ourselves as being the industry leader of computer control relay products. Our products are proven reliable and we are very excited to support Particle. For more information on NCD please visit www.controlanything.com

### Requirements
---
- Particle Photon (not tested with Electron but should work)
- Basic understanding of developing on the Particle platform

### How to use this library
---

The libary must be imported into your application. This can be done through the Particle WEB IDE:
1. Click Libraries
2. Select GenericI2c (type into textbox to filter libraries)
3. Click "Include in App" button
4. Select the App you want to include the library in
5. Click Add to this app

For more information see Particles's documentation

### Example use
---
Once the Library is included in your applicaiton you should see an include statement at the top like this:
```cpp
//This #include statement was automatically added by the Particle IDE.
#include "GenericI2c/GenericI2c.h"
```
Now you need to instanciate an object of the library for use in your application like this:
```cpp
GenericI2c i2cController;
```

Here is an example use case for the class, triggering relays based on a temperature using the following products:
- https://www.controleverything.com/content/Temperature?sku=MCP9808_I2CS
- https://www.controleverything.com/content/Relay-Controller?sku=MCP23008_SCIO6R2G5V1_1A

```cpp
// This #include statement was automatically added by the Particle IDE.
#include "GenericI2c/GenericI2c.h"

//Set variables for Particle cloud
String temperature = "";
String relayStatus = "";

//Instantiate I2C class
GenericI2c devices;

//Set addresses for connected devices
int relayAddr = 32;
int tempAddr = 72;

void processTemp(int vals[]);
void processRelays(int val[])

void setup() {
    Particle.variable("relayStatus", relayStatus);
    Particle.variable("Temperature", temperature);
    
    //Initialize I2C communication
    devices.init();
    
    //Add devices for use
    devices.addDevice(relayAddr, "0,252|6,252");
    devices.addDevice(tempAddr, "1,96");
    
    //initialize devices
    devices.initDevices();
}

void loop() {
    if(millis()%1000==0){
        //Once a second check the temperature and relay status
        devices.readI2cCommand(tempAddr, "0,2", processTemp);
        devices.readI2cCommand(relayAddr, "10,1", processRelays);
    }
}

//Callback function for reading temperature
void processTemp(int vals[]){
    //Process temperature to get a real world value
    float temp = vals[0];
    temp += (float)vals[1]/256.00;
    float tempF = temp*1.8+32;
    
    //Set the temperature variable
    temperature = String(tempF,2);
    
    if(tempF > 73){
        //If the temperature in Fahrenheit is greater than 73, turn on both relays
        devices.sendCommand(relayAddr, "10,3");
    }else{
        //Otherwise turn them both off
        devices.sendCommand(relayAddr, "10,0");
    }
}

//Callback function for checking relay status
void processRelays(int val[]){
    relayStatus = val[0];
    relayStatus += " - Relay 1 is ";
    
    if(val[0] == 3 || val[0] == 1) {
        relayStatus += "on";
    }else{
        relayStatus += "off";
    }
    relayStatus += ", Relay 2 is ";
    if(val[0]>1){
        relayStatus += "on";
    }else{
        relayStatus += "off";
    }
}
    
### Public accessible methods
---
```cpp
void init();
```
>This method simply initializes I2C communication, it MUST be called before any read or write commands.

```cpp
String scan();
```
>Scan the whole range of I2C busses and place the available devices into a Log.

```cpp
bool addDevice(int address);
bool addDevice(int address, String initCmds);
```
>This method adds a device to the registry of the class. If no commands are sent a placeholder of 0 is set for the initialization routine.

```cpp
int initDevices();
```
>Initialize all devices in registry with their appropriate initialization commands.

```cpp
bool deviceExists(int address);
```
>Checks if a device exists in the registry. This command DOES NOT validate that the device is connected.

```cpp
String getDevice(int address);
String getDevice(int index, int &address);
```
>Fetches the device from the registry, returns the initialization commands and, in the second case, sets the address. (index refers to the index in the registry)

```cpp
int sendCommand(String command);
int sendCommand(int addr, String command);
```
>Send command to a device. If the address is sent first, it must be omitted from the command string. The command string should be a comma delimitted list of integers and may be of a variable length. This method will retry 3 times if the command fails

```cpp
int sendCommands(String command);
int sendCommands(int addr, String command);
```
>Sends multiple commands to a device, exactly like sendCommand, except it expects a pipe (|) delimited list of commands. This method will retry the entire list of commands 3 times if the final command fails

```cpp
int processCommand(String command);
int processCommand(int addr, String command);
```
>Processes a command, similar to sendCommand except it will never retry

```cpp
int readI2cCommand(String command);
int readI2cCommand(int addr, String command);
int readI2cCommand(String command, void (*fptr)(int*));
int readI2cCommand(int addr, String command, void (*fptr)(int*));
```
>Send read commands to device, if address is sent it must be omitted from the commands. The final part of the comma delimited read command should be the number of bytes expected. A function may be sent in as the last argument and will be used as a callback, sending the bytes received to it.

```cpp
int read(int addr, int registerAddress, int bytes, int* buff);
```
>Not terribly useful by itself. This is the function that does the work for readI2cCommand
