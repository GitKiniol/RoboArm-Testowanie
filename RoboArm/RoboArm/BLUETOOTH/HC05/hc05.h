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
/* wartoœci wysy³ane do telefonu : */
#define DATA_OK					"1\n"			/* dane odebrane															*/
#define CONNECTION_OK			"2\n"			/* sterownik zosta³ pod³¹czony												*/
#define DISCONNECTION			"3\n"			/* sterownik zosta³ od³¹czony												*/
#define DRIVER_ERR				"10\n"			/* awaria sterownika														*/

/* wartoœci odbierane z telefonu : */
#define TRANSFER				"1\n"			/* telefon poinformowa³ ¿e zacznie wysy³aæ dane								*/
#define CONNECT					"2\n"			/* telefon za¿¹da³ po³¹czenia ze sterownikiem								*/
#define DISCONNECT				"3\n"			/* telefon za¿¹da³ od³¹czenia sterownika									*/

/*--------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------struktura modu³u HC-05-----------------------------------------------------*/

typedef struct HC_05_S
{
	void (*SendString)(char *txt);						/* funkcja do wysy³ania pojedynczego ³añcucha znaków				*/
	uint8_t (*SendStatus)(char *statusstate);			/* funkcja do wysy³ania statusu										*/
	void (*Read)(void);									/* funkcja odczytu danych											*/
}bluetooth_t;

/*--------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------Deklaracje zmiennych-------------------------------------------------------*/
/* EXTERN:	*/
extern bluetooth_t *Bluetooth;							/* modu³ komunikacyjny												*/

/* LOCAL:	*/

/*--------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------deklaracje funkcji---------------------------------------------------------*/

bluetooth_t *HC05_Init(bluetooth_t *module);			/* inicjalizacja modu³u komunikacyjnego								*/

void HC05_SendString(char *txt);						/* funkcja wysy³aj¹ca ³añcuch znaków								*/

void HC05_Read(void);									/* funkcja odbiorcza												*/

uint8_t HC05_SendStatus(char *statusstate);				/*funkcja wysy³a status sterownika									*/

/*--------------------------------------------------------------------------------------------------------------------------*/



#endif /* HC05_H_ */
