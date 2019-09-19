
#include "mpu6050.h"
#include "i2c_msp430f5529.h"

#include "msp430F5529.h"

#include "driverlib/MSP430F5xx_6xx/wdt_a.h"
#include "driverlib/MSP430F5xx_6xx/ucs.h"
#include "driverlib/MSP430F5xx_6xx/pmm.h"
#include "driverlib/MSP430F5xx_6xx/sfr.h"

// USB API #includes
#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/types.h"
#include "USB_API/USB_Common/usb.h"

#include "USB_app/usbConstructs.h"

// Application #includes
#include "BCUart.h"           // Include the backchannel UART "library"
#include "hal.h"              // Modify hal.h to select your hardware

#include <stdio.h>
#include <stdlib.h>

static unsigned char tx_buffer[8];
static unsigned char rx_buffer[8];

static unsigned char device_addr = ADDR_MPU6050;

void mpu6050_set_device_address(unsigned char addr) {
    device_addr = addr;
}

unsigned char mpu6050_read_register(unsigned char reg_addr) {
    tx_buffer[0] = reg_addr;
    i2c_master_transmitter_mode_init(device_addr);
    while (i2c_notready());
    i2c_transmit_data(1,tx_buffer);
    while (i2c_notready());
    i2c_master_receiver_mode_init(device_addr);
    while (i2c_notready());
    i2c_receive_data(1,rx_buffer);
    while (i2c_notready());
    return rx_buffer[0];
}

void mpu6050_write_register(unsigned char addr, unsigned char data) {
    // write pwr mgmt reg
    tx_buffer[0] = addr;
    tx_buffer[1] = data;
    i2c_master_transmitter_mode_init(device_addr);
    while (i2c_notready());
    i2c_transmit_data(2,tx_buffer);
    while (i2c_notready());
}

void mpu6050_read_accelerometer(accelerometer_t *acc) {
    unsigned int rh = 0, rl = 0;
    rh = mpu6050_read_register(ACCEL_XOUT_H);
    rl = mpu6050_read_register(ACCEL_XOUT_L);
    acc->xout = ((rh << REG_SIZE) | rl);
    rh = mpu6050_read_register(ACCEL_YOUT_H);
    rl = mpu6050_read_register(ACCEL_YOUT_L);
    acc->yout = ((rh << REG_SIZE) | rl);
    rh = mpu6050_read_register(ACCEL_ZOUT_H);
    rl = mpu6050_read_register(ACCEL_ZOUT_L);
    acc->zout = ((rh << REG_SIZE) | rl);
}

void mpu6050_read_gyroscope(gyroscope_t *acc) {
    unsigned int rh = 0, rl = 0;
    rh = mpu6050_read_register(GYRO_XOUT_H);
    rl = mpu6050_read_register(GYRO_XOUT_L);
    acc->xout = ((rh << REG_SIZE) | rl);
    rh = mpu6050_read_register(GYRO_YOUT_H);
    rl = mpu6050_read_register(GYRO_YOUT_L);
    acc->yout = ((rh << REG_SIZE) | rl);
    rh = mpu6050_read_register(GYRO_ZOUT_H);
    rl = mpu6050_read_register(GYRO_ZOUT_L);
    acc->zout = ((rh << REG_SIZE) | rl);
}

void mpu6050_init() {
    unsigned char ret;
    // set device address
    mpu6050_set_device_address(ADDR_MPU6050);
    // read who am i register
    ret = mpu6050_read_register(WHO_AM_I);

    // gyro xyz initialization
    mpu6050_write_register(GYRO_CONFIG, GYRO_BASE_SENS);
    // accel xyz initialization
    mpu6050_write_register(ACCEL_CONFIG, ACCEL_BASE_SENS);

    // gyro sensitivity CTRL2 address 0x11
    mpu6050_write_register(GYRO_BASE_CTRL,GYRO_BASE_CTRL_COMMAND);
    // accel sensitivity CTRL1 address 0x10
    mpu6050_write_register(ACCEL_BASE_CTRL,ACCEL_BASE_CTRL_COMMAND);
}

char buf_usbToBcuart[128];

void UsbInitialization()
{

    PMM_setVCore(PMM_CORE_LEVEL_3);

    initPorts();           // Config all the GPIOS for low-power (output low)

    initClocks(1100000);   // Config clocks. MCLK=SMCLK=FLL=8MHz; ACLK=REFO=32kHz 8000000


    bcUartInit();          // Init the back-channel UART
    USB_setup(TRUE,TRUE);  // Init USB; if a USB host (PC) is present, connect
    __enable_interrupt();  // Enable interrupts globally

}

void UsbExecution(accelerometer_t acc)
{
    __delay_cycles(10000); //10000 1000000
    sprintf(buf_usbToBcuart,"\r\n( %d,%d,%d )",acc.xout,acc.yout,acc.zout);
    bcUartSend(buf_usbToBcuart,11);
}
