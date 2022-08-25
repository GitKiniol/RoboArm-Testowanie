/*
 * drivers.c
 *
 * Created: 19.03.2022 19:28:12
 *  Author: rybka
 */ 


#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "drivers.h"
#include "../../BLUETOOTH/DATA/data.h"
#include "../WORK/work.h"

/*-------------------------------------------Deklaracje zmiennych---------------------------------------------------------------*/
/* EXTERN: */

/* LOCAL:  */
stepper_driver_t *axisA, *axisB, *axisC, *axisZ;		/* osie napêdzane silnikami krokowymi									*/
servo_driver_t *axisG, *axisT;							/* osie napêdzane silnikami serwo										*/
to_run_list_t *drvToRunList;							/* lista driverów przydzielona do zadania								*/

/*------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------Definicje funkcji------------------------------------------------------------------*/

void Driver_AxisInit(void)
{
	axisA = Driver_StepperDriverInit(axisA, &TCF1, &PORTF, 200, 16, 1);	
	axisA->MaximumPosition = axisA->Convert(90, axisA);
	axisA->MinimumPosition = axisA->Convert(-90, axisA);
	axisB = Driver_StepperDriverInit(axisB, &TCE1, &PORTE, 200, 16, 1);
	axisB->MaximumPosition = axisB->Convert(90, axisB);
	axisB->MinimumPosition = axisB->Convert(-90, axisB);
	axisC = Driver_StepperDriverInit(axisC, &TCD1, &PORTD, 200, 16, 1);
	axisC->MaximumPosition = axisC->Convert(90, axisC);
	axisC->MinimumPosition = axisC->Convert(-90, axisC);
	axisZ = Driver_StepperDriverInit(axisZ, &TCC1, &PORTC, 200, 16, 1);
	axisZ->MaximumPosition = axisZ->Convert(90, axisZ);
	axisZ->MinimumPosition = axisZ->Convert(-90, axisZ);
	axisG = Driver_ServoDriverInit(axisG, &TCC0, &PORTC, 0);
	axisT = Driver_ServoDriverInit(axisT, &TCC0, &PORTC, 1);
	
	drvToRunList = Driver_ToRunListInit();
}

stepper_driver_t *Driver_StepperDriverInit(stepper_driver_t *driver, TC1_t *timer, PORT_t *port, uint16_t motor_steps, uint8_t electrical_ratio, float mechanical_ratio)
{
	driver = (stepper_driver_t *)malloc(sizeof(stepper_driver_t));
	driver->CurrentPosition = 0;
	driver->SetpointPosition = 85;
	driver->MaximumPosition = 0;
	driver->MinimumPosition = 0;
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
	driver->DriverTimer->CTRLA = TC_CLKSEL_OFF_gc;						/* preskaler= 0												*/
	driver->DriverTimer->CTRLB = TC_WGMODE_FRQ_gc;						/* tryb FRQ													*/
	driver->DriverTimer->CTRLB |= (1<<TC1_CCAEN_bp);					/* za³¹czenie generowania impulsów na pinie wyjœciowym		*/
	driver->DriverTimer->INTCTRLA = TC1_OVFINTLVL0_bm;					/* odblokowanie przerwania od przepe³nienia timera			*/
	/* ustawienie wskaŸników na funkcje */
	driver->Start = &Driver_StartStepperDriver;							/* ustawienie wskaŸnika na funkcjê start					*/
	driver->Stop = &Driver_StopStepperDriver;							/* ustawienie wskaŸnika na funkcjê stop						*/
	driver->Free = &Driver_FreeStepper;									/* ustawienie wskaŸnika na funkcjê luzuj¹c¹ silnik			*/
	driver->Convert = &Driver_ConvertAngleToStep;						/* ustawienie wskaŸnika na funkcjê convert					*/
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	return driver;
}

