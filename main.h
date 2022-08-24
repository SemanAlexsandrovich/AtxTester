
#ifndef MAIN_H_
#define MAIN_H_


#define F_CPU 16000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <math.h>

#include "uart.h"
#include "adc.h"
#include "timers.h"
#include "twi.h"
#include "lcd.h"

#define A0	 0
#define A1	 1
#define A2	 2
#define A3	 3
#define A4	 4
#define A5	 5

#define DIVIDERS_NUM 4
#define ORANGE_DIVIDER 3.3
#define RED_DIVIDER 5.5
#define PURPLE_DIVIDER 5.5
#define YELLOW_DIVIDER 13.0//12.5
#define SCALE_OF_ORANGE_DIVIDER  (ORANGE_DIVIDER/ADC_RESOLUTION)
#define SCALE_OF_RED_DIVIDER  (RED_DIVIDER/ADC_RESOLUTION)
#define SCALE_OF_PURPLE_DIVIDER  (PURPLE_DIVIDER/ADC_RESOLUTION)
#define SCALE_OF_YELLOW_DIVIDER  (YELLOW_DIVIDER/ADC_RESOLUTION)

#define YELLOW_DIVIDER_OFFSET 363
#define RED_DIVIDER_OFFSET 186
#define PURPLE_DIVIDER_OFFSET 186
#define ORANGE_DIVIDER_OFFSET 95

#endif /* MAIN_H_ */