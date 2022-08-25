/*
 * frame.h
 *
 * Created: 07.02.2022 18:57:05
 *  Author: rybka
 */ 


#ifndef FRAME_H_
#define FRAME_H_

#include "../USART/usart.h"
#include "../USART/BUFFER/buffer.h"

/*-----------------------------------------------struktura ramki--------------------------------------------------------*/

typedef struct FRAME_S
{
	char *StartCode;							/* kod pocz¹tku ramki													*/
	char *FrameType;							/* typ ramki															*/
	char *Data1;								/* dane ramki															*/
	char *Data2;								/* dane ramki															*/
	char *Data3;								/* dane ramki															*/
	char *Data4;								/* dane ramki															*/
	char *Data5;								/* dane ramki															*/
	char *EndCode;								/* kod koñca ramki														*/
}frame_t;

/*----------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------Deklaracje zmiennych---------------------------------------------------*/
/* EXTERN:	*/
extern frame_t *ReceivingFrame;					/* ramka odbiorcza														*/
extern frame_t *SendingFrame;					/* ramka nadawcza														*/

/* LOCAL:	*/

/*----------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------deklaracje funkcji-----------------------------------------------------*/

frame_t *Frame_Init(frame_t *frame);							/* inicjalizacja ramki danych							*/

uint8_t Frame_Fill(buffer_t *buffer, frame_t *frame);			/* zapis danych z bufora do ramki						*/

uint8_t Frame_TypeCheck(frame_t *frame);						/* sprawdzenie typu ramki								*/

uint8_t Frame_StatusCheck(char *status);						/* sprawdzenie odebranego statusu						*/						

/*----------------------------------------------------------------------------------------------------------------------*/


#endif /* FRAME_H_ */