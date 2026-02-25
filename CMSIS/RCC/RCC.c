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
int ClockInit(void)
{	
	// Переменная для обработки ошибок
	__IO int StartUpCounetr;
	
	// 1. Запуск генератора HSE:
	RCC->CR |= (1<<RCC_CR_HSEON_Pos);
	for(StartUpCounetr=0; ; StartUpCounetr++)
	{
		if(RCC->CR & (1<<RCC_CR_HSERDY_Pos))
			break;
		
		if(StartUpCounetr > 0x1000)
		{
			RCC->CR &= ~(1<<RCC_CR_HSEON_Pos);
			return 1;
		}
	}
	
	// 2. Настройка PLL
	RCC->PLLCFGR |= (1<<RCC_PLLCFGR_PLLSRC_Pos); // Задание HSE в качестве источника
	RCC->PLLCFGR &= ~(0x3F<<RCC_PLLCFGR_PLLM_Pos); // Сброс M
	RCC->PLLCFGR |= (12<<RCC_PLLCFGR_PLLM_Pos); // M = 12
	RCC->PLLCFGR |= (0x1FF<<RCC_PLLCFGR_PLLN_Pos); // Сброс N
	RCC->PLLCFGR |= (96<<RCC_PLLCFGR_PLLN_Pos); // N = 96
	RCC->PLLCFGR &= ~(3<<RCC_PLLCFGR_PLLP_Pos); // P = 2
	
	// 3. Запуск PLL
	RCC->CR |= (1<<RCC_CR_PLLON_Pos);
	for(StartUpCounetr=0; ; StartUpCounetr++)
	{
		if(RCC->CR & (1<<RCC_CR_PLLRDY_Pos))
			break;
		
		if(StartUpCounetr > 0x1000)
		{
			RCC->CR &= ~(1<<RCC_CR_HSEON_Pos);
			RCC->CR &= ~(1<<RCC_CR_PLLON_Pos);
			return 1;
		}
	}
	// 4. Настройка циклов ожидания FLASH
	FLASH->ACR |= FLASH_ACR_LATENCY_3WS;
	
	// 5. Настройка делителя шин
	RCC->CFGR &= ~(0xF<<RCC_CFGR_HPRE_Pos);
	RCC->CFGR |= (0x4<<RCC_CFGR_PPRE1_Pos);
	RCC->CFGR &= ~(0x7<<RCC_CFGR_PPRE2_Pos);
	
	// 6. Переключение на работу от PLL
	RCC->CFGR |= (0x02<<RCC_CFGR_SW_Pos); //Переключаемся на работу от PLL

	  //Ждем, пока переключимся
  while((RCC->CFGR & RCC_CFGR_SWS_Msk) != (0x02<<RCC_CFGR_SWS_Pos))
  {
  }
	
	// Отключаем HSI
	 RCC->CR &= ~(1<<RCC_CR_HSION_Pos);
}

