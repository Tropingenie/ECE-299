//
//   EEEEE   CCC   EEEEE   222    999    999
//   E      C   C  E      2   2  9   9  9   9
//   E      C      E          2  9   9  9   9
//   EEEEE  C      EEEEE     2    9999   9999
//   E      C      E        2        9      9
//   E      C   C  E       2        9      9
//   EEEEE   CCC   EEEEE  22222   99     99
//
//
//    AAA   L       AAA    RRRR   M   M       CCC   L       OOO    CCC   K   K
//   A   A  L      A   A   R   R  MM MM      C   C  L      O   O  C   C  K  K
//   A   A  L      A   A   R   R  M M M      C      L      O   O  C      K K
//   AAAAA  L      AAAAA   RRRR   M   M      C      L      O   O  C      KK
//   A   A  L      A   A   R R    M   M      C      L      O   O  C      K K
//   A   A  L      A   A   R  R   M   M      C   C  L      O   O  C   C  K  K
//   A   A  LLLLL  A   A   R   R  M   M       CCC   LLLLL   OOO    CCC   K   K
//
// Version 1.01
//
// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "diag/Trace.h"
#include "stm32f4xx_hal.h"
#include "Timer.h"
#include "constants.h"
#include "led_control.h"

//
// Disable specific warnings
//

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

//
// remove comment slashes on line below to enable real time clock support
//
#define USE_RTC

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 Simple Alarm Clock Stub Code
//
// This code just plays an MP3 file off of a connected USB flash drive.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

void
	Display7Segment( void ),
	SetTime( void ),
	SetAlarm( void ),
	Snooze( void ),
	Sleep (void ),
	ToggleAlarm( void ),
	ProcessButtons( void ),
	GetCurrentTime( void ),
	GetCurrentAlarm( void ),
	SystemClock_Config( void ),
	ConfigureIoPins( void ),
	ConfigureTimer( void ),
	ConfigureRealTimeClock( void );


uint16_t
	CheckButtons( void );


//
// Global variables
//

#ifdef USE_RTC

RTC_InitTypeDef
	ClockInit;				// Structure used to initialize the real time clock

RTC_HandleTypeDef
	RealTimeClock;			// Structure for the real time clock subsystem

RTC_TimeTypeDef
	ClockTime;				// Structure to hold/store the current time

RTC_DateTypeDef
	ClockDate;				// Structure to hold the current date

RTC_AlarmTypeDef
	ClockAlarm;				// Structure to hold/store the current alarm time

#endif



TIM_HandleTypeDef
	DisplayTimer;			// Structure for the LED display timer subsystem



volatile int
	Alarm = FALSE,			// Flag indicating alarm
	Button_Debounce_Counter[6] = {BUTTON_STAB_COUNT,	// Button debounce counters
								  BUTTON_STAB_COUNT,
								  BUTTON_STAB_COUNT,
								  BUTTON_STAB_COUNT,
								  BUTTON_STAB_COUNT,
								  BUTTON_STAB_COUNT},
	button_slowdown_counter = 0;

volatile bool
	ButtonsPushed[6],		// Bit field containing the bits of which buttons have been pushed
	ConfigFinished = FALSE,			// Flag to indicate config is finished
	its_high_noon = true,	// Flag to tell if this is the first time 12 rolls around
	set_alarm_flag = false,
	alarm_ampm_toggle = true, // Flag to tell whether alarm is set for AM/PM
	show_pm_flag, // Flag used by the display functions to determine if AM/PM indicator should be on
	cache_original_alarm = true,
	alarm_enabled = false,
	AlarmPmFlag = false,
	TimePmFlag = false,
    setMinutes = false,
    setHours = false;

volatile int
	BcdTime[4],				// Array to hold the hours and minutes in BCD format
	DisplayedDigit = 0,		// Current digit being displayed on the LED display
	// Current format for the displayed time ( IE 12 or 24 hour format )
	ClockHourFormat = CLOCK_HOUR_FORMAT_12;

volatile struct{
	uint8_t hours;
	uint8_t minutes;
	uint8_t time_format;
}OriginalAlarm;

