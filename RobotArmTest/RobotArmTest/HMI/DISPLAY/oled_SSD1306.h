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

#define SSD1306_WRITE				0x00																			/* makro ustawia ¿¹danie zapisu do wyœwietlacza 				*/
#define SSD1306_READ				0x01																			/* makro ustawia ¿¹danie odczytu z wyœwietlacza  				*/
#define SSD1306_ADDR				0x78																			/* adres, numer identyfikacyjny wyœwietlacza					*/
#define SSD1306_CMD					0x80																			/* wys³ane dane interpretowane s¹ jako komenda					*/
#define SSD1306_DATA				0xc0																			/* wysy³ane dane interpretowane jako dane do GDDRAM				*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------TABLICA KOMEND STEROWNIKA SSD1306----------------------------------------------------------------------------------------------------------------------*/
/* Komendy podstawowe :																																								*/
#define CMD_SET_CONTRAST			0x81								/* komenda regulacji kontrastu, jako parametr przyjmuje wartoœæ okreœlaj¹c¹ kontrast						*/
#define CMD_ENTIRE_DISP_ON			0xa5								/* komenda za³¹cza wszystkie piksele ignoruj¹c wartoœæ w pamiêci GDDRAM										*/
#define CMD_ENTIRE_DISP_OFF			0xa4								/* komenda powoduje powrót do wyœwietlania danych z pamiêci GDDRAM											*/
#define CMD_NORMAL_DISP				0xa6								/* komenda ustawia normalny tryb wyœwietlania 0 = piksel off, 1 = piksel on									*/
#define CMD_INVERSE_DISP			0xa7								/* komenda ustawia odwrócone wyœwietlanie 1 = piksel off, 0 = piksel on										*/
#define CMD_DISP_OFF				0xae								/* wy³¹czenie wyœwietlacza i przejœcie w tryb "sleep"														*/
#define CMD_DISP_ON					0xaf								/* za³¹czenie wyœwietlacza w trybie normalnym																*/
	
/* Komendy adresowania :																																							*/
#define CMD_ADDR_MODE				0x20								/* ustawienie trybu adresowania, komendê stosowaæ z poni¿szymi parametrami									*/
#define PAR_HORIZONTAL_ADDR_MODE	0x00								/* adresowanie poziome																						*/	
#define PAR_VERTIVAL_ADDR_MODE		0x01								/* adresowanie pionowe																						*/
#define PAR_PAGE_ADDR_MODE			0x02								/* adresowanie stronami																						*/
	
#define CMD_SET_L_COLUMN_ADDR_PAM	0x00								/* m³odszy pó³bajt adresu kolumny w trybie adresowania stronami, do tej komendy trzeba dodaæ wartoœæ
																		 z zakresu 0-15 okreœlaj¹c¹ m³odsze 4 bity bajtu adresu kolumny												*/
#define CMD_SET_H_COLUMN_ADDR_PAM	0x10								/* starszy pó³bajt adresu kolumny trybie adresowania stronami, do tej komendy trzeba dodaæ wartoœæ 																						
																		 z zakresu 0-15 okreœlaj¹c¹ starsze 4 bity adresu kolumny													*/
#define CMD_SET_PAGE_ADDR_PAM		0xb0								/* adres strony w trybie adresowania stronami, do tej komendy trzeba dodaæ wartoœæ z zakresu
																		 0-7 okreœlaj¹c¹ numer strony																				*/
#define CMD_SET_COLUMN_ADDR			0x21								/* komenda przyjmuje dwa parametry z zakresu 0-127 okreœlaj¹ce pierwsz¹ i ostatni¹ kolumnê w trybie 
																		 adresowania poziomego lub pionowego																		*/
#define CMD_SET_PAGE_ADDR			0x22								/* komenda przyjmuje dwa parametry z zakresu 0-7 okreœlaj¹ce pierwsz¹ i ostatni¹ stronê w trybie 
																		 adresowania poziomego lub pionowego																		*/
#define CMD_SET_DISP_START_LINE		0x40								/* komenda ustawia liniê pocz¹tkow¹ wyœwietlacza, do komendy dodaæ numer linii 0-63							*/

/* Komendy pompy ³adunków :																																							*/
#define CMD_SET_CHARGE_PUMP			0x8d								/* komenda zmienia ustawienia pompy ³adunków, komenda przyjmuje dwa poni¿sze parametry :					*/
#define PAR_CHARGE_PUMP_ON			0x14								/* pompa za³¹czona																							*/
#define PAR_CHARGE_PUMP_OFF			0x10								/* pompa wy³¹czona																							*/

