/*
 * GPIO.h
 *
 *  Created on: Nov 8, 2014
 *      Author: Ola
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f4xx.h"

#define PIN_0 	(uint32_t)0x00000003
#define PIN_1 	(uint32_t)0x0000000C
#define PIN_2	(uint32_t)0x00000030
#define PIN_3 	(uint32_t)0x000000C0
#define PIN_4	(uint32_t)0x00000300
#define PIN_5 	(uint32_t)0x00000C00
#define PIN_6	(uint32_t)0x00003000
#define PIN_7 	(uint32_t)0x0000C000
#define PIN_8	(uint32_t)0x00030000
#define PIN_9 	(uint32_t)0x000C0000
#define PIN_10	(uint32_t)0x00300000
#define PIN_11 	(uint32_t)0x00C00000
#define PIN_12	(uint32_t)0x03000000
#define PIN_13 	(uint32_t)0x0C000000
#define PIN_14	(uint32_t)0x30000000
#define PIN_15 	(uint32_t)0xC0000000

#define AF0		(uint8_t)0x00
#define	AF1		(uint8_t)0x01
#define AF2		(uint8_t)0x02
#define AF3		(uint8_t)0x03
#define AF4		(uint8_t)0x04
#define AF5		(uint8_t)0x05
#define AF6		(uint8_t)0x06
#define AF7		(uint8_t)0x07
#define AF8		(uint8_t)0x08
#define AF9		(uint8_t)0x09
#define AF10	(uint8_t)0x0A
#define AF11	(uint8_t)0x0B
#define AF12	(uint8_t)0x0C
#define AF13	(uint8_t)0x0D
#define AF14	(uint8_t)0x0E
#define AF15	(uint8_t)0x0F

typedef enum
{
    gpio_speed_low = 'l',
    gpio_speed_medium = 'm',
    gpio_speed_high = 'h',
    gpio_speed_fast = 'f'
}gpio_speed_e;

typedef enum
{
    gpio_otyper_push_pull = 0,
    gpio_otyper_open_collector = 1
}gpio_otyper_e;

typedef enum
{
    gpio_pupd_no_pull = 'n',
    gpio_pupd_pull_up = 'u',
    gpio_pupd_pull_down = 'd'
}gpio_pupd_e;


void GPIO_InputConfigure(GPIO_TypeDef* GPIOx, uint32_t pins32_t, char speed, char pupd);
void GPIO_OutputConfigure(GPIO_TypeDef *GPIOx, uint32_t Pins32_t, char otyper, char speed, char pupd);
void GPIO_AlternateFunctionPrepare(GPIO_TypeDef *GPIOx, uint32_t pins32_t, char otyper, char speed, char pupd);
void GPIO_AlternateFunctionSet(GPIO_TypeDef* GPIOx, uint32_t pin32_t,uint8_t AF_functionNumber);
void GPIO_Analog_Configure(GPIO_TypeDef* GPIO, uint32_t pins32);
#endif /* GPIO_H_ */
