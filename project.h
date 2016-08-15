#ifndef PROJECT_H
#define PROJECT_H

#include <stdbool.h>
#include <stdint.h>
#include <msp430.h>

#define SET_1(reg, bit)		((reg) |= (bit))	// Aktivna vrednost 1
#define RESET_1(reg, bit)	((reg) &= ~(bit))
#define SET_0(reg, bit)		RESET_1(reg, bit)	// Aktivna vrednost 0
#define RESET_0(reg, bit)	SET_1(reg, bit)

#define ACLK_FREQ			(1U << 15)	// 32kHz
#define SMCLK_FREQ			(1UL << 16)	// 65kHz
#define ACLK_TO_CCR(hz)		(ACLK_FREQ / (hz) - 1)
#define SMCLK_TO_CCR(hz)	(SMCLK_FREQ / (hz) - 1)

#define DISP_FREQ			300
#define BAUD_RATE			9600
#define DATA_MAX			99
#define ADC_FREQ_MIN		1
#define ADC_FREQ_MAX		10

extern const uint8_t seg_table[10];
extern uint8_t data[4];
extern uint8_t adc_freq;

void init_TUI(void);
void init_UART(void);
void send(void);
void init_ADC(void);
bool convert(void);
void init_timers(void);

__interrupt void PORT2_ISR(void);
__interrupt void USCI_A0_ISR(void);
__interrupt void ADC12_ISR(void);
__interrupt void TIMER0_A0_ISR(void);
__interrupt void TIMER1_A0_ISR(void);

#endif
