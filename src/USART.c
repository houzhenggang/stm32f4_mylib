#include "USART.h"
#include "stm32f4xx.h"
#include "stdbool.h"
#include "fifo.h"
#include "error_types.h"
#include <stdlib.h>

#if USART_1_USED == 1
	uint8_t*				uart1_tx_buf;
	uint16_t				uart1_tx_byte_counter;
	uint8_t 				uart1_rx_buf[USART_1_RX_BUF_SIZE];
	fifo_t					uart1_rx_fifo;
	bool					uart1_tx_dyn_alloc_buf;
	uint16_t				uart1_tx_data_size;
	volatile bool			uart1_transmission_in_progress = false;
#endif

#if USART_2_USED == 1
	uint8_t*				uart2_tx_buf;
	uint16_t				uart2_tx_byte_counter;
	uint8_t 				uart2_rx_buf[USART_2_RX_BUF_SIZE];
	fifo_t					uart2_rx_fifo;
	bool					uart2_tx_dyn_alloc_buf;
	uint16_t				uart2_tx_data_size;
	volatile bool			uart2_transmission_in_progress = false;
#endif

#if USART_3_USED == 1
	uint8_t*				uart3_tx_buf;
	uint16_t				uart3_tx_byte_counter;
	uint8_t 				uart3_rx_buf[USART_3_RX_BUF_SIZE];
	fifo_t					uart3_rx_fifo;
	bool					uart3_tx_dyn_alloc_buf;
	uint16_t				uart3_tx_data_size;
	volatile bool			uart3_transmission_in_progress = false;
#endif

#if USART_4_USED == 1
	uint8_t*				uart4_tx_buf;
	uint16_t				uart4_tx_byte_counter;
	uint8_t 				uart4_rx_buf[USART_4_RX_BUF_SIZE];
	fifo_t					uart4_rx_fifo;
	bool					uart4_tx_dyn_alloc_buf;
	uint16_t				uart4_tx_data_size;
	volatile bool			uart4_transmission_in_progress = false;
#endif

#if USART_5_USED == 1
	uint8_t*				uart5_tx_buf;
	uint16_t				uart5_tx_byte_counter;
	uint8_t 				uart5_rx_buf[USART_5_RX_BUF_SIZE];
	fifo_t					uart5_rx_fifo;
	bool					uart5_tx_dyn_alloc_buf;
	uint16_t				uart5_tx_data_size;
	volatile bool			uart5_transmission_in_progress = false;
#endif

#if USART_6_USED == 1
	uint8_t*				uart6_tx_buf;
	uint16_t				uart6_tx_byte_counter;
	uint8_t 				uart6_rx_buf[USART_6_RX_BUF_SIZE];
	fifo_t					uart6_rx_fifo;
	bool					uart6_tx_dyn_alloc_buf;
	uint16_t				uart6_tx_data_size;
	volatile bool			uart6_transmission_in_progress = false;
#endif

/**
 * 	\brief This function resets entire UART configuration
 * 	\param USART - usart instatnion to reset
 */
void UART_DeInit(USART_TypeDef* USART)
{
	//	Clear the Baud rate register
	USART->BRR = 0x00000000;
	//	Clear the main configuration
	USART->CR1 = 0x00000000;
	//	Clear the additional configuration
	USART->CR2 = 0x00000000;
	USART->CR3 = 0x00000000;


}
/** \brief This function sets the parameters of UART Transmitter and/or Receiver.
 *
 * \param   USART_TypeDef*  USART                       - the chosen USART controller
 * \param   uint16_t        data_transmission_settings  - parameters which are mixed Data Word Lenght(9 or 8bit),
                                                            Parity Enable/Disable and chosen parity, Transmission Completed Interrupt, Idle Line Interrupt, Oversampling
   \param   uint16_t        baudRate                    - the chosen Baud Rate(only integer part)
    \param  bool			rx_enabled					- true if rx is to be enabled, false otherwise
 * \return
 *
 */
