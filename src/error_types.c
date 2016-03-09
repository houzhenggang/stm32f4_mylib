/*
 * error_types.c
 *
 *  Created on: 6 mar 2016
 *      Author: Konrad
 */

#include "error_types.h"
#include "diag/Trace.h"


__attribute__((optimize("O0")))
void err_check( uint8_t* file_name, size_t file_name_size, uint8_t* func_name, size_t function_name_size, uint32_t line_number)
{
	trace_write((const char*)file_name, file_name_size);
	trace_write("\n\r", 2);
	trace_write((const char*)func_name, function_name_size);
	trace_write("\n\r", 2);
	trace_printf("%d", line_number);

	while(1)
	{
		__WFE();
	}
}
