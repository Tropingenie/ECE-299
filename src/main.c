/*
* This file is part of the µOS++ distribution.
*   (https://github.com/micro-os-plus)
* Copyright (c) 2014 Liviu Ionescu.
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom
* the Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/trace.h"
#include "stm32f4xx.h"
#include "clockface.h"

#define DELAY 1000
#define SEG_L 10

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace-impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void SysTick_Handler(void){
	// SysTick Handler so we can use HAL_Delay() all the time
	// HAL_Delay will occasionally block infinitely; having the SysTick Handler
	// prevents that issue from occurring.
	HAL_IncTick();
}

int main(int argc, char* argv[]){
	HAL_Init();

	//Enable clock for port D
	__HAL_RCC_GPIOD_CLK_ENABLE();

	//handle for pointing GPIO of port D
	GPIO_InitTypeDef GPIO_InitStructureD;
	GPIO_InitStructureD.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_8 | GPIO_PIN_3 | GPIO_PIN_6;
	GPIO_InitStructureD.Mode = GPIO_MODE_OUTPUT_PP; //output mode
	GPIO_InitStructureD.Speed = GPIO_SPEED_LOW;
	GPIO_InitStructureD.Pull = GPIO_NOPULL; //no pull-up resistor
	GPIO_InitStructureD.Alternate = 0; //no alternate function
	//initialize port E with parameters of InitStructure
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructureD);

	// Init port E pins
	//Enable clock for port E
	__HAL_RCC_GPIOE_CLK_ENABLE();
	//handle for pointing GPIO of port E
	GPIO_InitTypeDef GPIO_InitStructureE;
	//assign pins 0, 1 and 2 for Port E
	GPIO_InitStructureE.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_13;
	GPIO_InitStructureE.Mode = GPIO_MODE_OUTPUT_PP; //output mode
	GPIO_InitStructureE.Speed = GPIO_SPEED_LOW;
	GPIO_InitStructureE.Pull = GPIO_NOPULL; //no pull-up resistor
	GPIO_InitStructureE.Alternate = 0; //no alternate function
	//initialize port E with parameters of InitStructure
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructureE);

	// Init port C pin
	// Use port C pin 6 to act as Timer 3 (alternate function)
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure; //A handle to initialize GPIO port C
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	//Configure Timer 3 to count up to 4 seconds (= 0.25 Hz)
	__HAL_RCC_TIM3_CLK_ENABLE();
	TIM_HandleTypeDef s_TimerInstance;
	s_TimerInstance.Instance = TIM3;
	s_TimerInstance.Init.Prescaler = 8399; //Maximum  clock frequency = 84 MHz = 84000 kHz
	s_TimerInstance.Init.CounterMode = TIM_COUNTERMODE_UP;
	s_TimerInstance.Init.Period = 39999; // Also important for setting timer period
	s_TimerInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	s_TimerInstance.Init.RepetitionCounter = 0;
	s_TimerInstance.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&s_TimerInstance);

	HAL_TIM_Base_Start(&s_TimerInstance);

	// Default clock settings
	ClockFace time_to_show;
	time_to_show.h1 = 0;
	time_to_show.h2 = 1;
	time_to_show.m1 = 0;
	time_to_show.m2 = 0;

	// Infinite loop
	while (1)
	{
	  show_time(time_to_show);//Note: NOT WORKING


	  //Check if the timer has counted up to 4 seconds
	  if(__HAL_TIM_GET_FLAG(&s_TimerInstance, TIM_FLAG_UPDATE)!=RESET)
	   {
		// if 4 seconds is reached, clear the timer flag
		__HAL_TIM_CLEAR_IT(&s_TimerInstance,TIM_IT_UPDATE);

		/* In polling, you will check the status once the timer flag is set.
		 * For example, if button press has to be polled, then timer counter
		 * will be configured. Once the timer flag is set, a statement checking
		 * button status will be written. That way, the button status will be
		 * checked only when timer flag gets set.
		 */
		increment_time(time_to_show);// Note: NOT WORKING


	   }
	}
}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
