/*
 * esp8266.h
 *
 *  Created on: 6 mar 2016
 *      Author: Konrad
 */

#ifndef ESP8266_H_
#define ESP8266_H_

#include <string.h>
#include <stdint-gcc.h>
#include "stm32f4xx.h"
#include "USART.h"


#define ESP8266_USART_MODULE						USART2

#define ESP_RESET									"AT+RST"
#define ESP_CHECK_FIRMWARE_VERSION					"AT+GMR"
#define ESP_WIFI_MODE								"AT+CWMODE"
#define ESP_JOIN_AP									"AT+CWJAP"
#define ESP_LIST_AP									"AT+CWLAP"
#define ESP_QUIT_AP									"AT+CWQAP"
#define ESP_SET_AP_PARAMS							"AT+CWSAP"
#define ESP_CHECK_JOIN_DEVICES_IP					"AT+CWLIF"
#define ESP_GET_CONN_STATUS							"AT+CIPSTATUS"
#define ESP_SET_UP_CONN								"AT+CIPSTART"
#define ESP_SET_DATA_TRANSMISSION_MODE				"AT+CIPMODE"
#define ESP_SEND_DATA								"AT+CIPSEND"
#define ESP_CLOSE_CONN								"AT+CIPCLOSE"
#define ESP_GET_IP_ADDRESS							"AT+CIFSR"
#define ESP_SET_MULTIPLE_CONNECTIONS				"AT+CIPMUX"
#define ESP_SET_AS_SERVER							"AT+CIPSERVER"
#define ESP_SET_SERVER_TIMEOUT						"AT+CIPSTO"
#define ESP_CHANGE_UART_CONFIG_CURRENT				"AT+UART_CUR"
#define ESP_CHANGE_UART_CONFIG_DEFAULT				"AT+UART_DEF"

#define ESP_UART_CONFIG_DATA_BITS_5						'5'
#define ESP_UART_CONFIG_DATA_BITS_6						'6'
#define ESP_UART_CONFIG_DATA_BITS_7						'7'
#define ESP_UART_CONFIG_DATA_BITS_8						'8'

#define ESP_UART_CONFIG_STOP_BITS_1						'1'
#define ESP_UART_CONFIG_STOP_BITS_1_5					'2'
#define ESP_UART_CONFIG_STOP_BITS_2						'3'

#define ESP_UART_CONFIG_PARTITY_NO_PARITY				'0'
#define ESP_UART_CONFIG_PARITY_ODD						'1'
#define ESP_UART_CONFIG_PARITY_EVEN						'2'

#define ESP_UART_CONFIG_FLOW_CONTROL_DISABLED			'0'
#define ESP_UART_CONFIG_FLOW_CONTROL_RTS_ENABLED		'1'
#define ESP_UART_CONFIG_FLOW_CONTROL_CTS_ENABLED		'2'
#define ESP_UART_CONFIG_FLOW_CONTROL_RTS_CTS_ENABLED	'3'


typedef enum
{
	mode_STA 	= '1',
	mode_AP 	= '2',
	mode_BOTH 	= '3'
}esp_wifi_mode_e;	/*< Enum connected with ESP_WIFI_MODE macro */

typedef struct
{
	char* ssid;
	char* pswd;
	char* channel;
	char* encryption;
}esp_ap_params_t;	/*< Struct connected with ESP_SET_AP_PARAMS macro */

typedef struct
{
	char  id;       /// '0' - '4' - id of the connection
	char* type;		///	"TCP" or "UDP"
	char* ip;
	char* port;
}esp_conn_start_t;	/*< Struct connected with ESP_SET_UP_CONN macro */

typedef enum
{
	not_data_mode = '0',
	data_mode = '1'
}esp_data_transmission_mode_e;	/*< Enum connected with ESP_SET_DATA_TRANSMISSION_MODE macro */

typedef enum
{
	single_connection_mode = '0',
	multiple_connection_mode = '1'
}esp_set_multiple_conn_e;		/*< Enum connected with ESP_SET_MULTIPLE_CONNECTIONS macro */

typedef enum
{
	stop_server = '0',
	start_server = '1'
}esp_server_mode_e;			/*< Static enum connected with esp_server_params_t structure */

typedef struct
{
	esp_server_mode_e	server_mode;
	uint16_t			port_number;
}esp_server_params_t;		/*< Structure connected with ESP_SET_AS_SERVER macro */

error_e Esp_Init();
error_e Esp_Reset();
error_e Esp_Configure_Uart_Current(char* baudrate, uint8_t baudrate_size, char data_bits, char stop_bits, char parity, char flow_control);
error_e Esp_Configure_Uart_Default(char* baudrate, uint8_t baudrate_size, char data_bits, char stop_bits, char parity, char flow_control);


#endif /* ESP8266_H_ */