servo_driver_t *Driver_ServoDriverInit(servo_driver_t *driver, TC0_t *timer, PORT_t *port, uint8_t pwmpin)
{
	driver = (servo_driver_t *)malloc(sizeof(servo_driver_t));
	driver->DriverPort = port;
	driver->DriverTimer = timer;
	driver->IsRunning = 0;
	driver->MaximumPosition = 90;
	driver->MinimumPosition = 0;
	driver->SetpointPosition = 0;
	driver->PwmPin = pwmpin;
	driver->DriverPort->DIRCLR = (1 << driver->PwmPin);
	driver->DriverTimer->CTRLB |= TC_WGMODE_DS_T_gc;
	driver->DriverTimer->CTRLB |= (1<<(7 - pwmpin));					/* przekazanie sterowania pinem do timer sprzêtowego		*/
	driver->DriverTimer->INTCTRLB |= (1<<(1 + pwmpin));
	driver->DriverTimer->PER = 40000;									/* ustawienie czêstotliwoœci pwm na 50Hz					*/
	driver->Start = &Driver_StartServoDriver;							/* ustawienie wskaŸnika na funkcjê start					*/
	driver->Stop = &Driver_StopServoDriver;								/* ustawienie wskaŸnika na funkcjê stop						*/
	driver->Convert = &Driver_ConvertAngleToPwm;						/* ustawienie wskaŸnika na funkcjê convert					*/
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	
	return driver;
}

int16_t Driver_ConvertAngleToStep(int8_t angle, void *driver)
{
	/* ca³kowita iloœæ impulsów na obrót									*/
	float pulsesPerRev = 0.0;	
	/*rzutowanie void do stepper_driver_t									*/													
	stepper_driver_t *drv = (stepper_driver_t *)driver;		
	/*ustalenie liczby impulsów sterownika programowego na obrót silnika	*/				
	pulsesPerRev = drv->MotorSteps * drv->ElectricalRatio * drv->MechanicalRatio;		
	return (int16_t)((pulsesPerRev / 360) * angle);
}

void Driver_SetStepperSpeed(stepper_driver_t *driver, uint8_t speed)
{
	//Ir = Sr * Er * Mr				=> Liczba impulsów drivera programowego na obrót
	//Fo = So / 60 * Ir				=> Obliczenie czêstotoliwoœci wyjœciowej na podstawie prêdkoœci i liczby impulsów
	//CCA = (Fi / 2 * N * Fo) - 1	=> Obliczenie rejestru CCA na podstawie czêstotliwoœci wyjœciowej
	//Sr - liczba kroków silnika
	//Er - przek³adnia elektryczna(podzia³ kroku, ustawiany switchami)
	//Mr - prze³o¿enie mechaniczne wynikaj¹ce ze œrednic kó³ zêbatych
	
	float ir = 0.0;
	float fo = 0.0;
	ir = driver->MotorSteps * driver->ElectricalRatio * driver->MechanicalRatio;	/*ustalenie liczby impulsów sterownika programowego na obrót silnika	*/
	fo = (speed / 60) * ir;															/*obliczenie czêstotliwoœci wyjœciowej sterownika programowego			*/
	driver->DriverTimer->CCA = (uint16_t)((F_CPU / (2.0 * 8.0 * fo)) - 1);			/*obliczenie wartoœci rejestru timera									*/
	
}

void Driver_StartStepperDriver(void *driver, uint8_t preskaler)
{
	stepper_driver_t *drv = (stepper_driver_t*)driver;				/* konwersja typu parametru	*/
	drv->IsRunning = 1;												/* sterownik uruchomiony	*/
	drv->DriverPort->OUTSET = (1<<drv->EnablePin);					/* odblokowanie silnika		*/
	if (drv->Direction)
	{
		drv->DriverPort->OUTSET = (1<<drv->DirectionPin);			/* prawe obroty				*/
	} 
	else
	{
		drv->DriverPort->OUTCLR = (1<<drv->DirectionPin);			/* lewe obroty				*/
	}
	drv->DriverTimer->CTRLA = preskaler;							/* uruchomienie timera		*/
}

void Driver_StartServoDriver(void *driver, uint8_t preskaler)
{
	servo_driver_t *drv = (servo_driver_t*)driver;					/* konwersja typu parametru						*/
	drv->DriverTimer->CTRLA = preskaler;							/* uruchomienie timera							*/
	drv->IsRunning = 1;												/* driver uruchomiony							*/
	drv->DriverPort->DIRSET = (1 << drv->PwmPin);					/* odblokowanie pinu pwm						*/
}

