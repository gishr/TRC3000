/*
 * mpu6050.h
 *
 *  Created on: 17 Jun 2019
 *      Author: disorder
 */

#ifndef INCLUDE_MPU6050_H_
#define INCLUDE_MPU6050_H_

#define ACCEL_BASE_ADDR 0x18

#define ACCEL_BASE_SENS 0x38

#define ACCEL_BASE_CTRL 0x10
#define ACCEL_BASE_CTRL_COMMAND 0x5D

#define ACCEL_XOUT_H  0x29
#define ACCEL_XOUT_L  0x28
#define ACCEL_YOUT_H  0x2B
#define ACCEL_YOUT_L  0x2A
#define ACCEL_ZOUT_H  0x2D
#define ACCEL_ZOUT_L  0x2C

#define TEMP_BASE_ADDR 0x41
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42

#define GYRO_BASE_ADDR 0x19

#define GYRO_BASE_SENS 0x3C

#define GYRO_BASE_CTRL 0x11
#define GYRO_BASE_CTRL_COMMAND 0x5E

#define GYRO_XOUT_H 0x23
#define GYRO_XOUT_L 0x22
#define GYRO_YOUT_H 0x25
#define GYRO_YOUT_L 0x24
#define GYRO_ZOUT_H 0x27
#define GYRO_ZOUT_L 0x26

#define XG_OFFS_H 0x13  //Gyro X-axis offset cancellation register high byte
#define XG_OFFS_L 0x14  //Gyro X-axis offset cancellation register low byte
#define YG_OFFS_H 0x15  //Gyro Y-axis offset cancellation register high byte
#define YG_OFFS_L 0x16  //Gyro Y-axis offset cancellation register low byte
#define ZG_OFFS_H 0x17  //Gyro Z-axis offset cancellation register high byte
#define ZG_OFFS_L 0x18  //Gyro Z-axis offset cancellation register low byte

#define XA_OFFS_H 0x06  // Accel X-axis offset cancellation register high byte
#define XA_OFFS_L 0x07  // Accel X-axis offset cancellation register low byte
#define YA_OFFS_H 0x08  // Accel Y-axis offset cancellation register high byte
#define YA_OFFS_L 0x09  // Accel Y-axis offset cancellation register low byte
#define ZA_OFFS_H 0x0A  // Accel Z-axis offset cancellation register high byte
#define ZA_OFFS_L 0x0B  // Accel Z-axis offset cancellation register low byte


#define WHO_AM_I     0x0F//0x75
#define PWR_MGMT_1   0x6B

#define ADDR_MPU6050 0x6B//0b01101000

#define REG_SIZE 8
#define CALIBRATION_READINGS 1000
#define GCONFIG      0x1A
#define GYRO_CONFIG  0x19
#define ACCEL_CONFIG 0x18
#define DDIV         0x19

#define UINT_H(x) ((x >> 8) & 0x00ff)
#define UINT_L(x) (x & 0x00ff)

typedef struct {
    unsigned int xout;
    unsigned int yout;
    unsigned int zout;
} accelerometer_t;

typedef struct {
    unsigned int xout;
    unsigned int yout;
    unsigned int zout;
} gyroscope_t;

void mpu6050_set_device_address(unsigned char);
unsigned char mpu6050_read_register(unsigned char);
void mpu6050_write_register(unsigned char, unsigned char);
void mpu6050_read_temperature(unsigned int *temperature);
void mpu6050_read_accelerometer(accelerometer_t *acc);
void mpu6050_read_gyroscope(gyroscope_t *acc);
void mpu6050_init();

void UsbInitialization();
void UsbExecution(accelerometer_t acc);

#endif /* INCLUDE_MPU6050_H_ */
