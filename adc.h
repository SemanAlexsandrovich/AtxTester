
#ifndef ADC_H_
#define ADC_H_

#define MUX_CHANGE (ADMUX & 0xF0)
#define ION	1.1
#define ADC_RESOLUTION	1023
#define SCALE_OF_VOLTAGE	(ION/ADC_RESOLUTION)

void adc_init(void);
void adc_read(void);
void analog_read(uint8_t);

#endif /* ADC_H_ */