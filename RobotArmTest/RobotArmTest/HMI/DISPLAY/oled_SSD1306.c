/*
 * oled_SSD1306.c
 *
 * Created: 2017-08-01 14:59:13
 *  Author: rybka
 */ 


#include <avr/io.h>
#include "oled_SSD1306.h"
#include "../TWI_BUS/twi.h"
#include "displaytypes.h"


/*---------------------------INICJALIZACJA ZMIENNYCH--------------------------------------------------------------------------------*/

uint8_t gddramBuff[8][128];


/*-----------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------DEFINICJE FUNKCJI OBS�UGI STEROWNIKA SSD1306------------------------------------------------------------*/

void ssd1306Init(TWI_t *twiModule)
{
	ssd1306SendCmdOnePar(twiModule, SSD1306_ADDR, CMD_COM_HARDWARE_CONFIG, PAR_ALTER_LEFT_RIGHT);		/* ustawienie trybu pracy pin�w "COM"			*/
	ssd1306SendCmdNoPar(twiModule, SSD1306_ADDR, CMD_SET_SCAN_DIR_INVERT);								/* ustawienie kierunku skanowania				*/
	ssd1306SendCmdOnePar(twiModule, SSD1306_ADDR, CMD_ADDR_MODE, PAR_PAGE_ADDR_MODE);					/* ustawienie trybu adresowania					*/
	ssd1306SendCmdNoPar(twiModule, SSD1306_ADDR, (CMD_SET_PAGE_ADDR_PAM + 0));							/* ustawienie adresu strony						*/
	ssd1306SendCmdNoPar(twiModule, SSD1306_ADDR, CMD_SET_L_COLUMN_ADDR_PAM);							/* ustawienie adresu kolumny-m�odszy p�bajt	*/
	ssd1306SendCmdNoPar(twiModule, SSD1306_ADDR, CMD_SET_H_COLUMN_ADDR_PAM);							/* ustawienie adresu kolumny-starszy p�bajt	*/
	ssd1306SendCmdNoPar(twiModule, SSD1306_ADDR, (CMD_SET_DISP_START_LINE + 0));						/* ustawienie linii pocz�tkowej					*/
	ssd1306SendCmdNoPar(twiModule, SSD1306_ADDR, 0xa1);
	ssd1306DispOn(twiModule);																			/* za��czenie wy�wietlacza						*/
}

void ssd1306DispOff(TWI_t *twiModule)
{
	ssd1306SendCmdNoPar(twiModule, SSD1306_ADDR, CMD_DISP_OFF);											/* wy��cz wy�wietlacz							*/
	ssd1306SendCmdOnePar(twiModule, SSD1306_ADDR, CMD_SET_CHARGE_PUMP, PAR_CHARGE_PUMP_OFF);			/* wy��cz pomp� �adunk�w						*/
}

void ssd1306DispOn(TWI_t *twiModule)
{
	ssd1306SendCmdOnePar(twiModule, SSD1306_ADDR, CMD_SET_CHARGE_PUMP, PAR_CHARGE_PUMP_ON);				/* za��cz pomp� �adunk�w						*/
	ssd1306SendCmdNoPar(twiModule, SSD1306_ADDR, CMD_DISP_ON);											/* za��cz wy�wietlacz							*/
}

void ssd1306SendCmdNoPar(TWI_t *twiModule, uint8_t oledAddress, uint8_t cmd)
{
	twiSendAddress(twiModule, (oledAddress + SSD1306_WRITE));										/* wy�lij na magistral� adres wy�wietlacza		*/
	twiSendData(twiModule, SSD1306_CMD);															/* wy�lij bajt konfiguruj�cy zapis komend		*/
	twiSendData(twiModule, cmd);																	/* wy�lij kod komendy							*/
	twiSendData(twiModule, SSD1306_CMD);															/* ponownie wy�lij bajt konfiguracyjny			*/
	twiSendData(twiModule, cmd);																	/* ponownie wy�lij kod komendy					*/
	twiStop(twiModule);																				/* zatrzymaj transmisj�							*/
}

