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
USART_t *RS232Port = NULL;														/* wska�nik na port komunikacyjny										*/ 
char *ReceivedString;															/* �a�cuch odebrany z usart												*/
char *SendingString;															/* �a�cuch znak�w do wys�ania przez usart								*/
char ReceivedChars[RECEIVE_MAX_CHARS];											/* tablica znak�w odebranych z usart									*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------Definicje funkcji---------------------------------------------------------------------------------------------------*/

void Usart_Init(USART_t *module, PORT_t *moduleport, uint8_t nrtxpin, uint8_t nrrxpin)
{	
	RS232Port = module;															/* ustawienie wska�nika na modu� usart									*/
	
	moduleport->OUTSET = (1<<nrtxpin);											/* pin RX w stan wysoki													*/
	moduleport->DIRSET = (1<<nrtxpin);											/* pin RX jako wyj�cie													*/
	moduleport->DIRCLR = (1<<nrrxpin);											/* pin TX jako wej�cie													*/
	
	RS232Port->BAUDCTRLA = 12;													/* BSEL BITY - 0:7	(ustawienie pr�dko�ci transmisji)					*/
	RS232Port->BAUDCTRLB = 64;													/* BSEL BITY - 8:11, BSCALE BITY - 0:3(ustawienie pr�dko�ci transmisji) */
	RS232Port->CTRLC = USART_CHSIZE_8BIT_gc;									/* ustawienie ramki danych: 8bit�w danych 1bit stopu, bez parzysto�ci	*/
	RS232Port->CTRLA = (USART_RXCINTLVL_MED_gc);								/* ustawienie priorytetu przerwania receive								*/
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;												/* globalne odblokowanie przerwa� �redniego priorytetu					*/
	RS232Port->CTRLB |= (USART_RXEN_bm | USART_TXEN_bm);						/* odblokowanie odbiornika i nadajnika USATR							*/

	/* inicjalizacja tablicy znak�w	*/
	for (uint8_t i = 0; i < RECEIVE_MAX_CHARS; i++)
	{
		ReceivedChars[i] = 0;
	}
	/* inicjalizacja �a�cuch�w, odbiorczego i nadawczego	*/
	ReceivedString = (char *)malloc(sizeof(char) * RECEIVE_MAX_CHARS);			/* alokacja pami�ci dla �a�cucha odbiorczego							*/
	SendingString = (char *)malloc(sizeof(char) * SENDING_MAX_CHARS);			/* alokacja pami�ci dla �a�cucha nadawczego								*/
	
}

void Usart_SendString(char *str)
{
	strcpy(SendingString, str);													/* przygotowanie wskazanego �a�cucha do wys�ania						*/
	RS232Port->CTRLA |= (USART_DREINTLVL1_bm);									/* ustawienie priorytetu przerwania data reg empty						*/
}

char *Usart_GetString(char znak)
{
	static uint8_t CharsCount = 0;												/* licznik znak�w odebranych z usart									*/
	
	if (znak > 31 && znak < 127)												/* je�li odebrany znak jest drukowalny to:								*/
	{
		ReceivedChars[CharsCount++] = znak;										/* dodaj znak do tablicy												*/
	}
	
	if (znak == CR || znak == NUL || CharsCount >= RECEIVE_MAX_CHARS - 3)		/* je�li wykryty zostanie koniec �a�cucha to:							*/
	{
		ReceivedChars[CharsCount++] = LF;										/* dodaj znak nowego wiersza											*/
		ReceivedChars[CharsCount++] = CR;										/* dodaj znak powrotu karetki											*/
		ReceivedChars[CharsCount] = NUL;										/* dodaj znak NUL koniec wiersza										*/
		CharsCount = 0;															/* zeruj licznik odebranych znak�w										*/
		return &ReceivedChars[0];												/* zwr�� wska�nik na tablic� odebranych znak�w							*/
	} 
	else																		/* a je�li koniec wiersza nie zosta� wykryty to :						*/
	{
		return NULL;															/* zwr�� warto�� pust�(odbi�r wiersza jeszcze si� nie zako�czy�)		*/
	}
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/


/*---------------------------PRZERWANIA-----------------------------------------------------------------------------------------------------------------*/

//ISR(USARTD1_DRE_vect)
//{
	//static uint8_t i = 0;																					/* licznik znak�w														*/
	//if (!SendingString[i] || (SendingString[i] < 32 && SendingString[i] != 10 && SendingString[i] != 13))	/* je�li znak NUL lub niedrukowalny to zako�cz wysy�anie				*/
	//{
		//RS232Port->DATA = 0;																				/* wy�lij NUL, znak ko�cz�cy �a�cuch									*/
		//RS232Port->CTRLA &= ~(USART_DREINTLVL1_bm);															/* zablokowanie przerwania data reg empty								*/
		//i = 0;																								/* zerowanie licznik znak�w												*/
	//}
	//else																									/* je�li aktualny znak to liczba lub litera t:							*/
	//{
		//RS232Port->DATA = SendingString[i++];																/* wy�lij znak															*/
	//}
//}
//
//ISR(USARTD1_RXC_vect)
//{
	//char c = RS232Port->DATA;																				/* pobierz znak z usart													*/
	//ReceivedString = Usart_GetString(c);																	/* utw�rz �a�cuch z odebranych znak�w									*/
	//if (ReceivedString != NULL)																				/* je�li �a�cuch znak�w nie jest pusty to:								*/
	//{
		//Buffer_InsertString(ReceivingBuffer, ReceivedString);												/* dodaj �a�cuch do bufora odbiorczego									*/
		//ReceivedString = NULL;																				/* zeruj �a�cuch odbiorczy												*/
	//}
//}

ISR(USARTD0_DRE_vect)
{
	static uint8_t i = 0;																					/* licznik znak�w														*/
	if (!SendingString[i] || (SendingString[i] < 32 && SendingString[i] != 10 && SendingString[i] != 13))	/* je�li znak NUL lub niedrukowalny to zako�cz wysy�anie				*/
	{
		RS232Port->DATA = 0;																				/* wy�lij NUL, znak ko�cz�cy �a�cuch									*/
		RS232Port->CTRLA &= ~(USART_DREINTLVL1_bm);															/* zablokowanie przerwania data reg empty								*/
		i = 0;																								/* zerowanie licznik znak�w												*/
	}
	else																									/* je�li aktualny znak to liczba lub litera t:							*/
	{
		RS232Port->DATA = SendingString[i++];																/* wy�lij znak															*/
	}
}

ISR(USARTD0_RXC_vect)
{
	char c = RS232Port->DATA;																				/* pobierz znak z usart													*/
	ReceivedString = Usart_GetString(c);																	/* utw�rz �a�cuch z odebranych znak�w									*/
	if (ReceivedString != NULL)																				/* je�li �a�cuch znak�w nie jest pusty to:								*/
	{
		Buffer_InsertString(ReceivingBuffer, ReceivedString);												/* dodaj �a�cuch do bufora odbiorczego									*/
		ReceivedString = NULL;																				/* zeruj �a�cuch odbiorczy												*/
	}
}


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/