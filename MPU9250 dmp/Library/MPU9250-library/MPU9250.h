#ifndef MPU9250_H
#define MPU9250_H

#include <Arduino.h>

// MPU9250 Register
#define MPU9250_ADDRESS 0x68  // Device address when ADO = 0
#define WHO_AM_I_MPU9250 0x75 // Should return 0x71
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_CONFIG2 0x1D
#define USER_CTRL 0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define PWR_MGMT_1 0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_2 0x6C
#define FIFO_COUNTH 0x72
#define FIFO_R_W 0x74

#define ACCEL_XOUT_H 0x3B
#define TEMP_OUT_H 0x41
#define GYRO_XOUT_H 0x43

// Offset Registers
#define XA_OFFSET_H 0x77
#define XA_OFFSET_L 0x78
#define YA_OFFSET_H 0x7A
#define YA_OFFSET_L 0x7B
#define ZA_OFFSET_H 0x7D
#define ZA_OFFSET_L 0x7E

#define XG_OFFSET_H 0x13 // User-defined trim values for gyroscope
#define XG_OFFSET_L 0x14
#define YG_OFFSET_H 0x15
#define YG_OFFSET_L 0x16
#define ZG_OFFSET_H 0x17
#define ZG_OFFSET_L 0x18

#define FIFO_EN 0x23
#define I2C_MST_CTRL 0x24
#define INT_PIN_CFG 0x37
#define INT_ENABLE 0x38
#define INT_STATUS 0x3A

// Selftest Registers
#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02

#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F

// Magnetometer Registers
#define AK8963_ADDRESS 0x0C
#define WHO_AM_I_AK8963 0x00 // should return 0x48
#define AK8963_ST1 0x02      // data ready status bit 0
#define AK8963_XOUT_L 0x03   // data
#define AK8963_CNTL 0x0A     // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define AK8963_ASAX 0x10     // Fuse ROM x-axis sensitivity adjustment value

#define Kp 2.0f * 5.0f // these are the free parameters in the Mahony filter and fusion scheme, Kp for proportional feedback, Ki for integral
#define Ki 0.0f

// Set initial input parameters
enum Ascale
{
    AFS_2G = 0,
    AFS_4G,
    AFS_8G,
    AFS_16G
};

enum Gscale
{
    GFS_250DPS = 0,
    GFS_500DPS,
    GFS_1000DPS,
    GFS_2000DPS
};

enum Mscale
{
    MFS_14BITS = 0, // 0.6 mG per LSB
    MFS_16BITS      // 0.15 mG per LSB
};

#define MAG_8HZ 0x02;
#define MAG_100HZ 0x06;

class MPU9250
{
public:
    MPU9250();
    bool begin();
    void MPU9250SelfTest(float *destination);
    void calibrateMPU9250(float *dest1, float *dest2);
    void MahonyQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz, float *q, float *deltat);
    void initMPU9250();
    void initAK8963(float *destination);
    void setGScale(uint8_t Gscale);
    void setAScale(uint8_t Ascale);
    void setMScale(uint8_t Mscale);
    void setMmode(uint8_t mode);
    void readAccelData(int16_t *destination);
    void readGyroData(int16_t *destination);
    void readMagData(int16_t *destination);
    float getGres();
    float getAres();
    float getMres();
    uint8_t dataReady();
    int16_t readTempData();

private:
    void writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
    uint8_t readByte(uint8_t address, uint8_t subAddress);
    void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t *dest);

    uint8_t Gscale;
    uint8_t Ascale;
    uint8_t Mscale;
    uint8_t Mmode = 0x02;
    float eInt[3] = {0.0f, 0.0f, 0.0f}; // vector to hold integral error for Mahony method
};

#endif