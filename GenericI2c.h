#include "spark_wiring.h"
#include "spark_wiring_i2c.h"
#include "spark_wiring_constants.h"
#include "spark_wiring_usbserial.h"

int indexOfNth(String str, char c, int ind);

class i2cDevices{
public:
    void init();
    bool addDevice(int address);
    bool addDevice(int address, String initCmds);
    bool deviceExists(int address);
    String getDevice(int address);
    String getDevice(int index, int &address);
    int sendCommand(String command);
    int sendCommands(String command);
    int processCommand(String command);
    int readI2cCommand(String command);
    int readI2cCommand(String command, void (*fptr)(int*));
    int read(int addr, int registerAddress, int bytes, int* buff);
    String updateLog();
    int initDevices();
    String scan();
private:
    bool initialized = FALSE;
    String addresses = "32 72";
    String initCommands = "32,0,252|32,6,252 72,1,96";
    String Log = "";
};
