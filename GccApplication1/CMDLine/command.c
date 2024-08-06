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

/* Private typedef -----------------------------------------------------------*/
typedef struct _Command_TaskContextTypedef_
{
	SCH_TASK_HANDLE               taskHandle;
	SCH_TaskPropertyTypedef       taskProperty;
} Command_TaskContextTypedef;


//typedef enum{OK = 0, CMDLINE_BAD_CMD, CMDLINE_TOO_MANY_ARGS, CMDLINE_TOO_FEW_ARGS, CMDLINE_INVALID_ARG} command_error_code_t;
const char  * ErrorCode[5] = {"OK\r\n", "CMDLINE_BAD_CMD\r\n", "CMDLINE_TOO_MANY_ARGS\r\n",
"CMDLINE_TOO_FEW_ARGS\r\n", "CMDLINE_INVALID_ARG\r\n" };

static	void	command_task_update(void);
tCmdLineEntry g_psCmdTable[] = {{"help", Cmd_help," | format: help" },
								{"moto", Cmd_moto_control_speed," | format: moto <step> <accel> <decel> <speed>"},
								{0,0,0}
								};

volatile static	ringbuffer_t *p_CommandRingBuffer;
volatile static	char s_commandBuffer[COMMAND_MAX_LENGTH];
static uint8_t	s_commandBufferIndex = 0;



static Command_TaskContextTypedef           s_CommandTaskContext =
{
	SCH_INVALID_TASK_HANDLE,                 // Will be updated by Schedular
	{
		SCH_TASK_SYNC,                      // taskType;
		SCH_TASK_PRIO_0,                    // taskPriority;
		10,                                // taskPeriodInMS;
		command_task_update                // taskFunction;
	}
};

static	void	command_task_update(void)
{
	char rxData;
	int8_t	ret_val;
	while (! rbuffer_empty(p_CommandRingBuffer))
	{
		rxData = rbuffer_remove(p_CommandRingBuffer);
		usart0_send_char(rxData);
		if ((rxData == '\r') || (rxData == '\n'))		//got a return or new line
		{
			if (s_commandBufferIndex > 0)		//if we got the CR or LF at the begining, discard
			{
				s_commandBuffer[s_commandBufferIndex] = 0;
				s_commandBufferIndex++;
				ret_val = CmdLineProcess(s_commandBuffer);
				s_commandBufferIndex = 0;
				usart0_send_string(ErrorCode[ret_val]);
				usart0_send_string("> ");
			}
			else usart0_send_string("\r\n> ");
		}
		else if ((rxData == 8) || (rxData == 127))
		{
			if (s_commandBufferIndex > 0) s_commandBufferIndex--;
		}
		else
		{
			s_commandBuffer[s_commandBufferIndex] = rxData;
			s_commandBufferIndex ++;
			if (s_commandBufferIndex > COMMAND_MAX_LENGTH) s_commandBufferIndex= 0;
		}
	}
}

void	command_init(void)
{
	usart0_init();
	p_CommandRingBuffer = uart_get_uart0_rx_buffer_address();
	memset((void *)s_commandBuffer, 0, sizeof(s_commandBuffer));
	s_commandBufferIndex = 0;
	usart0_send_string("> ");
/*	command_send_splash();*/
}

void	command_create_task(void)
{
	SCH_TASK_CreateTask(&s_CommandTaskContext.taskHandle, &s_CommandTaskContext.taskProperty);
}

int Cmd_help(int argc, char *argv[]) {
	tCmdLineEntry *pEntry;

	usart0_send_string("\nAvailable commands\r\n");
	usart0_send_string("------------------\r\n");

	// Point at the beginning of the command table.
	pEntry = &g_psCmdTable[0];

	// Enter a loop to read each entry from the command table.  The
	// end of the table has been reached when the command name is NULL.
	while (pEntry->pcCmd) {
		// Print the command name and the brief description.
		usart0_send_string(pEntry->pcCmd);
		usart0_send_string(pEntry->pcHelp);
		usart0_send_string("\r\n");

		// Advance to the next entry in the table.
		pEntry++;

	}
	// Return success.
	return (CMDLINE_OK);
}


int Cmd_moto_control_speed (int argc, char *argv[]) {
	if (argc < 5) return CMDLINE_TOO_FEW_ARGS;
	if (argc > 5) return CMDLINE_TOO_MANY_ARGS;
	int32_t step = atoi(argv[1]);
	int32_t accel = atoi(argv[2]);
	int32_t decel = atoi(argv[3]);
	int32_t speed = atoi(argv[4]);
	speed_ctrl_Move(step, accel, decel, speed);
	return CMDLINE_OK;
}


void	command_send_splash(void)
{
	usart0_send_string("> ");
}