void Driver_StopStepperDriver(void *driver)
{
	stepper_driver_t *drv = (stepper_driver_t *)driver;				/* konwersja typu parametru	*/
	drv->IsRunning = 0;												/* sterownik zatrzymany		*/
	drv->DriverTimer->CTRLA = TC_CLKSEL_OFF_gc;						/* zatrzymanie timera		*/
}

void Driver_StopServoDriver(void *driver)
{
	servo_driver_t *drv = (servo_driver_t*)driver;					/* konwersja typu parametru						*/
	drv->IsRunning = 0;												/* driver zatrzymany							*/
	drv->DriverPort->DIRCLR = (1<<drv->PwmPin);						/* zablokowanie pinu pwm						*/
	if(axisT->IsRunning == 0 && axisG->IsRunning == 0)				/* jeœli servosterowniki zakoñczy³y pracê, to:	*/
	{
		drv->DriverTimer->CTRLA = TC_CLKSEL_OFF_gc;					/* zatrzymanie timera							*/
	}
}

uint16_t Driver_ConvertAngleToPwm(uint8_t angle)
{
	return (uint16_t)((angle * 18.88) + 2300);						/* przeliczanie k¹ta na wartoœæ rejestru timera	*/
}

void Driver_SetDriverParameters(move_t *move)
{
	switch(move->AxisName)
	{
		case 65:
			Driver_SetStepperParameters(axisA, move->Speed, move->Angle, move->Direction);		/* parametryzacja osi A										*/
			Driver_ToRunListAdd(drvToRunList, axisA, STEPPER);									/* umieszczenie sterownika osi na liœcie do uruchomienia	*/
			break;
		case 66:
			Driver_SetStepperParameters(axisB, move->Speed, move->Angle, move->Direction);		/* parametryzacja osi B										*/
			Driver_ToRunListAdd(drvToRunList, axisB, STEPPER);									/* umieszczenie sterownika osi na liœcie do uruchomienia	*/
			break;
		case 67:
			Driver_SetStepperParameters(axisC, move->Speed, move->Angle, move->Direction);		/* parametryzacja osi C										*/
			Driver_ToRunListAdd(drvToRunList, axisC, STEPPER);									/* umieszczenie sterownika osi na liœcie do uruchomienia	*/
			break;
		case 90:
			Driver_SetStepperParameters(axisZ, move->Speed, move->Angle, move->Direction);		/* parametryzacja osi Z										*/
			Driver_ToRunListAdd(drvToRunList, axisZ, STEPPER);									/* umieszczenie sterownika osi na liœcie do uruchomienia	*/
			break;
		case 71:
			Driver_SetServoParameters(axisG, move->Angle);										/* parametryzacja osi G										*/
			Driver_ToRunListAdd(drvToRunList, axisG, SERVO);									/* umieszczenie sterownika osi na liœcie do uruchomienia	*/
			break;
		case 84:
			Driver_SetServoParameters(axisT, move->Angle);										/* parametryzacja osi T										*/
			Driver_ToRunListAdd(drvToRunList, axisT, SERVO);									/* umieszczenie sterownika osi na liœcie do uruchomienia	*/
			break;
		default:
			break;
	}
}

void Driver_SetStepperParameters(stepper_driver_t *driver, uint8_t speed, uint8_t angle, uint8_t dir)
{
	Driver_SetStepperSpeed(driver, speed);												/* ustawienie prêdkoœci silnika				*/
	driver->SetpointPosition = driver->Convert(angle, driver);							/* ustawienie pozycji zadanej				*/
	driver->Direction = dir;															/* ustawienie kierunku pracy				*/
}

void Driver_SetServoParameters(servo_driver_t *driver, uint8_t angle)
{
	driver->SetpointPosition = driver->Convert(angle);									/* ustawienie pozycji zadanej serva			*/
}

to_run_list_t *Driver_ToRunListInit(void)
{
	to_run_list_t *ptrToRunList = (to_run_list_t*)malloc(sizeof(to_run_list_t));		/* alokacja pamiêci dla listy				*/
	ptrToRunList->Head = NULL;															/* wyzerowanie wskaŸnika pocz¹tku listy		*/
	ptrToRunList->Tail = NULL;															/* wyzerowanie wskaŸnika koñca listy		*/
	return ptrToRunList;
}

