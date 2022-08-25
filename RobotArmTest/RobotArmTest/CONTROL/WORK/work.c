/*
 * work.c
 *
 * Created: 19.03.2022 19:29:04
 *  Author: rybka
 */ 


#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "work.h"
#include "../../BLUETOOTH/DATA/data.h"
#include "../../BLUETOOTH/HC05/hc05.h"
#include "../DRIVERS/drivers.h"

/*-------------------------------------------Deklaracje zmiennych-------------------------------------------------------------------------------------------*/
/* EXTERN: */
TC0_t *RunTaskTimer = &TCF0;												/* timer taktuj�cy wykonywanie zada�											*/

/* LOCAL:  */
uint8_t IsJobInProgress = 0;												/* czy trwa wykonywanie pracy (sekwencji zada�) ?								*/
uint8_t IsTaskInProgress = 0;												/* czy trwa wykonywanie zadania ?												*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------Definicje funkcji----------------------------------------------------------------------------------------------*/

void Work_TimerInit(TC0_t *timer)
{
	timer->PER = 2;																/* ustawienie warto�ci TOP dla timera										*/
	timer->CNT = 0;																/* zerowanie aktualnej warto�ci timera										*/
	Work_TimerStop(timer);														/* zatrzymanie timera														*/
	timer->INTCTRLA = TC_OVFINTLVL_LO_gc;										/* odblokowanie przerwania przepe�nienia timera								*/
	PMIC.CTRL |= PMIC_LOLVLEN_bm;												/* odblokowanie przerwa� o niskim priorytecie								*/
}

void Work_TimerStart(TC0_t *timer)
{
	timer->CTRLA = TC_CLKSEL_DIV1024_gc;										/* preskaler = 1024, timer uruchomiony										*/
}

void Work_TimerStop(TC0_t *timer)
{
	timer->CTRLA = TC_CLKSEL_OFF_gc;											/* preskaler = 0, timer zatrzymany											*/
}

uint8_t Work_GetParameters(list_t *list)
{
	if (list->Current != NULL)													/* je�li lista nie jest pusta, to:											*/						
	{
		list_t *task;															/* deklaracja wska�nika na list� ruch�w pobran� z listy Job					*/
		task = Data_GetTaskFromList(list);										/* pobranie listy ruch�w													*/
		while (task->Current != NULL)											/* do czasu gdy wska�nik na ruch nie jest pusty, to:						*/
		{
			move_t *move;														/* deklaracja wska�nika na ruch												*/
			move = Data_GetMoveFromList(task);									/* pobranie ruchu															*/
 			Driver_SetDriverParameters(move);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

void Work_RunRobot(void)
{
	Work_TimerInit(RunTaskTimer);												/* inicjalizacje timera														*/
	Work_TimerStart(RunTaskTimer);												/* uruchomienie timera														*/
}

void Work_StopRobot(void)
{
	Work_TimerStop(RunTaskTimer);												/* zatrzymanie timera														*/
	Driver_EmergencyStop();														/* zatrzymanie robota														*/
}

void Work_RunTask(list_t *joblist, uint8_t(*sendstatus)(char *))
{
	uint8_t IsParametersOk = 0;													/* zmienna do przechowywania rezultatu pobrania parametr�w					*/
	if (!IsTaskInProgress)														/* je�li zadanie nie zosta�o uruchomione, to:								*/
	{
		IsParametersOk = Work_GetParameters(joblist);							/* pobierz parametry i zapisz je w sterownikach								*/
	} 
	if (IsTaskInProgress || IsParametersOk)										/* je�li zadanie jest ju� uruchomione lub pobranie parametr�w ok, to:		*/								
	{
		Driver_RunTaskAxes();													/* uruchomienie driver�w													*/
		IsJobInProgress = 1;
		IsTaskInProgress = 1;
	}
	else if (!IsParametersOk)													/* je�li pobieranie parametr�w zako�czy�o si� niepowodzeniem, to:			*/								
	{
		if (IsJobInProgress)													/* je�li za��czone zosta�o wykonywanie pracy, to:							*/
		{
			IsJobInProgress = 0;												/* wyzeruj flag� informuj�c� o wykonywaniu pracy							*/
			Data_ClearJob();													/* wyczy�� list� job														*/
			sendstatus("6\n");													/* wy�lij informacj� �e robot zako�czy� prac�								*/
		} 
		else
		{
			sendstatus("99\n");													/* b��d wykonywania pracy													*/
		}
	}
	
}


/*----------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------przerwania---------------------------------------------------------------------------------*/

ISR(TCF0_OVF_vect)
{
	static uint16_t x = 0;
	if (x >= 32000 || IsJobInProgress == 0)
	{
		IsTaskInProgress = 0;
		Work_TimerStop(&TCF0);
		Work_RunTask(Job, &HC05_SendStatus);
		x = 1;
	}
	x++;
}
