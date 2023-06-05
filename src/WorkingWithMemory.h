#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>

void WriteFile(uint16_t *hArr, int key);
void ReadFile(uint16_t *hArray, int key);
void WriteBrightness(int value);
int ReadBrightness();
bool InitSPIFFS();
void WriteMassage(String value);
String ReadMassage();



