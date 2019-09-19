#include <msp430.h>
#include "mpu6050.h"

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

    P3REN |= BIT0 | BIT1;
    P3SEL |= BIT0 | BIT1;

    // Structure Definition (Empty)
    accelerometer_t acc_t;
    gyroscope_t gyro_t;

    __bis_SR_register(GIE);

    // IMU initialization
    mpu6050_init();

    while (1) {
       // get accelerometer data
       mpu6050_read_accelerometer(&acc_t);
       // get gyroscope data
       mpu6050_read_gyroscope(&gyro_t);
    }
}
