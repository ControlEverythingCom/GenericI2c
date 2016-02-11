#include "i2c-devices.h"

String myLog = "";
String temperature = "";
String relayStatus = "";
i2cDevices devices;
int counter = 0;

int scanDevices(String a);
int sendI2cCommand(String a);
int readI2cCommand(String a);
int setTemp(String a);
void processTemp(int vals[]);

void setup() {
    Particle.function("findDevices", scanDevices);
    Particle.function("sendCommand", sendI2cCommand);
    Particle.function("readCommand", readI2cCommand);
    Particle.variable("relayStatus", relayStatus);
    Particle.variable("Temperature", temperature);
    Particle.variable("myLog", myLog);
    devices.init();
    devices.initDevices();
}

void loop() {
    if(millis()%1000==0){
        devices.readI2cCommand("72,0,2", processTemp);
        devices.readI2cCommand("32,10,1", processRelays);
        myLog=devices.updateLog();
    }
}

int scanDevices(String a){
    devices.scan();
    return 1;
}

int sendI2cCommand(String a){
    return devices.sendCommands(a);
}

int readI2cCommand(String a){
    return devices.readI2cCommand(a);
}

void processTemp(int vals[]){
    
    float temp = vals[0];
    temp += (float)vals[1]/256.00;
    
    float tempF = temp*1.8+32;
    
    temperature = String(tempF,2);
}

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
