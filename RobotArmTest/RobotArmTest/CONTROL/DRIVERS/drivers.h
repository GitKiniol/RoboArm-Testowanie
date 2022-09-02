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
/* definicja typów sterowników osi */
#define STEPPER		0
#define SERVO		1

/*--------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------Definicje struktur danych--------------------------------------------------*/
/* Pozycja zadana odebrana z telefonu jest z zakresu 0-90, mimo ¿e na ekranie telefonu zakres wynosi -90 do 90,
sterownik wartoœci ujemne interpretuje jako wartoœci dodatnie ale z ustawion¹ flag¹ kierunku w lewo. Wartoœæ 
pozycji przeliczana jest na iloœæ impulsów jakie trzeba wygenerowaæ do wykonania obrotu o zadany k¹t, i ta 
w³aœnie liczba jest parametrem "SetpointPosition" w poni¿szej strukturze sterownika. Wszystkie inne  
parametry w strukturze sterownika odnosz¹ce siê do pozycji wyra¿one s¹ w impulsach a nie w stopniach */
typedef struct STEPPER_DRIVER_STRUCT
{
	int16_t SetpointPosition;					/* pozycja zadana																				*/
	int16_t CurrentPosition;					/* aktualna pozycja																				*/
	uint16_t MaximumPosition;					/* maksymalna wartoœæ pozycji																	*/
	int16_t MinimumPosition;					/* minimalna wartoœæ pozycji																	*/
	uint8_t Direction;							/* kierunek ruchu																				*/
	TC1_t *DriverTimer;							/* timer sterownika																				*/
	PORT_t *DriverPort;							/* port pinów sterownika																		*/
	uint8_t EnablePin;							/* numer pinu dla sygna³u enable																*/
	uint8_t DirectionPin;						/* numer pinu dla sygna³u dir																	*/
	uint8_t PulsePin;							/* numer pinu dla sygna³u pul																	*/
	uint8_t IsRunning:1;						/* flaga informuj¹ca o pracy/zatrzymaniu sterownika												*/
	uint16_t MotorSteps;						/* Liczba kroków silnika na jeden obrót															*/
	uint8_t ElectricalRatio;					/* wartoœæ prze³o¿enia elektrycznego(iloœæ impulsów na krok, ustawienie pinów na sterowniku)	*/
	float MechanicalRatio;						/* wartoœæ prze³o¿enia mechanicznego (obroty przek³adni na obrót silnika)						*/
	uint8_t Speed;								/* prêdkoœæ zadana dla silnika	[obr/min]														*/
	void(*Start)(void*, uint8_t);				/* wskaŸnik do funkcji uruchamiaj¹cej driver													*/
	void(*Stop)(void*);							/* wskaŸnik do funkcji zatrzymuj¹cej driver														*/
	void(*Free)(void*);							/* wskaŸnik na funkcjê luzuj¹c¹ silnik															*/
	int16_t (*Convert)(int8_t, void*);			/* wskaŸnik do funkcji konwertuj¹cej k¹t na liczbê impulsów										*/
	
}stepper_driver_t;

/* Pozycja zadana oraz pozycja max i minimum odbierane z telefonu s¹ w zakresie 0 - 90, mikrokontroler przelicza je na 
wartoœci odpowiednie dla rejestru timera i w takiej formie wstawiane s¹ do parametrów w poni¿szej strukturze*/
typedef struct SERVO_DRIVER_STRUCT
{
	uint16_t SetpointPosition;					/* pozycja zadana																				*/
	uint16_t MaximumPosition;					/* maksymalna wartoœæ pozycji																	*/
	uint16_t MinimumPosition;					/* minimalna wartoœæ pozycji																	*/
	TC0_t *DriverTimer;							/* timer sterownika																				*/
	PORT_t *DriverPort;							/* port pinów sterownika																		*/
	uint8_t PwmPin;								/* numer pinu steruj¹cego serwem																*/
	uint8_t IsRunning:1;						/* flaga informuj¹ca o pracy/zatrzymaniu sterownika												*/
	void(*Start)(void*, uint8_t);				/* wskaŸnik do funkcji uruchamiaj¹cej driver													*/
	void(*Stop)(void*);							/* wskaŸnik do funkcji zatrzymuj¹cej driver														*/
	uint16_t (*Convert)(uint8_t);				/* wskaŸnik do funkcji konwertuj¹cej k¹t na wartoœæ rejestru wype³nienie impulsu PWM			*/

}servo_driver_t;

typedef struct TO_RUN_DRV_STRUCT
{
	uint8_t DriveType;							/* zmienna okreœlaj¹c tym wskazywanego drivera 1-servo, 0-stepper								*/
	void *ToRunDriver;							/* wskaŸnik na driver do uruchomienia															*/
}to_run_drv_t;

typedef struct TO_RUN_ITEM_STRUCT
{
	void *Next;									/* wskaŸnik na kolejny element listy															*/
	to_run_drv_t *Data;							/* wskaŸnik na dane zawarte w elemencie listy													*/
}to_run_item_t;

typedef struct TO_RUN_LIST_STRUCT
{
	to_run_item_t *Head;						/* wskaŸnik na pierwszy element listy															*/					
	to_run_item_t *Tail;						/* wskaŸnik na ostatni element listy															*/
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