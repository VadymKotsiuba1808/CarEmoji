#include <Arduino.h>
#include <BitmapProcessing.h>
#include <ReceivingPacketsBT.h>
#include <AnalogMeasurements.h>
#include <WorkingWithMemory.h>

ReceivedData dataStruct;
unsigned long britnesTimer = 0;
int counter;
bool flag = true;
bool startLine = false;
int buutFlag = 0;
int oldBrigtness = 0;
uint16_t Bitmap[1024] = {0};
unsigned long debounceTimer = 0, timerBatary = 0;
int TestBatCounter = 0;
uint16_t BitmapBatary[1024];
bool LogFlag = false;

extern uint16_t DefaultBitmap[1024];

void setup()
{

  Serial.begin(115200);
  if (!Init_BT("ESP32MatrixPic"))
  {
    Serial.print("Bluetooth init fail... ");
  }
  if (!InitSPIFFS())
  {
    Serial.print("SPIFFS mount fail... ");
  }
  InitPin();
  dataStruct.brightness = oldBrigtness = ReadBrightness();
  log_e("oldBrigtness value is %i", oldBrigtness);
  Init_matrix(oldBrigtness);                       

}

void loop()
{
  // Serial.print("Start loop ");
  GetReceivedDataBT(&dataStruct);
  BatarryMonitor();
  // Serial.print("point 1 ");

  if (dataStruct.brightness != oldBrigtness) // change brightness
  {
    log_e("Set new brightness %i old britnes %i", dataStruct.brightness, oldBrigtness);
    WriteBrightness(dataStruct.brightness);
    SetBrightness(dataStruct.brightness);
    oldBrigtness = dataStruct.brightness;
    // Serial.print("point 2 ");
  }
  // Serial.print("point 3 ");

  if (startLine && dataStruct.crippingLine != "") // draw crippingline
  {
    int arrSize = dataStruct.crippingLine.length() + 1;
    static int i = 0, X = 32;
    static String str = dataStruct.crippingLine;
    char charArr[arrSize];
    dataStruct.crippingLine.toCharArray(charArr, arrSize);
    if (dataStruct.crippingLine.length() > 4)
    {
      if (str != dataStruct.crippingLine)
      {
        str = dataStruct.crippingLine;
        i = 0;
        X = 32;
      }
      if (i < (dataStruct.crippingLine.length() * 8) + 32)
      {
        CreepingLine(X, 9, charArr);
        delay(5);
        i++;
        X--;
      }
      else
      {
        i = 0;
        X = 32;
      }
    }
    else
    {
      CreepingLine(0, 9, charArr);
    }
  }
  // Serial.print("point 4 ");

  if (!dataStruct.ifDrawString) // processing command
  {
    dataStruct.ifDrawString = true;

    if (dataStruct.command == "WRITE_CURRENT")
    {
      log_e("WRITE_CURRENT");
      startLine = false;
      dataStruct.crippingLine = "";
      DrawBitmap(dataStruct.bitmap);
      WriteFile(dataStruct.bitmap, 5);
    }

    if (dataStruct.command == "WRITE_LINE")
    {
      log_e("WRITE_LINE");
      startLine = true;
    }

    if (dataStruct.command == "READ_CURRENT")
    {
      log_e("READ_CURRENT");
      ReadFile(Bitmap, 5);
      SendCurrent(Bitmap);
    }

    if (dataStruct.command == "WRITE_FAV")
    {
      startLine = false;
      dataStruct.crippingLine = "";
      log_e("WRITE_FAV %i", dataStruct.numberBitmap);
      WriteFile(dataStruct.bitmap, dataStruct.numberBitmap);
      DrawBitmap(dataStruct.bitmap);
    }

    if (dataStruct.command == "READ_FAV")
    {
      log_e("READ_FAV %i", dataStruct.numberBitmap);
      ReadFile(Bitmap, dataStruct.numberBitmap);
      SendCurrent(Bitmap);
    }
  }
  // Serial.print("point 5 ");
  if (dataStruct.command == "" && flag)
  {
    log_e("Draw LastBitmap");
    ReadFile(Bitmap, 5);
    DrawBitmap(Bitmap);
    flag = false;
  }
  // Serial.print("point 6 ");
  // buuton procesing
  if (digitalRead(0))
  {
    debounceTimer = millis();
  }

  if (!digitalRead(0) && buutFlag == 0 && millis() - debounceTimer > 5)
  {
    buutFlag = 1;
  }
  //  Serial.print("point 7 ");
  if (digitalRead(0) && buutFlag == 1)
  {
    startLine = false;
    log_e("Button pressed, Draw Bitmap num: %i", counter);
    Serial.print("Bat_Lvl_is: ");
    Serial.println(GetBatLvl());
    Serial.print("Temperature_is: ");
    Serial.println(GetTemperature());
    ReadFile(Bitmap, counter);
    DrawBitmap(Bitmap);
    counter = counter >= 4 ? 0 : counter + 1;
    buutFlag = 0;
    
  }
  // Serial.print("point 8 ");
  /////Battery Test/////////////////////////////////////////////////////////////////////////////////

  // if (Serial.available() > 0)
  // {
  //   char data = (char)Serial.read();
  //   if (data == '0')
  //   {
  //     Serial.println("Batary log paused...");
  //     LogFlag = false;
  //   }
  //   if (data == '1')
  //   {
  //     Serial.println("Start Batary log...");
  //     LogFlag = true;
  //   }
  // }

  // if (LogFlag)
  // {

  //   if (millis() - timerBatary > 300000 || TestBatCounter == 0)
  //   {
  //     log_e("Time is: %imin Bat lvl is: %1.2fv", (TestBatCounter * 5), GetBatLvl());
  //     timerBatary = millis();
  //     TestBatCounter++;
  //   }
  // }
}
