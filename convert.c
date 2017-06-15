#include "project.h"

#define SCALE_ADC_RES(reg)		(reg * (DATA_MAX+1L) >> 12)	// reg * 100 / 4096

uint8_t data[4] = { 0 };
static bool completed = false;

void init_ADC(void)
{
	SET_1(P7SEL, BIT6 | BIT7);		// Bira A/D kanale A14 (pot1) i A15 (pot2)

	/* Osobine A/D konvertora */
	ADC12CTL0 = ADC12ON;			// Pali ADC
	SET_1(ADC12CTL0, ADC12SHT0_15);	// Produzeno vreme trajanja uzorka
	SET_1(ADC12CTL0, ADC12MSC);		// Vise konverzija u sekvenci

	/* Mod konverzije (http://bit.ly/adc12-modes) */
	ADC12CTL1 = ADC12CONSEQ_1;		// Sequence-of-channels
	SET_1(ADC12CTL1, ADC12SHP);		// Tajmer upravlja prelaz sa uzorka na konv.
//	SET_1(ADC12CTL1, ADC12SHS_1);	// Izvor prelaza: TA0 - odradjeno softverski

	/* Pamcenje rezultata */
	ADC12MCTL0 = ADC12INCH_14;				// Rez. A14 u MEM0
	ADC12MCTL1 = ADC12INCH_15 | ADC12EOS;	// Rez. A15 u MEM1, kraj sekvence

	SET_1(ADC12CTL0, ADC12ENC);		// Dozvola konverzije
	SET_1(ADC12IE, ADC12IE1);		// Dozvola prekida po zavrsenoj sekvenci
}

bool convert(void)
{
	if (!completed) {
		return false;
	}

	int8_t res = SCALE_ADC_RES(ADC12MEM0);
	data[0] = res / 10;
	data[1] = res % 10;
	TB0CCR3 = res;	// PWM duty cycle (LD1)

	res = SCALE_ADC_RES(ADC12MEM1);
	data[2] = res / 10;
	data[3] = res % 10;
	TB0CCR4 = res;	// PWM duty cycle (LD2)

	completed = false;
	return true;
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
	completed = true;
	ADC12IFG = 0;	// Citanje ADC12IV u if ili switch ne resetuje flag?
}
