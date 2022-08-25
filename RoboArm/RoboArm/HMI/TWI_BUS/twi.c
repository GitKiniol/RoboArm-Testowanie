/*
 * twi.c
 *
 * Created: 2017-06-30 09:59:03
 *  Author: rybka
 */ 


#include <avr/io.h>
#include "twi.h"


/*---------------------------DEFINICJE FUNKCI OBS£UGI TWI-----------------------------------------------------------------------*/

void twiMasterInit(TWI_t *twi, uint8_t baudrate)
{
	twi->MASTER.BAUD = baudrate;							/* ustawienie prêdkoœci transmisji									*/
	twi->MASTER.CTRLB |= TWI_MASTER_SMEN_bm;				/* w³¹czenie trybu smart											*/
	twi->MASTER.CTRLA |= TWI_MASTER_ENABLE_bm;				/* uruchomienie modu³u twi											*/
	twi->MASTER.CTRLA |= TWI_MASTER_INTLVL_HI_gc;			/* wysoki priorytet przerwañ										*/
	twi->MASTER.STATUS |= TWI_MASTER_BUSSTATE_IDLE_gc;		/* ustawienie magistrali w stan IDLE								*/
}

void twiSendAddress(TWI_t *twi, uint8_t address)
{
	twi->MASTER.ADDR = address;
	while(!(twi->MASTER.STATUS & TWI_MASTER_CLKHOLD_bm));
}

void twiSendData(TWI_t *twi, uint8_t data)
{
	twi->MASTER.DATA = data;
	while(!(twi->MASTER.STATUS & TWI_MASTER_CLKHOLD_bm));
}

void twiStop(TWI_t *twi)
{
	twi->MASTER.CTRLC |= TWI_MASTER_CMD_STOP_gc;
}