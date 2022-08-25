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
char *StartCode = "SF";														/* kod pocz¹tku ramki						*/
char *EndCode = "EF";														/* kod koñca ramki							*/
char *FrameTypes[6] = { "STATUS", "MOVE", "TASK", "ETASK", "JOB", "EJOB" };	/* typy ramek								*/

/*----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------Definicje funkcji-------------------------------------------------------------------*/

frame_t *Frame_Init(frame_t *frame)
{
	frame = (frame_t *)malloc(sizeof(frame_t));								/* alokacja pamiêci dla ramki				*/
	frame->StartCode = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);	/* alokacja pamiêci na kodu startu  ramki	*/
	strcpy(frame->StartCode, "");											/* zawartoœæ pocz¹tkowa kodu startu  ramki	*/
	frame->FrameType = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);	/* alokacja pamiêci na typ ramki			*/
	strcpy(frame->FrameType, "");											/* zawartoœæ pocz¹tkowa typu ramki			*/
	frame->Data1 = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);		/* alokacja pamiêci na dane ramki			*/
	strcpy(frame->Data1, "");												/* zawartoœæ pocz¹tkowa danych ramki		*/
	frame->Data2 = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);		/* alokacja pamiêci na dane ramki			*/
	strcpy(frame->Data2, "");												/* zawartoœæ pocz¹tkowa danych ramki		*/
	frame->Data3 = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);		/* alokacja pamiêci na dane ramki			*/
	strcpy(frame->Data3, "");												/* zawartoœæ pocz¹tkowa danych ramki		*/
	frame->Data4 = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);		/* alokacja pamiêci na dane ramki			*/
	strcpy(frame->Data4, "");												/* zawartoœæ pocz¹tkowa danych ramki		*/
	frame->EndCode = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);		/* alokacja pamiêci na kod koñca ramki		*/
	strcpy(frame->EndCode, "");												/* zawartoœæ pocz¹tkowa kodu koñca ramki	*/
	return frame;
}

uint8_t Frame_Fill(buffer_t *buffer, frame_t *frame)
{
	/* Funkcja kopiuje dane z bufora do zmiennej typu frame i zwraca 1 gdy ramka jest OK lub 0 gdy ramka uszkodzona		*/
	
	strcpy(frame->StartCode, Buffer_GetString(buffer));						/* pobranie kodu ramki						*/
	if (!(strncmp(StartCode, frame->StartCode, 2)))							/* jeœli kod ramki jest poprawny to:		*/
	{
		strcpy(frame->FrameType, Buffer_GetString(buffer));					/* pobierz typ ramki						*/
		strcpy(frame->Data1, Buffer_GetString(buffer));						/* pobierz pierwszy wiersz danych			*/
		strcpy(frame->Data2, Buffer_GetString(buffer));						/* pobierz drugi wiersz danych				*/
		strcpy(frame->Data3, Buffer_GetString(buffer));						/* pobierz trzeci wiersz danych				*/
		strcpy(frame->Data4, Buffer_GetString(buffer));						/* pobierz czwarty wiersz danych			*/
		strcpy(frame->Data5, Buffer_GetString(buffer));						/* pobierz pi¹ty wiersz danych				*/
		strcpy(frame->EndCode, Buffer_GetString(buffer));					/* pobierz kod koñca ramki					*/
		if (!(strncmp(EndCode, frame->EndCode, 2)))							/* jeœli kod koñca ramki jest poprawny to:	*/
		{
			Buffer_Clear(buffer);											/* czyszczenie bufora						*/
			return 1;														/* zwróæ 1 (odczyt OK)						*/													
		}
		else
		{
			Buffer_Clear(buffer);											/* czyszczenie bufora						*/
			return 0;														/* jeœli kod koñca nie w³aœciwy to zwróæ 0	*/
		}
	}
	else
	{
		Buffer_Clear(buffer);												/* czyszczenie bufora						*/
		return 0;															/* jeœli kod startu nie w³aœciwy to zwróæ 0	*/
	}
}

uint8_t Frame_TypeCheck(frame_t *frame)
{
	/* funkcja sprawdza jaki typ ramki zosta³ odebrany i zwraca liczbê która mu odpowiada				*/
	if (!(strncmp(FrameTypes[5], frame->FrameType, 3))) return 6;			/* jeœli typ ramki to: EJOB, zwróæ 6		*/	
	else if(!(strncmp(FrameTypes[4], frame->FrameType, 3))) return 5;		/* jeœli typ ramki to: JOB, zwróæ 5			*/
	else if(!(strncmp(FrameTypes[3], frame->FrameType, 3))) return 4;		/* jeœli typ ramki to: ETASK, zwróæ 4		*/
	else if(!(strncmp(FrameTypes[2], frame->FrameType, 3))) return 3;		/* jeœli typ ramki to: TASK, zwróæ 3		*/
	else if(!(strncmp(FrameTypes[1], frame->FrameType, 3))) return 2;		/* jeœli typ ramki to: MOVE, zwróæ 2		*/
	else return 1;															/* jeœli typ ramki to: STATUS, zwróæ 1		*/
}

uint8_t Frame_StatusCheck(char *status)
{
	if (!(strncmp(status, "1", 1))) return 1;								/* nast¹pi przesy³ danych					*/
	else if (!(strncmp(status, "2", 1))) return 2;							/* nast¹pi pod³¹czenie sterownika			*/
	else if (!(strncmp(status, "3", 1))) return 3;							/* nast¹pi od³¹czenie sterownika			*/
	else return 99;															/* nieznany status							*/
}

/*----------------------------------------------------------------------------------------------------------------------*/
