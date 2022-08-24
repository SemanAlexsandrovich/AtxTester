
#include "main.h"

volatile extern uint8_t end_of_the_sec_flag;

void int1_init(void) {	// 5 Hz 
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 3124;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS12) | (1 << CS10);
	TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
	cli();
	static uint8_t count_to_the_second = 0;
	count_to_the_second++;
	if (count_to_the_second >= 5) {
		if(!end_of_the_sec_flag) {
			count_to_the_second = 0;
			end_of_the_sec_flag = 1;
		}
	}
	sei();
}