void ssd1306SendCmdOnePar(TWI_t *twiModule, uint8_t oledAddress, uint8_t cmd, uint8_t parameter)
{
	twiSendAddress(twiModule, (oledAddress + SSD1306_WRITE));										/* wy�lij na magistral� adres wy�wietlacza		*/
	twiSendData(twiModule, SSD1306_CMD);															/* wy�lij bajt konfiguruj�cy zapis komend		*/
	twiSendData(twiModule, cmd);																	/* wy�lij kod komendy							*/
	twiSendData(twiModule, SSD1306_CMD);															/* ponownie wy�lij bajt konfiguracyjny			*/
	twiSendData(twiModule, cmd);																	/* ponownie wy�lij kod komendy					*/
	twiSendData(twiModule, parameter);;																/* wy�lij parametr								*/
	twiStop(twiModule);																				/* zatrzymaj transmisj�							*/
}

void ssd1306SendCmdManyPar(TWI_t *twiModule, uint8_t oledAddress, uint8_t cmd, uint8_t *parameters, uint8_t parCount)
{
	uint8_t i = 0;
	twiSendAddress(twiModule, (oledAddress + SSD1306_WRITE));										/* wy�lij na magistral� adres wy�wietlacza		*/
	twiSendData(twiModule, SSD1306_CMD);															/* wy�lij bajt konfiguruj�cy zapis komend		*/
	twiSendData(twiModule, cmd);																	/* wy�lij kod komendy							*/
	for(; i < parCount; i++)																		/* wy�lij tyle parametr�w ile okre�lone jest	*/
	{																								/* zmienn� parCount								*/
		twiSendData(twiModule, SSD1306_CMD);														/* ponownie wy�lij bajt konfiguracyjny			*/
		twiSendData(twiModule, parameters[i]);														/* wy�lij parametr								*/
	}																								/*												*/
	twiStop(twiModule);																				/* zatrzymaj transmisj�							*/
}

void ssd1306SendByte(TWI_t *twiModule, uint8_t oledAddress, uint8_t byte)
{
	twiSendAddress(twiModule, (oledAddress + SSD1306_WRITE));										/* wy�lij na magistral� adres wy�wietlacza		*/
	twiSendData(twiModule, SSD1306_DATA);															/* wy�lij bajt konfiguruj�cy zapis do GDDRAM	*/
	twiSendData(twiModule, byte);																	/* wy�lij bajt danych							*/
	twiStop(twiModule);																				/* zatrzymaj transmisj�							*/
}

void ssd1306Fill(TWI_t *twiModule, uint8_t oledAddress, uint8_t *data)
{
	uint16_t i = 0;
	uint16_t j = 0;
	for (; i < 8; i++)																		
	{
		ssd1306Locate(twiModule,0, i);																/* ustaw kursor w wybranej pozycji				*/
		j = 0;
		for (; j < 128; j++)
		{
			ssd1306SendByte(twiModule, oledAddress, *data++);										/* wysy�aj dane									*/
		}
	}
}

void ssd1306FillArea(TWI_t *twiModule, uint8_t oledAddress, uint8_t *data, t_point_t startArea, t_point_t endArea)
{
	uint8_t curX = startArea.X;
	uint8_t curY = startArea.Y;
	ssd1306Locate(twiModule,curX, curY);															/* ustaw kursor w lewym g�rnym rogu pola		*/
	while (curY < endArea.Y)																		/* wykonuj dop�ki aktualny Y mniejszy ni� zadany*/
	{
		ssd1306SendByte(twiModule, oledAddress, *data);												/* wy�lij bajt do gddram						*/
		data++;																						/* przesu� wska�nik na kolejne dane				*/
		curX++;																						/* zwi�ksz aktualn� pozycj� X					*/
		if (curX > endArea.X)																		/* je�li X osi�gnie koniec pola to :			*/
		{
			curY++;																					/* zwi�ksz aktualna pozycj� Y					*/
			curX = startArea.X;																		/* ustaw kursor na pocz�tek pola				*/
			ssd1306Locate(twiModule,curX, curY);													/* ustaw kursor w kolejnym wierszu pola			*/
		}
	}
}

