/*
 * TimerCountDownTest.c
 *
 * Created: 20.04.2022 17:06:34
 * Author : rybka
 */ 

/*
M1		M2		M3		M4
PC6		PD6		PE6		PF6 	- enable
PC5 	PD5		PE5		PF5		- direction
PC4 	PD4		PE4		PF4		- pulse

SERVO 1			Servo 2
PC0 - PWM		PC1 - PWM

USART<->BLUETOOTH
PD3 - RX
PD2 - TX
PD1 - STATE

LCD_TWI
PE0 - SDA
PE1 - SCL
*/

/*
	Fo = Fi / 2 * N * (CCA + 1)	=> Obliczenie czêstotliwoœci wyjœciowej na podstawie rejestru CCA
	
	CCA = (Fi / 2 * N * Fo) - 1	=> Obliczenie rejestru CCA na podstawie czêstotliwoœci wyjœciowej
		
	Fo  - czêstotliwoœæ wyjœciowa
	Fi  - czêstotliwoœæ wejœciowa (F_CPU)
	N   - preskaler
	CCA - rejestr porównania
	
	
	Ir = Sr * Er * Mr		=> Liczba impulsów drivera programowego na obrót	
	Ia = Ir / 360			=> Liczba impulsów drivera programowego na stopieñ k¹towy
	Fo = So / 60 * Ir		=> Obliczenie czêstotoliwoœci wyjœciowej na podstawie prêdkoœci i liczby impulsów
	
	So - prêdkoœæ obrotowa [obr/min]
	Ir - impulsy drivera programowego na obrót
	Sr - liczba kroków silnika na obrót
	Er - elektryczna przek³adnia (podzia³ kroków silnika)
	Mr - przek³adnia mechaniczna (stosunek ko³a napêdowego do napêdzanego)
	Ia - liczba impulsów drivera programowego na jeden stopieñ k¹towy
	
*/

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "Clock/sysclock.h"
#include "Driver/driver.h"


int main(void)
{
	ClkSys32MHz();
	sterownik = Driver_Init(sterownik, &TCC1, &PORTC, 200, 16, 1.0);
	//sterownik->DriverPort->OUTCLR = (1<<sterownik->PulsePin);
	sterownik->DriverPort->OUTSET = (1<<sterownik->EnablePin);
	//sterownik->DriverPort->OUTCLR = (1<<sterownik->EnablePin);
	/*                              D  A   S */
	Driver_SetParameters(sterownik, 0, 180, 200);
	PMIC.CTRL = PMIC_LOLVLEN_bm;
	sei();
	_delay_ms(3000);
	sterownik->DriverPort->OUTCLR = (1<<sterownik->EnablePin);
    //Driver_Run(sterownik);
    while (1) 
    {
		//Driver_Run(sterownik);
		//_delay_ms(15000);
		
    }
}

