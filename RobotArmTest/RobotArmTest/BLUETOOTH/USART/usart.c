/*
 * usart.c
 *
 * Created: 2018-08-03 11:08:58
 *  Author: rybka
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"
#include "BUFFER/buffer.h"


/*----------------------------------Deklaracje zmiennych------------------------------------------------------------------------------------------------*/
/* LOCAL	*/
USART_t *RS232Port = NULL;														/* wskaŸnik na port komunikacyjny										*/ 
char *ReceivedString;															/* ³añcuch odebrany z usart												*/
char *SendingString;															/* ³añcuch znaków do wys³ania przez usart								*/
char ReceivedChars[RECEIVE_MAX_CHARS];											/* tablica znaków odebranych z usart									*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------Definicje funkcji---------------------------------------------------------------------------------------------------*/

void Usart_Init(USART_t *module, PORT_t *moduleport, uint8_t nrtxpin, uint8_t nrrxpin)
{	
	RS232Port = module;															/* ustawienie wskaŸnika na modu³ usart									*/
	
	moduleport->OUTSET = (1<<nrtxpin);											/* pin RX w stan wysoki													*/
	moduleport->DIRSET = (1<<nrtxpin);											/* pin RX jako wyjœcie													*/
	moduleport->DIRCLR = (1<<nrrxpin);											/* pin TX jako wejœcie													*/
	
	RS232Port->BAUDCTRLA = 12;													/* BSEL BITY - 0:7	(ustawienie prêdkoœci transmisji)					*/
	RS232Port->BAUDCTRLB = 64;													/* BSEL BITY - 8:11, BSCALE BITY - 0:3(ustawienie prêdkoœci transmisji) */
	RS232Port->CTRLC = USART_CHSIZE_8BIT_gc;									/* ustawienie ramki danych: 8bitów danych 1bit stopu, bez parzystoœci	*/
	RS232Port->CTRLA = (USART_RXCINTLVL_MED_gc);								/* ustawienie priorytetu przerwania receive								*/
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;												/* globalne odblokowanie przerwañ œredniego priorytetu					*/
	RS232Port->CTRLB |= (USART_RXEN_bm | USART_TXEN_bm);						/* odblokowanie odbiornika i nadajnika USATR							*/

	/* inicjalizacja tablicy znaków	*/
	for (uint8_t i = 0; i < RECEIVE_MAX_CHARS; i++)
	{
		ReceivedChars[i] = 0;
	}
	/* inicjalizacja ³añcuchów, odbiorczego i nadawczego	*/
	ReceivedString = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);			/* alokacja pamiêci dla ³añcucha odbiorczego							*/
	SendingString = (char *)malloc(sizeof(char) * SENDING_MAX_CHARS);			/* alokacja pamiêci dla ³añcucha nadawczego								*/
	
}

void Usart_SendString(char *str)
{
	strcpy(SendingString, str);													/* przygotowanie wskazanego ³añcucha do wys³ania						*/
	RS232Port->CTRLA |= (USART_DREINTLVL1_bm);									/* ustawienie priorytetu przerwania data reg empty						*/
}

char *Usart_GetString(char znak)
{
	static uint8_t CharsCount = 0;												/* licznik znaków odebranych z usart									*/
	
	if (znak > 31 && znak < 127)												/* jeœli odebrany znak jest drukowalny to:								*/
	{
		ReceivedChars[CharsCount++] = znak;										/* dodaj znak do tablicy												*/
	}
	
	if (znak == CR || znak == NUL || CharsCount >= RECEIVE_MAX_CHARS - 3)		/* jeœli wykryty zostanie koniec ³añcucha to:							*/
	{
		ReceivedChars[CharsCount++] = LF;										/* dodaj znak nowego wiersza											*/
		ReceivedChars[CharsCount++] = CR;										/* dodaj znak powrotu karetki											*/
		ReceivedChars[CharsCount] = NUL;										/* dodaj znak NUL koniec wiersza										*/
		CharsCount = 0;															/* zeruj licznik odebranych znaków										*/
		return &ReceivedChars[0];												/* zwróæ wskaŸnik na tablicê odebranych znaków							*/
	} 
	else																		/* a jeœli koniec wiersza nie zosta³ wykryty to :						*/
	{
		return NULL;															/* zwróæ wartoœæ pust¹(odbiór wiersza jeszcze siê nie zakoñczy³)		*/
	}
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/


/*---------------------------PRZERWANIA-----------------------------------------------------------------------------------------------------------------*/

//ISR(USARTD1_DRE_vect)
//{
	//static uint8_t i = 0;																					/* licznik znaków														*/
	//if (!SendingString[i] || (SendingString[i] < 32 && SendingString[i] != 10 && SendingString[i] != 13))	/* jeœli znak NUL lub niedrukowalny to zakoñcz wysy³anie				*/
	//{
		//RS232Port->DATA = 0;																				/* wyœlij NUL, znak koñcz¹cy ³añcuch									*/
		//RS232Port->CTRLA &= ~(USART_DREINTLVL1_bm);															/* zablokowanie przerwania data reg empty								*/
		//i = 0;																								/* zerowanie licznik znaków												*/
	//}
	//else																									/* jeœli aktualny znak to liczba lub litera t:							*/
	//{
		//RS232Port->DATA = SendingString[i++];																/* wyœlij znak															*/
	//}
//}
//
//ISR(USARTD1_RXC_vect)
//{
	//char c = RS232Port->DATA;																				/* pobierz znak z usart													*/
	//ReceivedString = Usart_GetString(c);																	/* utwórz ³añcuch z odebranych znaków									*/
	//if (ReceivedString != NULL)																				/* jeœli ³añcuch znaków nie jest pusty to:								*/
	//{
		//Buffer_InsertString(ReceivingBuffer, ReceivedString);												/* dodaj ³añcuch do bufora odbiorczego									*/
		//ReceivedString = NULL;																				/* zeruj ³añcuch odbiorczy												*/
	//}
//}

ISR(USARTD0_DRE_vect)
{
	static uint8_t i = 0;																					/* licznik znaków														*/
	if (!SendingString[i] || (SendingString[i] < 32 && SendingString[i] != 10 && SendingString[i] != 13))	/* jeœli znak NUL lub niedrukowalny to zakoñcz wysy³anie				*/
	{
		RS232Port->DATA = 0;																				/* wyœlij NUL, znak koñcz¹cy ³añcuch									*/
		RS232Port->CTRLA &= ~(USART_DREINTLVL1_bm);															/* zablokowanie przerwania data reg empty								*/
		i = 0;																								/* zerowanie licznik znaków												*/
	}
	else																									/* jeœli aktualny znak to liczba lub litera t:							*/
	{
		RS232Port->DATA = SendingString[i++];																/* wyœlij znak															*/
	}
}

ISR(USARTD0_RXC_vect)
{
	char c = RS232Port->DATA;																				/* pobierz znak z usart													*/
	ReceivedString = Usart_GetString(c);																	/* utwórz ³añcuch z odebranych znaków									*/
	if (ReceivedString != NULL)																				/* jeœli ³añcuch znaków nie jest pusty to:								*/
	{
		Buffer_InsertString(ReceivingBuffer, ReceivedString);												/* dodaj ³añcuch do bufora odbiorczego									*/
		ReceivedString = NULL;																				/* zeruj ³añcuch odbiorczy												*/
	}
}


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/