
#include "main.h"

char buff_rx[buff_rx_SIZE] = {0};
char rpm_buff_twi[SIZEOF_SENDBUF];
char power_buff_twi[SIZEOF_SENDBUF];
char buff_to_send[SIZEOF_SENDBUF];
char orange[SIZEOF_SENDBUF];
char yellow[SIZEOF_SENDBUF];
char red[SIZEOF_SENDBUF];
char purple[SIZEOF_SENDBUF];
char *buff_rx_begin = buff_rx;
volatile uint8_t pos = 0;
volatile char flag_recive = 0;

volatile uint8_t pin_crossing = 0;

volatile uint16_t adc_on_orange_divider = 0.0;
volatile uint16_t adc_on_red_divider = 0.0;
volatile uint16_t adc_on_purple_divider = 0.0;
volatile uint16_t adc_on_yellow_divider = 0.0;

volatile uint16_t max_adc_on_orange_divider = 0;
volatile uint16_t min_adc_on_orange_divider = 2000;
volatile uint16_t max_adc_on_red_divider = 0;
volatile uint16_t min_adc_on_red_divider = 2000;
volatile uint16_t max_adc_on_purple_divider = 0;
volatile uint16_t min_adc_on_purple_divider = 2000;
volatile uint16_t max_adc_on_yellow_divider = 0;
volatile uint16_t min_adc_on_yellow_divider =2000;

volatile uint8_t end_of_the_sec_flag = 0;
volatile uint8_t indicator_flag = 0;

void port_setup(void) {
	//adc
	DDRC = 0x00;
	PORTC = 0x00;
}

