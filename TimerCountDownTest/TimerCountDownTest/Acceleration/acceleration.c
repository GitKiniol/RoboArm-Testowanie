/*
 * acceleration.c
 *
 * Created: 15.07.2022 09:06:15
 *  Author: rybka
 */ 


#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "acceleration.h"


void Acceleration_TimerInit(void)
{
	TCF0_CTRLA = TC_CLKSEL_OFF_gc;						//preskaler = 0, timer zatrzymany
	TCF0_CTRLB = TC_WGMODE_FRQ_gc;						//tryb generowania czêstotliwoœci
	
}

void Acceleration_Start(driver_t driver)
{
	// speed_low = 65534 - driver.timer.cca				//zakres prêdkoœci poni¿ej prêdkoœci zadanej
	// 1. cca = 65535 - (speed_low / 100) * 10;
	// 2. cca = 65535 - (speed_low / 100) * 20;
	// 3. cca = 65535 - (speed_low / 100) * 40;
	// 4. cca = 65535 - (speed_low / 100) * 65;
	// 5. cca = driver.sepoint_speed;
}

ISR(TCF0_OVF_vect)
{
	
}
