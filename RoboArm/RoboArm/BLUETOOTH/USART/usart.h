/*
 * usart.h
 *
 * Created: 2018-08-03 11:08:39
 *  Author: rybka
 */ 


#ifndef USART_H_
#define USART_H_

/*----------------------------------------------------------Makroinstrukcje-----------------------------------------------------*/

#define RECEIVE_MAX_CHARS		16							/* maksymalna ilo�� znak�w w odbieranym �a�cuchu					*/
#define SENDING_MAX_CHARS		16							/* maksymalna ilo�� znak�w	w nadawanym �a�cuchu					*/
#define CR						13							/* definicja znaku CR - caret return '\r'							*/
#define LF						10							/* definicja znaku LF - line feed '\n'								*/
#define NUL						0							/* definicja znaku NUL - nic '\0'									*/

/*------------------------------------------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------deklaracje zmiennych----------------------------------------------*/

/* EXTERN:	*/

/* LOCAL:	*/

/*------------------------------------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------Deklaracje funkcji-------------------------------------------------*/

/* USART:	*/
void Usart_Init(USART_t *module, PORT_t *moduleport, uint8_t nrtxpin, uint8_t nrrxpin);			/* inicjalizacja modu�u USART	*/

void Usart_SendString(char *str);										/* funkcja wysy�aj�ca znaki z podanego �a�cucha			*/

char *Usart_GetString(char znak);										/* funkcja zwraca �a�cuch ze znak�w odebranych z usart 	*/

/*------------------------------------------------------------------------------------------------------------------------------*/

#endif /* USART_H_ */