int main(void) {
	port_setup();
	adc_init();	
	int1_init();	
    UartInit();
	I2C_Init();
	LCD_ini();
	clearlcd();
	sprintf((char*)buff_to_send, "Hello\r\n");
	DebagUart(buff_to_send);
	UCSR0B |= (1 << UDRIE0);
    while (1) {	
		if (end_of_the_sec_flag) {		
				uint16_t mid_adc_on_orange_divider = ((max_adc_on_orange_divider + min_adc_on_orange_divider) >> 1);
				uint16_t mid_adc_on_red_divider = ((max_adc_on_red_divider + min_adc_on_red_divider) >> 1);	
				uint16_t mid_adc_on_purple_divider = ((max_adc_on_purple_divider + min_adc_on_purple_divider) >> 1);				
				uint16_t mid_adc_on_yellow_divider = ((max_adc_on_yellow_divider + min_adc_on_yellow_divider) >> 1);

				float voltage_on_orange_divider = mid_adc_on_orange_divider * SCALE_OF_ORANGE_DIVIDER;
				float voltage_on_red_divider = mid_adc_on_red_divider * SCALE_OF_RED_DIVIDER;
				float voltage_on_purple_divider = mid_adc_on_purple_divider * SCALE_OF_RED_DIVIDER;
				float voltage_on_yellow_divider = mid_adc_on_yellow_divider * SCALE_OF_YELLOW_DIVIDER;

				float whole_part_of_voltage_on_orange_divider, fractional_part_of_voltage_on_orange_divider;
				float whole_part_of_voltage_on_red_divider, fractional_part_of_voltage_on_red_divider;
				float whole_part_of_voltage_on_purple_divider, fractional_part_of_voltage_on_purple_divider;
				float whole_part_of_voltage_on_yellow_divider, fractional_part_of_voltage_on_yellow_divider;
				
				fractional_part_of_voltage_on_orange_divider = modff (voltage_on_orange_divider,&whole_part_of_voltage_on_orange_divider);
				fractional_part_of_voltage_on_red_divider = modff (voltage_on_red_divider,&whole_part_of_voltage_on_red_divider);
				fractional_part_of_voltage_on_purple_divider = modff (voltage_on_purple_divider,&whole_part_of_voltage_on_purple_divider);
				fractional_part_of_voltage_on_yellow_divider = modff (voltage_on_yellow_divider,&whole_part_of_voltage_on_yellow_divider);
				
				uint8_t whole_part_to_transmit_on_orange_divider = (uint8_t)whole_part_of_voltage_on_orange_divider;
				uint8_t whole_part_to_transmit_on_red_divider = (uint8_t)whole_part_of_voltage_on_red_divider;
				uint8_t whole_part_to_transmit_on_purple_divider = (uint8_t)whole_part_of_voltage_on_purple_divider;
				uint8_t whole_part_to_transmit_on_yellow_divider = (uint8_t)whole_part_of_voltage_on_yellow_divider;
				
				uint8_t fractional_part_to_transmit_on_orange_divider = (uint8_t)(fractional_part_of_voltage_on_orange_divider * 100);
				uint8_t fractional_part_to_transmit_on_red_divider = (uint8_t)(fractional_part_of_voltage_on_red_divider * 100);
				uint8_t fractional_part_to_transmit_on_purple_divider = (uint8_t)(fractional_part_of_voltage_on_purple_divider * 100);
				uint8_t fractional_part_to_transmit_on_yellow_divider = (uint8_t)(fractional_part_of_voltage_on_yellow_divider * 100);
				
				//put on uart	
				sprintf((char*)buff_to_send, "o:%d.%d  r:%d.%d  p:%d.%d y:%d.%d  \r\n",
				whole_part_to_transmit_on_orange_divider, fractional_part_to_transmit_on_orange_divider,
				whole_part_to_transmit_on_red_divider, fractional_part_to_transmit_on_red_divider,
				whole_part_to_transmit_on_purple_divider, fractional_part_to_transmit_on_purple_divider,
				whole_part_to_transmit_on_yellow_divider, fractional_part_to_transmit_on_yellow_divider);				
				//sprintf((char*)buff_to_send, "o:%d r:%d p:%d y:%d  \r\n",
				//mid_adc_on_orange_divider, mid_adc_on_red_divider,mid_adc_on_purple_divider,mid_adc_on_yellow_divider);	
				DebagUart(buff_to_send);
				UCSR0B |= (1 << UDRIE0);
				
				//put on lcd				
				sprintf((char*)orange, "o:%d.%d ", whole_part_to_transmit_on_orange_divider, fractional_part_to_transmit_on_orange_divider);//3.3v
				sprintf((char*)yellow, "y:%d.%d ", whole_part_to_transmit_on_yellow_divider, fractional_part_to_transmit_on_yellow_divider);//12v
				sprintf((char*)red, "r:%d.%d ", whole_part_to_transmit_on_red_divider, fractional_part_to_transmit_on_red_divider);//5v
				sprintf((char*)purple, "p:%d.%d ", whole_part_to_transmit_on_purple_divider, fractional_part_to_transmit_on_purple_divider);//5v
				setpos(0,0);
				str_lcd(orange);
				setpos(10,0);
				str_lcd(yellow);
				setpos(0,1);
				str_lcd(red);
				setpos(10,1);
				str_lcd(purple);
				
				max_adc_on_orange_divider = 0;
				min_adc_on_orange_divider = 2000;
				max_adc_on_red_divider = 0;
				min_adc_on_red_divider = 2000;
				max_adc_on_purple_divider = 0;
				min_adc_on_purple_divider = 2000;
				max_adc_on_yellow_divider = 0;
				min_adc_on_yellow_divider = 2000;
				end_of_the_sec_flag = 0;
			}
		analog_read(pin_crossing);//Getting rid of noise by several reading
		analog_read(pin_crossing);
		analog_read(pin_crossing);
		analog_read(pin_crossing);
		uint16_t adc_on_pin = ADC;
		
		if (pin_crossing == 0) {
			adc_on_yellow_divider = adc_on_pin + YELLOW_DIVIDER_OFFSET;//A0:12V
			if (adc_on_yellow_divider == YELLOW_DIVIDER_OFFSET) {
				adc_on_yellow_divider = 0;
			}
			if (adc_on_yellow_divider > max_adc_on_yellow_divider) {
				max_adc_on_yellow_divider = adc_on_yellow_divider;
			}
			if (adc_on_yellow_divider < min_adc_on_yellow_divider) {
				min_adc_on_yellow_divider = adc_on_yellow_divider;
			}		
		}
		if (pin_crossing == 1) {
			adc_on_red_divider = adc_on_pin + RED_DIVIDER_OFFSET;//A1:5.5v
			if (adc_on_red_divider == RED_DIVIDER_OFFSET) {
				adc_on_red_divider = 0;
			}
			if (adc_on_red_divider > max_adc_on_red_divider) {
				max_adc_on_red_divider = adc_on_red_divider;
			}
			if (adc_on_red_divider < min_adc_on_red_divider) {
				min_adc_on_red_divider = adc_on_red_divider;
			}	
		}
		if (pin_crossing == 2) {
			adc_on_purple_divider = adc_on_pin + PURPLE_DIVIDER_OFFSET;//A2:5.5v
			if (adc_on_purple_divider == PURPLE_DIVIDER_OFFSET) {
				adc_on_purple_divider = 0;
			}
			if (adc_on_purple_divider > max_adc_on_purple_divider) {
				max_adc_on_purple_divider = adc_on_purple_divider;
			}
			if (adc_on_purple_divider < min_adc_on_purple_divider) {
				min_adc_on_purple_divider = adc_on_purple_divider;
			}
		}	
		if (pin_crossing == 3) {
			adc_on_orange_divider = adc_on_pin + ORANGE_DIVIDER_OFFSET;//A3:3.3V
			if (adc_on_orange_divider == ORANGE_DIVIDER_OFFSET) {
				adc_on_orange_divider = 0;
			}
			if (adc_on_orange_divider > max_adc_on_orange_divider) {
				max_adc_on_orange_divider = adc_on_orange_divider;
			}
			if (adc_on_orange_divider < min_adc_on_orange_divider) {
				min_adc_on_orange_divider = adc_on_orange_divider;
			}		
		}
		pin_crossing++;
		if (pin_crossing == 4) {
			pin_crossing = 0;
		}
	}
}

