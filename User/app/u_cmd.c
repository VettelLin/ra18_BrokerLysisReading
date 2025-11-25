/*
 * cmd.c
 *
 *  Created on: Sep 4, 2021
 *      Author: abel
 */




#include <stdlib.h>
#include <string.h>

#include "lib/l_uart.h"



int com_app(int argc, char *argv[])
{
	int err = 0;
	char *cmd = argv[0];
//	argv[0] = argv[argc];

	if(strcmp(cmd, "temp") == 0){

	}


	return err;
}
