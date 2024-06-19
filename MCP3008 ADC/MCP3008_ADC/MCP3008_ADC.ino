#include <MCP3008.h>

#define CS_PIN 12
#define CLOCK_PIN 9
#define MOSI_PIN 11
#define MISO_PIN 10

MCP3008 adc(CLOCK_PIN, MOSI_PIN, MISO_PIN, CS_PIN);

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    int val = adc.readADC(0); // read Chanel 0 from MCP3008 ADC
    Serial.println(val);
    delay(1000);
}