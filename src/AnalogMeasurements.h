#define NTS_SENSOR 35 
#define PHOTO_SENSOR 39 
#define BAT_LVL 34 
#define BUTTON_SELECT 0
#define WORK_LED 25
#define POWER_BAT 26

void InitPin();
float GetTemperature();
float GetBatLvl();
void BatarryMonitor();