void UART_Config(USART_TypeDef* USART, uint16_t data_transmission_settings, uint16_t baudRate, bool rx_used)
{
	float usart_div = 0;

	//	Reset the UART configuration
    UART_DeInit(USART);

    if(rx_used)
    {
    	//  Enable Rx Interrupt
    	USART->CR1 |= USART_CR1_RXNEIE;
    }
    else
    {
    	//  Disable Rx Interrupt
		USART->CR1 &= ~USART_CR1_RXNEIE;
    }

    //  Set the Data length, enable end set the chosen parity
    USART->CR1 |= data_transmission_settings;
    //  Set the Baud Rate
            //  Fractional part
           // USART_BRR = 16*(APB2*1000000)/baudRate;
    //  Integer part

    if((USART->CR1 & USART_CR1_OVER8) == 0)
    {

		if(USART == USART1)
		{
			usart_div = ((float)(APB2*1000000)/(16*baudRate));
		}
		else
		{
			usart_div = ((float)(APB1*1000000)/(16*baudRate));
		}
    }
    else
    {
		if(USART == USART1)
		{
			usart_div = ((float)(APB2*1000000)/(8*baudRate));
		}
		else
		{
			usart_div = ((float)(APB1*1000000)/(8*baudRate));
		}
    }
    USART->BRR = (uint16_t)((uint16_t)(usart_div)<<4 | (uint16_t)((16*(usart_div - (uint16_t)usart_div)) + 0.5));
}

inline void Uart_Enable(USART_TypeDef* USART)
{
    //  Enable USART
    USART->CR1 |= USART_CR1_UE;
}

inline void Uart_Disable(USART_TypeDef* USART)
{
    //  Enable USART
    USART->CR1 &= ~USART_CR1_UE;
}

inline void Uart_Enable_Tx(USART_TypeDef* USART)
{
	USART->CR1 |= USART_CR1_TE;
}

inline void Uart_Disable_Tx(USART_TypeDef* USART)
{
	USART->CR1 &= ~USART_CR1_TE;
}

inline void Uart_Enable_Rx(USART_TypeDef* USART)
{
	USART->CR1 |= USART_CR1_RE;
}

inline void Uart_Disable_Rx(USART_TypeDef* USART)
{
	USART->CR1 &= ~USART_CR1_RE;
}

inline void Uart_Enable_Tx_IRQ(USART_TypeDef* USART)
{
	USART->CR1 |= USART_CR1_TXEIE | USART_CR1_TCIE;
}

inline void Uart_Enable_Rx_IRQ(USART_TypeDef* USART)
{
	USART->CR1 |= USART_CR1_RXNEIE;
}

inline void Uart_Disable_Tx_IRQ(USART_TypeDef* USART)
{
	USART->CR1 &= ~(USART_CR1_TXEIE | USART_CR1_TCIE);
}

inline void Uart_Disable_Rx_IRQ(USART_TypeDef* USART)
{
	USART->CR1 &= ~USART_CR1_RXNEIE;
}

static void Uart_Wait_Till_Transmitted(volatile bool* flag)
{
	while(*flag)
	{
		__WFE();
	}
}

#if USART_1_USED == 1
	static error_e Uart_1_Send_Data(uint8_t* data_buf, uint16_t data_size, bool dyn_alloc_buf, bool wait_for_transmission_end)
	{
		if(uart1_transmission_in_progress)
			return ERROR_BUSY;

		uart1_tx_buf = malloc(data_size);
		if(uart1_tx_buf == NULL)
		{
			return ERROR_NO_MEM;
		}

		if(!dyn_alloc_buf)
		{
			///	Copy the data in the buffer
			memcpy(uart1_tx_buf, data_buf, data_size);
		}
		///	Set the flag if the buffer was dynamically allocated
		uart1_tx_dyn_alloc_buf = true;

		///	Set the data size
		uart1_tx_data_size = data_size;

		///	Clear the TX byte counter
		uart1_tx_byte_counter = 0;

		Uart_Enable(USART1);
		Uart_Enable_Tx(USART1);
		Uart_Enable_Tx_IRQ(USART1);
		NVIC_SetPendingIRQ(USART1_IRQn);
		if(wait_for_transmission_end)
		{
			Uart_Wait_Till_Transmitted(&uart1_transmission_in_progress);
		}
		return OP_SUCCESS;
	}
