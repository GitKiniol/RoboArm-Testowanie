/*
 * data.h
 *
 * Created: 31.03.2022 11:20:10
 *  Author: rybka
 */ 


#ifndef DATA_H_
#define DATA_H_

#include "../FRAMES/frame.h"

/*-----------------------------------------------Deklaracje makroinstrukcji-----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------Definicje struktur danych------------------------------------------------------------------------------*/

typedef struct MOVE_STRUCT										/* struktura zawiera parametry ruchu pojedynczej osi									*/
{
	char AxisName;												/* literka oznaczaj�ca o�: Z,A,B,C,G lub T												*/
	uint8_t Angle;												/* k�t obrotu osi 0� - 90�																*/
	uint8_t Speed;												/* pr�dko�� obrotowa osi 0% - 100%														*/
	uint8_t Direction : 1;										/* kierunek obrot�w osi 1 - prawo, 0 - lewo												*/
}move_t;

typedef struct LIST_ELEMENT_STRUCT								/* element listy 																		*/
{
	void *Next;													/* wska�nik na kolejny element															*/
	void *Data;													/* wska�nik na dane elementu															*/
}list_element_t;

typedef struct LIST_STRUCT										/* lista powi�zana ruch�w i mutiruch�w													*/
{
	list_element_t *Head;										/* pierwszy element na li�cie															*/
	list_element_t *Tail;										/* ostatni element na li�cie															*/
	list_element_t *Current;									/* aktualny element																		*/
	uint8_t Count;												/* licznik element�w																	*/
}list_t;

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------Deklaracje zmiennych---------------------------------------------------------------------------------*/
/* EXTERN:	*/
extern list_t *Job;												/* lista zada� utworzona z danych odebranych przez bluetooth z telefonu					*/

/* LOCAL:	*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------Deklaracje funkcji---------------------------------------------------------------------------------*/

move_t *Data_CreateMove(char axis, uint8_t angle, uint8_t speed, uint8_t dir);	/* funkcja alokuje pami�� dla struktury typu Move						*/

list_element_t *Data_CreateListElement(void *data, void *next);					/* funkcja alokuje pami�� dla elementu listy							*/

list_t *Data_CreateList(void);													/* funkcja alokuje pami�� dla listy										*/

void Data_InsertElementToList(list_t *list, list_element_t *element);			/* funkcja umieszcza element na li�cie									*/

list_t *Data_GetTaskFromList(list_t *list);										/* funkcja pobiera list� ruch�w z listy zada�							*/

move_t *Data_GetMoveFromList(list_t *list);										/* funkcja pobiera ruch z listy ruch�w									*/

void Data_DeleteElementFromList(list_t *list);									/* funkcja zwalnia pami�� zajmowan� przez element listy					*/

void Data_ClearList(list_t *list);												/* funkcja zwalnia pami�� zajmowan� przez elementy w li�cie				*/

void Data_DeleteList(list_t *list);												/* funkcja zwalnia pami�� zajmowan� przez list�							*/

void Data_InsertMoveToJob(list_t *job, frame_t *frame);							/* funkcja dodaje do listy element z ruchem jednej osi					*/

void Data_InsertTaskToJob(list_t *job, frame_t *frame, uint8_t islastmove);		/* funkcja dodaje do listy element z ruchem wielu osi					*/

void Data_ClearJob(void);														/* zwalnianie pami�ci zajmowanej przez list� Job						*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/


#endif /* DATA_H_ */