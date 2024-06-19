#include <Wire.h>
#include <MCP4725.h>

MCP4725 MCP1(0x62);
MCP4725 MCP2(0x61);

void setup(){
    Serial.begin(9600);

    Wire.begin();
    
    Serial.println(MCP1.begin());
    Serial.println(MCP2.begin());
    MCP1.setMaxVoltage(5.1);
    MCP2.setMaxVoltage(5.1);
}

void loop(){
    MCP1.setValue(4095);
    MCP2.setValue(4095);
    delay(1000);
    MCP1.setValue(2048);
    MCP2.setValue(2048);
}