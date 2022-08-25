/*
 * data.c
 *
 * Created: 31.03.2022 11:19:57
 *  Author: rybka
 */ 


#include <avr/io.h>
#include <stdlib.h>
#include "data.h"

/*-------------------------------------------Deklaracje zmiennych-------------------------------------------------------------------------------------------*/
/* EXTERN: */
list_t *Job;														/* lista zada� utworzona z danych odebranych przez bluetooth z telefonu					*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------Definicje funkcji----------------------------------------------------------------------------------------------*/

move_t *Data_CreateMove(char axis, uint8_t angle, uint8_t speed, uint8_t dir)
{
	move_t *ptrMove;																/* deklaracja wska�nika na ruch											*/
	ptrMove = (move_t *)malloc(sizeof(move_t));										/* alokacja pami�ci dla wska�nika na ruch								*/
	ptrMove->AxisName = axis;														/* przypisanie litery osi												*/
	ptrMove->Angle = angle;															/* przypisanie k�ta osi													*/
	ptrMove->Speed = speed;															/* przypisanie pr�dko�ci osi											*/
	ptrMove->Direction = dir;														/* przypisanie kierunku osi												*/
	return ptrMove;																	/* zwr�cenie adresu na alokowan� pami��									*/
}

list_element_t *Data_CreateListElement(void *data, void *next)
{
	list_element_t *ptrElement;														/* deklaracja wska�nika na element listy								*/
	ptrElement = (list_element_t *)malloc(sizeof(list_element_t));					/* alokacja pami�ci dla elementu listy									*/
	ptrElement->Data = data;														/* ustawienie wska�nika na dane elementu listy							*/
	ptrElement->Next = next;														/* ustawienie wska�nika na nast�pny element listy						*/
	return ptrElement;																/* zwr�cenie wska�nika na alokowan� pami��								*/
}

list_t *Data_CreateList(void)
{
	list_t *ptrList;																/* deklaracja wska�nika na list�										*/
	ptrList = (list_t *)malloc(sizeof(list_t));										/* alokacja pami�ci dla listy											*/
	ptrList->Count = 0;																/* ustawienie licznika listy na 0										*/
	ptrList->Current = NULL;														/* ustawienie wska�nika na aktualny element listy						*/
	ptrList->Head = NULL;															/* ustawienie wska�nika na pierwszy element listy						*/
	ptrList->Tail = NULL;															/* ustawienie wska�nika na ostatni element listy						*/
	return ptrList;																	/* zwr�cenie wska�nika na alokowan� pami��								*/
}

void Data_InsertElementToList(list_t *list, list_element_t *element)
{
	if (list->Tail == NULL)															/* je�li lista jest pusta, to:  (umieszczanie pierwszego elementu)		*/
	{
		list->Tail = element;														/* wstaw element na koniec listy										*/
		list->Tail->Next = NULL;													/* element na ko�cu listy nie mo�e ju� na nic wskazywa� wi�c ma NULL	*/
		list->Head = list->Tail;													/* w li�cie jest jeden element wi�c pierwszy i ostatni s� tym samym		*/
	}
	else
	{
		element->Next = list->Head;													/* ustaw wstawianemu do listy elementowi wska�nik na poprzedni			*/
		list->Head = element;														/* ustaw wstawiany element na pocz�tku listy							*/
	}
	list->Current = list->Head;														/* ustaw pierwszy element listy jako bie��cy element					*/
	list->Count++;																	/* inkrementacja licznika element�w listy								*/
}

list_t *Data_GetTaskFromList(list_t *list)
{
	list_t *ptrTask = list->Current->Data;											/* odczyt wska�nika na list� ruch�w	(lista ruch�w = Task)				*/
	list_element_t *ptrNext = list->Current->Next;									/* odczyt wska�nika na poprzedni element								*/
	list->Current = ptrNext;														/* przesuni�cie wska�nika Current na poprzedni element					*/
	return ptrTask;																	/* zwr�cenie wska�nika na list� ruch�w									*/
}

move_t *Data_GetMoveFromList(list_t *list)
{
	move_t *ptrMove = list->Current->Data;											/* odczyt wska�nika na dane ruchu										*/
	list_element_t *ptrNext = list->Current->Next;									/* odczyt wska�nika na poprzedni element								*/
	list->Current = ptrNext;														/* przesuni�cie wska�nika Current na poprzedni element					*/
	return ptrMove;																	/* zwr�cenie wska�nika na dane ruchu									*/
}

