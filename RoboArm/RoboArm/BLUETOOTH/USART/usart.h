/*
 * usart.h
 *
 * Created: 2018-08-03 11:08:39
 *  Author: rybka
 */ 


#ifndef USART_H_
#define USART_H_

/*----------------------------------------------------------Makroinstrukcje-----------------------------------------------------*/

#define RECEIVE_MAX_CHARS		16							/* maksymalna iloœæ znaków w odbieranym ³añcuchu					*/
#define SENDING_MAX_CHARS		16							/* maksymalna iloœæ znaków	w nadawanym ³añcuchu					*/
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
void Usart_Init(USART_t *module, PORT_t *moduleport, uint8_t nrtxpin, uint8_t nrrxpin);			/* inicjalizacja modu³u USART	*/

void Usart_SendString(char *str);										/* funkcja wysy³aj¹ca znaki z podanego ³añcucha			*/

char *Usart_GetString(char znak);										/* funkcja zwraca ³añcuch ze znaków odebranych z usart 	*/

/*------------------------------------------------------------------------------------------------------------------------------*/

#endif /* USART_H_ */