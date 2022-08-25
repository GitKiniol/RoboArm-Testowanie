/*
 * frame.c
 *
 * Created: 07.02.2022 18:56:46
 *  Author: rybka
 */ 


#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include "frame.h"
#include "../USART/usart.h"
#include "../USART/BUFFER/buffer.h"

/*----------------------------------Deklaracje zmiennych----------------------------------------------------------------*/
/* EXTERN:	*/
frame_t *ReceivingFrame = NULL;												/* ramka odbiorcza							*/
frame_t *SendingFrame = NULL;												/* ramka nadawcza							*/

/* LOCAL:	*/
char *StartCode = "SF";														/* kod pocz�tku ramki						*/
char *EndCode = "EF";														/* kod ko�ca ramki							*/
char *FrameTypes[6] = { "STATUS", "MOVE", "TASK", "ETASK", "JOB", "EJOB" };	/* typy ramek								*/

/*----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------Definicje funkcji-------------------------------------------------------------------*/

frame_t *Frame_Init(frame_t *frame)
{
	frame = (frame_t *)malloc(sizeof(frame_t));								/* alokacja pami�ci dla ramki				*/
	frame->StartCode = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);	/* alokacja pami�ci na kodu startu  ramki	*/
	strcpy(frame->StartCode, "");											/* zawarto�� pocz�tkowa kodu startu  ramki	*/
	frame->FrameType = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);	/* alokacja pami�ci na typ ramki			*/
	strcpy(frame->FrameType, "");											/* zawarto�� pocz�tkowa typu ramki			*/
	frame->Data1 = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);		/* alokacja pami�ci na dane ramki			*/
	strcpy(frame->Data1, "");												/* zawarto�� pocz�tkowa danych ramki		*/
	frame->Data2 = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);		/* alokacja pami�ci na dane ramki			*/
	strcpy(frame->Data2, "");												/* zawarto�� pocz�tkowa danych ramki		*/
	frame->Data3 = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);		/* alokacja pami�ci na dane ramki			*/
	strcpy(frame->Data3, "");												/* zawarto�� pocz�tkowa danych ramki		*/
	frame->Data4 = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);		/* alokacja pami�ci na dane ramki			*/
	strcpy(frame->Data4, "");												/* zawarto�� pocz�tkowa danych ramki		*/
	frame->EndCode = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);		/* alokacja pami�ci na kod ko�ca ramki		*/
	strcpy(frame->EndCode, "");												/* zawarto�� pocz�tkowa kodu ko�ca ramki	*/
	return frame;
}

uint8_t Frame_Fill(buffer_t *buffer, frame_t *frame)
{
	/* Funkcja kopiuje dane z bufora do zmiennej typu frame i zwraca 1 gdy ramka jest OK lub 0 gdy ramka uszkodzona		*/
	
	strcpy(frame->StartCode, Buffer_GetString(buffer));						/* pobranie kodu ramki						*/
	if (!(strncmp(StartCode, frame->StartCode, 2)))							/* je�li kod ramki jest poprawny to:		*/
	{
		strcpy(frame->FrameType, Buffer_GetString(buffer));					/* pobierz typ ramki						*/
		strcpy(frame->Data1, Buffer_GetString(buffer));						/* pobierz pierwszy wiersz danych			*/
		strcpy(frame->Data2, Buffer_GetString(buffer));						/* pobierz drugi wiersz danych				*/
		strcpy(frame->Data3, Buffer_GetString(buffer));						/* pobierz trzeci wiersz danych				*/
		strcpy(frame->Data4, Buffer_GetString(buffer));						/* pobierz czwarty wiersz danych			*/
		strcpy(frame->Data5, Buffer_GetString(buffer));						/* pobierz pi�ty wiersz danych				*/
		strcpy(frame->EndCode, Buffer_GetString(buffer));					/* pobierz kod ko�ca ramki					*/
		if (!(strncmp(EndCode, frame->EndCode, 2)))							/* je�li kod ko�ca ramki jest poprawny to:	*/
		{
			Buffer_Clear(buffer);											/* czyszczenie bufora						*/
			return 1;														/* zwr�� 1 (odczyt OK)						*/													
		}
		else
		{
			Buffer_Clear(buffer);											/* czyszczenie bufora						*/
			return 0;														/* je�li kod ko�ca nie w�a�ciwy to zwr�� 0	*/
		}
	}
	else
	{
		Buffer_Clear(buffer);												/* czyszczenie bufora						*/
		return 0;															/* je�li kod startu nie w�a�ciwy to zwr�� 0	*/
	}
}

uint8_t Frame_TypeCheck(frame_t *frame)
{
	/* funkcja sprawdza jaki typ ramki zosta� odebrany i zwraca liczb� kt�ra mu odpowiada				*/
	if (!(strncmp(FrameTypes[5], frame->FrameType, 3))) return 6;			/* je�li typ ramki to: EJOB, zwr�� 6		*/	
	else if(!(strncmp(FrameTypes[4], frame->FrameType, 3))) return 5;		/* je�li typ ramki to: JOB, zwr�� 5			*/
	else if(!(strncmp(FrameTypes[3], frame->FrameType, 3))) return 4;		/* je�li typ ramki to: ETASK, zwr�� 4		*/
	else if(!(strncmp(FrameTypes[2], frame->FrameType, 3))) return 3;		/* je�li typ ramki to: TASK, zwr�� 3		*/
	else if(!(strncmp(FrameTypes[1], frame->FrameType, 3))) return 2;		/* je�li typ ramki to: MOVE, zwr�� 2		*/
	else return 1;															/* je�li typ ramki to: STATUS, zwr�� 1		*/
}

uint8_t Frame_StatusCheck(char *status)
{
	if (!(strncmp(status, "1", 1))) return 1;								/* nast�pi przesy� danych					*/
	else if (!(strncmp(status, "2", 1))) return 2;							/* nast�pi pod��czenie sterownika			*/
	else if (!(strncmp(status, "3", 1))) return 3;							/* nast�pi od��czenie sterownika			*/
	else return 99;															/* nieznany status							*/
}

/*----------------------------------------------------------------------------------------------------------------------*/