void Data_DeleteElementFromList(list_t *list)
{
	if (list->Head)
	{
		list_element_t *ptrNext;													/* wska�nik na poprzedni element listy									*/
		ptrNext = list->Head->Next;													/* pobranie wska�nika na poprzedni element listy						*/
		free(list->Head->Data);														/* zwolnienie pami�ci zajmowanej przez dane elementu					*/
		
		list->Head->Next = NULL;													/* po zwolnieniu pami�ci wska�niki Data oraz Next wskazywa�y by na		*/
		list->Head->Data = NULL;													/* b��dne dane wi�c trzeba ustawi� je na NULL							*/
		
		free(list->Head);															/* zwolnienie pami�ci zajmowanej przez element listy					*/
		list->Head = ptrNext;														/* przesuni�cie wska�nika Head na poprzedni element						*/
		list->Current = ptrNext;													/* ustawienie wska�nika Current na ostatni element listy				*/
		list->Count--;																/* dekrementacja licznika element�w										*/
	}
}

void Data_ClearList(list_t *list)
{
	
	while(list->Head)																/* je�li na li�cie znajduj� si� jakie� elementy, to:					*/
	{
		Data_DeleteElementFromList(list);											/* usuni�cie elementu z listy											*/
	}
	list->Head = NULL;																/* po usuni�ciu wszystkich element�w z listy, wska�niki					*/
	list->Current = NULL;															/* pokazywa� b�d� b��dne dane, dlatego trzeba je wyzerowa�				*/
	list->Tail = NULL;
	list->Count = 0;
}

void Data_DeleteList(list_t *list)
{
	if (list != NULL)																/* je�li wska�nik nie jest pusty, to:									*/
	{
		free(list);																	/* zwolnij wskazywan� przez niego pami��								*/
	}
}

void Data_InsertMoveToJob(list_t *job, frame_t *frame)
{
	/* funkcja tworzy list� zagnie�d�on� w li�cie job */
	Data_InsertElementToList(job, Data_CreateListElement(Data_CreateList(), job->Head));
	
	/* funkcja umieszcza w zagnie�d�onej li�cie dane ruchu osi */
	Data_InsertElementToList(job->Head->Data, Data_CreateListElement(Data_CreateMove(*frame->Data1, atoi(frame->Data2), atoi(frame->Data3), atoi(frame->Data4)), NULL));
}

void Data_InsertTaskToJob(list_t *job, frame_t *frame, uint8_t islastmove)
{
	static uint8_t NeedNewList = 1;
	list_t *ptrList = job->Head->Data;
	
	if (NeedNewList)
	{
		/* funkcja tworzy list� zagnie�d�on� w li�cie job */
		Data_InsertElementToList(job, Data_CreateListElement(Data_CreateList(), job->Head));
		
		/* pobranie wska�nika na list� zagnie�d�on� */
		ptrList = job->Head->Data;
		
		/* nowa lista nie potrzebna (bo w�a�nie zosta�a utworzona */
		NeedNewList = 0;
	}
	
	/* funkcja umieszcza w zagnie�d�onej li�cie dane ruchu osi */
	Data_InsertElementToList(ptrList, Data_CreateListElement(Data_CreateMove(*frame->Data1, atoi(frame->Data2), atoi(frame->Data3), atoi(frame->Data4)), ptrList->Head));
	
	if (islastmove)
	{
		/* je�li wstawiony do listy zosta� ostatni ruch, to ustaw informacj� �e potrzebna jest nowa lista */
		NeedNewList = 1;
	}

}

void Data_ClearJob(void)
{
	Job->Current = Job->Head;										/* ustawienie wska�nika aktualnego elementu na pocz�tek listy							*/
	while(Job->Current)												/* do czasu gdy wska�nik na bie��cy element nie wskazuje null, wykonuj:					*/
	{
		Data_ClearList(Job->Current->Data);							/* zwolnij pami�� zajmowana przez sublist�												*/
		Job->Current = Job->Current->Next;							/* przesu� wska�nik na element nast�pny													*/
	}
	Data_ClearList(Job);											/* zwolnij pami�� zajmowan� przez list� JOB												*/
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------------*/