/*
 * GccApplication1.c
 *
 * Created: 31/07/2024 1:32:53 CH
 * Author : Asus
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "sm_driver.h"
#include "speed_cntr.h"
#include "global.h"
#include "uart.h"
#include "command.h"
#include "step_motor.h"
#include "scheduler.h"

int64_t str1[30];
int64_t str2[30];
int64_t str3[30];

speedRampData srd;
struct GLOBAL_FLAGS status = {FALSE, FALSE, 0};


int main(void)
{	
	SCH_Initialize();
	command_init();
	command_create_task();
 	step_motor_init();
 	step_motor_create_task();
	SCH_StartSchedular();
	while(1)
	{
/*		speed_cntr_Move(steps, acceleration, deceleration, speed);*/
		SCH_HandleScheduledTask();
	}
}
