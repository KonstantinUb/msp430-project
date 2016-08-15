#include "project.h"

const uint8_t seg_table[] = {
	0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B
};

void init_TUI(void)
{
	/* 7-segmentni displeji */
	SET_1(P6DIR, ~BIT7);		// Aktivira sve segmente
	SET_1(P11DIR, BIT1 | BIT0);	// Selektuje displeje 1 i 2
	SET_1(P10DIR, BIT7 | BIT6);	// Selektuje displeje 3 i 4

	/* Tasteri */
	SET_1(P2IE,  BIT4 | BIT5);	// Dozvola prekida za tastere S1 i S2
	SET_1(P2IES, BIT4 | BIT5);	// Opadajuca ivica aktivira prekid

	/* Diode */
	SET_1(P4DIR, BIT3 | BIT4);	// Selektuje diode LD1 i LD2
	SET_1(P4SEL, BIT3 | BIT4);	// Dodeljuje funkcije dioda periferiji

//	init_LCD();
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void) {
	if (P2IFG & BIT4) {			// Taster S1
		if (++adc_freq > ADC_FREQ_MAX) {
			adc_freq = ADC_FREQ_MIN;
		}
		RESET_1(P2IFG, BIT4);	// Brisanje flega za prekid
	} else if (P2IFG & BIT5) {	// Taster S2
		if (--adc_freq < ADC_FREQ_MIN) {
			adc_freq = ADC_FREQ_MAX;
		}
		RESET_1(P2IFG, BIT5);	// Brisanje flega za prekid
	}

	TA0CCR0 = SMCLK_TO_CCR(adc_freq);
};
