/*
 * twi.h
 *
 * Created: 2017-06-30 09:59:24
 *  Author: rybka
 */ 


#ifndef TWI_H_
#define TWI_H_

/*---------------------------DEFINICJE MAKRINSTRUKCJI-------------------------------------------------------------------------------------------------------------------*/

#define TWI_FRQ					400000																		/* cz�stotliwo�� magistrali 100KHz							*/
#define TWI_BAUDRATE			((F_CPU / (2 * TWI_FRQ)) - 5)												/* obliczenie warto�ci dla rejestru BAUD					*/
		
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------DEKLARACJE FUNKCJI OBS�UGI TWI-------------------------------------------------------------------------------------------------------------*/

void twiMasterInit(TWI_t *twi, uint8_t baudrate);															/* funkcja inicjuj�ca magistral� twi						*/

void twiSendAddress(TWI_t *twi, uint8_t address);															/* funkcja wysy�a na magistral� adres uk�adu do komunikacji */

void twiSendData(TWI_t *twi, uint8_t data);																	/* funkcja wysy�a dane do uk�adu slave						*/

void twiStop(TWI_t *twi);																					/* zatrzymanie transmisji									*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif /* TWI_H_ */