t_point_t ssd1306FillAreaFromFlash(TWI_t *twiModule, t_point_t startPos, g_size_t size, __memx const uint8_t *data)
{
	 uint8_t i = 0;
	 uint8_t j = 0;
	 t_point_t cursor;
	 cursor.X = startPos.X;
	 cursor.Y = startPos.Y;
	 for (; i < size.height; i++)										/* iteracja po wierszach													*/
	 {
		 ssd1306Locate(twiModule, cursor.X, cursor.Y + i);				/* ustaw kursor w osi y na pozycji startowej przesuni�tej o warto�� "i"		*/
		 j = 0;
		 for (; j < size.width; j++)									/* iteracja po kolumnach													*/
		 {
			 ssd1306SendByte(twiModule, SSD1306_ADDR, *data++);			/* wstaw dane																*/
		 }
	 }
	 cursor.X += size.width;											/* aktualna pozycja w osi X													*/
	 cursor.Y += size.height;											/* aktualna pozycja w osi Y													*/
	 return cursor;														/* zwr�cenie aktualnej pozycji kursora										*/
}

void ssd1306FillFromFash(TWI_t *twiModule, uint8_t oledAddress, __memx const uint8_t *data)
{
	uint16_t i = 0;
	uint16_t j = 0;
	for (; i < 8; i++)																		
	{
		ssd1306Locate(twiModule, 0, i);																/* ustaw kursor w wybranej pozycji				*/
		j = 0;
		for (; j < 128; j++)
		{
			ssd1306SendByte(twiModule, oledAddress, *data++);										/* wysy�aj dane									*/
		}
	}
}

void ssd1306ClrScr(TWI_t *twiModule, uint8_t patern)
{
	
	uint16_t i = 0;
	uint16_t j = 0;
	for(; i < 8; i++)
	{
		ssd1306Locate(twiModule, 0, i);																/* ustaw kursor w wybranej pozycji				*/
		j = 0;
		for(; j < 128; j++)
		{
			ssd1306SendByte(twiModule, SSD1306_ADDR, patern);										/* zapis 0 do GDDRAM, czyszczenie ekranu		*/
		}
	}	
}

void ssd1306WriteTxt(TWI_t *twiModule, t_point_t pozycja, __memx const char *tablicaZnakow, char *napis, uint8_t neg)
{
	uint16_t znak = 0;
	uint8_t i = 0;
	uint8_t actCol = 0;
	uint8_t actPag = 0;
	ssd1306Locate(twiModule, pozycja.X, pozycja.Y);													/* ustaw kursor w wybranej pozycji				*/
	actPag = pozycja.Y;																				/* ustalenie aktualnej pozycji Y				*/
	actCol = pozycja.X;																				/* ustalenie aktualnej pozycji X				*/
	while(*napis)																					/* wykonuj dop�ki s� litery						*/
	{
		znak = (uint16_t)*napis * 6;																/* ustalenie adresu litery/znaku w tablicy ASCII*/
		i = 0;																						/* zerowanie licznika p�tli wysy�aj�cej litery	*/
		if ((actCol + 6) > 127)																		/* je�li nast�pna litera nie mie�ci to :		*/
		{																							
			actPag++;																				/* przesu� kursor w d�							*/
			if(actPag == 8) actPag = 0;																/* je�li pozycja Y poza zakresem ustaw Y na 0	*/
			actCol = 1;																				/* ustaw aktualn� pozycj� X na pierwsz� kolumn�	*/
			ssd1306Locate(twiModule, actCol, actPag);												/* ustaw kursor w wybranej pozycji				*/
		}
		for (; i < 6; i++)																			/* p�tla wysy�a 6 bajt�w sk�adowych litery/znaku*/
		{
			actCol++;																				/* przesu� kursor w prawo						*/
			if(neg) ssd1306SendByte(twiModule, SSD1306_ADDR, tablicaZnakow[znak + i]);				/* wy�lij bajt sk�adowy litery/znaku			*/
			else ssd1306SendByte(twiModule, SSD1306_ADDR, (tablicaZnakow[znak + i] ^ 0xff));		/* wy�lij zanegowany bajt sk�adowy litery/znaku	*/
		}
		napis++;																					/* przesu� wska�nik na kolej� liter�			*/
	}	
}

void ssd1306SetPixel(TWI_t *twiModule, uint8_t x, uint8_t y)
{
	uint8_t page, bit;
	page = y / 8;
	bit = y % 8;
	ssd1306Locate(twiModule, x, page);																/* ustaw kursor w wybranej pozycji				*/
	ssd1306SendByte(twiModule, SSD1306_ADDR, (1 << bit));											/* zapis do GDDRAM, za��czenie pixela			*/
}

