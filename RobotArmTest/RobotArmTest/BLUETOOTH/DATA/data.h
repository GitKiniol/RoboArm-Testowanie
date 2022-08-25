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
	char AxisName;												/* literka oznaczaj¹ca oœ: Z,A,B,C,G lub T												*/
	uint8_t Angle;												/* k¹t obrotu osi 0° - 90°																*/
	uint8_t Speed;												/* prêdkoœæ obrotowa osi 0% - 100%														*/
	uint8_t Direction : 1;										/* kierunek obrotów osi 1 - prawo, 0 - lewo												*/
}move_t;

typedef struct LIST_ELEMENT_STRUCT								/* element listy 																		*/
{
	void *Next;													/* wskaŸnik na kolejny element															*/
	void *Data;													/* wskaŸnik na dane elementu															*/
}list_element_t;

typedef struct LIST_STRUCT										/* lista powi¹zana ruchów i mutiruchów													*/
{
	list_element_t *Head;										/* pierwszy element na liœcie															*/
	list_element_t *Tail;										/* ostatni element na liœcie															*/
	list_element_t *Current;									/* aktualny element																		*/
	uint8_t Count;												/* licznik elementów																	*/
}list_t;

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------Deklaracje zmiennych---------------------------------------------------------------------------------*/
/* EXTERN:	*/
extern list_t *Job;												/* lista zadañ utworzona z danych odebranych przez bluetooth z telefonu					*/

/* LOCAL:	*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------Deklaracje funkcji---------------------------------------------------------------------------------*/

move_t *Data_CreateMove(char axis, uint8_t angle, uint8_t speed, uint8_t dir);	/* funkcja alokuje pamiêæ dla struktury typu Move						*/

list_element_t *Data_CreateListElement(void *data, void *next);					/* funkcja alokuje pamiêæ dla elementu listy							*/

list_t *Data_CreateList(void);													/* funkcja alokuje pamiêæ dla listy										*/

void Data_InsertElementToList(list_t *list, list_element_t *element);			/* funkcja umieszcza element na liœcie									*/

list_t *Data_GetTaskFromList(list_t *list);										/* funkcja pobiera listê ruchów z listy zadañ							*/

move_t *Data_GetMoveFromList(list_t *list);										/* funkcja pobiera ruch z listy ruchów									*/

void Data_DeleteElementFromList(list_t *list);									/* funkcja zwalnia pamiêæ zajmowan¹ przez element listy					*/

void Data_ClearList(list_t *list);												/* funkcja zwalnia pamiêæ zajmowan¹ przez elementy w liœcie				*/

void Data_DeleteList(list_t *list);												/* funkcja zwalnia pamiêæ zajmowan¹ przez listê							*/

void Data_InsertMoveToJob(list_t *job, frame_t *frame);							/* funkcja dodaje do listy element z ruchem jednej osi					*/

void Data_InsertTaskToJob(list_t *job, frame_t *frame, uint8_t islastmove);		/* funkcja dodaje do listy element z ruchem wielu osi					*/

void Data_ClearJob(void);														/* zwalnianie pamiêci zajmowanej przez listê Job						*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/


#endif /* DATA_H_ */