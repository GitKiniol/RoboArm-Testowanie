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
list_t *Job;														/* lista zadañ utworzona z danych odebranych przez bluetooth z telefonu					*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------Definicje funkcji----------------------------------------------------------------------------------------------*/

move_t *Data_CreateMove(char axis, uint8_t angle, uint8_t speed, uint8_t dir)
{
	move_t *ptrMove;																/* deklaracja wskaŸnika na ruch											*/
	ptrMove = (move_t *)malloc(sizeof(move_t));										/* alokacja pamiêci dla wskaŸnika na ruch								*/
	ptrMove->AxisName = axis;														/* przypisanie litery osi												*/
	ptrMove->Angle = angle;															/* przypisanie k¹ta osi													*/
	ptrMove->Speed = speed;															/* przypisanie prêdkoœci osi											*/
	ptrMove->Direction = dir;														/* przypisanie kierunku osi												*/
	return ptrMove;																	/* zwrócenie adresu na alokowan¹ pamiêæ									*/
}

list_element_t *Data_CreateListElement(void *data, void *next)
{
	list_element_t *ptrElement;														/* deklaracja wskaŸnika na element listy								*/
	ptrElement = (list_element_t *)malloc(sizeof(list_element_t));					/* alokacja pamiêci dla elementu listy									*/
	ptrElement->Data = data;														/* ustawienie wskaŸnika na dane elementu listy							*/
	ptrElement->Next = next;														/* ustawienie wskaŸnika na nastêpny element listy						*/
	return ptrElement;																/* zwrócenie wskaŸnika na alokowan¹ pamiêæ								*/
}

list_t *Data_CreateList(void)
{
	list_t *ptrList;																/* deklaracja wskaŸnika na listê										*/
	ptrList = (list_t *)malloc(sizeof(list_t));										/* alokacja pamiêci dla listy											*/
	ptrList->Count = 0;																/* ustawienie licznika listy na 0										*/
	ptrList->Current = NULL;														/* ustawienie wskaŸnika na aktualny element listy						*/
	ptrList->Head = NULL;															/* ustawienie wskaŸnika na pierwszy element listy						*/
	ptrList->Tail = NULL;															/* ustawienie wskaŸnika na ostatni element listy						*/
	return ptrList;																	/* zwrócenie wskaŸnika na alokowan¹ pamiêæ								*/
}

void Data_InsertElementToList(list_t *list, list_element_t *element)
{
	if (list->Tail == NULL)															/* jeœli lista jest pusta, to:  (umieszczanie pierwszego elementu)		*/
	{
		list->Tail = element;														/* wstaw element na koniec listy										*/
		list->Tail->Next = NULL;													/* element na koñcu listy nie mo¿e ju¿ na nic wskazywaæ wiêc ma NULL	*/
		list->Head = list->Tail;													/* w liœcie jest jeden element wiêc pierwszy i ostatni s¹ tym samym		*/
	}
	else
	{
		element->Next = list->Head;													/* ustaw wstawianemu do listy elementowi wskaŸnik na poprzedni			*/
		list->Head = element;														/* ustaw wstawiany element na pocz¹tku listy							*/
	}
	list->Current = list->Head;														/* ustaw pierwszy element listy jako bie¿¹cy element					*/
	list->Count++;																	/* inkrementacja licznika elementów listy								*/
}

list_t *Data_GetTaskFromList(list_t *list)
{
	list_t *ptrTask = list->Current->Data;											/* odczyt wskaŸnika na listê ruchów	(lista ruchów = Task)				*/
	list_element_t *ptrNext = list->Current->Next;									/* odczyt wskaŸnika na poprzedni element								*/
	list->Current = ptrNext;														/* przesuniêcie wskaŸnika Current na poprzedni element					*/
	return ptrTask;																	/* zwrócenie wskaŸnika na listê ruchów									*/
}

move_t *Data_GetMoveFromList(list_t *list)
{
	move_t *ptrMove = list->Current->Data;											/* odczyt wskaŸnika na dane ruchu										*/
	list_element_t *ptrNext = list->Current->Next;									/* odczyt wskaŸnika na poprzedni element								*/
	list->Current = ptrNext;														/* przesuniêcie wskaŸnika Current na poprzedni element					*/
	return ptrMove;																	/* zwrócenie wskaŸnika na dane ruchu									*/
}

