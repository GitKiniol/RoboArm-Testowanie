/*
 * interface.h
 *
 * Created: 02.08.2022 19:29:13
 *  Author: rybka
 */ 


#ifndef INTERFACE_H_
#define INTERFACE_H_


typedef struct LIST_ELEMENT_STRUCT								/* element listy 																		*/
{
	void *Next;													/* wskaŸnik na kolejny element															*/
	void *Data;													/* wskaŸnik na dane elementu															*/
}list_element_t;

typedef struct LIST_STRUCT										/* lista powi¹zana ruchów i multiruchów													*/
{
	list_element_t *Head;										/* pierwszy element na liœcie															*/
	list_element_t *Tail;										/* ostatni element na liœcie															*/
	list_element_t *Current;									/* aktualny element																		*/
	uint8_t Count;												/* licznik elementów																	*/
}list_t;

typedef struct DRIVER_1_STRUCT
{
	uint8_t speed;
	void(*Start)(void *, uint8_t);	
}drv1_t;

typedef struct DRIVER_2_STRUCT
{
	uint8_t speed;
	uint16_t position;
	void(*Start)(void *, uint8_t);
}drv2_t;

extern drv1_t *driver1_1, *driver1_2;
extern drv2_t *driver2_1;

list_t *ListInit(list_t *list);

void ListPush(list_element_t *element);

list_element_t *ListPull(list_t *list);

list_element_t *ElementInit(list_element_t *element);

drv1_t *Driver1Init(drv1_t *driver);

drv2_t *Driver2Init(drv2_t *driver);

void StartDrv1(drv1_t *driver, uint8_t dir);

void StartDrv2(drv2_t *driver, uint8_t dir);



#endif /* INTERFACE_H_ */