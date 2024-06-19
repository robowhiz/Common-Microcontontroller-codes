#include <Arduino.h>
#include <BMP280.h>

BMP280 bmp;
void get_pressure_and_internal_temparature();

void setup()
{
    Serial.begin(115200);

    bmp.begin();
    bmp.setOversampling(4);
}

void loop()
{
    get_pressure_and_internal_temparature();
    delay(1000);
}

void get_pressure_and_internal_temparature()
{
    double T, P;
    char result = bmp.startMeasurment();

    if (result != 0)
    {
        delay(result);
        result = bmp.getTemperatureAndPressure(T, P);

        if (result != 0)
        {
            Serial.printf("%f C, %f F\n", T, P);
        }
        else
        {
            Serial.println("Error reading the data");
        }
    }
    else
    {
        Serial.println("Error starting measurement");
    }
}