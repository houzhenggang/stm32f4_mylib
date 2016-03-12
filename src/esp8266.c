/*
 * esp8266.c
 *
 *  Created on: 6 mar 2016
 *      Author: Konrad
 */

#include "esp8266.h"
#include "USART.h"
#include "error_types.h"


/**
 * \brief This function forms query from the given AT command. It returns the created command with "?\r\n" in \v new_command variable
 *
 * \param command - ptr to the command
 * \param command_size - size of command
 * \param command_dyn_alloc - flag informing whether \v command was dynamically allocated. If so, it frees it
 * \param new_command - pointer to the just created command. NOTE: IT IS ALWAYS DYNAMICALLY ALLOCATED
 *
 * return 	OP_SUCCESS
 * 			ERROR_NO_MEM
 *  */
static error_e Esp_Form_Query(uint8_t* command, uint16_t command_size, bool command_dyn_alloc, uint8_t** new_command)
{
	error_e retval;

	uint8_t* __new_command = malloc(command_size + 3);
	if(__new_command)
	{
		memcpy(__new_command, command, command_size);
		memcpy(__new_command+command_size, "?\r\n", 3);
		if(command_dyn_alloc)
			free(command);

		*new_command = __new_command;
		return OP_SUCCESS;
	}

	return ERROR_NO_MEM;
}

/**
 * \brief This is Uart_Send_Data() wrapper. It just uses the ESP8266_USART_MODULE constantly to transmit data
 *
 * \param command - buffer containing the entire command with optional arguments
 * \param data_size - size of command to be sent
 * \param command_dyn_allocated - bool value which points whether the buffer should be freed or not
 * \param wait_till_tx_end - flag which contains information whether the function should block program execution untill data is transmitted
 *
 * \return	OP_SUCCESS
 * 			ERROR_BUSY - transmission is already in progress
 * 			ERROR_NO_MEM - if command is statically allocated (on stack) it is internally copied in dynamic buffer. This retval informs that heap is out of memory
 *
 */
static inline error_e Esp_Send_Command(uint8_t* command, uint16_t data_size, bool command_dyn_allocated, bool wait_till_tx_end)
{
	return Uart_Send_Data(ESP8266_USART_MODULE, command, data_size, command_dyn_allocated, wait_till_tx_end);
}

/**
 * \brief This function sends the AT command without any argument to the esp module. It just appends the command with "\r\n" suffix
 *
 * \param command - command to process
 * \param command_size - size of the command (INCLUDING '\0' character)
 *
 * \return	OP_SUCCESS
 * 			ERROR_NO_MEM
 */
