/*
 * error_types.h
 *
 *  Created on: 6 mar 2016
 *      Author: Konrad
 */

#ifndef ERROR_TYPES_H_
#define ERROR_TYPES_H_

#include "stdlib.h"
#include "diag/Trace.h"
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define APP_ERR_CHECK(variable, condition_on_continue)		variable!=condition_on_continue ? err_check(__FILENAME__, sizeof(__FILENAME__, __FUNCTION__, sizeof(__FUNCTION__), __LINE__);

typedef enum
{
	OP_SUCCESS = 0,
	ERROR_NO_MEM,
	ERROR_INTERNAL,
	ERROR_TIMEOUT,
	ERROR_WRONG_ARG,
	ERROR_BUSY

}error_e;


void err_check(uint8_t* file_name, size_t file_name_size, uint8_t* func_name, size_t function_name_size, uint32_t line_number);




#endif /* ERROR_TYPES_H_ */
