#include <AnalogMeasurements.h>
#include <Arduino.h>

void InitPin()
{
    pinMode(NTS_SENSOR, INPUT);
    pinMode(BUTTON_SELECT, INPUT);
    pinMode(PHOTO_SENSOR, INPUT);
    pinMode(BAT_LVL, INPUT);
    pinMode(WORK_LED, OUTPUT);
    pinMode(POWER_BAT, OUTPUT);
}

float GetTemperature()
{
    float VRT = analogRead(NTS_SENSOR);
    VRT = (3.3 / 4095.00) * VRT;
    // VRT+=0.2;
    // Serial.println(VRT);

    float VR = 3.3 - VRT;
    float RT = VRT / (VR / 10000.0);
    float ln = log(RT / 10000.0);
    float TX = (1.0 / ((ln / 3950.0) + (1.0 / (25.0 + 273.15))));
    TX = TX - 273.15;

    return TX;
}

float GetBatLvl()
{
    float VRT = analogRead(BAT_LVL);
    VRT = (3.3 / 4095.00) * VRT;
    return VRT / (5100.0 / (10530.0 + 5100.0));
}

void BatarryMonitor()
{
    if (GetBatLvl() < 6.0)
    {
        // digitalWrite(WORK_LED, LOW);
        // digitalWrite(POWER_BAT, LOW);
    }
    else
    {
        digitalWrite(WORK_LED, HIGH);
        digitalWrite(POWER_BAT, HIGH);
    }
}