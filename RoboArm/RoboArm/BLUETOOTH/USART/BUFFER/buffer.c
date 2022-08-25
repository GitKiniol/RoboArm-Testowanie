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
	
	buffer = (buffer_t *)malloc(sizeof(buffer_t));								/* alokacja pamiêci dla bufora											*/
	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
	{
		buffer->Strings[i] = (char *)malloc(sizeof(char) * maxchars);			/* alokacja pamiêci dla elementów bufora								*/
		strcpy(buffer->Strings[i], "");											/* wype³nienie elementów bufora pustymi znakami							*/
	}
	buffer->DumyString = (char *)malloc(sizeof(char) * maxchars);				/* alokacja pamiêci dla ³añcucha pomocniczego							*/
	strcpy(buffer->DumyString, "");												/* wype³nienie ³añcucha pomocniczego pustymi znakami					*/
	buffer->Count = 0;															/* zerowanie licznika elementów											*/
	buffer->IsFull = 0;															/* zerowanie flagi nape³nienia bufora									*/
	buffer->IsEmpty = 1;														/* ustawienie flagi informuj¹cej ¿e bufor jest pusty					*/
	return buffer;																/* zwrócenie zainicjalizowanego bufora									*/
}

void Buffer_InsertString(buffer_t *buffer, char *str)
{
	if (buffer->Count < BUFFER_SIZE)											/* jeœli licznik elementów bufora mniejszy ni¿ rozmiar bufora to :		*/
	{
		strcpy(buffer->Strings[buffer->Count++], str);							/* dodaj element do bufora												*/
	}
	buffer->IsEmpty = 0;														/* zerowanie flagi informuj¹cej ¿e bufor jest pusty						*/
	if (buffer->Count == BUFFER_SIZE - 1) buffer->IsFull = 1;					/* ustawienie flagi informuj¹cej ¿e bufor jest pe³ny					*/
}

char *Buffer_GetString(buffer_t *buffer)
{
	strcpy(buffer->DumyString, buffer->Strings[0]);								/* kopiowanie pierwszego elementu bufora do zmiennej pomocniczej		*/
	for (uint8_t i = 0; i < buffer->Count; i++)									/* przesuwanie elementów buforze										*/
	{
		strcpy(buffer->Strings[i], buffer->Strings[i + 1]);
	}
	if (buffer->Count > 0) buffer->Count--;										/* dekrementacja licznika elementów bufora								*/
	buffer->IsFull = 0;															/* zerowanie flagi informuj¹cej ¿e bufor jest pe³ny						*/
	if (buffer->Count == 0) buffer->IsEmpty = 1;								/* ustawienie flagi informuj¹cej ¿e bufor jest pusty					*/
	return buffer->DumyString;													/* zwrócenie pierwszego elementu bufora									*/
}

void Buffer_Clear(buffer_t *buffer)
{
	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
	{
		strcpy(buffer->Strings[i], "");											/* wype³nienie elementów bufora pustymi znakami							*/
	}
	strcpy(buffer->DumyString, "");												/* wype³nienie ³añcucha pomocniczego pustymi znakami					*/
	buffer->Count = 0;															/* zerowanie licznika elementów											*/
	buffer->IsFull = 0;															/* zerowanie flagi nape³nienia bufora									*/
	buffer->IsEmpty = 1;														/* ustawienie flagi informuj¹cej ¿e bufor jest pusty					*/
}

/*------------------------------------------------------------------------------------------------------------------------------*/
