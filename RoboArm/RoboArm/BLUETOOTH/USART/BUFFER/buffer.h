/*
 * buffer.h
 *
 * Created: 11.02.2022 21:36:57
 *  Author: rybka
 */ 


#ifndef BUFFER_H_
#define BUFFER_H_


/*----------------------------------------------------------Makroinstrukcje-----------------------------------------------------*/

#define BUFFER_SIZE				9							/* maksymalna ilo�� element�w w buforze								*/
//#define TRUE					1							/* definicja warto�ci prawda (1)									*/
//#define FALSE					0							/* definicja warto�ci fa�sz (0)										*/

/*------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------struktura bufora---------------------------------------------------*/

typedef struct BUFFER_struct
{
	char *Strings[BUFFER_SIZE];								/* tablica string�w													*/
	char *DumyString;										/* string pomocniczy u�ywany podczas operacji na powy�szej tablicy	*/
	uint8_t Count;											/* licznik element�w												*/
	uint8_t IsFull :1;										/* flaga informuj�ca �e bufor jest pe�ny							*/
	uint8_t IsEmpty :1;										/* flaga informuj�ca �e bufor jest pusty							*/
	
}buffer_t;

/*------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------deklaracje zmiennych----------------------------------------------*/

/* EXTERN:	*/
extern buffer_t *ReceivingBuffer;							/* bufor odbiorczy													*/
extern buffer_t *SendingBuffer;								/* bufor nadawczy													*/

/* LOCAL:	*/

/*------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------Deklaracje funkcji-------------------------------------------------*/

buffer_t *Buffer_Init(buffer_t *buffer, uint8_t maxchars);		/* inicjalizacja bufora											*/

void Buffer_InsertString(buffer_t *buffer, char *str);			/* funkcja umieszcza �a�cuch w buforze							*/

char *Buffer_GetString(buffer_t *buffer);						/* funkcja zwraca i usuwa �a�cuch z bufora						*/

void Buffer_Clear(buffer_t *buffer);							/* funkcja czy�ci bufor											*/

/*------------------------------------------------------------------------------------------------------------------------------*/

#endif /* BUFFER_H_ */