/*
 * step_motor.c
 *
 * Created: 8/2/2024 4:08:43 PM
 *  Author: HTSANG
 */ 


/*
 * command.c
 *
 * Created: 5/19/2024 8:37:53 PM
 *  Author: Admin
 */ 
#include "scheduler.h"
#include "command.h"
#include "uart.h"
#include <stdlib.h>
#include "cmdline.h"
#include <avr/pgmspace.h>
#include "speed_cntr.h"
#include <avr/interrupt.h>


/* Private typedef -----------------------------------------------------------*/
typedef struct _StepMotor_TaskContextTypedef_
{
	SCH_TASK_HANDLE               taskHandle;
	SCH_TaskPropertyTypedef       taskProperty;
} StepMotor_TaskContextTypedef;

/* Private function -----------------------------------------------------------*/
static	void	step_motor_update(void);

/* Private data     -----------------------------------------------------------*/
static StepMotor_TaskContextTypedef	s_StepMotorTaskContext =
{
	SCH_INVALID_TASK_HANDLE,                 // Will be updated by Schedular
	{
		SCH_TASK_SYNC,                      // taskType;
		SCH_TASK_PRIO_0,                    // taskPriority;
		50,                                // taskPeriodInMS;
		step_motor_update                // taskFunction;
	}
};

static void step_motor_update(void)
{
	//Update motor status in here
}

void step_motor_init(void)
{
	// Init of IO pins
	sm_driver_Init_IO();

	// Init of Timer/Counter1
	speed_ctrl_Init_Timer1();
	
	sei();
	
}

void step_motor_create_task(void)
{
	SCH_TASK_CreateTask(&s_StepMotorTaskContext.taskHandle, &s_StepMotorTaskContext.taskProperty);
}
