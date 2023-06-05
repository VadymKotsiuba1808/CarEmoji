#include <Arduino.h>
#include <BluetoothSerial.h>

struct ReceivedData
{
    
    bool ifDrawString = false;
    String command = "";
    String crippingLine = "";
    uint16_t bitmap[1024] ;
    int numberBitmap = 0;
    int brightness = 50;
};

bool Init_BT(String BT_Name);
void GetReceivedDataBT(ReceivedData* data);
void SendCurrent(uint16_t* bitmap);
