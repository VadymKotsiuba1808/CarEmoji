#include <ReceivingPacketsBT.h>

BluetoothSerial SerialBT;
bool Init_BT(String BT_Name)
{
    
    return SerialBT.begin(BT_Name);
    
}

void GetReceivedDataBT(ReceivedData *dataStruct)
{
    // static ReceivedData dataStruct;

    if (SerialBT.available() > 0)
    {
        String command = "";
        Serial.println("START get data");
        
        
        uint8_t firstBufer[2500] = {0};
        
        static int comandArrSize = 2;
        static int dataArrSize = 1024;
        int resultreadBytes = SerialBT.readBytes(firstBufer, 2500);
        uint8_t data[resultreadBytes] = {0};
        Serial.println("END get data");
        for (size_t i = 0; i < resultreadBytes; i++)
        {
            if ((char)firstBufer[i] == '\n')
            {
                comandArrSize = i - 1;
                dataArrSize = (((resultreadBytes - 1) - i) / 2);
            }
            data[i] = firstBufer[i];
        }

        log_e("comandArrSize is %i dataArrSize is %i", comandArrSize, dataArrSize);
        for (size_t i = 0; i < comandArrSize; i++)
        {
            command.concat((char)firstBufer[i]);
        }

        if (command.indexOf(' ') != -1)
        {
            if ((command.substring(0, command.indexOf(' '))) == "WRITE_LINE")
            {
                dataStruct->crippingLine = (command.substring(command.indexOf(' ') + 1));
                dataStruct->command = command.substring(0, command.indexOf(' '));
            }
            if ((command.substring(0, command.indexOf(' '))) == "SET_BRIGHTNESS")
            {
                dataStruct->brightness = (command.substring(command.indexOf(' ') + 1)).toInt();
                dataStruct->command = command.substring(0, command.indexOf(' '));
            }
            else
            {
                dataStruct->numberBitmap = (command.substring(command.indexOf(' ') + 1)).toInt();
                dataStruct->command = command.substring(0, command.indexOf(' '));
            }
        }
        else
        {
            dataStruct->command = command;
        }

        for (int i = resultreadBytes - 1, j = dataArrSize - 1; j >= 0; j--, i -= 2)
        {
            dataStruct->bitmap[j] = ((data[i - 1] << 8) | data[i]);
        }

        Serial.print("Command_is ");
        Serial.println(dataStruct->command);
        Serial.print("Brightness_is ");
        Serial.println(dataStruct->brightness);
        Serial.print("Cripping_line_is ");
        Serial.println(dataStruct->crippingLine);
        Serial.print("Is_drawing_string_is ");
        Serial.println(dataStruct->ifDrawString);
        Serial.print("Nubber_Bitmap is ");
        Serial.println(dataStruct->numberBitmap);
        dataStruct->ifDrawString = false;
    }

    // return dataStruct;
}



void SendCurrent(uint16_t *bitmap)
{
    uint8_t sendArr[2048] = {0};
    for (float i = 0, j = 0; j < 2048; j += 1, i += 0.5)
    {
        if ((int)j % 2 == 0)
        {
            sendArr[(int)j] = (bitmap[(int)i] >> 8);
        }
        else
        {
            sendArr[(int)j] = (bitmap[(int)i]);
        }
    }
    /////////////////////TEST///////////

    // Serial.println("*************************************Send to phone*****************************************************");
    // Serial.write(sendArr, 2048);
    // Serial.println();

    ///////////////////////////////////
    SerialBT.write(sendArr, 2048);
}