void Data_DeleteElementFromList(list_t *list)
{
	if (list->Head)
	{
		list_element_t *ptrNext;													/* wskaŸnik na poprzedni element listy									*/
		ptrNext = list->Head->Next;													/* pobranie wskaŸnika na poprzedni element listy						*/
		free(list->Head->Data);														/* zwolnienie pamiêci zajmowanej przez dane elementu					*/
		
		list->Head->Next = NULL;													/* po zwolnieniu pamiêci wskaŸniki Data oraz Next wskazywa³y by na		*/
		list->Head->Data = NULL;													/* b³êdne dane wiêc trzeba ustawiæ je na NULL							*/
		
		free(list->Head);															/* zwolnienie pamiêci zajmowanej przez element listy					*/
		list->Head = ptrNext;														/* przesuniêcie wskaŸnika Head na poprzedni element						*/
		list->Current = ptrNext;													/* ustawienie wskaŸnika Current na ostatni element listy				*/
		list->Count--;																/* dekrementacja licznika elementów										*/
	}
}

void Data_ClearList(list_t *list)
{
	
	while(list->Head)																/* jeœli na liœcie znajduj¹ siê jakieœ elementy, to:					*/
	{
		Data_DeleteElementFromList(list);											/* usuniêcie elementu z listy											*/
	}
	list->Head = NULL;																/* po usuniêciu wszystkich elementów z listy, wskaŸniki					*/
	list->Current = NULL;															/* pokazywaæ bêd¹ b³êdne dane, dlatego trzeba je wyzerowaæ				*/
	list->Tail = NULL;
	list->Count = 0;
}

void Data_DeleteList(list_t *list)
{
	if (list != NULL)																/* jeœli wskaŸnik nie jest pusty, to:									*/
	{
		free(list);																	/* zwolnij wskazywan¹ przez niego pamiêæ								*/
	}
}

void Data_InsertMoveToJob(list_t *job, frame_t *frame)
{
	/* funkcja tworzy listê zagnie¿d¿on¹ w liœcie job */
	Data_InsertElementToList(job, Data_CreateListElement(Data_CreateList(), job->Head));
	
	/* funkcja umieszcza w zagnie¿d¿onej liœcie dane ruchu osi */
	Data_InsertElementToList(job->Head->Data, Data_CreateListElement(Data_CreateMove(*frame->Data1, atoi(frame->Data2), atoi(frame->Data3), atoi(frame->Data4)), NULL));
}

void Data_InsertTaskToJob(list_t *job, frame_t *frame, uint8_t islastmove)
{
	static uint8_t NeedNewList = 1;
	list_t *ptrList = job->Head->Data;
	
	if (NeedNewList)
	{
		/* funkcja tworzy listê zagnie¿d¿on¹ w liœcie job */
		Data_InsertElementToList(job, Data_CreateListElement(Data_CreateList(), job->Head));
		
		/* pobranie wskaŸnika na listê zagnie¿d¿on¹ */
		ptrList = job->Head->Data;
		
		/* nowa lista nie potrzebna (bo w³aœnie zosta³a utworzona */
		NeedNewList = 0;
	}
	
	/* funkcja umieszcza w zagnie¿d¿onej liœcie dane ruchu osi */
	Data_InsertElementToList(ptrList, Data_CreateListElement(Data_CreateMove(*frame->Data1, atoi(frame->Data2), atoi(frame->Data3), atoi(frame->Data4)), ptrList->Head));
	
	if (islastmove)
	{
		/* jeœli wstawiony do listy zosta³ ostatni ruch, to ustaw informacjê ¿e potrzebna jest nowa lista */
		NeedNewList = 1;
	}

}

void Data_ClearJob(void)
{
	Job->Current = Job->Head;										/* ustawienie wskaŸnika aktualnego elementu na pocz¹tek listy							*/
	while(Job->Current)												/* do czasu gdy wskaŸnik na bie¿¹cy element nie wskazuje null, wykonuj:					*/
	{
		Data_ClearList(Job->Current->Data);							/* zwolnij pamiêæ zajmowana przez sublistê												*/
		Job->Current = Job->Current->Next;							/* przesuñ wskaŸnik na element nastêpny													*/
	}
	Data_ClearList(Job);											/* zwolnij pamiêæ zajmowan¹ przez listê JOB												*/
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------------*/