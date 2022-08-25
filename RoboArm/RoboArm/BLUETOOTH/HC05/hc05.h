/*
 * hc05.h
 *
 * Created: 09.02.2022 14:55:52
 *  Author: rybka
 */ 


#ifndef HC05_H_
#define HC05_H_

#include "../USART/usart.h"
#include "../FRAMES/frame.h"

/*-----------------------------------------------deklaracje makroinstrukcji-------------------------------------------------*/
/* warto�ci wysy�ane do telefonu : */
#define DATA_OK					"1\n"			/* dane odebrane															*/
#define CONNECTION_OK			"2\n"			/* sterownik zosta� pod��czony												*/
#define DISCONNECTION			"3\n"			/* sterownik zosta� od��czony												*/
#define DRIVER_ERR				"10\n"			/* awaria sterownika														*/

/* warto�ci odbierane z telefonu : */
#define TRANSFER				"1\n"			/* telefon poinformowa� �e zacznie wysy�a� dane								*/
#define CONNECT					"2\n"			/* telefon za��da� po��czenia ze sterownikiem								*/
#define DISCONNECT				"3\n"			/* telefon za��da� od��czenia sterownika									*/

/*--------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------struktura modu�u HC-05-----------------------------------------------------*/

typedef struct HC_05_S
{
	void (*SendString)(char *txt);						/* funkcja do wysy�ania pojedynczego �a�cucha znak�w				*/
	uint8_t (*SendStatus)(char *statusstate);			/* funkcja do wysy�ania statusu										*/
	void (*Read)(void);									/* funkcja odczytu danych											*/
}bluetooth_t;

/*--------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------Deklaracje zmiennych-------------------------------------------------------*/
/* EXTERN:	*/
extern bluetooth_t *Bluetooth;							/* modu� komunikacyjny												*/

/* LOCAL:	*/

/*--------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------deklaracje funkcji---------------------------------------------------------*/

bluetooth_t *HC05_Init(bluetooth_t *module);			/* inicjalizacja modu�u komunikacyjnego								*/

void HC05_SendString(char *txt);						/* funkcja wysy�aj�ca �a�cuch znak�w								*/

void HC05_Read(void);									/* funkcja odbiorcza												*/

uint8_t HC05_SendStatus(char *statusstate);				/*funkcja wysy�a status sterownika									*/

/*--------------------------------------------------------------------------------------------------------------------------*/



#endif /* HC05_H_ */
