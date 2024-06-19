#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS PB4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
  sensors.setResolution(12);
}

void loop(void)
{
  sensors.requestTemperatures();

  Serial.print("Celsius temperature: 1: ");
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.print("  2: ");
  Serial.println(sensors.getTempCByIndex(1));
  delay(1000);
}