#endif

#if USART_2_USED == 1
	static error_e Uart_2_Send_Data(uint8_t* data_buf, uint16_t data_size, bool dyn_alloc_buf, bool wait_for_transmission_end)
	{
		if(uart2_transmission_in_progress)
			return ERROR_BUSY;

		uart2_tx_buf = malloc(data_size);
		if(uart2_tx_buf == NULL)
		{
			return ERROR_NO_MEM;
		}

		if(!dyn_alloc_buf)
		{
			///	Copy the data in the buffer
			memcpy(uart2_tx_buf, data_buf, data_size);
		}
		///	Set the flag if the buffer was dynamically allocated
		uart2_tx_dyn_alloc_buf = true;

		///	Set the data size
		uart2_tx_data_size = data_size;

		Uart_Enable(USART2);
		Uart_Enable_Tx(USART2);
		Uart_Enable_Tx_IRQ(USART2);
		NVIC_SetPendingIRQ(USART2_IRQn);
		if(wait_for_transmission_end)
		{
			Uart_Wait_Till_Transmitted(&uart2_transmission_in_progress);
		}
		return OP_SUCCESS;
	}
#endif

#if USART_3_USED == 1
	static error_e Uart_3_Send_Data(uint8_t* data_buf, uint16_t data_size, bool dyn_alloc_buf, bool wait_for_transmission_end)
	{
		if(uart3_transmission_in_progress)
			return ERROR_BUSY;

		uart3_tx_buf = malloc(data_size);
		if(uart3_tx_buf == NULL)
		{
			return ERROR_NO_MEM;
		}

		if(!dyn_alloc_buf)
		{
			///	Copy the data in the buffer
			memcpy(uart3_tx_buf, data_buf, data_size);
		}
		///	Set the flag if the buffer was dynamically allocated
		uart3_tx_dyn_alloc_buf = true;

		///	Set the data size
		uart3_tx_data_size = data_size;

		Uart_Enable(USART3);
		Uart_Enable_Tx(USART3);
		Uart_Enable_Tx_IRQ(USART3);
		NVIC_SetPendingIRQ(USART3_IRQn);
		if(wait_for_transmission_end)
		{
			Uart_Wait_Till_Transmitted(&uart3_transmission_in_progress);
		}
		return OP_SUCCESS;
	}
#endif

#if USART_4_USED
	static error_e Uart_4_Send_Data(uint8_t* data_buf, uint16_t data_size, bool dyn_alloc_buf, bool wait_for_transmission_end)
	{
		if(uart4_transmission_in_progress)
			return ERROR_BUSY;

		uart4_tx_buf = malloc(data_size);
		if(uart4_tx_buf == NULL)
		{
			return ERROR_NO_MEM;
		}

		if(!dyn_alloc_buf)
		{
			///	Copy the data in the buffer
			memcpy(uart4_tx_buf, data_buf, data_size);
		}
		///	Set the flag if the buffer was dynamically allocated
		uart4_tx_dyn_alloc_buf = true;

		///	Set the data size
		uart4_tx_data_size = data_size;

		Uart_Enable(UART4);
		Uart_Enable_Tx(UART4);
		Uart_Enable_Tx_IRQ(UART4);
		NVIC_SetPendingIRQ(UART4_IRQn);

		if(wait_for_transmission_end)
		{
			Uart_Wait_Till_Transmitted(&uart4_transmission_in_progress);
		}
		return OP_SUCCESS;
	}
#endif


