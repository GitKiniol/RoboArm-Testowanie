/*
 * driver.c
 *
 * Created: 15.07.2022 09:05:06
 *  Author: rybka
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "driver.h"

uint16_t CalculateTimerFrq(float motor_pulse_per_rev, float pulse_divider, float mechanical_divider, float speed_rpm)
{
	//Ir = Sr * Er * Mr				=> Liczba impulsów drivera programowego na obrót
	//Fo = So / 60 * Ir				=> Obliczenie czêstotoliwoœci wyjœciowej na podstawie prêdkoœci i liczby impulsów
	//CCA = (Fi / 2 * N * Fo) - 1	=> Obliczenie rejestru CCA na podstawie czêstotliwoœci wyjœciowej
	
	float ir = 0.0;
	float fo = 0.0;
	uint16_t cca = 0;
	ir = motor_pulse_per_rev * pulse_divider * mechanical_divider;					/*ustalenie liczby impulsów sterownika programowego na obrót silnika	*/
	fo = (speed_rpm / 60) * ir;														/*obliczenie czêstotliwoœci wyjœciowej sterownika programowego			*/
	cca = (uint16_t)((F_CPU / (2.0 * 8.0 * fo)) - 1);								/*obliczenie wartoœci rejestru timera									*/
	return cca;
}

driver_t *Driver_Init(driver_t *driver, TC1_t *timer, PORT_t *port, uint16_t motor_steps, uint8_t electrical_ratio, float mechanical_ratio)
{
	driver = (driver_t *)malloc(sizeof(driver_t));
	driver->CurrentPosition = 0;
	driver->SetpointPosition = 85;
	driver->MaximumPosition = 90;
	driver->MinimumPosition = -90;
	driver->Direction = 0;
	driver->EnablePin = 6;
	driver->DirectionPin = 5;
	driver->PulsePin = 4;
	driver->IsRunning = 0;
	driver->DriverTimer = timer;
	driver->DriverPort = port;
	driver->MotorSteps = motor_steps;
	driver->ElectricalRatio = electrical_ratio;
	driver->MechanicalRatio = mechanical_ratio;
	driver->Speed = 150;
	/* ustawienie portu pinów wyjœciowych sterownika */
	driver->DriverPort->DIRSET = (1<<driver->DirectionPin);
	driver->DriverPort->DIRSET = (1<<driver->EnablePin);
	driver->DriverPort->DIRSET = (1<<driver->PulsePin);
	/* parametryzacja timera */
	driver->DriverTimer->CTRLA = TC_CLKSEL_OFF_gc;							/* preskaler= 0												*/
	driver->DriverTimer->CTRLB = TC_WGMODE_FRQ_gc;							/* tryb FRQ													*/
	driver->DriverTimer->CTRLB |= (1<<TC1_CCAEN_bp);						/* za³¹czenie generowania impulsów na pinie wyjœciowym		*/
	driver->DriverTimer->INTCTRLA = TC1_OVFINTLVL0_bm;						/* odblokowanie przerwania od przepe³nienia timera			*/
	
	
	return driver;
}

void Driver_Run(driver_t *driver)
{
	driver->DriverPort->OUTSET = (1<<driver->EnablePin);
	driver->DriverPort->OUTCLR = (1<<driver->PulsePin);
	driver->IsRunning = 1;
	_delay_ms(1000);
	driver->DriverTimer->CTRLA = TC_CLKSEL_DIV8_gc;							/* preskaler = 8, start										*/
}

void Driver_Stop(driver_t *driver)
{
	driver->IsRunning = 0;
	driver->DriverTimer->CTRLA = TC_CLKSEL_OFF_gc;							/* preskaler = 8, start										*/
	driver->DriverPort->OUTCLR = (1<<driver->EnablePin);
}

void Driver_SetParameters(driver_t *driver, uint8_t dir, uint8_t angle, uint8_t speed)
{
	dir ? (driver->DriverPort->OUTSET = (1<<driver->DirectionPin)) : (driver->DriverPort->OUTCLR = (1<<driver->DirectionPin));	/*ustawienie kierunku obrotów			*/
	driver->Direction = dir;
	driver->SetpointPosition = (int16_t)((((driver->MotorSteps * driver->ElectricalRatio * driver->MechanicalRatio) / 360) * angle) * 2);																							/*ustawienie pozycji zadanej			*/
	driver->Speed = speed;
	driver->DriverTimer->CCA = CalculateTimerFrq((float)sterownik->MotorSteps, (float)sterownik->ElectricalRatio, sterownik->MechanicalRatio, (float)speed);	/*ustawienie rejestru czêstotliwoœci	*/
}


ISR(TCC1_OVF_vect)
{
	//if(!sterownik->Direction)
	//{
		//sterownik->CurrentPosition++;
		//if (sterownik->CurrentPosition >= sterownik->SetpointPosition)
		//{
			//Driver_Stop(sterownik);
		//}
	//}
	//else
	//{
		//sterownik->CurrentPosition--;
		//if (sterownik->CurrentPosition <= sterownik->SetpointPosition)
		//{
			//Driver_Stop(sterownik);
		//}
	//}
}