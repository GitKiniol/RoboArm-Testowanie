/*
 * buffer.c
 *
 * Created: 11.02.2022 21:36:41
 *  Author: rybka
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"

/*----------------------------------Deklaracje zmiennych------------------------------------------------------------------------*/
/* EXTERN:	*/
buffer_t *ReceivingBuffer = NULL;							/* bufor odbiorczy													*/
buffer_t *SendingBuffer = NULL;								/* bufor nadawczy													*/

/*------------------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------Definicje funkcji-----------------------------------------------------------------------------*/

buffer_t *Buffer_Init(buffer_t *buffer, uint8_t maxchars)
{
	
	buffer = (buffer_t *)malloc(sizeof(buffer_t));								/* alokacja pami�ci dla bufora											*/
	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
	{
		buffer->Strings[i] = (char *)malloc(sizeof(char) * maxchars);			/* alokacja pami�ci dla element�w bufora								*/
		strcpy(buffer->Strings[i], "");											/* wype�nienie element�w bufora pustymi znakami							*/
	}
	buffer->DumyString = (char *)malloc(sizeof(char) * maxchars);				/* alokacja pami�ci dla �a�cucha pomocniczego							*/
	strcpy(buffer->DumyString, "");												/* wype�nienie �a�cucha pomocniczego pustymi znakami					*/
	buffer->Count = 0;															/* zerowanie licznika element�w											*/
	buffer->IsFull = 0;															/* zerowanie flagi nape�nienia bufora									*/
	buffer->IsEmpty = 1;														/* ustawienie flagi informuj�cej �e bufor jest pusty					*/
	return buffer;																/* zwr�cenie zainicjalizowanego bufora									*/
}

void Buffer_InsertString(buffer_t *buffer, char *str)
{
	if (buffer->Count < BUFFER_SIZE)											/* je�li licznik element�w bufora mniejszy ni� rozmiar bufora to :		*/
	{
		strcpy(buffer->Strings[buffer->Count++], str);							/* dodaj element do bufora												*/
	}
	buffer->IsEmpty = 0;														/* zerowanie flagi informuj�cej �e bufor jest pusty						*/
	if (buffer->Count == BUFFER_SIZE - 1) buffer->IsFull = 1;					/* ustawienie flagi informuj�cej �e bufor jest pe�ny					*/
}

char *Buffer_GetString(buffer_t *buffer)
{
	strcpy(buffer->DumyString, buffer->Strings[0]);								/* kopiowanie pierwszego elementu bufora do zmiennej pomocniczej		*/
	for (uint8_t i = 0; i < buffer->Count; i++)									/* przesuwanie element�w buforze										*/
	{
		strcpy(buffer->Strings[i], buffer->Strings[i + 1]);
	}
	if (buffer->Count > 0) buffer->Count--;										/* dekrementacja licznika element�w bufora								*/
	buffer->IsFull = 0;															/* zerowanie flagi informuj�cej �e bufor jest pe�ny						*/
	if (buffer->Count == 0) buffer->IsEmpty = 1;								/* ustawienie flagi informuj�cej �e bufor jest pusty					*/
	return buffer->DumyString;													/* zwr�cenie pierwszego elementu bufora									*/
}

void Buffer_Clear(buffer_t *buffer)
{
	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
	{
		strcpy(buffer->Strings[i], "");											/* wype�nienie element�w bufora pustymi znakami							*/
	}
	strcpy(buffer->DumyString, "");												/* wype�nienie �a�cucha pomocniczego pustymi znakami					*/
	buffer->Count = 0;															/* zerowanie licznika element�w											*/
	buffer->IsFull = 0;															/* zerowanie flagi nape�nienia bufora									*/
	buffer->IsEmpty = 1;														/* ustawienie flagi informuj�cej �e bufor jest pusty					*/
}

/*------------------------------------------------------------------------------------------------------------------------------*/
