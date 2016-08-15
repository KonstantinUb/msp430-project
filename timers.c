#include "project.h"

uint8_t adc_freq = ADC_FREQ_MIN;

void init_timers(void)
{
	SET_1(UCSCTL5, DIVS2);			// SMCLK /= 16 -> 65kHz

	/* Tajmer TA0 (taktovanje ADC) */
	TA0CTL   = TASSEL_2 | MC_1;		// Izvor: SMCLK, mod: up
	TA0CCR0  = SMCLK_TO_CCR(adc_freq);
	TA0CCTL0 = CCIE;				// Dozvola prekida CCR0

	/* Tajmer TA1 (osvezavanje displeja) */
	TA1CTL   = TASSEL_1 | MC_1;		// Izvor: ACLK, mod: up
	TA1CCR0  = ACLK_TO_CCR(DISP_FREQ);
	TA1CCTL0 = CCIE;				// Dozvola prekida CCR0

	/* Tajmer TB0 (PWM za diode) */
	TB0CTL   = TBSSEL_1 | MC_3;		// Izvor: ACLK, mod: up-down 
	TB0CCR0  = DATA_MAX;			// PWM period
	TB0CCTL3 = TB0CCTL4 = OUTMOD_2;	// CCR3 (LD1) i CCR4 (LD2) toggle/reset
	TB0CCR3  = TB0CCR4  = 0;		// Pocetni PWM duty cycle
}

#pragma vector = TIMER0_A0_VECTOR	// TA0 ISR
__interrupt void TIMER0_A0_ISR(void)
{
	SET_1(ADC12CTL0, ADC12SC);
}

#pragma vector = TIMER1_A0_VECTOR	// TA1 ISR
__interrupt void TIMER1_A0_ISR(void)
{
	static uint8_t index = 0;

	if (index % 2 == 0 && data[index] == 0) {	// Cifra desetica je 0
		++index;
		return;
	}

	RESET_0(P11OUT, BIT1 | BIT0);	// Iskljucuje displeje 1 i 2
	RESET_0(P10OUT, BIT7 | BIT6);	// Iskljucuje displeje 3 i 4
	P6OUT = seg_table[data[index]];	// Ispisuje cifru

	switch (++index) {
	case 1:
		SET_0(P11OUT, BIT1);	// SEL1
		break;
	case 2:
		SET_0(P11OUT, BIT0);	// SEL2
		break;
	case 3:
		SET_0(P10OUT, BIT7);	// SEL3
		break;
	case 4:
		SET_0(P10OUT, BIT6);	// SEL4
		index = 0;
		break;
	}
}
