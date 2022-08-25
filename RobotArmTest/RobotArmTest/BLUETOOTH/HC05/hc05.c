/*
 * hc05.c
 *
 * Created: 10.02.2022 17:41:30
 *  Author: rybka
 */ 


#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "hc05.h"
#include "../DATA/data.h"
#include "../USART/BUFFER/buffer.h"
#include "../../CONTROL/WORK/work.h"

/*-------------------------------------------Deklaracje zmiennych---------------------------------------------------------------*/
/* EXTERN: */
bluetooth_t *Bluetooth = NULL;											/* modu� komunikacyjny									*/

/*------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------Definicje funkcji------------------------------------------------------------------*/

bluetooth_t *HC05_Init(bluetooth_t *module)
{
	module = (bluetooth_t *)malloc(sizeof(bluetooth_t));				/* alokacja pami�ci dla modu�u bluetooth				*/
	ReceivingBuffer = Buffer_Init(ReceivingBuffer, RECEIVE_MAX_CHARS);	/*inicjalizacja bufora odbiorczego						*/
		//Usart_Init(&USARTD1, &PORTD, PIN7_bp, PIN6_bp);					/* inicjalizacja sprz�towego uart'u						*/
		Usart_Init(&USARTD0, &PORTD, PIN3_bp, PIN2_bp);					/* inicjalizacja sprz�towego uart'u						*/
		
	ReceivingFrame = Frame_Init(ReceivingFrame);						/* inicjalizacja obs�ugi ramek danych					*/
	module->Read = &HC05_Read;											/* ustawienie wska�nika na funkcj� odbiorcz�			*/
	module->SendString = &HC05_SendString;								/* ustawienie wska�nika na funkcj� wysy�aj�c� tekst		*/
	module->SendStatus = &HC05_SendStatus;								/* ustawienia wska�nika na funkcj� wysy�aj�c� status	*/
	
	return module;														/* zwr�cenie wska�nika na zainicjowany modu�			*/
}

void HC05_SendString(char *txt)
{
	Usart_SendString(txt);												/* wys�anie tekstu przez bluetooth						*/
}

void HC05_Read(void)
{
	if (ReceivingBuffer->IsFull == TRUE)								/* je�li bufor odbiorczy pe�ny, to:						*/
	{
		if (Frame_Fill(ReceivingBuffer, ReceivingFrame))
		{
			switch(Frame_TypeCheck(ReceivingFrame))
			{
				case 1:
					switch(Frame_StatusCheck(ReceivingFrame->Data1))
					{
						case 1:
							HC05_SendStatus("1\n");
							break;
						case 2:
							HC05_SendStatus("2\n");
							break;
						case 3:
							HC05_SendStatus("3\n");
							//Sys_Reset();
							break;
						default:
							break;
					}
					break;
				case 2:
					Data_InsertMoveToJob(Job, ReceivingFrame);
					HC05_SendStatus("1\n");
					break;
				case 3:
					Data_InsertTaskToJob(Job, ReceivingFrame, 0);
					HC05_SendStatus("1\n");
					break;
				case 4:
					Data_InsertTaskToJob(Job, ReceivingFrame, 1);
					HC05_SendStatus("1\n");
					break;
				case 5:
					HC05_SendStatus("4\n");
					Work_RunRobot();
					break;
				case 6:
					HC05_SendStatus("5\n");
					Work_StopRobot();
					break;
				default:
					break;
			}
		} 
		else
		{
			HC05_SendStatus("99\n");
		}
	}
}

uint8_t HC05_SendStatus(char *statusstate)
{
	Usart_SendString("SF\n");
	_delay_ms(7);
	Usart_SendString("STATUS\n");
	_delay_ms(17);
	Usart_SendString(statusstate);
	_delay_ms(7);
	Usart_SendString("0\n");
	_delay_ms(7);
	Usart_SendString("0\n");
	_delay_ms(7);
	Usart_SendString("0\n");
	_delay_ms(7);
	Usart_SendString("0\n");
	_delay_ms(7);
	Usart_SendString("EF\n");
	_delay_ms(7);
	return 1;
}

/*------------------------------------------------------------------------------------------------------------------------------*/