to_run_item_t *Driver_ToRunItemInit(to_run_drv_t *torundrv)
{
	to_run_item_t *ptrToRunItem = (to_run_item_t*)malloc(sizeof(to_run_item_t));		/* alokacja pamiêci dla elementu listy		*/
	ptrToRunItem->Next = NULL;															/* zerowanie wskaŸnika na element nastêpny	*/
	ptrToRunItem->Data = torundrv;														/* ustawienie wskaŸnika na dane elementu	*/
	return ptrToRunItem;
}

to_run_drv_t *Driver_ToRunDrvInit(void *driver, uint8_t drvtype)
{
	to_run_drv_t *ptrToRunDrv = (to_run_drv_t*)malloc(sizeof(to_run_drv_t));			/* alokacja pamiêci dla drivera				*/
	ptrToRunDrv->DriveType = drvtype;													/* ustawienie typu drivera					*/
	ptrToRunDrv->ToRunDriver = driver;													/* ustawienie wskaŸnika na driver do pracy	*/
	return ptrToRunDrv;	
}

void Driver_ToRunListAdd(to_run_list_t *list, void *driver, uint8_t drvtype)
{
	to_run_drv_t *newDrv = (to_run_drv_t*)malloc(sizeof(to_run_drv_t));					/* alokacja pamiêci napêdu do uruchomienia		*/
	newDrv = Driver_ToRunDrvInit(driver, drvtype);										/* inicjalizacja napêdu do uruchomienia			*/
	to_run_item_t *newItem = (to_run_item_t*)malloc(sizeof(to_run_item_t));				/* alokacja pamiêci na item listy				*/
	newItem = Driver_ToRunItemInit(newDrv);												/* inicjalizacja elementu listy					*/
	if (list->Tail == NULL)
	{
		list->Tail = newItem;															/* wstawienie nowego elementu na koniec listy	*/
		list->Head = list->Tail;														/* ustawienie pocz¹tku listy na koniec listy	*/
		newItem->Next = NULL;															/* zerowanie wskaŸnika na kolejny element		*/
	} 
	else
	{
		newItem->Next = list->Head;														/* ustawienie wskaŸnika nowemu na stary element	*/
		list->Head = newItem;															/* wstawienie nowego elementu na pocz¹tek listy	*/
	}
}

to_run_drv_t *Driver_ToRunListGet(to_run_list_t *list)
{
	if (list->Tail != NULL)
	{
		to_run_drv_t *copyDrv = (to_run_drv_t*)malloc(sizeof(to_run_drv_t));		/* alokacja pamiêci na kopie drivera do uruchomienia			*/
		copyDrv->DriveType = list->Head->Data->DriveType;							/* wykonanie kopi typu drivera									*/
		copyDrv->ToRunDriver = list->Head->Data->ToRunDriver;						/* wykonanie kopi drivera										*/
		to_run_item_t *nextItem = list->Head->Next;									/* kopia wskaŸnika na nastêpny element listy					*/
		list->Head->Data->ToRunDriver = NULL;										/* zerowanie wskaŸnika											*/
		free(list->Head->Data);														/* zwolnienie pamiêci zajmowanej przez driver do uruchomienia	*/
		list->Head->Data = NULL;													/* zerowanie wskaŸnika na dane elementu							*/
		list->Head->Next = NULL;													/* zerowanie wskaŸnika na nastêpny element						*/
		free(list->Head);															/* zwolnienie pamiêci zajmowanej przez element listy			*/
		list->Head = nextItem;														/* ustawienie wskaŸnika pocz¹tku listy							*/
		if(list->Head == NULL) list->Tail = NULL;									/* jeœli pobrano wszystkie elementy to wyzeruj wskaŸnik koñca	*/
		return copyDrv;																/* zwrócenie kopi drivera do uruchomienia						*/
	}
	else
	{
		return NULL;																/* zwrócenie pustego wskaŸnika w przypadku gdy lista pusta		*/
	}
}

