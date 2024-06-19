#include <Arduino.h>
#include <MPU9250.h>
#include <Wire.h>

MPU9250 mpu;
void get_Orientation();
// Specify sensor full scale
float aRes, gRes, mRes; // scale resolutions per LSB for the sensors

// Pin definitions
int intPin = PB5; // These can be changed, 2 and 3 are the Arduinos ext int pins

int16_t accelCount[3];                                       // Stores the 16-bit signed accelerometer sensor output
int16_t gyroCount[3];                                        // Stores the 16-bit signed gyro sensor output
int16_t magCount[3];                                         // Stores the 16-bit signed magnetometer sensor output
float magCalibration[3] = {0, 0, 0}, magbias[3] = {0, 0, 0}; // Factory mag calibration and mag bias
float gyroBias[3] = {0, 0, 0}, accelBias[3] = {0, 0, 0};     // Bias corrections for gyro and accelerometer
int16_t tempCount;                                           // temperature raw count output
float temperature;                                           // Stores the real internal chip temperature in degrees Celsius
float SelfTest[6];                                           // holds results of gyro and accelerometer self test

uint32_t count = 0, sumCount = 0; // used to control display output rate
float pitch, yaw, roll;
float deltat = 0.0f, sum = 0.0f;          // integration interval for both filter schemes
uint32_t lastUpdate = 0, firstUpdate = 0; // used to calculate integration interval
uint32_t Now = 0;                         // used to calculate integration interval

float ax, ay, az, gx, gy, gz, mx, my, mz; // variables to hold latest sensor data values
float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};    // vector to hold quaternion

void setup()
{
    Wire.begin();
    pinMode(intPin, INPUT);
    digitalWrite(intPin, LOW);
    delay(1000);

    if (mpu.begin()) // WHO_AM_I should always be 0x68
    {
        Serial.println("MPU9250 is online...");
        mpu.setGScale(GFS_250DPS);
        mpu.setAScale(AFS_2G);
        mpu.MPU9250SelfTest(SelfTest);

        mpu.calibrateMPU9250(gyroBias, accelBias); // Calibrate gyro and accelerometers, load biases in bias registers

        delay(1000);

        mpu.initMPU9250();
        Serial.println("MPU9250 initialized for active data mode...."); // Initialize device for active mode read of acclerometer, gyroscope, and temperature

        // Get magnetometer calibration from AK8963 ROM
        mpu.setMScale(MFS_16BITS);
        mpu.initAK8963(magCalibration);
        Serial.println("AK8963 initialized for active data mode...."); // Initialize device for active mode read of magnetometer

        delay(1000);
    }
    else
    {
        Serial.print("Could not connect to MPU9250: 0x");
        while (1)
            ; // Loop forever if communication doesn't happen
    }
    aRes = mpu.getAres();
    gRes = mpu.getGres();
    mRes = mpu.getMres();
}

void loop()
{
    get_Orientation();
}

void get_Orientation()
{
    // If intPin goes high, all data registers have new data
    if (mpu.dataReady())
    {                                  // On interrupt, check if data ready interrupt
        mpu.readAccelData(accelCount); // Read the x/y/z adc values

        // Now we'll calculate the accleration value into actual g's
        ax = (float)accelCount[0] * aRes; // - accelBias[0];  // get actual g value, this depends on scale being set
        ay = (float)accelCount[1] * aRes; // - accelBias[1];
        az = (float)accelCount[2] * aRes; // - accelBias[2];

        mpu.readGyroData(gyroCount); // Read the x/y/z adc values

        // Calculate the gyro value into actual degrees per second
        gx = (float)gyroCount[0] * gRes; // get actual gyro value, this depends on scale being set
        gy = (float)gyroCount[1] * gRes;
        gz = (float)gyroCount[2] * gRes;

        mpu.readMagData(magCount); // Read the x/y/z adc values
        magbias[0] = +470.;        // User environmental x-axis correction in milliGauss, should be automatically calculated
        magbias[1] = +120.;        // User environmental x-axis correction in milliGauss
        magbias[2] = +125.;        // User environmental x-axis correction in milliGauss

        // Calculate the magnetometer values in milliGauss
        // Include factory calibration per data sheet and user environmental corrections
        mx = (float)magCount[0] * mRes * magCalibration[0] - magbias[0]; // get actual magnetometer value, this depends on scale being set
        my = (float)magCount[1] * mRes * magCalibration[1] - magbias[1];
        mz = (float)magCount[2] * mRes * magCalibration[2] - magbias[2];
    }

    Now = micros();
    deltat = ((Now - lastUpdate) / 1000000.0f); // set integration time by time elapsed since last filter update
    lastUpdate = Now;

    sum += deltat; // sum for averaging filter update rate
    sumCount++;

    mpu.MahonyQuaternionUpdate(ax, ay, az, gx * PI / 180.0f, gy * PI / 180.0f, gz * PI / 180.0f, my, mx, mz, q, &deltat);

    // Serial print and/or display at 0.5 s rate independent of data rates
    yaw = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]), q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);
    pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
    roll = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]), q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
    pitch *= 180.0f / PI;
    yaw *= 180.0f / PI;
    yaw -= 13.8; // Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04
    roll *= 180.0f / PI;

    Serial.printf("Yaw %f, Pitch %f, Roll %f\n", yaw, pitch, roll);

    count = millis();
    sumCount = 0;
    sum = 0;
}