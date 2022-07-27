/*
 * acceleration.h
 *
 * Created: 15.07.2022 09:06:01
 *  Author: rybka
 */ 

#include "../Driver/driver.h"

#ifndef ACCELERATION_H_
#define ACCELERATION_H_


void Acceleration_TimerInit(void);

void Acceleration_Start(driver_t driver);


#endif /* ACCELERATION_H_ */