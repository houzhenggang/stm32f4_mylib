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
static inline error_e Esp_Send_Command(char* command, uint16_t data_size, bool command_dyn_allocated, bool wait_till_tx_end)
{
	return Uart_Send_Data(ESP8266_USART_MODULE, (uint8_t*)command, data_size, command_dyn_allocated, wait_till_tx_end);
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

/**
 * \brief This function sets the max power of transmission.
 *
 * \param power - char value in between [0; 82] in units of 0.25dBm
 * \param power_size - number of bytes of power
 *
 * \return 	OP_SUCCESS
 * 			ERROR_NO_MEM
 */
error_e	Esp_Set_Tx_Power(char* power, uint8_t power_size)
{
	uint8_t index = 0;
	char* cmd = malloc(sizeof(ESP_SET_TX_POWER) + power_size + 2);
	if(cmd)
	{
		memcpy(cmd, ESP_SET_TX_POWER, sizeof(ESP_SET_TX_POWER) - 1);
		cmd[sizeof(ESP_SET_TX_POWER-1)] = '=';
		memcpy(&cmd[sizeof(ESP_SET_TX_POWER)], power, power_size);
		index = sizeof(ESP_SET_TX_POWER) + power_size;
		cmd[index++] = power[0];
		if(power_size > 1)
		{
			cmd[index] = power[1];
		}
		return Esp_Send_Command(cmd, sizeof(ESP_SET_TX_POWER) + power_size + 2, true, true);
	}

	return ERROR_NO_MEM;
}

/**
 * \brief This function enables the command echo. It causes the ESP module to send back to the transmitter the command it received via UART interface
 *
 * \return 	OP_SUCCESS
 * 			ERROR_NO_MEM
 */
error_e Esp_Command_Echo_Turn_On()
{
	return Esp_Send_Command_Without_Args(ESP_CMD_ECHO_ON, sizeof(ESP_CMD_ECHO_ON));
}

/**
 * \brief This function disables the command echo.
 *
 * \return 	OP_SUCCESS
 * 			ERROR_NO_MEM
 */
error_e Esp_Command_Echo_Turn_Off()
{
	return Esp_Send_Command_Without_Args(ESP_CMD_ECHO_OFF, sizeof(ESP_CMD_ECHO_OFF));
}

/**
 * \brief This function restores the configuration saved in its flash memory
 *
 * \return 	OP_SUCCESS
 * 			ERROR_NO_MEM
 */
error_e Esp_Factory_Reset()
{
	return Esp_Send_Command_Without_Args(ESP_FACTORY_RESET, sizeof(ESP_FACTORY_RESET));
}

/**
 * \brief This function puts esp module in sleep mode or exits it
 *
 * \param sleep_mode
 *
 * \return	OP_SUCCESS,
 * 			ERROR_NO_MEM
 */
error_e Esp_Sleep(esp_sleep_mode_e sleep_mode)
{
	uint8_t index = 0;
	uint8_t* cmd = malloc(sizeof(ESP_SLEEP_MODE) + 3);
	if(cmd)
	{
		memcpy(cmd, ESP_SLEEP_MODE, sizeof(ESP_SLEEP_MODE)-1);
		index = sizeof(ESP_SLEEP_MODE)-1;
		cmd[index++] = '=';
		cmd[index++] = sleep_mode;
		cmd[index++] = '\r';
		cmd[index++] = '\n';

		return Esp_Send_Command(cmd, index, true, true);
	}
	return ERROR_NO_MEM;
}

error_e Esp_Set_Wifi_Mode(esp_wifi_mode_e mode, esp_config_time_e config)
{
	char* cmd = malloc(sizeof(ESP_WIFI_MODE_CUR) + 3);
	uint8_t index = 0;
	if(cmd)
	{
		if(config == ESP_CONFIG_CURRENTLY)
			strcpy(cmd, ESP_WIFI_MODE_CUR);
		else
			strcpy(cmd, ESP_WIFI_MODE_DEF);

		index = sizeof(ESP_WIFI_MODE_CUR);
		cmd[index++] = '=';
		cmd[index++] = mode;
		cmd[index++] = '\r';
		cmd[index++] = '\n';

		return Esp_Send_Command(cmd, sizeof(ESP_WIFI_MODE_CUR) + 3, true, true);
	}

	return ERROR_NO_MEM;
}

error_e Esp_Connect_To_AP(esp_ap_params_t access_point_params, esp_config_time_e config)
{
	char* cmd = malloc(sizeof(ESP_JOIN_AP_CUR) + access_point_params.pswd_size + access_point_params.ssid_size + 7);
	uint8_t index = 0;
	if(cmd)
	{
		if(config == ESP_CONFIG_CURRENTLY)
			strcpy(cmd, ESP_JOIN_AP_CUR);
		else
			strcpy(cmd, ESP_JOIN_AP_DEF);

		index = sizeof(ESP_JOIN_AP_CUR);
		cmd[index++] = '=';
		cmd[index++] = '\"';
		strcpy(&cmd[index], access_point_params.ssid);
		index += access_point_params.ssid_size;
		cmd[index++] = '\"';
		cmd[index++] = ',';
		cmd[index++] = '\"';
		strcpy(&cmd[index], access_point_params.pswd);
		index += access_point_params.pswd_size;
		cmd[index++] = '\"';
		cmd[index++] = '\r';
		cmd[index++] = '\n';

		return Esp_Send_Command(cmd, sizeof(ESP_JOIN_AP_CUR) + 3, true, true);
	}

	return ERROR_NO_MEM;
}

