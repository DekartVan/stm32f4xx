#include "RCC.h"

/*
Последовательность настройки тактирования системы:
1. Запск генератора HSE
2. Настройка PLL
3. Запуск PLL
4. Настройка циклов ожидания FLASH
5. Настройка делителя шин
6. Переключение на работу от PLL
*/

RCC_ErrorStatus ClockInit(void)
{	
	// 1. Запуск генератора HSE:
	RCC->CR |= RCC_CR_HSEON;
	for (int StartUpCounetr = 0; StartUpCounetr < HSE_TIMEOUT; StartUpCounetr++)
	{
		if (RCC->CR & RCC_CR_HSERDY)
			break;
	}
		
	if (!(RCC->CR & RCC_CR_HSERDY))
	{
		RCC->CR &= ~RCC_CR_HSEON;
		return RCC_ERROR;
	}
	
	// 2. Настройка PLL
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP);
	RCC->PLLCFGR |= (12 << RCC_PLLCFGR_PLLM_Pos | 96  << RCC_PLLCFGR_PLLN_Pos | 0x02 << RCC_PLLCFGR_PLLP_Pos);
	
	
	// 3. Запуск PLL
	RCC->CR |= RCC_CR_PLLON;
	for(int StartUpCounetr=0; StartUpCounetr < PLL_TIMEOUT; StartUpCounetr++)
	{
		if(RCC->CR & RCC_CR_PLLRDY)
			break;
	}
	
	if(!(RCC->CR & RCC_CR_PLLRDY))
	{
			RCC->CR &= ~RCC_CR_HSEON;
			RCC->CR &= ~RCC_CR_PLLON;
			return RCC_ERROR;
	}
	
	// 4. Настройка циклов ожидания FLASH
	FLASH->ACR |= FLASH_ACR_LATENCY_3WS;
	
  // 5. Настройка делителей
	RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
	RCC->CFGR |= (0x4 << RCC_CFGR_PPRE1_Pos); 
	
	// 6. Переключение на PLL
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
		
	// 7. Отключение HSI
	RCC->CR &= ~RCC_CR_HSION;

	return RCC_OK;
}