void Driver_RunTaskAxes(void)
{
	if(drvToRunList->Tail != NULL)													/* jeœli lista osi do uruchomienia nie jest pusta		*/
	{
		to_run_drv_t *toRun;														/* element pobrany z listy								*/
		ATOMIC_BLOCK(ATOMIC_FORCEON)												/* wykonaj poni¿sze instrukcje ATOMOWO					*/
		{
			while (drvToRunList->Tail)												/* pêtla wykonywana do opró¿nienia listy osi			*/
			{
				toRun = Driver_ToRunListGet(drvToRunList);							/* pobierz element z listy								*/
				if (toRun->DriveType == STEPPER)									/* jeœli pobrany element dotyczy osi z krokówk¹			*/
				{
					stepper_driver_t *stdt = (stepper_driver_t*)toRun->ToRunDriver;	/* pobierz driver do uruchomienia						*/
					stdt->Start(stdt, TC_CLKSEL_DIV8_gc);							/* uruchom driver										*/
				} 
				else																/* jeœli pobrany element dotyczy osi z serwem			*/
				{
					servo_driver_t *svdt = (servo_driver_t*)toRun->ToRunDriver;		/* pobierz driver do uruchomienia						*/
					svdt->Start(svdt, TC_CLKSEL_DIV8_gc);							/* uruchom driver										*/
				}
				toRun->ToRunDriver = NULL;											/* zerowanie wskaŸnika									*/
				free(toRun);														/* zwalnianie pamiêci									*/
			}
		};
	}
}

void Driver_FreeStepper(void *driver)
{
	stepper_driver_t *drv = (stepper_driver_t*)driver;
	drv->DriverPort->OUTCLR = (1<<drv->EnablePin);					/* luzowanie silnika				*/
}

void Driver_FreeAxes(void)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		axisA->Free(axisA);											/* luzowanie silnika osi A			*/
		axisB->Free(axisB);											/* luzowanie silnika osi B			*/
		axisC->Free(axisC);											/* luzowanie silnika osi C			*/
		axisZ->Free(axisZ);											/* luzowanie silnika osi Z			*/
	}
}

void Driver_StopRobot(void)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		axisA->Stop(axisA);											/* zatrzymanie silnika osi A		*/
		axisB->Stop(axisB);											/* zatrzymanie silnika osi B		*/
		axisC->Stop(axisC);											/* zatrzymanie silnika osi C		*/
		axisZ->Stop(axisZ);											/* zatrzymanie silnika osi Z		*/
		axisG->Stop(axisG);											/* zatrzymanie silnika osi G		*/
		axisT->Stop(axisT);											/* zatrzymanie silnika osi T		*/								
	}
}

void Driver_EmergencyStop(void)
{
	Driver_StopRobot();
	Driver_FreeAxes();
}

void Driver_StepperTimerIsr(stepper_driver_t *driver)
{
	static uint8_t ar = 0;
	static uint8_t br = 0;
	static uint8_t cr = 0;
	static uint8_t zr = 0;
	ar = axisA->IsRunning;
	br = axisB->IsRunning;
	cr = axisC->IsRunning;
	zr = axisZ->IsRunning;
	driver->Direction ? driver->CurrentPosition++ : driver->CurrentPosition--;		/* w zale¿noœci od kierunku obrotów, zwiêkszaj lub zmniejszaj wartoœæ pozycji aktualnej	*/
	if (driver->CurrentPosition == driver->SetpointPosition || 
		driver->CurrentPosition >= driver->MaximumPosition ||
		driver->CurrentPosition <= driver->MinimumPosition)							/* jeœli osi¹gniêto pozycjê zadan¹ lub skrajn¹, to:										*/		
	{
		driver->Stop(driver);														/* zatrzymaj napêd																		*/			
	}
	if (ar == 0 && br == 0 && cr== 0 && zr == 0)									/* sprawdzenie czy jeszcze pracuje któraœ z osi, jeœli nie to:							*/
	{
		Work_TimerStart(RunTaskTimer);												/* uruchom kolejne zadanie, odbywa siê to poprzez uruchomienie timera taktuj¹cego		*/
	}
}

/*----------------------------------------------------------------------------------------------------------------------------------*/


/*---------------------------------Przerwania driverów------------------------------------------------------------------------------*/
/* axis Z*/
ISR(TCC1_OVF_vect)
{
	Driver_StepperTimerIsr(axisZ);
}

ISR(TCD1_OVF_vect)
{
	Driver_StepperTimerIsr(axisC);
}

ISR(TCE1_OVF_vect)
{
	Driver_StepperTimerIsr(axisB);
}

ISR(TCF1_OVF_vect)
{
	Driver_StepperTimerIsr(axisA);
}
/*----------------------------------------------------------------------------------------------------------------------------------*/