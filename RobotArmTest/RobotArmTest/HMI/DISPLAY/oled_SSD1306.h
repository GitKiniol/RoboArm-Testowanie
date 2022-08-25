/*
 * oled_SSD1306.h
 *
 * Created: 2017-08-01 14:59:32
 *  Author: rybka
 */ 


#ifndef OLED_SSD1306_H_
#define OLED_SSD1306_H_

#include "displaytypes.h"

/*---------------------------DEFINICJE MAKRINSTRUKCJI-------------------------------------------------------------------------------------------------------------------------------*/

#define SSD1306_WRITE				0x00																			/* makro ustawia ��danie zapisu do wy�wietlacza 				*/
#define SSD1306_READ				0x01																			/* makro ustawia ��danie odczytu z wy�wietlacza  				*/
#define SSD1306_ADDR				0x78																			/* adres, numer identyfikacyjny wy�wietlacza					*/
#define SSD1306_CMD					0x80																			/* wys�ane dane interpretowane s� jako komenda					*/
#define SSD1306_DATA				0xc0																			/* wysy�ane dane interpretowane jako dane do GDDRAM				*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------TABLICA KOMEND STEROWNIKA SSD1306----------------------------------------------------------------------------------------------------------------------*/
/* Komendy podstawowe :																																								*/
#define CMD_SET_CONTRAST			0x81								/* komenda regulacji kontrastu, jako parametr przyjmuje warto�� okre�laj�c� kontrast						*/
#define CMD_ENTIRE_DISP_ON			0xa5								/* komenda za��cza wszystkie piksele ignoruj�c warto�� w pami�ci GDDRAM										*/
#define CMD_ENTIRE_DISP_OFF			0xa4								/* komenda powoduje powr�t do wy�wietlania danych z pami�ci GDDRAM											*/
#define CMD_NORMAL_DISP				0xa6								/* komenda ustawia normalny tryb wy�wietlania 0 = piksel off, 1 = piksel on									*/
#define CMD_INVERSE_DISP			0xa7								/* komenda ustawia odwr�cone wy�wietlanie 1 = piksel off, 0 = piksel on										*/
#define CMD_DISP_OFF				0xae								/* wy��czenie wy�wietlacza i przej�cie w tryb "sleep"														*/
#define CMD_DISP_ON					0xaf								/* za��czenie wy�wietlacza w trybie normalnym																*/
	
/* Komendy adresowania :																																							*/
#define CMD_ADDR_MODE				0x20								/* ustawienie trybu adresowania, komend� stosowa� z poni�szymi parametrami									*/
#define PAR_HORIZONTAL_ADDR_MODE	0x00								/* adresowanie poziome																						*/	
#define PAR_VERTIVAL_ADDR_MODE		0x01								/* adresowanie pionowe																						*/
#define PAR_PAGE_ADDR_MODE			0x02								/* adresowanie stronami																						*/
	
#define CMD_SET_L_COLUMN_ADDR_PAM	0x00								/* m�odszy p�bajt adresu kolumny w trybie adresowania stronami, do tej komendy trzeba doda� warto��
																		 z zakresu 0-15 okre�laj�c� m�odsze 4 bity bajtu adresu kolumny												*/
#define CMD_SET_H_COLUMN_ADDR_PAM	0x10								/* starszy p�bajt adresu kolumny trybie adresowania stronami, do tej komendy trzeba doda� warto�� 																						
																		 z zakresu 0-15 okre�laj�c� starsze 4 bity adresu kolumny													*/
#define CMD_SET_PAGE_ADDR_PAM		0xb0								/* adres strony w trybie adresowania stronami, do tej komendy trzeba doda� warto�� z zakresu
																		 0-7 okre�laj�c� numer strony																				*/
#define CMD_SET_COLUMN_ADDR			0x21								/* komenda przyjmuje dwa parametry z zakresu 0-127 okre�laj�ce pierwsz� i ostatni� kolumn� w trybie 
																		 adresowania poziomego lub pionowego																		*/
#define CMD_SET_PAGE_ADDR			0x22								/* komenda przyjmuje dwa parametry z zakresu 0-7 okre�laj�ce pierwsz� i ostatni� stron� w trybie 
																		 adresowania poziomego lub pionowego																		*/
#define CMD_SET_DISP_START_LINE		0x40								/* komenda ustawia lini� pocz�tkow� wy�wietlacza, do komendy doda� numer linii 0-63							*/

/* Komendy pompy �adunk�w :																																							*/
#define CMD_SET_CHARGE_PUMP			0x8d								/* komenda zmienia ustawienia pompy �adunk�w, komenda przyjmuje dwa poni�sze parametry :					*/
#define PAR_CHARGE_PUMP_ON			0x14								/* pompa za��czona																							*/
#define PAR_CHARGE_PUMP_OFF			0x10								/* pompa wy��czona																							*/

