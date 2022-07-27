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


#ifndef DRIVER_H_
#define DRIVER_H_

#define SPR			200							/* iloœæ kroków silnika na obrót														*/
#define EPD			4							/* podzia³ kroków silnika (switch'e na driverze)										*/
#define MD			1.0							/* wartoœæ przek³adni mechanicznej														*/

typedef struct DRIVER_STRUCT
{
	int16_t SetpointPosition;								/* pozycja zadana															*/
	int16_t CurrentPosition;								/* aktualna pozycja															*/
	uint16_t MaximumPosition;								/* maksymalna wartoœæ pozycji												*/
	int16_t MinimumPosition;								/* minimalna wartoœæ pozycji												*/
	uint8_t Direction;										/* kierunek ruchu															*/
	TC1_t *DriverTimer;										/* timer sterownika															*/
	PORT_t *DriverPort;										/* port pinów sterownika													*/
	uint8_t EnablePin;										/* numer pinu dla sygna³u enable											*/
	uint8_t DirectionPin;									/* numer pinu dla sygna³u dir												*/
	uint8_t PulsePin;										/* numer pinu dla sygna³u pul												*/
	uint8_t IsRunning:1;									/* flaga informuj¹ca o pracy/zatrzymaniu sterownika							*/
	uint16_t MotorSteps;									/* Liczba kroków silnika na jeden obrót										*/
	uint8_t ElectricalRatio;								/* wartoœæ prze³o¿enia elektrycznego(iloœæ impulsów na krok)				*/
	float MechanicalRatio;									/* wartoœæ prze³o¿enia mechanicznego (obroty przek³adni na obrót silnika)	*/
	uint8_t Speed;											/* prêdkoœæ silnika	[obr/min]												*/
	
}driver_t;

driver_t *sterownik;

uint16_t CalculateTimerFrq(float motor_pulse_per_rev, float pulse_divider, float mechanical_divider, float speed_rpm); /* obliczanie wartoœci rejestru timera	*/

driver_t *Driver_Init(driver_t *driver, TC1_t *timer, PORT_t *port, uint16_t motor_steps, uint8_t electrical_ratio, float mechanical_ratio);	/* inicjalizacja drivera				*/

void Driver_SetParameters(driver_t *driver, uint8_t dir, uint8_t angle, uint8_t speed);									/* ustawianie parametrów drivera		*/

void Driver_Run(driver_t *driver);																						/* uruchomienie drivera					*/

void Driver_Stop(driver_t *driver);																						/* zatrzymanie drivera					*/




#endif /* DRIVER_H_ */