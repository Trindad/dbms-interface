#include "shell.h"

void shell()
{
	int option = 0;

	while(1)
	{
		if (option > 1)
		{
			break;
		}
	}

	executeLine(option);

	shell();
		
}

void executeLine(int option)
{
	switch(option){

		case INSERT:
		{
			break;
		}
		case CONNECT:{
			break;
		}
		case SHOW:{
			break;
		}
		case EXIT:{
			break;
		}
		default:{
			break;
		}
	}
}