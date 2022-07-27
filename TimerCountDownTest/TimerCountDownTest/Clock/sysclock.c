/*
 * sysclock.c
 *
 * Created: 05.02.2022 07:19:11
 *  Author: rybka
 */ 

#include <avr/io.h>
#include "sysclock.h"

/*-----------------definicje funkcji---------------------------------------------------------------*/

void ClkSys32MHz(void)
{
	OSC.CTRL = OSC_RC32MEN_bm;
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));
	CPU_CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
}

void ClkSys2MHz(void)
{
	OSC.CTRL = OSC_RC2MEN_bm;
	while(!(OSC.STATUS & OSC_RC2MRDY_bm));
	CPU_CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC2M_gc;
}

/*-------------------------------------------------------------------------------------------------*/