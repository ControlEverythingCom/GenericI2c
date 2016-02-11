#include "spark_wiring.h"
#include "spark_wiring_i2c.h"
#include "spark_wiring_constants.h"


int indexOfNth(String str, char c, int ind);

class i2cDevices{
public:
    i2cDevices(void);
    void init();
    bool addDevice(int address);
    bool addDevice(int address, String initCmds);
    bool deviceExists(int address);
    String getDevice(int address);
    String getDevice(int index, int &address);
    
    int sendCommand(String command);
    int sendCommand(int addr, String command);
    int sendCommands(String command);
    int sendCommands(int addr, String command);
    
    int processCommand(String command);
    int processCommand(int addr, String command);
    
    int readI2cCommand(String command);
    int readI2cCommand(int addr, String command);
    int readI2cCommand(String command, void (*fptr)(int*));
    int readI2cCommand(int addr, String command, void (*fptr)(int*));
    
    int read(int addr, int registerAddress, int bytes, int* buff);
    String updateLog();
    int initDevices();
    String scan();
private:
    bool initialized = FALSE;
    String addresses = "";
    String initCommands = "";
    String Log = "";
};
