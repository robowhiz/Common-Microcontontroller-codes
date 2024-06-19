#include <Arduino.h>
#include <Wire.h>

static uint8_t gyro_address = 0x68;

uint32_t timer;

void getdata() {
  Wire.beginTransmission(gyro_address);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(gyro_address, 14);

  byte data[14] = {0};
  Wire.readBytes(data, 14);
}

void setup(){
  Serial.begin(9600);
  Wire.begin();
  delay(500);
  Wire.beginTransmission(gyro_address);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(100);
  Wire.beginTransmission(gyro_address);
  Wire.write(0x1B);
  Wire.write(0b00001000);
  Wire.endTransmission();
  delay(100);
  Wire.beginTransmission(gyro_address);
  Wire.write(0x1C);
  Wire.write(0b00001000);
  Wire.endTransmission();
  delay(100);
  Wire.beginTransmission(gyro_address);
  Wire.write(0x1A);
  Wire.write(0b00000110);
  Wire.endTransmission();
  delay(100);
  Wire.beginTransmission(gyro_address);
  Wire.write(0x19);
  Wire.write(0x00);
  Wire.endTransmission();
  Serial.println("hai");
}

void loop(){
  timer = micros();
  getdata();
  Serial.println(micros() - timer);
}