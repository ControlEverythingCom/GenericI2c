#include "i2c-devices.h"

String i2cDevices::updateLog(){
    return Log;
}

void i2cDevices::init(){
    Log = "Init --";
    Wire.begin();
    initialized = true;
}
int i2cDevices::initDevices(){
    int index = 0;
    int addr = 0;
    int status = 1;
    String commands = getDevice(index, addr);
    
    while(addr>0){
        status = sendCommands(addr, commands);
        index++;
        addr=0;
        commands = getDevice(index, addr);
    }
    return status;
}
bool i2cDevices::addDevice(int address){
    if (!deviceExists(address)) {
        addresses += address + " ";
        initCommands +=  "0 ";
        return true;
    }
    return false;
}
bool i2cDevices::addDevice(int address, String initCmds){
    if (!deviceExists(address)) {
        addresses += String(address) + " ";
        initCommands += initCmds + " ";
        return true;
    }
    return false;
}
bool i2cDevices::deviceExists(int address){
    char * addrs = new char[addresses.length() + 1];
    strcpy(addrs, addresses.c_str());
    //grab first part of string
    char * p = strtok(addrs, " ");
    while (p!=NULL) {
        if (atoi(p) == address) {
            delete addrs;
            return true;
        }
        p = strtok(NULL, " ");
    }
    delete addrs;
    return false;
}

