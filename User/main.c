#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "DMA.h"
#include "ADC.h"

int main(void)
{
	OLED_Init();
	DMA_Initilize();
	ADC_Initilize();
	while(1)
	{
		ADC_GetValue();
		OLED_ShowNum(1,1,AD_Value[0],4);
		OLED_ShowNum(2,1,AD_Value[1],4);
		OLED_ShowNum(3,1,AD_Value[2],4);
		OLED_ShowNum(4,1,AD_Value[3],4);
		Delay_ms(100);
	}
}