/* Komendy konfiguracyjne :																																							*/
#define CMD_SET_SCAN_DIR_NORMAL		0xc0								/* ustawienie kierunku skanowania pin�w "COM", tryb normalny, skanowanie od COM0 do COM[N-1]				*/			
#define CMD_SET_SCAN_DIR_INVERT		0xC8								/* tryb odwr�cony, skanowanie od COM[N-1] do COM0	( N - multiplex ratio )									*/
#define CMD_COM_HARDWARE_CONFIG		0xda								/* konfiguracja sprz�towej obs�ugi pin�w "COM", komenda przyjmuje poni�sze parametry:						*/
#define PAR_SEQENTIAL_LEFT_RIGHT	0x02								/* sterowanie sekwencyjne z lewej do prawej																	*/
#define PAR_SEQENTIAL_RIGHT_LEFT	0x00								/* sterowanie sekwencyjne z prawej do lewej																	*/
#define PAR_ALTER_LEFT_RIGHT		0x03								/* sterowanie alternatywne z lewej do prawej																*/
#define PAR_ALTER_RIGHT_LEFT 		0x01								/* sterowanie alternatywne z prawej do lewej																*/
#define CMD_NOP						0xe3								/* pusta komenda																							*/

/* Komendy scrollowania :																																							*/
#define CMD_HOR_SCROLL_RIGHT		0x26								/* ustawienie przewijania poziomego w prawo																	*/
#define CMD_HOR_SCROLL_LEFT			0x27								/* ustawienie przewijania poziomego w lewo																	*/
#define CMD_ACTIVATE_SCROLL			0x2f								/* aktywacja przewijania																					*/
#define CMD_DEACTIVATE_SCROLL		0x2e								/* dezaktywacja przewijania																					*/


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


/*---------------------------DEKLARACJE FUNKCJI OBS�UGI STEROWNIKA SSD1306----------------------------------------------------------------------------------------------------------*/

void ssd1306Init(TWI_t *twiModule);																						/* funkcja inicjalizuje wy�wietlacz							*/			

void ssd1306SendCmdNoPar(TWI_t *twiModule, uint8_t oledAddress, uint8_t cmd);											/* funkcja wysy�a bezparametrow� komend� do wy�wietlacza	*/

void ssd1306SendCmdOnePar(TWI_t *twiModule, uint8_t oledAddress, uint8_t cmd, uint8_t parameter);						/* funkcja wysy�a komend� z jednym parametrem				*/

void ssd1306SendCmdManyPar(TWI_t *twiModule, uint8_t oledAddress, uint8_t cmd, uint8_t *parameters, uint8_t parCount);	/* funkcja wysy�a komend� z wieloma parametrami				*/

void ssd1306SendByte(TWI_t *twiModule, uint8_t oledAddress, uint8_t byte);												/* funkcja wysy�a jeden bajt do pami�ci GDDRAM				*/

void ssd1306Fill(TWI_t *twiModule, uint8_t oledAddress, uint8_t *data);													/* funkcja wype�nia GDDRAM wskazanymi danymi				*/

void ssd1306FillArea(TWI_t *twiModule, uint8_t oledAddress, uint8_t *data, t_point_t startArea, t_point_t endArea);		/* funkcja wype�nia wybrany obszar GDDRAM wskazanymi danymi	*/

t_point_t ssd1306FillAreaFromFlash(TWI_t *twiModule, t_point_t startPos, g_size_t size, __memx const uint8_t *data);	/* wype�nienie GDDRAM danymi zaczynaj�c od wskazanej pozycji*/

void ssd1306FillFromFash(TWI_t *twiModule, uint8_t oledAddress, __memx const uint8_t *data);							/* funkcja wype�nia GDDRAM wskazanymi danymi z pami�ci flash*/

void ssd1306DispOn(TWI_t *twiModule);																					/* za��czenie wy�wietlacza									*/

void ssd1306DispOff(TWI_t *twiModule);																					/* wy��czenie wy�wietlacza									*/

void ssd1306ClrScr(TWI_t *twiModule, uint8_t patern);																	/* funkcja czy�ci ekran zadanym wzorem						*/

void ssd1306Locate(TWI_t *twiModule, uint8_t x, uint8_t y);																/* ustawienie wska�nika na wybranej pozycji					*/

void ssd1306SetPixel(TWI_t *twiModule, uint8_t x, uint8_t y);															/* funkcja za��cza pixel we wskazanym miejscu				*/

void ssd1306ClrPixel(TWI_t *twiModule, uint8_t x, uint8_t y);															/* funkcja wy��cza pixel we wskazanym miejscu				*/

void ssd1306WriteTxt(TWI_t *twiModule, t_point_t pozycja, __memx const char *tablicaZnakow, char *napis, uint8_t neg);	/* funkcja wy�wietla tekst									*/

g_point_t createGpoint(uint8_t x, uint8_t y);																			/* funkcja tworzy punkt graficzny z podanych warto�ci		*/

t_point_t creatrTpoint(uint8_t x, uint8_t y);																			/* funkcja tworzy punkt tekstowy z podanych warto�ci		*/

void ssd1306DrawLine(TWI_t *twiModule, g_point_t startLine, g_point_t endLine);											/* funkcja rysuje lini�										*/

void ssd1306DrawRectangle(TWI_t *twiModule, g_point_t leftUpCorner, g_point_t rightDownCorner);							/* funkcja rysuje prostok�t									*/

void ssd1306DrawCircle(TWI_t *twiModule, g_point_t centerPoint, uint8_t diameter);										/* funkcja rysuje ko�o										*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif /* OLED_SSD1306_H_ */