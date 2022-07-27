/*
 * driver.h
 *
 * Created: 15.07.2022 09:05:18
 *  Author: rybka
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


#ifndef DRIVER_H_
#define DRIVER_H_

#define SPR			200							/* ilo�� krok�w silnika na obr�t														*/
#define EPD			4							/* podzia� krok�w silnika (switch'e na driverze)										*/
#define MD			1.0							/* warto�� przek�adni mechanicznej														*/

typedef struct DRIVER_STRUCT
{
	int16_t SetpointPosition;								/* pozycja zadana															*/
	int16_t CurrentPosition;								/* aktualna pozycja															*/
	uint16_t MaximumPosition;								/* maksymalna warto�� pozycji												*/
	int16_t MinimumPosition;								/* minimalna warto�� pozycji												*/
	uint8_t Direction;										/* kierunek ruchu															*/
	TC1_t *DriverTimer;										/* timer sterownika															*/
	PORT_t *DriverPort;										/* port pin�w sterownika													*/
	uint8_t EnablePin;										/* numer pinu dla sygna�u enable											*/
	uint8_t DirectionPin;									/* numer pinu dla sygna�u dir												*/
	uint8_t PulsePin;										/* numer pinu dla sygna�u pul												*/
	uint8_t IsRunning:1;									/* flaga informuj�ca o pracy/zatrzymaniu sterownika							*/
	uint16_t MotorSteps;									/* Liczba krok�w silnika na jeden obr�t										*/
	uint8_t ElectricalRatio;								/* warto�� prze�o�enia elektrycznego(ilo�� impuls�w na krok)				*/
	float MechanicalRatio;									/* warto�� prze�o�enia mechanicznego (obroty przek�adni na obr�t silnika)	*/
	uint8_t Speed;											/* pr�dko�� silnika	[obr/min]												*/
	
}driver_t;

driver_t *sterownik;

uint16_t CalculateTimerFrq(float motor_pulse_per_rev, float pulse_divider, float mechanical_divider, float speed_rpm); /* obliczanie warto�ci rejestru timera	*/

driver_t *Driver_Init(driver_t *driver, TC1_t *timer, PORT_t *port, uint16_t motor_steps, uint8_t electrical_ratio, float mechanical_ratio);	/* inicjalizacja drivera				*/

void Driver_SetParameters(driver_t *driver, uint8_t dir, uint8_t angle, uint8_t speed);									/* ustawianie parametr�w drivera		*/

void Driver_Run(driver_t *driver);																						/* uruchomienie drivera					*/

void Driver_Stop(driver_t *driver);																						/* zatrzymanie drivera					*/




#endif /* DRIVER_H_ */