void ConfigureDisplay( void )
{
	ConfigureIoPins();
	ConfigureTimer();
	ConfigureRealTimeClock();
}


int main(int argc, char* argv[])
 {

//
// Reset of all peripherals, Initializes the Flash interface and the System timer.
//
	HAL_Init();

//
// Configure the system clock
//
//	SystemClock_Config(); // New library calls this before main

//
// Display project name with version number
//
	trace_puts(
			"*\n"
			"*\n"
			"* Alarm clock project for stm32f4discovery board V2.00\n"
			"*\n"
			"*\n"
			);

//
// Initialize the seven segment display pins and push buttons
//
	ConfigureDisplay();

//
// Send a greeting to the trace device (skipped on Release).
//
	trace_puts("Initialization Complete");

//
// At this stage the system clock should have already been configured at high speed.
//

	trace_printf("System clock: %u Hz\n", HAL_RCC_GetSysClockFreq() /* SystemCoreClock */ );
    trace_printf( "HClk frequency %u\r\n", HAL_RCC_GetHCLKFreq());
    trace_printf( "PClk 1 frequency %u\r\n", HAL_RCC_GetPCLK1Freq());
    trace_printf( "PClk 2 frequency %u\r\n", HAL_RCC_GetPCLK2Freq());

	Alarm = FALSE;

//
// Start the display timer (TIM5)
//
	__HAL_TIM_ENABLE( &DisplayTimer ); // Enable timer to start
	ConfigFinished = TRUE;


	while ( TRUE )
	{

//
// Wait for an interrupt to occur
//
		__asm__ volatile ( "wfi" );

	}
}




/*
 * Function: TIM5_IRQHandler
 *
 * Description:
 *
 * 		Timer interrupt handler that is called at a rate of 250Hz.
 *
 * functionality:
 * 		Poll the time and displays it on the 7 segment display
 * 		Checks for button presses and handle any bounce conditions
 * 		Do the appropriate operation based on the button pressed
 *
 */


void TIM5_IRQHandler(void)
{
	// Check that the board is set up (ie, that this is not a spurious interrupt)
	if(!ConfigFinished){
		// clear the timer interrupt flag
		__HAL_TIM_CLEAR_FLAG( &DisplayTimer, TIM_IT_UPDATE );
		return;
	}

//
// Validate the correct interrupt has occurred
//

	if( __HAL_TIM_GET_FLAG( &DisplayTimer, TIM_IT_UPDATE ) != RESET )
	{

		if(!set_alarm_flag){
			GetCurrentTime();
		}
		else{
			GetCurrentAlarm();
		}

		Display7Segment();

//
// Process any button events
//
		if ( TRUE == CheckButtons())
		{

//
// Debug code
//
//			trace_printf( "B1:%d B2:%d B3:%d B4:%d B5:%d B6:%d\n",
//					ButtonsPushed[0], ButtonsPushed[1], ButtonsPushed[2],
//					ButtonsPushed[3], ButtonsPushed[4], ButtonsPushed[5]);

			ProcessButtons();
		}

//
// clear the timer interrupt flag
//
		__HAL_TIM_CLEAR_FLAG( &DisplayTimer, TIM_IT_UPDATE );
    }

}


/*
 * Function: RTC_Alarm_IRQHandler
 *
 * Description:
 *
 * When alarm occurs, clear all the interrupt bits and flags then start playing music.
 *
 */

#ifdef USE_RTC

void RTC_Alarm_IRQHandler(void)
{

//
// Verify that this is a real time clock interrupt
//
	if( __HAL_RTC_ALARM_GET_IT( &RealTimeClock, RTC_IT_ALRA ) != RESET )
	{

//
// Clear the alarm flag and the external interrupt flag
//
    	__HAL_RTC_ALARM_CLEAR_FLAG( &RealTimeClock, RTC_FLAG_ALRAF );
    	__HAL_RTC_EXTI_CLEAR_FLAG( RTC_EXTI_LINE_ALARM_EVENT );

//
// Restore the alarm to it's original time. This could have been a snooze alarm
//
    	HAL_RTC_SetAlarm_IT( &RealTimeClock, &ClockAlarm, RTC_FORMAT_BCD );

    	Alarm = TRUE;
    	trace_printf("Completed RTC_Alarm_IRQHandler\r\n");

	}
}

