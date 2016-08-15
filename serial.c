#include "project.h"

#define BR_FREQ				(SMCLK_FREQ / BAUD_RATE)
#define BR_FREQ_L			(BR_FREQ & 0xFF)
#define BR_FREQ_H			(BR_FREQ >> 8 & 0xFF)
#define BR_FREQ_REM			(int)((SMCLK_FREQ/(double)BAUD_RATE-BR_FREQ)*8)
#define BR_MODULATION		(BR_FREQ_REM << 1)	// UCBRS_0 .. UCBRS_7

#define DIGIT_TO_ASCII(d)	(d + '0')

static uint8_t index = 0;

void init_UART(void)
{
	SET_1(P3SEL, BIT4);			// Dodeljuje funkciju P3.4 (UCA0TXD) UART-u
	SET_1(UCA0CTL1, UCSWRST);	// Drzi RESET tokom podesavanja
	
	/* Baud rate */
	SET_1(UCA0CTL1, UCSSEL_2);	// Izvor: SMCLK
	UCA0BR0  = BR_FREQ_L;		// Nizi biti
	UCA0BR1  = BR_FREQ_H;		// Visi biti
	UCA0MCTL = BR_MODULATION;	// Kompenzaciona modulacija 
	
	RESET_1(UCA0CTL1, UCSWRST);	// Otpusta RESET
}

void send(void)
{
	UCA0TXBUF = DIGIT_TO_ASCII(data[0]);	// Prvi se rucno salje ...
	SET_1(UCA0IE, UCTXIE);		// Dozvola prekida predajnika
}

#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	if (UCA0IV & USCI_UCTXIFG) {	// Automatski se resetuje fleg za prekid
		switch (++index) {			// ... ostali do kraja jedan za drugim
		case 0: case 1:
			UCA0TXBUF = DIGIT_TO_ASCII(data[index]);	// data[0..1]
			break;
		case 2:
			UCA0TXBUF = ':';
			break;
		case 3: case 4:
			UCA0TXBUF = DIGIT_TO_ASCII(data[index-1]);	// data[2..3]
			break;
		case 5:
			UCA0TXBUF = '\r';
			break;
		case 6:
			UCA0TXBUF = '\n';
			RESET_1(UCA0IE, UCTXIE);
			index = 0;
			break;
		}	// EOT, ne radi nista
	}
}
