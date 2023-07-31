#include "stm32f10x.h"
void ADC_Initilize(void)
{
    // 1.使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 漏了为ADC时钟分频！！！！！！
    // 2.初始化GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // 3.设置软件触发
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);// 通道，序号，采样时间
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
    // 4.初始化ADC
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // 独立模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 // 单次转换模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                        // 扫描模式
    ADC_InitStructure.ADC_NbrOfChannel = 4;                             // 通道数
    ADC_Init(ADC1, &ADC_InitStructure);
    // 5.使能ADC
    //ADC_DMACmd(ADC1, ENABLE); 
    ADC_Cmd(ADC1, ENABLE);
    // 6.校准
    ADC_ResetCalibration(ADC1);//复位校准寄存器
    while (ADC_GetResetCalibrationStatus(ADC1) == SET)//等待校准寄存器复位完成
        ;
    ADC_StartCalibration(ADC1);//开始校准
    while (ADC_GetCalibrationStatus(ADC1) == SET)//等待校准完成
        ;
}
void ADC_GetValue(void)
{
    // 假如ADC是单次转换模式，DMA也是单次转运模式，则需要在每次转换完后重新设置DMA的传输计数器
    DMA_Cmd(DMA1_Channel1, DISABLE); // 在修改DMA1_Channel1->CNDTR寄存器前，必须先让DMA1_Channel1失能
    DMA_SetCurrDataCounter(DMA1_Channel1, 4);//设置传输计数器为4
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);//没转运完就等待
    DMA_ClearFlag(DMA1_FLAG_TC1); // 转运完了就清除标志位
}