/*
 * RCC.h
 *
 *  Created on: Nov 8, 2014
 *      Author: Konrad
 */

#ifndef RCC_H_
#define RCC_H_

#include "stm32f4xx.h"
#include "stdint-gcc.h"


/* TODO (Konrad#1#): Set CPU_FREQ (default 168) */
#define CPU_FREQ         168	//	In MHz


#define RCC_PLLM       	8
#define RCC_PLLN      	336
#define RCC_PLLQ       	7
#define RCC_PLLP       	2
#define CRYSTAL_FREQ    8

#if (CPU_FREQ != ((CRYSTAL_FREQ*RCC_PLLN/RCC_PLLM)/RCC_PLLP))
    #error "CLK Setting Fault - check PLL parameters and the chosen CPU_FREQ"
#endif

#if CPU_FREQ > 84
    #define APB1 CPU_FREQ/4
    #define APB2 CPU_FREQ/2
#else
    #define APB1 CPU_FREQ/2
    #define APB2 CPU_FREQ/1
#endif

//void ResetRCC(void);
void RCC_SetClockFrequency(uint32_t PLLM, uint32_t PLLN, uint32_t PLLQ, uint32_t PLLP);

#endif /* RCC_H_ */