#endif

/*
 * Function: Display7Segment
 *
 * Description:
 *
 * Displays the current time, alarm time or time format
 *
 */

void Display7Segment(void)
{
//
// clear digit selection bits
//
	hide_digits();
//
// clear segment selection bits
//

	clear_segments();

//
// Select current digit
//
	show_digit(DisplayedDigit);

//
// Enable segments to be illuminated
//
	int ValueToDisplay;
	if( DisplayedDigit == 4 ){
		ValueToDisplay = COLON;
	}
	else{
		ValueToDisplay = BcdTime[DisplayedDigit];
	}
	if(ValueToDisplay == 0 && DisplayedDigit == 0){
		ValueToDisplay = BLANK_DIGIT;
	}
	display_number(ValueToDisplay);
//
// Advance to the next digit to be display on next interrupt
//
	if(DisplayedDigit == 4){
		DisplayedDigit = 0;
	}
	else{
		DisplayedDigit++;
	}

//
// Separate from the rest of the display, turn the alarm on or off if needed
//
	if(Alarm == TRUE){
		HAL_GPIO_WritePin(GPIOE, PIN_ALARM, 1);
	}
	else{
		HAL_GPIO_WritePin(GPIOE, PIN_ALARM, 0);
	}
}


/*
 * Function: SetTime
 *
 * Description:
 *
 * Advance either the time hours or minutes field. Validate the new time and then update the clock
 *
 */

void SetTime(void)
{
    if (setMinutes == true){
        ClockTime.Minutes++;
    }
    if (setHours == true){
        ClockTime.Hours++;
    }

    trace_printf("Set time to %02d:%02d, which is ", ClockTime.Hours, ClockTime.Minutes);

    // These all must be non-nested if statements for validation to work
    if ( ClockTime.Minutes >= 60 )
    {
        ClockTime.Minutes %= 60;

        ClockTime.Hours++;
    }
	if(ClockTime.Hours == 12 && its_high_noon){
		TimePmFlag = !TimePmFlag;//Toggle the AM/PM flag
		its_high_noon = false;
		if(TimePmFlag){
			ClockTime.TimeFormat = RTC_HOURFORMAT12_PM;
		}
		else{
			ClockTime.TimeFormat = RTC_HOURFORMAT12_AM;
		}
	}
	if ( ClockTime.Hours > 12 )
	{
		ClockTime.Hours %= 12;
		its_high_noon = true;
	}
    trace_printf("%02d:%02d after validation steps.\r\n", ClockTime.Hours, ClockTime.Minutes);

    HAL_RTC_SetTime(&RealTimeClock, &ClockTime, RTC_FORMAT_BIN);
}

/*
 * Function: SetAlarm
 *
 * Description:
 *
 * Advance either the alarm hours or minutes field. Validate the new alarm time and then set the alarm
 *
 */

void SetAlarm(void)
{
    if (setMinutes == true){
        ClockAlarm.AlarmTime.Minutes++;
    }
    if (setHours == true){
        ClockAlarm.AlarmTime.Hours++;
    }

    // These all must be non-nested if statements for validation to work
    if ( ClockAlarm.AlarmTime.Minutes >= 60 ){
        ClockAlarm.AlarmTime.Minutes %= 60;
        ClockAlarm.AlarmTime.Hours++;
    }
	if(ClockAlarm.AlarmTime.Hours == 12 && alarm_ampm_toggle){
		AlarmPmFlag = !AlarmPmFlag;//Toggle the AM/PM flag
		alarm_ampm_toggle = false;
		if(AlarmPmFlag){
			ClockAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_PM;
		}
		else{
			ClockAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
		}
	}
	if ( ClockAlarm.AlarmTime.Hours > 12 ){
		ClockAlarm.AlarmTime.Hours %= 12;
		alarm_ampm_toggle = true;
	}
    HAL_RTC_SetAlarm_IT( &RealTimeClock, &ClockAlarm, RTC_FORMAT_BIN );
}


