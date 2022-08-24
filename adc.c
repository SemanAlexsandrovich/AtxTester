
#include "main.h"

void adc_init() {
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADATE);
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADMUX &= ~((1 << REFS0) | (1 << REFS1));
	ADMUX |= (1 << REFS0) | (1 << REFS1);

	ADMUX &= ~(1 <<  ADLAR);
	ADCSRA |= (1 << ADSC);
}

void analog_read(uint8_t pin) {
	ADMUX = MUX_CHANGE | pin;
	 _delay_us(50);
	while(!(ADCSRA & (1 << ADIF)));
	ADCSRA |= (1 << ADIF);
}

