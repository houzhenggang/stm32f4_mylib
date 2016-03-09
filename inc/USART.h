#ifndef _USART_H_
#define _USART_H_

#include "stm32f4xx.h"
#include "RCC.h"
#include "stdbool.h"
#include "fifo.h"
#include "error_types.h"


#define USART_1_USED	0
#define USART_2_USED	1
#define USART_3_USED	0
#define USART_4_USED	0
#define USART_5_USED	0
#define USART_6_USED	0

#define LOG_USART									USART2

#define USART_1_RX_BUF_SIZE							0
#define USART_2_RX_BUF_SIZE							128
#define USART_3_RX_BUF_SIZE							0
#define USART_4_RX_BUF_SIZE							0
#define USART_5_RX_BUF_SIZE							0
#define USART_6_RX_BUF_SIZE							0

#define BAUD_RATE_DIVIDER_USART1(Baud_rate)			(uint32_t)(Baud_rate/(8*APB2*1000000)<<4)
#define BAUD_RATE_DIVIDER_USART2_USART3(Baud_rate)	(uint32_t)(Baud_rate/(8*APB1*1000000)<<4)

#if USART_1_USED == 1
	extern	uint8_t*				uart1_tx_buf;
	extern  uint16_t				uart1_tx_byte_counter;
	extern	uint8_t 				uart1_rx_buf[USART_1_RX_BUF_SIZE];
	extern	fifo_t					uart1_rx_fifo;
	extern	bool					uart1_tx_dyn_alloc_buf;
	extern	uint16_t				uart1_tx_data_size;
	extern	volatile bool			uart1_transmission_in_progress;
#endif

#if USART_2_USED == 1
	extern	uint8_t*				uart2_tx_buf;
	extern  uint16_t				uart2_tx_byte_counter;
	extern	uint8_t 				uart2_rx_buf[USART_2_RX_BUF_SIZE];
	extern	fifo_t					uart2_rx_fifo;
	extern	bool					uart2_tx_dyn_alloc_buf;
	extern	uint16_t				uart2_tx_data_size;
	extern	volatile bool			uart2_transmission_in_progress;
#endif

#if USART_3_USED == 1
	extern	uint8_t*				uart3_tx_buf;
	extern  uint16_t				uart3_tx_byte_counter;
	extern	uint8_t 				uart3_rx_buf[USART_3_RX_BUF_SIZE];
	extern	fifo_t					uart3_rx_fifo;
	extern	bool					uart3_tx_dyn_alloc_buf;
	extern	uint16_t				uart3_tx_data_size;
	extern	volatile bool			uart3_transmission_in_progress;
#endif

#if USART_4_USED == 1
	extern	uint8_t*				uart4_tx_buf;
	extern  uint16_t				uart4_tx_byte_counter;
	extern	uint8_t 				uart4_rx_buf[USART_4_RX_BUF_SIZE];
	extern	fifo_t					uart4_rx_fifo;
	extern	bool					uart4_tx_dyn_alloc_buf;
	extern	uint16_t				uart4_tx_data_size;
	extern	volatile bool			uart4_transmission_in_progress;
#endif

#if USART_5_USED == 1
	extern	uint8_t*				uart5_tx_buf;
	extern  uint16_t				uart5_tx_byte_counter;
	extern	uint8_t 				uart5_rx_buf[USART_5_RX_BUF_SIZE];
	extern	fifo_t					uart5_rx_fifo;
	extern	bool					uart5_tx_dyn_alloc_buf;
	extern	uint16_t				uart5_tx_data_size;
	extern	volatile bool			uart5_transmission_in_progress;
#endif

#if USART_6_USED == 1
	extern	uint8_t*				uart6_tx_buf;
	extern  uint16_t				uart6_tx_byte_counter;
	extern	uint8_t 				uart6_rx_buf[USART_6_RX_BUF_SIZE];
	extern	fifo_t					uart6_rx_fifo;
	extern	bool					uart6_tx_dyn_alloc_buf;
	extern	uint16_t				uart6_tx_data_size;
	extern	volatile bool			uart6_transmission_in_progress;
#endif

void UART_DeInit(USART_TypeDef* USART);
void UART_Config(USART_TypeDef* USART, uint16_t data_transmission_settings, uint16_t baudRate, bool rx_enabled);


void Uart_Enable(USART_TypeDef* USART);

void Uart_Disable(USART_TypeDef* USART);

void Uart_Enable_Tx(USART_TypeDef* USART);

void Uart_Disable_Tx(USART_TypeDef* USART);

void Uart_Enable_Rx(USART_TypeDef* USART);

void Uart_Disable_Rx(USART_TypeDef* USART);

void Uart_Enable_Tx_IRQ(USART_TypeDef* USART);

void Uart_Enable_Rx_IRQ(USART_TypeDef* USART);

void Uart_Disable_Tx_IRQ(USART_TypeDef* USART);

void Uart_Disable_Rx_IRQ(USART_TypeDef* USART);

error_e Uart_Send_Data(USART_TypeDef* USART, uint8_t* data_buf, uint16_t data_size, bool dyn_alloc_buf, bool wait_for_transmission_end);
//void Log_Uart(char* text);



#endif