static error_e	Esp_Send_Command_Without_Args(uint8_t* command, uint16_t command_size)
{
	uint8_t* cmd = malloc(command_size+1);
	if(cmd)
	{
		///	Copy the command without '\0' character
		memcpy(cmd, command, command_size-1);
		///	Copy the "\r\n" suffix
		memcpy(cmd+command_size-1, "\r\n", 2);
		return Esp_Send_Command(cmd, command_size+1, true, true);
	}

	return ERROR_NO_MEM;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

error_e Esp_Init()
{

	return OP_SUCCESS;
}

/**
 * \brief This command sends the reset AT command to the ESP8266 module
 *
 * \return 	OP_SUCCESS
 * 			ERROR_NO_MEM - no mem to create command
 */
error_e Esp_Reset()
{
	return Esp_Send_Command_Without_Args(ESP_RESET, sizeof(ESP_RESET));
}



/**
 * \brief This function changes the UART communication parameters until the device is powered on.
 *
 * \param baudrate - char array containing readable baudrate, e.g "115200"
 * \param baudrate_size - size of the \v baudrate array, including '\0' character
 * \param data_bits - number of data bits in character:
							ESP_UART_CONFIG_DATA_BITS_5
							ESP_UART_CONFIG_DATA_BITS_6
							ESP_UART_CONFIG_DATA_BITS_7
							ESP_UART_CONFIG_DATA_BITS_8
 * \param stop_bits - number of stop_bits:
 * 							ESP_UART_CONFIG_STOP_BITS_1
 * 							ESP_UART_CONFIG_STOP_BITS_1_5
 * 							ESP_UART_CONFIG_STOP_BITS_2
 * \param parity - parity configuration
 * 							ESP_UART_CONFIG_PARTITY_NO_PARITY
 * 							ESP_UART_CONFIG_PARTITY_ODD
 * 							ESP_UART_CONFIG_PARTITY_EVEN
 * \param flow_control	- flow control configuration
 * 							ESP_UART_CONFIG_FLOW_CONTROL_DISABLED
 * 							ESP_UART_CONFIG_FLOW_CONTROL_RTS_ENABLED
 * 							ESP_UART_CONFIG_FLOW_CONTROL_CTS_ENABLED
 * 							ESP_UART_CONFIG_FLOW_CONTROL_RTS_CTS_ENABLED
 *
 *	\return	OP_SUCCESS
 *			ERROR_NO_MEM
 */
error_e Esp_Configure_Uart_Current(char* baudrate, uint8_t baudrate_size, char data_bits, char stop_bits, char parity, char flow_control)
{
	uint8_t* cmd = malloc(sizeof(ESP_CHANGE_UART_CONFIG_CURRENT) + baudrate_size + 9);

	if(cmd)
	{
		memcpy(cmd, ESP_CHANGE_UART_CONFIG_CURRENT, sizeof(ESP_CHANGE_UART_CONFIG_CURRENT)-1);

		cmd[sizeof(ESP_CHANGE_UART_CONFIG_CURRENT)-1] = '=';
		///	Copy the baudrate config
		memcpy(&cmd[sizeof(ESP_CHANGE_UART_CONFIG_CURRENT)], baudrate, baudrate_size-1);

		uint8_t args_index = sizeof(ESP_CHANGE_UART_CONFIG_CURRENT) + baudrate_size-1;
		cmd[args_index++] = ',';
		cmd[args_index++] = (uint8_t)data_bits;
		cmd[args_index++] = ',';
		cmd[args_index++] = (uint8_t)stop_bits;
		cmd[args_index++] = ',';
		cmd[args_index++] = (uint8_t)parity;
		cmd[args_index++] = ',';
		cmd[args_index++] = (uint8_t)flow_control;
		memcpy(cmd[args_index], "\r\n", 2);

		return Esp_Send_Command(cmd, sizeof(ESP_CHANGE_UART_CONFIG_CURRENT) + baudrate_size + 9, true, true);
	}

	return ERROR_NO_MEM;
}

/**
 * \brief This function changes the UART communication parameters permanently.
 *
 * \param baudrate - char array containing readable baudrate, e.g "115200"
 * \param baudrate_size - size of the \v baudrate array, including '\0' character
 * \param data_bits - number of data bits in character:
							ESP_UART_CONFIG_DATA_BITS_5
							ESP_UART_CONFIG_DATA_BITS_6
							ESP_UART_CONFIG_DATA_BITS_7
							ESP_UART_CONFIG_DATA_BITS_8
 * \param stop_bits - number of stop_bits:
 * 							ESP_UART_CONFIG_STOP_BITS_1
 * 							ESP_UART_CONFIG_STOP_BITS_1_5
 * 							ESP_UART_CONFIG_STOP_BITS_2
 * \param parity - parity configuration
 * 							ESP_UART_CONFIG_PARTITY_NO_PARITY
 * 							ESP_UART_CONFIG_PARTITY_ODD
 * 							ESP_UART_CONFIG_PARTITY_EVEN
 * \param flow_control	- flow control configuration
 * 							ESP_UART_CONFIG_FLOW_CONTROL_DISABLED
 * 							ESP_UART_CONFIG_FLOW_CONTROL_RTS_ENABLED
 * 							ESP_UART_CONFIG_FLOW_CONTROL_CTS_ENABLED
 * 							ESP_UART_CONFIG_FLOW_CONTROL_RTS_CTS_ENABLED
 *
 *	\return	OP_SUCCESS
 *			ERROR_NO_MEM
 */
error_e Esp_Configure_Uart_Default(char* baudrate, uint8_t baudrate_size, char data_bits, char stop_bits, char parity, char flow_control)
{
	uint8_t* cmd = malloc(sizeof(ESP_CHANGE_UART_CONFIG_DEFAULT) + baudrate_size + 9);

	if(cmd)
	{
		memcpy(cmd, ESP_CHANGE_UART_CONFIG_DEFAULT, sizeof(ESP_CHANGE_UART_CONFIG_DEFAULT)-1);

		cmd[sizeof(ESP_CHANGE_UART_CONFIG_DEFAULT)-1] = '=';
		///	Copy the baudrate config
		memcpy(&cmd[sizeof(ESP_CHANGE_UART_CONFIG_DEFAULT)], baudrate, baudrate_size-1);

		uint8_t args_index = sizeof(ESP_CHANGE_UART_CONFIG_DEFAULT) + baudrate_size-1;
		cmd[args_index++] = ',';
		cmd[args_index++] = (uint8_t)data_bits;
		cmd[args_index++] = ',';
		cmd[args_index++] = (uint8_t)stop_bits;
		cmd[args_index++] = ',';
		cmd[args_index++] = (uint8_t)parity;
		cmd[args_index++] = ',';
		cmd[args_index++] = (uint8_t)flow_control;
		memcpy(cmd[args_index], "\r\n", 2);

		return Esp_Send_Command(cmd, sizeof(ESP_CHANGE_UART_CONFIG_DEFAULT) + baudrate_size + 9, true, true);
	}

	return ERROR_NO_MEM;
}
