/*
 * work.h
 *
 * Created: 19.03.2022 19:28:49
 *  Author: rybka
 */ 


#ifndef WORK_H_
#define WORK_H_

#include "../../BLUETOOTH/DATA/data.h"


/*-----------------------------------------------Deklaracje makroinstrukcji--------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------Definicje struktur danych----------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------Deklaracje zmiennych-------------------------------------------------------------------------*/
/* EXTERN:	*/
extern TC0_t *RunTaskTimer;										/* timer taktuj¹cy wykonywanie zadañ											*/

/* LOCAL:	*/

/*----------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------Deklaracje funkcji-------------------------------------------------------------------------*/

void Work_TimerInit(TC0_t *timer);								/* inicjalizacja timera taktuj¹cego wykonywanie zadañ							*/

void Work_TimerStart(TC0_t *timer);								/* uruchomienie timera															*/

void Work_TimerStop(TC0_t *timer);								/* zatrzymanie timera															*/

uint8_t Work_GetParameters(list_t *list);						/* funkcja pobiera parametry ruchu z listy JOB									*/

void Work_RunRobot(void);										/* uruchomienie robota															*/

void Work_StopRobot(void);										/* zatrzymanie robota															*/

void Work_RunTask(list_t *joblist, uint8_t(*sendstatus)(char *));	/* uruchomienie zadania pobranego z listy Job								*/

/*----------------------------------------------------------------------------------------------------------------------------------------------*/


#endif /* WORK_H_ */