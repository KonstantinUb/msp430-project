#include "project.h"

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Zaustavlja watchdog tajmer

	init_TUI();
	init_UART();
	init_ADC();
	init_timers();

	__bis_SR_register(GIE);		// Globalna dozvola maskirajucih prekida

	SET_1(ADC12CTL0, ADC12SC);	// Pokrece prvu konverziju

	while (true) {
		if (convert()) {
			send();
		}
	}
}