/*
 * Function: Snooze
 *
 * Description:
 *
 * Add 10 Minutes to the current time and validate. Update the alarm and enable.
 *
 */

void Snooze(void)
{
	if ( TRUE == Alarm ){
		Alarm = FALSE;

		if ( HAL_RTC_WaitForSynchro( &RealTimeClock ) == HAL_OK ){
			HAL_RTC_GetTime(&RealTimeClock, &ClockTime, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&RealTimeClock, &ClockDate, RTC_FORMAT_BIN);

			//trace_printf("Time: %02d:%02d:%02d\n", ClockTime.Hours, ClockTime.Minutes, ClockTime.Seconds);
			if(cache_original_alarm){
				OriginalAlarm.hours = ClockAlarm.AlarmTime.Hours;
				OriginalAlarm.minutes = ClockAlarm.AlarmTime.Minutes;
				OriginalAlarm.time_format = ClockAlarm.AlarmTime.TimeFormat;
				cache_original_alarm = false;
				trace_printf("Cached alarm time of %02d:%02d\r\n", OriginalAlarm.hours, OriginalAlarm.minutes);
			}

			ClockAlarm.AlarmTime.Hours = ClockTime.Hours;
			ClockAlarm.AlarmTime.Minutes = ClockTime.Minutes += 1;

			if ( ClockAlarm.AlarmTime.Minutes >= 60 ){
				ClockAlarm.AlarmTime.Minutes %= 60;
				ClockAlarm.AlarmTime.Hours++;
				if ( ClockAlarm.AlarmTime.Hours >= 12 ){
					ClockAlarm.AlarmTime.Hours %= 12;
				}
			}

			trace_printf("Snoozed the alarm. Next alarm is at %02d:%02d\r\n", ClockAlarm.AlarmTime.Hours, ClockAlarm.AlarmTime.Minutes);
			HAL_RTC_SetAlarm_IT( &RealTimeClock, &ClockAlarm, RTC_FORMAT_BIN );
		}
	}
}

/*
 * Function: Sleep
 *
 * Description:
 *
 * Reset the alarm to the original time, so that it will go off in 24 hours
 */
void Sleep(void){
	if ( TRUE == Alarm ){
		Alarm = FALSE;
		if(!cache_original_alarm){
			ClockAlarm.AlarmTime.Hours = OriginalAlarm.hours;
			ClockAlarm.AlarmTime.Minutes = OriginalAlarm.minutes;
			ClockAlarm.AlarmTime.TimeFormat = OriginalAlarm.time_format;
			cache_original_alarm = true;
		}

		trace_printf("Resetting alarm to %02d:%02d\r\n", ClockAlarm.AlarmTime.Hours, ClockAlarm.AlarmTime.Minutes);
		HAL_RTC_SetAlarm_IT( &RealTimeClock, &ClockAlarm, RTC_FORMAT_BIN );
	}
}

/*
 * Function: ToggleAlarm
 *
 * Description:
 *
 * Toggles the alarm interrupt request on and off
 */
void ToggleAlarm(void){
	if(alarm_enabled){
		HAL_NVIC_DisableIRQ( RTC_Alarm_IRQn );
		if(Alarm == true){
			Sleep();
		}
	}
	else{
		HAL_NVIC_EnableIRQ( RTC_Alarm_IRQn );
	}
	alarm_enabled = !alarm_enabled;
	trace_printf("alarm_enabled flag set to %d\r\n", alarm_enabled);
}


/*
 * Function: GetCurrentTime
 *
 * Description:
 *
 * Return either the alarm time or current time in binary coded decimal format store in the array BcdTime.
 *
 */

