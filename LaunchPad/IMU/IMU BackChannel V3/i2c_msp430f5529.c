/*
 * i2c_msp430f5529.c
 *
 *  Created on: 17 Jun 2019
 *      Author: disorder
 */

#include <msp430.h>
#include "i2c_msp430f5529.h"

static unsigned char *RXData = 0;
static unsigned char *TXData = 0;
static unsigned int TXByteCtr = 0;
static unsigned int RXByteCtr = 0;

unsigned char i2c_notready(void) {
  return (UCB0STAT & UCBBUSY);
}

void i2c_master_receiver_mode_init(unsigned char addr){
    P3REN |= BIT0 | BIT1;
    P3SEL |= BIT0 | BIT1;                            // Assign I2C pins to USCI_B0
    UCB0CTL1 |= UCSWRST;                      // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
    UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
    UCB0BR1 = 0;
    UCB0I2CSA = addr;                         // Slave Address is addr
    UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
    UCB0IE |= UCRXIE + UCSTPIE + UCSTTIE;                         // Enable RX interrupt
}

void i2c_master_transmitter_mode_init(unsigned char addr) {
    P3REN |= BIT0 | BIT1;
    P3SEL |= BIT0 | BIT1;                            // Assign I2C pins to USCI_B0
    UCB0CTL1 |= UCSWRST;                      // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, Transmitter mode, keep SW reset
    UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
    UCB0BR1 = 0;
    UCB0I2CSA = addr;                         // Slave Address is 048h
    UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
    UCB0IE |= UCTXIE + UCSTPIE + UCSTTIE;                          // Enable TX interrupt
}

void i2c_receive_data(unsigned int size, unsigned char *buffer) {
    RXData = buffer;
    RXByteCtr = size;
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    UCB0CTL1 &= ~UCTR;                      // i2c receiver mode
    UCB0CTL1 |= UCTXSTT;                    // I2C start condition
    while (UCB0CTL1 & UCTXSTT);             // slave acknowledges the address
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
}

void i2c_transmit_data(unsigned int size, unsigned char *buffer) {
    TXData = buffer;
    TXByteCtr = size;
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    UCB0CTL1 |= UCTR;                       // i2c transmit
    UCB0CTL1 |= UCTXSTT;                    // start condition
    while (UCB0CTL1 & UCTXSTT);             // slave acknowledges the address
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    //UCB0IFG
    //UCRXIFG
}

// USCI_B0 Data ISR
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCB0IV,12))
  {
  case  0: break;                               // Vector  0: No interrupts
  case  2: break;                               // Vector  2: ALIFG
  case  4: break;                               // Vector  4: NACKIFG
  case  6:                                      // Vector  6: STTIFG
      UCB0IFG &= ~UCSTTIFG;                     // Clear start condition int flag
      break;
  case  8:                                      // Vector  8: STPIFG
      UCB0IFG &= ~UCSTPIFG;                     // Clear stop condition int flag
      break;
  case 10:                                      // Vector 10: RXIFG
      // Chec Rx byte Counter
      if (RXByteCtr) {
          *RXData++ = UCB0RXBUF;                // Move RX data to address PRxData
          RXByteCtr--;                          // Decrement RX byte counter
      } else {
          UCB0CTL1 |= UCTXSTP;                  // Generate I2C stop condition
          UCB0IFG &= ~UCRXIFG;                  // Clear USCI_B0 TX int flag
      }
      break;
  case 12:                                      // Vector 12: TXIFG
      // Check TX byte Counter
      if (TXByteCtr) {
          UCB0TXBUF = *TXData++;                   // Load TX buffer
          TXByteCtr--;                          // Decrement TX byte counter
      } else {
          UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
          UCB0IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
        //__bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
      }
      break;
  default:
      break;
  }
}