/* Komendy konfiguracyjne :																																							*/
#define CMD_SET_SCAN_DIR_NORMAL		0xc0								/* ustawienie kierunku skanowania pinów "COM", tryb normalny, skanowanie od COM0 do COM[N-1]				*/			
#define CMD_SET_SCAN_DIR_INVERT		0xC8								/* tryb odwrócony, skanowanie od COM[N-1] do COM0	( N - multiplex ratio )									*/
#define CMD_COM_HARDWARE_CONFIG		0xda								/* konfiguracja sprzêtowej obs³ugi pinów "COM", komenda przyjmuje poni¿sze parametry:						*/
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


/*---------------------------DEKLARACJE FUNKCJI OBS£UGI STEROWNIKA SSD1306----------------------------------------------------------------------------------------------------------*/

void ssd1306Init(TWI_t *twiModule);																						/* funkcja inicjalizuje wyœwietlacz							*/			

void ssd1306SendCmdNoPar(TWI_t *twiModule, uint8_t oledAddress, uint8_t cmd);											/* funkcja wysy³a bezparametrow¹ komendê do wyœwietlacza	*/

void ssd1306SendCmdOnePar(TWI_t *twiModule, uint8_t oledAddress, uint8_t cmd, uint8_t parameter);						/* funkcja wysy³a komendê z jednym parametrem				*/

void ssd1306SendCmdManyPar(TWI_t *twiModule, uint8_t oledAddress, uint8_t cmd, uint8_t *parameters, uint8_t parCount);	/* funkcja wysy³a komendê z wieloma parametrami				*/

void ssd1306SendByte(TWI_t *twiModule, uint8_t oledAddress, uint8_t byte);												/* funkcja wysy³a jeden bajt do pamiêci GDDRAM				*/

void ssd1306Fill(TWI_t *twiModule, uint8_t oledAddress, uint8_t *data);													/* funkcja wype³nia GDDRAM wskazanymi danymi				*/

void ssd1306FillArea(TWI_t *twiModule, uint8_t oledAddress, uint8_t *data, t_point_t startArea, t_point_t endArea);		/* funkcja wype³nia wybrany obszar GDDRAM wskazanymi danymi	*/

t_point_t ssd1306FillAreaFromFlash(TWI_t *twiModule, t_point_t startPos, g_size_t size, __memx const uint8_t *data);	/* wype³nienie GDDRAM danymi zaczynaj¹c od wskazanej pozycji*/

void ssd1306FillFromFash(TWI_t *twiModule, uint8_t oledAddress, __memx const uint8_t *data);							/* funkcja wype³nia GDDRAM wskazanymi danymi z pamiêci flash*/

void ssd1306DispOn(TWI_t *twiModule);																					/* za³¹czenie wyœwietlacza									*/

void ssd1306DispOff(TWI_t *twiModule);																					/* wy³¹czenie wyœwietlacza									*/

void ssd1306ClrScr(TWI_t *twiModule, uint8_t patern);																	/* funkcja czyœci ekran zadanym wzorem						*/

void ssd1306Locate(TWI_t *twiModule, uint8_t x, uint8_t y);																/* ustawienie wskaŸnika na wybranej pozycji					*/

void ssd1306SetPixel(TWI_t *twiModule, uint8_t x, uint8_t y);															/* funkcja za³¹cza pixel we wskazanym miejscu				*/

void ssd1306ClrPixel(TWI_t *twiModule, uint8_t x, uint8_t y);															/* funkcja wy³¹cza pixel we wskazanym miejscu				*/

void ssd1306WriteTxt(TWI_t *twiModule, t_point_t pozycja, __memx const char *tablicaZnakow, char *napis, uint8_t neg);	/* funkcja wyœwietla tekst									*/

g_point_t createGpoint(uint8_t x, uint8_t y);																			/* funkcja tworzy punkt graficzny z podanych wartoœci		*/

t_point_t creatrTpoint(uint8_t x, uint8_t y);																			/* funkcja tworzy punkt tekstowy z podanych wartoœci		*/

void ssd1306DrawLine(TWI_t *twiModule, g_point_t startLine, g_point_t endLine);											/* funkcja rysuje liniê										*/

void ssd1306DrawRectangle(TWI_t *twiModule, g_point_t leftUpCorner, g_point_t rightDownCorner);							/* funkcja rysuje prostok¹t									*/

void ssd1306DrawCircle(TWI_t *twiModule, g_point_t centerPoint, uint8_t diameter);										/* funkcja rysuje ko³o										*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif /* OLED_SSD1306_H_ */