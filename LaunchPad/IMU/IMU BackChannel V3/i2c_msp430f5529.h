/*
 * i2c_msp430f5529.h
 *
 *  Created on: 17 Jun 2019
 *      Author: disorder
 */

#ifndef INCLUDE_I2C_MSP430F5529_H_
#define INCLUDE_I2C_MSP430F5529_H_

unsigned char i2c_notready(void);
void i2c_master_receiver_mode_init(unsigned char);
void i2c_master_transmitter_mode_init(unsigned char);

void i2c_receive_data(unsigned int, unsigned char *);
void i2c_transmit_data(unsigned int, unsigned char*);

#endif /* INCLUDE_I2C_MSP430F5529_H_ */