void GetCurrentTime(void)
{
	int hour_tens = 0, hours = 0, minute_tens = 0, minutes = 0;
	if ( HAL_RTC_WaitForSynchro( &RealTimeClock ) == HAL_OK ){
		// Update the time and date structures
		HAL_RTC_GetTime(&RealTimeClock, &ClockTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&RealTimeClock, &ClockDate, RTC_FORMAT_BIN);

		// The purpose of this is to toggle the TimePmFlag exactly once, once it flips over to 12
		if ( ClockTime.Hours == 12 && its_high_noon)
		{
			TimePmFlag = !TimePmFlag;// Toggle the AM/PM flag
			its_high_noon = false;
		}
		else if ( ClockTime.Hours == 1 ){
			its_high_noon = true;
		}

		// Extract the value to set each digit
		hours = ClockTime.Hours;
		while(hours >= 10){
			hours -= 10;
			hour_tens++;
		}
		minutes = ClockTime.Minutes;
		while(minutes >= 10){
			minutes -= 10;
			minute_tens++;
		}
	}
	// Set the digits in the BCDTime array
	BcdTime[0] = hour_tens;
	BcdTime[1] = hours;
	BcdTime[2] = minute_tens;
	BcdTime[3] = minutes;
	show_pm_flag = TimePmFlag;
	// Display Debug
//	BcdTime[0] = 1;
//	BcdTime[1] = 2;
//	BcdTime[2] = 3;
//	BcdTime[3] = 8;
}

void GetCurrentAlarm(void){

    int hour_tens = 0, AlarmHours = 0, minute_tens = 0, AlarmMinutes = 0;
    if (ClockAlarm.AlarmTime.Hours == 12 && alarm_ampm_toggle){
        // toggle AlarmPmFlag
        AlarmPmFlag = !AlarmPmFlag;
        alarm_ampm_toggle = false;
    }

    else if (ClockAlarm.AlarmTime.Hours == 1){
        alarm_ampm_toggle = true;
        }

    AlarmHours = ClockAlarm.AlarmTime.Hours;
    while(AlarmHours >= 10){
            AlarmHours -= 10;
            hour_tens++;
        }
    AlarmMinutes = ClockAlarm.AlarmTime.Minutes;
    while(AlarmMinutes >= 10){
            AlarmMinutes -= 10;
            minute_tens++;
        }
    // Set the digits in the BCDTime array
        BcdTime[0] = hour_tens;
        BcdTime[1] = AlarmHours;
        BcdTime[2] = minute_tens;
        BcdTime[3] = AlarmMinutes;
        show_pm_flag = AlarmPmFlag;

}


/*
 * Function: CheckButtons
 *
 * Description:
 *
 * Check the current state of all the buttons and apply debounce algorithm. Return TRUE with the ButtonPushed
 * variable set indicating the button or buttons pushed if button press is detected.
 *
 */

uint16_t CheckButtons( void )
{
	// First, check the current value of the buttons
	bool button_current_value[6] = {HAL_GPIO_ReadPin(GPIOD, PIN_BUT_1),
									HAL_GPIO_ReadPin(GPIOD, PIN_BUT_2),
									HAL_GPIO_ReadPin(GPIOD, PIN_BUT_3),
									HAL_GPIO_ReadPin(GPIOD, PIN_BUT_4),
									HAL_GPIO_ReadPin(GPIOD, PIN_BUT_5),
									HAL_GPIO_ReadPin(GPIOD, PIN_BUT_6)};
	if(button_slowdown_counter != 0) button_slowdown_counter--;

	for(unsigned char button = 0; button < 6; button++){
		if (button_current_value[button] != ButtonsPushed[button]){
			// If the value returned has changed from the stored value
			if(Button_Debounce_Counter[button] == 0){
				// Once the counter reaches zero, the value should have stabilized
				// Update the state of the button to what it has stabilized to
				ButtonsPushed[button] = button_current_value[button];
				Button_Debounce_Counter[button] = BUTTON_STAB_COUNT;
			}
			else{
				// Decrement the counter to wait until the return of ReadPin is stable
				Button_Debounce_Counter[button]--;
			}
		}
		else{
			// Reset the counter--the value is either not very stable, or has not changed
			Button_Debounce_Counter[button] = BUTTON_STAB_COUNT;
		}
	}

	// If any of the button values are true, return TRUE
	for(unsigned char button = 0; button < 6; button++){
		if(ButtonsPushed[button]){
			return( TRUE );
		}
	}
	// Otherwise return false
	return( FALSE );
}


/*
 * Button 1: advance minute
 * Button 2: advance hours
 * Button 3: toggle between setting alarm or time as well as which is shown on the display
 * Button 4: Snooze
 * Button 5: toggle alarm
 * Button 6: potentially show alarm
 */
