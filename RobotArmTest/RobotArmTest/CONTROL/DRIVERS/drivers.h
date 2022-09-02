/*
 * drivers.h
 *
 * Created: 19.03.2022 19:28:30
 *  Author: rybka
 */ 


#ifndef DRIVERS_H_
#define DRIVERS_H_

#include "../../BLUETOOTH/DATA/data.h"


/*-----------------------------------------------Deklaracje makroinstrukcji-------------------------------------------------*/
/* definicja typ�w sterownik�w osi */
#define STEPPER		0
#define SERVO		1

/*--------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------Definicje struktur danych--------------------------------------------------*/
/* Pozycja zadana odebrana z telefonu jest z zakresu 0-90, mimo �e na ekranie telefonu zakres wynosi -90 do 90,
sterownik warto�ci ujemne interpretuje jako warto�ci dodatnie ale z ustawion� flag� kierunku w lewo. Warto�� 
pozycji przeliczana jest na ilo�� impuls�w jakie trzeba wygenerowa� do wykonania obrotu o zadany k�t, i ta 
w�a�nie liczba jest parametrem "SetpointPosition" w poni�szej strukturze sterownika. Wszystkie inne  
parametry w strukturze sterownika odnosz�ce si� do pozycji wyra�one s� w impulsach a nie w stopniach */
typedef struct STEPPER_DRIVER_STRUCT
{
	int16_t SetpointPosition;					/* pozycja zadana																				*/
	int16_t CurrentPosition;					/* aktualna pozycja																				*/
	uint16_t MaximumPosition;					/* maksymalna warto�� pozycji																	*/
	int16_t MinimumPosition;					/* minimalna warto�� pozycji																	*/
	uint8_t Direction;							/* kierunek ruchu																				*/
	TC1_t *DriverTimer;							/* timer sterownika																				*/
	PORT_t *DriverPort;							/* port pin�w sterownika																		*/
	uint8_t EnablePin;							/* numer pinu dla sygna�u enable																*/
	uint8_t DirectionPin;						/* numer pinu dla sygna�u dir																	*/
	uint8_t PulsePin;							/* numer pinu dla sygna�u pul																	*/
	uint8_t IsRunning:1;						/* flaga informuj�ca o pracy/zatrzymaniu sterownika												*/
	uint16_t MotorSteps;						/* Liczba krok�w silnika na jeden obr�t															*/
	uint8_t ElectricalRatio;					/* warto�� prze�o�enia elektrycznego(ilo�� impuls�w na krok, ustawienie pin�w na sterowniku)	*/
	float MechanicalRatio;						/* warto�� prze�o�enia mechanicznego (obroty przek�adni na obr�t silnika)						*/
	uint8_t Speed;								/* pr�dko�� zadana dla silnika	[obr/min]														*/
	void(*Start)(void*, uint8_t);				/* wska�nik do funkcji uruchamiaj�cej driver													*/
	void(*Stop)(void*);							/* wska�nik do funkcji zatrzymuj�cej driver														*/
	void(*Free)(void*);							/* wska�nik na funkcj� luzuj�c� silnik															*/
	int16_t (*Convert)(int8_t, void*);			/* wska�nik do funkcji konwertuj�cej k�t na liczb� impuls�w										*/
	
}stepper_driver_t;

/* Pozycja zadana oraz pozycja max i minimum odbierane z telefonu s� w zakresie 0 - 90, mikrokontroler przelicza je na 
warto�ci odpowiednie dla rejestru timera i w takiej formie wstawiane s� do parametr�w w poni�szej strukturze*/
typedef struct SERVO_DRIVER_STRUCT
{
	uint16_t SetpointPosition;					/* pozycja zadana																				*/
	uint16_t MaximumPosition;					/* maksymalna warto�� pozycji																	*/
	uint16_t MinimumPosition;					/* minimalna warto�� pozycji																	*/
	TC0_t *DriverTimer;							/* timer sterownika																				*/
	PORT_t *DriverPort;							/* port pin�w sterownika																		*/
	uint8_t PwmPin;								/* numer pinu steruj�cego serwem																*/
	uint8_t IsRunning:1;						/* flaga informuj�ca o pracy/zatrzymaniu sterownika												*/
	void(*Start)(void*, uint8_t);				/* wska�nik do funkcji uruchamiaj�cej driver													*/
	void(*Stop)(void*);							/* wska�nik do funkcji zatrzymuj�cej driver														*/
	uint16_t (*Convert)(uint8_t);				/* wska�nik do funkcji konwertuj�cej k�t na warto�� rejestru wype�nienie impulsu PWM			*/

}servo_driver_t;

typedef struct TO_RUN_DRV_STRUCT
{
	uint8_t DriveType;							/* zmienna okre�laj�c tym wskazywanego drivera 1-servo, 0-stepper								*/
	void *ToRunDriver;							/* wska�nik na driver do uruchomienia															*/
}to_run_drv_t;

typedef struct TO_RUN_ITEM_STRUCT
{
	void *Next;									/* wska�nik na kolejny element listy															*/
	to_run_drv_t *Data;							/* wska�nik na dane zawarte w elemencie listy													*/
}to_run_item_t;

typedef struct TO_RUN_LIST_STRUCT
{
	to_run_item_t *Head;						/* wska�nik na pierwszy element listy															*/					
	to_run_item_t *Tail;						/* wska�nik na ostatni element listy															*/
}to_run_list_t;

/*--------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------Deklaracje zmiennych-----------------------------------------------------*/
/* EXTERN:	*/

/* LOCAL:	*/

/*--------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------Deklaracje funkcji-----------------------------------------------------*/

void Driver_AxisInit(void);

stepper_driver_t *Driver_StepperDriverInit(stepper_driver_t *driver, TC1_t *timer, PORT_t *port, uint16_t motor_steps, uint8_t electrical_ratio, float mechanical_ratio);

servo_driver_t *Driver_ServoDriverInit(servo_driver_t *driver, TC0_t *timer, PORT_t *port, uint8_t pwmpin);

int16_t Driver_ConvertAngleToStep(int8_t angle, void *driver);

uint16_t Driver_ConvertAngleToPwm(uint8_t angle);

void Driver_SetStepperSpeed(stepper_driver_t *driver, uint8_t speed);

void Driver_SetDriverParameters(move_t *move);

void Driver_SetStepperParameters(stepper_driver_t *driver, uint8_t speed, uint8_t angle, uint8_t dir);

void Driver_SetServoParameters(servo_driver_t *driver, uint8_t angle);

void Driver_StartStepperDriver(void *driver, uint8_t preskaler);

void Driver_StopStepperDriver(void *driver);

void Driver_StartServoDriver(void *driver, uint8_t preskaler);

void Driver_StopServoDriver(void *driver);

to_run_list_t *Driver_ToRunListInit(void);

to_run_item_t *Driver_ToRunItemInit(to_run_drv_t *torundrv);

to_run_drv_t *Driver_ToRunDrvInit(void *driver, uint8_t drvtype);

void Driver_ToRunListAdd(to_run_list_t *list, void *driver, uint8_t drvtype);

to_run_drv_t *Driver_ToRunListGet(to_run_list_t *list);

void Driver_RunTaskAxes(void);

void Driver_FreeStepper(void *driver);

void Driver_FreeAxes(void);

void Driver_StopRobot(void);

void Driver_EmergencyStop(void);

void Driver_StepperTimerIsr(stepper_driver_t *driver);

uint8_t Driver_IsAnyAxisRunning(void);

/*--------------------------------------------------------------------------------------------------------------------------*/


#endif /* DRIVERS_H_ */