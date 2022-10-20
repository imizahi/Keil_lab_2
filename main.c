#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

#define TIM_CCER_CC3NE  ((uint16_t)0x0400)
#define TIMER_PRESCALER 720
#define EXT_TIM_PULSE   150
#define TIM_PULSE  50
int TIM_pulse_1=0;  
uint16_t previousState;
GPIO_InitTypeDef port;
TIM_TimeBaseInitTypeDef timer;
TIM_OCInitTypeDef timerPWM;
uint16_t buttonPreviousState;

void initAll()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    GPIO_StructInit(&port);
	
    port.GPIO_Mode = GPIO_Mode_AF_PP;
    port.GPIO_Pin = GPIO_Pin_6;
    port.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &port);
	
    port.GPIO_Mode = GPIO_Mode_IPD;
    port.GPIO_Pin = GPIO_Pin_0;
    port.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &port);
	
    TIM_TimeBaseStructInit(&timer);
    timer.TIM_Prescaler = TIMER_PRESCALER;
    timer.TIM_Period = 1000;
    TIM_TimeBaseInit(TIM4, &timer);
    TIM_OCStructInit(&timerPWM);
    timerPWM.TIM_Pulse = 10;
    timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
    timerPWM.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(TIM4, &timerPWM);
}


int main()
{
    __enable_irq();
    initAll();
    buttonPreviousState = 0;
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
    NVIC_EnableIRQ(TIM4_IRQn);
    while(1)
    {
        __NOP();
    }
}

void TIM4_IRQHandler()
{ 
    uint16_t button = 0;
    button = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    
    if (button == 1)
    {
    		if (TIM_pulse_1 > 0)
    			TIM_pulse_1--;
    		TIM4->CCR1 = TIM_pulse_1;
    }
    if (button == 0)
    {
			//TIM4->CCR1 = TIM_PULSE;
			//buttonPreviousState = 0;
			 if (TIM_pulse_1 < 1000)
    			TIM_pulse_1++;
    		TIM4->CCR1 = TIM_pulse_1;
    }
}