void ssd1306ClrPixel(TWI_t *twiModule, uint8_t x, uint8_t y)
{
	uint8_t page, bit;
	page = y / 8;
	bit = y % 8;
	ssd1306Locate(twiModule, x, page);																/* ustaw kursor w wybranej pozycji				*/
	ssd1306SendByte(twiModule, SSD1306_ADDR, (0 << bit));											/* zapis do GDDRAM, za��czenie pixela			*/
}				

void ssd1306Locate(TWI_t *twiModule, uint8_t x, uint8_t y)
{
	ssd1306SendCmdNoPar(twiModule, SSD1306_ADDR, CMD_SET_PAGE_ADDR_PAM + y);						/* ustawienie adresu strony do wyczyszczenia	*/
	ssd1306SendCmdNoPar(twiModule, SSD1306_ADDR, CMD_SET_L_COLUMN_ADDR_PAM + (x & 0x0f));			/* ustawienie adresu kolumny-m�odszy p�bajt	*/
	ssd1306SendCmdNoPar(twiModule, SSD1306_ADDR, CMD_SET_H_COLUMN_ADDR_PAM + (x >> 4));				/* ustawienie adresu kolumny-starszy p�bajt	*/
}

g_point_t createGpoint(uint8_t x, uint8_t y)
{
	g_point_t tempG = {0, 0};
	if (x < 128) tempG.X = x;
	else tempG.X = 127;
	if (y < 64) tempG.Y = y;
	else tempG.Y = 63;
	return tempG;	
}

t_point_t creatrTpoint(uint8_t x, uint8_t y)
{
	t_point_t tempT = {0, 0};
		if (x < 128) tempT.X = x;
		else tempT.X = 127;
		if (y < 8) tempT.Y = y;
		else tempT.Y = 7;
	return tempT;
}

void ssd1306DrawLine(TWI_t *twiModule, g_point_t startLine, g_point_t endLine)
{
	 int8_t d, dx, dy, ai, bi, xi, yi;
	 int8_t x = startLine.X, y = startLine.Y;
	 
	 if (startLine.X < endLine.X)
	 {
		 xi = 1;
		 dx = endLine.X - startLine.X;
	 }
	 else
	 {
		 xi = -1;
		 dx = startLine.X - endLine.X;
	 }
	 
	 if (startLine.Y < endLine.Y)
	 {
		 yi = 1;
		 dy = endLine.Y - startLine.Y;
	 }
	 else
	 {
		 yi = -1;
		 dy = startLine.Y - endLine.Y;
	 }
	
	 ssd1306SetPixel(twiModule, x, y);
	
	 if (dx > dy)
	 {
		 ai = (dy - dx) * 2;
		 bi = dy * 2;
		 d = bi - dx;
		
		 while (x != endLine.X)
		 {
			 
			 if (d >= 0)
			 {
				 x += xi;
				 y += yi;
				 d += ai;
			 }
			 else
			 {
				 d += bi;
				 x += xi;
			 }
			 ssd1306SetPixel(twiModule, x, y);
		 }
	 }
	 
	 else
	 {
		 ai = ( dx - dy ) * 2;
		 bi = dx * 2;
		 d = bi - dy;
		 
		 while (y != endLine.Y)
		 {
			 
			 if (d >= 0)
			 {
				 x += xi;
				 y += yi;
				 d += ai;
			 }
			 else
			 {
				 d += bi;
				 y += yi;
			 }
			 ssd1306SetPixel(twiModule, x, y);;
		 }
	 }
}

void ssd1306DrawRectangle(TWI_t *twiModule, g_point_t leftUpCorner, g_point_t rightDownCorner)
{
	ssd1306DrawLine(twiModule, leftUpCorner, createGpoint(rightDownCorner.X, leftUpCorner.Y));
	ssd1306DrawLine(twiModule, leftUpCorner, createGpoint(leftUpCorner.X, rightDownCorner.Y));
	ssd1306DrawLine(twiModule, createGpoint(leftUpCorner.X, rightDownCorner.Y), rightDownCorner);
	ssd1306DrawLine(twiModule, createGpoint(rightDownCorner.X, leftUpCorner.Y), rightDownCorner);
}