String i2cDevices::getDevice(int address){
    char * addrs = new char[addresses.length() + 1];
    strcpy(addrs, addresses.c_str());
    int ind=0;
    int device=-1;
    //grab first part of string
    char * p = strtok(addrs, " ");
    while (p!=NULL && device<0) {
        if(atoi(p) == address){
            device=ind;
        }
        p = strtok(NULL, " ");
        ind++;
    }
    delete addrs;
    char * cmds = new char[initCommands.length() + 1];
    strcpy(cmds, initCommands.c_str());
    ind=0;
    //grab first part of string
    p = strtok(cmds, " ");
    while (p!=NULL) {
        if(ind==device){
            delete cmds;
            return String(p);
        }
        p = strtok(NULL, " ");
        ind++;
    }
    delete cmds;
    return "";
}
String i2cDevices::getDevice(int index, int &addr){
    char delimiter = ' ';
    int to = -1;
    int pos = indexOfNth(addresses, delimiter, index)+1;
    
    if(pos<0 && index==0){
        addr = addresses.toInt();
    }else if(index>0){
        return "";
    }else{
        to = addresses.indexOf(delimiter, pos);
        if(to>pos){
            addr = addresses.substring(pos, to).toInt();
        }else{
            addr = addresses.substring(pos).toInt();
        }
    }
    
    pos = indexOfNth(initCommands, delimiter, index)+1;
    String commands = 0;
    if(pos<0 && index==0){
        return initCommands;
    }else if(index>0){
        return "";
    }else{
        to = initCommands.indexOf(delimiter, pos);
        if(to>pos){
            return initCommands.substring(pos, to);
        }else{
            return initCommands.substring(pos);
        }
    }
}
int i2cDevices::sendCommand(int addr, String command){
    int tries=0;
    int status=1;
    while (tries<3 && status!=0) {
        status = processCommand(addr, command);
        tries++;
    }
    return status;
};
int i2cDevices::sendCommand(String command){
    int tries=0;
    int status=1;
    while (tries<3 && status!=0) {
        status = processCommand(command);
        tries++;
    }
    return status;
};
int i2cDevices::sendCommands(int addr, String command){
    int tries=0;
    int status=1;
    //char * cmdstr;
    //char * p;
    int del = command.indexOf('|');
    String current;
    if (del == -1) return sendCommand(addr, command);
    while (del > 0) {
        tries=0;
        current = command.substring(0, del);
        command = command.substring(del+1);
        del = command.indexOf('|');
        while (tries<3 && status!=0){
            status = processCommand(addr, current);
            tries++;
        }
    }
    tries=0;
    status=1;
    while (tries<3 && status!=0){
        status = processCommand(addr, command);
        tries++;
    }
    return status;
};
int i2cDevices::sendCommands(String command){
    int tries=0;
    int status=1;
    //char * cmdstr;
    //char * p;
    int del = command.indexOf('|');
    String current;
    if (del == -1) return sendCommand(command);
    while (del > 0) {
        tries=0;
        current = command.substring(0, del);
        command = command.substring(del+1);
        del = command.indexOf('|');
        while (tries<3 && status!=0){
            status = processCommand(current);
            tries++;
        }
    }
    tries=0;
    status=1;
    while (tries<3 && status!=0){
        status = processCommand(command);
        tries++;
    }
    return status;
};
int i2cDevices::processCommand(int addr, String command){
     //copy command into a char array
    char * cmdstr = new char[command.length() + 1];
    strcpy(cmdstr, command.c_str());
    
    //initialize variable
    int c = 0;
    
    //grab first part of string
    char * p = strtok(cmdstr, ",");
    
    Wire.beginTransmission(addr);
    //loop through commands (',' delimited), using the first as the address and subsequent ones as commands
    while (p != NULL) {
        //copy command into int
        c = atoi(p);
        Wire.write(c);
        //grab next part of string
        p = strtok(NULL, ",");
    }
    
    //end i2c transmission
    int status = (int)Wire.endTransmission();
    delete cmdstr;
    
    return status;
}
int i2cDevices::processCommand(String command){
     //copy command into a char array
    char * cmdstr = new char[command.length() + 1];
    strcpy(cmdstr, command.c_str());
    
    //initialize variables
    bool init = FALSE;
    int c = 0;
    
    //grab first part of string
    char * p = strtok(cmdstr, ",");
    
    //loop through commands (',' delimited), using the first as the address and subsequent ones as commands
    while (p != NULL) {
        //copy command into int
        c = atoi(p);
        if (!init) {
            //is address
            init = TRUE;
            Wire.beginTransmission(c);
        } else {
            Wire.write(c);
        }
        //grab next part of string
        p = strtok(NULL, ",");
    }
    
    //end i2c transmission
    int status = (int)Wire.endTransmission();
    delete cmdstr;
    
    return status;
}
int i2cDevices::readI2cCommand(String command){
    int s = command.indexOf(',');
    int a = command.substring(0, s).toInt();
    String p = command.substring(s+1);
    s = p.indexOf(',');
    int r = p.substring(0, s).toInt();
    int c = p.substring(s+1).toInt();
    int buff[c];
    
    int status = read(a, r, c, buff);
    
    return status;
}
int i2cDevices::readI2cCommand(int addr, String command){
    int s = command.indexOf(',');
    s = command.indexOf(',');
    int r = command.substring(0, s).toInt();
    int c = command.substring(s+1).toInt();
    int buff[c];
    
    int status = read(addr, r, c, buff);
    
    return status;
}
int i2cDevices::readI2cCommand(String command, void (*fptr)(int*)){
    int s = command.indexOf(',');
    int a = command.substring(0, s).toInt();
    String p = command.substring(s+1);
    s = p.indexOf(',');
    int r = p.substring(0, s).toInt();
    int c = p.substring(s+1).toInt();
    int buff[c];
    
    int status = read(a, r, c, buff);
    
    fptr(buff);
    return status;
}
int i2cDevices::readI2cCommand(int addr, String command, void (*fptr)(int*)){
    int s = command.indexOf(',');
    s = command.indexOf(',');
    int r = command.substring(0, s).toInt();
    int c = command.substring(s+1).toInt();
    int buff[c];
    
    int status = read(addr, r, c, buff);
    
    fptr(buff);
    return status;
}
int i2cDevices::read(int addr, int registerAddress, int bytes, int* buff){
    Wire.beginTransmission(addr);
    Wire.write(registerAddress);
    int status = Wire.endTransmission();
    Wire.requestFrom(addr, bytes);
    int ind=0;
    while(ind<bytes){
        byte bv = Wire.read();
        buff[ind] = bv;
        ind++;
    }
    return status;
}

String i2cDevices::scan(){
    for(int i = 0; i < 127; i++){
        Wire.beginTransmission(i);
        int status = Wire.endTransmission();
        if(status==0){
            Log += "device found: ";
            Log += i;
            Log += " -- ";
        }
    }
    return "finished";
}





int indexOfNth(String str, char c, int ind){
    int count = 0;
    int pos = str.indexOf(c);
    if(pos<0) return pos;
    while (count < ind) {
        count++;
        pos = str.indexOf(c, pos);
        if(pos < 0) return pos;
    }
    return pos;
}
