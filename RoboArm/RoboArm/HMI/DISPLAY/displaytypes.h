/*
 * displaytypes.h
 *
 * Created: 05.02.2022 17:58:26
 *  Author: rybka
 */ 


#ifndef DISPLAYTYPES_H_
#define DISPLAYTYPES_H_

/*-------------------------------------DEFINICJE TYP�W DANYCH-----------------------------------------------------------------------------------*/

typedef struct GPoint								/* struktura opisuje punkt graficzny														*/
{
	uint8_t X : 7;									/* zmienna siedmio-bitowa okre�laj�ca kolumn� wy�wietlacza, zakres warto�ci 0-127			*/
	uint8_t Y : 6;									/* zmienna sze�cio-bitowa okre�laj�ca wiersz wy�wietlacza, zakres warto�ci 0-63				*/
}g_point_t;

typedef struct TPoint								/* struktura opisuje punkt tekstowy															*/
{
	uint8_t X : 7;									/* zmienna siedmio-bitowa okre�laj�ca kolumn� wy�wietlacza, zakres warto�ci 0-127			*/
	uint8_t Y : 3;									/* zmienna trzy-bitowa okre�laj�ca stron�(8 wierszy) wy�wietlacza, zakres warto�ci 0-7		*/
}t_point_t;

typedef struct GItem								/* struktura opisuje element graficzny														*/
{
	uint8_t ID;										/* numer identyfikacyjny elementu graficznego												*/
	t_point_t beginPoint;							/* pocz�tek pola zajmowanego przez element													*/
	t_point_t endPoit;								/* koniec pola zajmowanego przez element													*/
	uint8_t *gData;									/* wska�nik na dane tworz�ce element														*/
	void *next;										/* wska�nik na nast�pny element																*/
}g_item_t;

typedef struct GitemsList							/* struktura opisuje list� element�w graficznych											*/
{
	g_item_t *current;								/* wska�nik na aktualny element graficzny													*/
	g_item_t *head;									/* wska�nik na pierwszy element graficzny													*/
	g_item_t *tail;									/* wska�nik na ostatni element graficzny													*/
}g_itemsList_t;

typedef struct TItem								/* struktura opisuje element tekstowy														*/
{
	uint8_t ID;										/* numer identyfikacyjny elementu tekstowego												*/
	uint8_t negateTxt;								/* zmienna okre�la czy tekst b�dzie normalny czy pod�wietlony								*/
	t_point_t beginPoint;							/* pocz�tek tekstu																			*/
	char *tData;									/* wska�nik na tekst																		*/
	void *next;										/* wska�nik na nast�pny element																*/
}t_item_t;

typedef struct TitemsList							/* struktura opisuje list� element�w tekstowych												*/
{
	t_item_t *current;								/* wska�nik na aktualny element tekstowy													*/
	t_item_t *head;									/* wska�nik na pierwszy element tekstowy													*/
	t_item_t *tail;									/* wska�nik na ostatni element tekstowy														*/
}t_itemsList_t;

typedef struct Screen								/* struktura opisuje ekran wy�wietlacza														*/
{
	uint8_t ID;										/* numer identyfikacyjny ekranu																*/
	g_itemsList_t *gItems;							/* wska�nik na list� element�w graficznych do wy�wietlenia na ekranie						*/
	t_itemsList_t *tItems;							/* wska�nik na list� element�w tekstowych do wy�wietlenia na ekranie						*/
	void *next;										/* wska�nik na nast�pny ekran																*/
	void *previous;									/* wska�nik na poprzedni ekran																*/
}screen_t;

typedef struct ScreensList							/* struktura opisuje list� ekran�w															*/
{
	screen_t *current;								/* wska�nik na aktualny ekran																*/
	screen_t *head;									/* wska�nik na pierwszy ekran																*/
	screen_t *tail;									/* wska�nik na ostatni ekran																*/
}screensList_t;


//typedef struct List									/* struktura opisuje list� powi�zan� w postaci og�lnej										*/
//{
	//void *current;									/* wska�nik na aktualny ekran																*/
	//void *head;										/* wska�nik na pierwszy ekran																*/
	//void *tail;										/* wska�nik na ostatni ekran																*/
//}list_t;

typedef struct GddramScreen							/* struktura opisuje GDDRAM wy�wietlacza													*/
{
	uint8_t memoryMap[128][8];						/* zmienna okre�la struktur� pami�ci GDDRAM													*/
}gddram_t;

typedef struct GraphicSize							/* struktura opisuje rozmiar elementu graficznego (obrazka, litery, itp.)					*/
{
	uint8_t width;									/* szeroko�� elementu																		*/
	uint8_t height;									/* wysoko�� elementu																		*/
}g_size_t;

/*----------------------------------------------------------------------------------------------------------------------------------------------*/




#endif /* DISPLAYTYPES_H_ */