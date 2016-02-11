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