#if USART_5_USED
	static error_e Uart_5_Send_Data(uint8_t* data_buf, uint16_t data_size, bool dyn_alloc_buf, bool wait_for_transmission_end)
	{
		if(uart5_transmission_in_progress)
			return ERROR_BUSY;

		uart5_tx_buf = malloc(data_size);
		if(uart5_tx_buf == NULL)
		{
			return ERROR_NO_MEM;
		}

		if(!dyn_alloc_buf)
		{
			///	Copy the data in the buffer
			memcpy(uart5_tx_buf, data_buf, data_size);
		}
		///	Set the flag if the buffer was dynamically allocated
		uart5_tx_dyn_alloc_buf = true;

		///	Set the data size
		uart5_tx_data_size = data_size;

		Uart_Enable(UART5);
		Uart_Enable_Tx(UART5);
		Uart_Enable_Tx_IRQ(UART5);
		NVIC_SetPendingIRQ(UART5_IRQn);

		if(wait_for_transmission_end)
		{
			Uart_Wait_Till_Transmitted(&uart5_transmission_in_progress);
		}
		return OP_SUCCESS;
	}
#endif


#if USART_6_USED
	static error_e Uart_6_Send_Data(uint8_t* data_buf, uint16_t data_size, bool dyn_alloc_buf, bool wait_for_transmission_end)
	{
		if(uart6_transmission_in_progress)
			return ERROR_BUSY;

		uart6_tx_buf = malloc(data_size);
		if(uart6_tx_buf == NULL)
		{
			return ERROR_NO_MEM;
		}

		if(!dyn_alloc_buf)
		{
			///	Copy the data in the buffer
			memcpy(uart6_tx_buf, data_buf, data_size);
		}
		///	Set the flag if the buffer was dynamically allocated
		uart6_tx_dyn_alloc_buf = true;

		///	Set the data size
		uart6_tx_data_size = data_size;

		Uart_Enable(USART6);
		Uart_Enable_Tx(USART6);
		Uart_Enable_Tx_IRQ(USART6);
		NVIC_SetPendingIRQ(USART6_IRQn);

		if(wait_for_transmission_end)
		{
			Uart_Wait_Till_Transmitted(&uart6_transmission_in_progress);
		}

		return OP_SUCCESS;
	}
#endif

error_e Uart_Send_Data(USART_TypeDef* USART, uint8_t* data_buf, uint16_t data_size, bool dyn_alloc_buf, bool wait_for_transmission_end)
{
	error_e ret_val;
	switch((uint32_t)USART)
	{
#if USART_1_USED
		case (uint32_t)USART1:
		{
			ret_val = Uart_1_Send_Data(data_buf, data_size, dyn_alloc_buf, wait_for_transmission_end);
		}break;
#endif
#if USART_2_USED
		case (uint32_t)USART2:
		{
			ret_val = Uart_2_Send_Data(data_buf, data_size, dyn_alloc_buf, wait_for_transmission_end);
		}break;
#endif
#if USART_3_USED
		case (uint32_t)USART3:
		{
			ret_val = Uart_3_Send_Data(data_buf, data_size, dyn_alloc_buf, wait_for_transmission_end);
		}break;
#endif
#if USART_4_USED
		case (uint32_t)UART4:
		{
			ret_val = Uart_4_Send_Data(data_buf, data_size, dyn_alloc_buf, wait_for_transmission_end);
		}break;
#endif
#if USART_5_USED
		case (uint32_t)UART5:
		{
			ret_val = Uart_5_Send_Data(data_buf, data_size, dyn_alloc_buf, wait_for_transmission_end);
		}break;
#endif
#if USART_6_USED
		case (uint32_t)USART6:
		{
			ret_val = Uart_6_Send_Data(data_buf, data_size, dyn_alloc_buf, wait_for_transmission_end);
		}break;
#endif
		default:
			while(1) __WFI();
	}

	return ret_val;
}

//void Log_Uart(char* text)
//{
//	//	Set the pointer to the text
//	uart_tx_buf = text;
//	//	Set the flag indicating that transmission is in progress
//	uart_transmission_in_progress = true;
//	//	Load the first character;
//	LOG_USART->DR = *uart_tx_buf++;
//	//  Enable Tx interrupt
//	LOG_USART->CR1 |= USART_CR1_TXEIE;
//	//	Sleep until the transmission is in progress
//	while(uart_transmission_in_progress)
//	{
//		//__WFI();
//	}
//}