void ProcessButtons( void )
{
	// Buttons are low true so we alias them here for readability
    bool Button1 = !ButtonsPushed[0];
    bool Button2 = !ButtonsPushed[1];
    bool Button3 = !ButtonsPushed[2];
    bool Button4 = !ButtonsPushed[3];
    bool Button5 = !ButtonsPushed[4];
    bool Button6 = !ButtonsPushed[5];

    if(button_slowdown_counter == 0){
    	button_slowdown_counter = BUTTON_USABILITY_COUNT;
		if (Button3){
			set_alarm_flag = true;
			//debug
//			trace_printf("Updated set alarm flag to %d\r\n", set_alarm_flag);
		}
		else{
			set_alarm_flag = false;
		}
		if (Button1 || Button2){

			if(Button1){
				setMinutes = true;
				setHours = false;
			}
			else if (Button2){
				setHours = true;
				setMinutes = false;
			}
			else{
				setHours = false;
				setMinutes = false;
			}

			if(set_alarm_flag){
				SetAlarm();
			}
			else{
				SetTime();
			}
		}
		if(Button4){
			Snooze();
		}
		if(Button5){
			Sleep();
		}
		if(Button6){
			ToggleAlarm();
		}
    }
    // Button slowdown counter is decremented elsewhere
}



// ----------------------------------------------------------------------------
// Initialization functions

/*
 *
 *  System Clock Configuration
 *
 */

void SystemClock_Config( void )
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if( HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK )
	{
		trace_printf( "HAL_RCC_OscConfig failed\r\n");
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if( HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK )
	{
		trace_printf( "HAL_RCC_ClockConfig failed\r\n");
	}

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
	PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
	PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		trace_printf( "HAL_RCCEx_PeriphCLKConfig failed\r\n");
	}


	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void ConfigureIoPins( void ){
//
// Configure the 7 segments, 5 digit controls and alarm signal as outputs
//
	__HAL_RCC_GPIOE_CLK_ENABLE(); // enabling clock for port E

	GPIO_InitTypeDef GPIOE_InitStructure; // handle for pointing GPIO of port E

	GPIOE_InitStructure.Pin = PIN_ALL_DIGITS|PIN_ALL_SEGMENTS;
	GPIOE_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOE_InitStructure.Speed = GPIO_SPEED_LOW;
	GPIOE_InitStructure.Pull = GPIO_NOPULL;
	GPIOE_InitStructure.Alternate = 0;

	HAL_GPIO_Init(GPIOE, &GPIOE_InitStructure);

//
// Configure the push button switches ( maximum 6 ) as inputs with pull-ups
//
	__HAL_RCC_GPIOD_CLK_ENABLE(); // enabling clock for port D

	GPIO_InitTypeDef GPIOD_InitStructure; // handle for pointing GPIO of port D

	GPIOD_InitStructure.Pin = PIN_ALL_BUTTONS;
	GPIOD_InitStructure.Mode = GPIO_MODE_INPUT; // set mode to be input
	GPIOD_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIOD_InitStructure.Pull = GPIO_PULLUP; // configure pull-up resistor
	GPIOD_InitStructure.Alternate = 0;

	HAL_GPIO_Init(GPIOD, &GPIOD_InitStructure);

//
// Configure port C pin 6 to act as Timer 5 (alternate function)
//
	__HAL_RCC_GPIOC_CLK_ENABLE(); // enabling clock for port C

	GPIO_InitTypeDef GPIOC_InitStructure; //A handle to initialize GPIO port C

	GPIOC_InitStructure.Pin = GPIO_PIN_6;
	GPIOC_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOC_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIOC_InitStructure.Pull = GPIO_NOPULL;
	GPIOC_InitStructure.Alternate = GPIO_AF2_TIM5;// TIM_5 as alternate function

	HAL_GPIO_Init(GPIOC, &GPIOC_InitStructure);
}

