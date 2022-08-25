/*
 * interface.c
 *
 * Created: 02.08.2022 19:28:55
 *  Author: rybka
 */ 
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "interface.h"

drv1_t *driver1_1, *driver1_2;
drv2_t *driver2_1;

drv1_t *Driver1Init(drv1_t *driver)
{
	drv1_t *dr1 = (drv1_t*)malloc(sizeof(drv1_t));
	dr1->speed = 0;
	dr1->Start = &StartDrv1;
	return dr1;
}

drv2_t *Driver2Init(drv2_t *driver)
{
	drv2_t *dr2 = (drv2_t*)malloc(sizeof(drv2_t));
	dr2->position = 1200;
	dr2->speed = 0;
	dr2->Start = &StartDrv2;
	return dr2;
}

void StartDrv1(drv1_t *driver, uint8_t dir)
{
	driver->speed = 1;
}

void StartDrv2(drv2_t *driver, uint8_t dir)
{
	driver->speed = 1;
}