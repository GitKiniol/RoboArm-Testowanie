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
	Fo = Fi / 2 * N * (CCA + 1)	=> Obliczenie cz�stotliwo�ci wyj�ciowej na podstawie rejestru CCA
	
	CCA = (Fi / 2 * N * Fo) - 1	=> Obliczenie rejestru CCA na podstawie cz�stotliwo�ci wyj�ciowej
		
	Fo  - cz�stotliwo�� wyj�ciowa
	Fi  - cz�stotliwo�� wej�ciowa (F_CPU)
	N   - preskaler
	CCA - rejestr por�wnania
	
	
	Ir = Sr * Er * Mr		=> Liczba impuls�w drivera programowego na obr�t	
	Ia = Ir / 360			=> Liczba impuls�w drivera programowego na stopie� k�towy
	Fo = So / 60 * Ir		=> Obliczenie cz�stotoliwo�ci wyj�ciowej na podstawie pr�dko�ci i liczby impuls�w
	
	So - pr�dko�� obrotowa [obr/min]
	Ir - impulsy drivera programowego na obr�t
	Sr - liczba krok�w silnika na obr�t
	Er - elektryczna przek�adnia (podzia� krok�w silnika)
	Mr - przek�adnia mechaniczna (stosunek ko�a nap�dowego do nap�dzanego)
	Ia - liczba impuls�w drivera programowego na jeden stopie� k�towy
	
*/

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "Clock/sysclock.h"
#include "Driver/driver.h"
#include "INTERFACE/interface.h"


int main(void)
{
	ClkSys32MHz();
	
	driver1_1 = Driver1Init(driver1_1);
	driver1_2 = Driver1Init(driver1_2);
	driver2_1 = Driver2Init(driver2_1);
	
    while (1) 
    {
		
    }
}

