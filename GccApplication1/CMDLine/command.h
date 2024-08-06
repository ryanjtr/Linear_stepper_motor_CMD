/*
 * command.h
 *
 * Created: 5/19/2024 8:38:57 PM
 *  Author: Admin
 */ 


#ifndef COMMAND_H_
#define COMMAND_H_

#include "cmdline.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define	COMMAND_MAX_LENGTH	64

void command_init(void);
void command_create_task(void);
void command_send_splash(void);

int Cmd_help(int argc, char *argv[]);
int Cmd_moto_control_speed (int argc, char *argv[]);

#endif /* COMMAND_H_ */