void ConfigureTimer( void )
{
//
// Enable the LED multiplexing display and push button timer (TIM5) at a frequency of 250Hz
//
	__HAL_RCC_TIM5_CLK_ENABLE();
	DisplayTimer.Instance = TIM5;
	DisplayTimer.Init.Period = 39; // period & prescaler combination for 0.004 seconds count (ie 250 Hz)
	DisplayTimer.Init.Prescaler = 8399;
	DisplayTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
	DisplayTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_Base_Init( &DisplayTimer ) != HAL_OK){
		trace_printf("HAL_TIM_Base_Init failed\r\n");
	}

	HAL_NVIC_SetPriority( TIM5_IRQn, 0, 0); // set priority for the interrupt. Value 0 corresponds to highest priority
	HAL_NVIC_EnableIRQ( TIM5_IRQn ); // Enable interrupt function request of Timer5

	__HAL_TIM_ENABLE_IT( &DisplayTimer, TIM_IT_UPDATE ); // Enable timer interrupt flag to be set when timer count is reached
	// Note: The timer has not been started yet, to prevent spurious interrupts
}



void ConfigureRealTimeClock( void )
{
	RCC_OscInitTypeDef
		RCC_OscInitStruct;

	RCC_PeriphCLKInitTypeDef
		PeriphClkInitStruct;

//
// Configure LSI as RTC clock source
//
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;

	if( HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK )
	{
		trace_printf( "HAL_RCC_OscConfig failed\r\n");
	}

//
// Assign the LSI clock to the RTC
//
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		trace_printf( "HAL_RCCEx_PeriphCLKConfig failed\r\n");
	}

//
// Enable the RTC
//
	__HAL_RCC_RTC_ENABLE();

//
// Configure the RTC format and clock divisor
//

	RealTimeClock.Instance = RTC;
	RealTimeClock.Init.HourFormat = RTC_HOURFORMAT_12;

	RealTimeClock.Init.AsynchPrediv = 127;
	RealTimeClock.Init.SynchPrediv = 0xFF;
	RealTimeClock.Init.OutPut = RTC_OUTPUT_DISABLE;
	RealTimeClock.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	RealTimeClock.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	HAL_RTC_Init(&RealTimeClock );

//
// Disable the write protection for RTC registers
//
	__HAL_RTC_WRITEPROTECTION_DISABLE( &RealTimeClock );

//
// Disable the Alarm A interrupt
//
	__HAL_RTC_ALARMA_DISABLE( &RealTimeClock );

//
// Clear flag alarm A
//
	__HAL_RTC_ALARM_CLEAR_FLAG(&RealTimeClock, RTC_FLAG_ALRAF);

//
// Structure to set the time in the RTC
//
	ClockTime.Hours = 11;
	ClockTime.Minutes = 59;
	ClockTime.Seconds = 55;
	ClockTime.SubSeconds = 0;
	ClockTime.TimeFormat = RTC_HOURFORMAT12_AM;
	ClockTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	ClockTime.StoreOperation = RTC_STOREOPERATION_RESET;

//
// Structure to set the date in the RTC
//

	ClockDate.Date = 	21;
	ClockDate.Month = 	RTC_MONTH_JUNE;
	ClockDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
	ClockDate.Year =	17;

//
// Set the date and time in the RTC
//

	HAL_RTC_SetDate(&RealTimeClock, &ClockDate, RTC_FORMAT_BIN);
	HAL_RTC_SetTime(&RealTimeClock, &ClockTime, RTC_FORMAT_BIN);



	HAL_NVIC_SetPriority( RTC_Alarm_IRQn, 0, 0 );
	HAL_NVIC_EnableIRQ( RTC_Alarm_IRQn );
	alarm_enabled = true;


//
// Set the initial alarm time
//
	ClockAlarm.Alarm = RTC_ALARM_A;
	ClockAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_PM;
	ClockAlarm.AlarmTime.Hours = 12;
	ClockAlarm.AlarmTime.Minutes = 00;
	ClockAlarm.AlarmTime.Seconds = 00;
	ClockAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
	ClockAlarm.AlarmDateWeekDay = 1;

	// sets the alarm in RTC
	HAL_RTC_SetAlarm_IT( &RealTimeClock, &ClockAlarm, RTC_FORMAT_BIN );

}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
