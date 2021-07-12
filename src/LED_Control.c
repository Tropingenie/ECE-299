// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/trace.h"
#include "stm32f4xx.h"
#include "clockface.h"

#define DELAY 1000
#define SEG_L 10

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void display_number(int n){

	// First, reset the display
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_6, GPIO_PIN_RESET ); // Pin A
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_7, GPIO_PIN_RESET ); // Pin B
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8, GPIO_PIN_RESET ); // Pin C
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_9, GPIO_PIN_RESET ); // Pin D
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_10, GPIO_PIN_RESET ); // Pin E
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_11, GPIO_PIN_RESET ); // Pin F
	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_13, GPIO_PIN_RESET ); // Pin G


	switch (n){
	case 0:
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_6, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_7, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_9, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_10, GPIO_PIN_SET ); // Pin E
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_11, GPIO_PIN_SET ); // Pin F
		break;
	case 1:
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_7, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8, GPIO_PIN_SET ); // Pin C
		break;
	case 2:
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_6, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_7, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_9, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_10, GPIO_PIN_SET ); // Pin E
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_13, GPIO_PIN_SET ); // Pin G
		break;
	case 3:
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_6, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_7, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_9, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_13, GPIO_PIN_SET ); // Pin G
		break;
	case 4:
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_7, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_11, GPIO_PIN_SET ); // Pin F
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_13, GPIO_PIN_SET ); // Pin G
		break;
	case 5:
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_6, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_9, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_11, GPIO_PIN_SET ); // Pin F
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_13, GPIO_PIN_SET ); // Pin G
		break;
	case 6:
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_9, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_10, GPIO_PIN_SET ); // Pin E
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_11, GPIO_PIN_SET ); // Pin F
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_13, GPIO_PIN_SET ); // Pin G
		break;
	case 7:
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_6, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_7, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8, GPIO_PIN_SET ); // Pin C
		break;
	case 8:
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_6, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_7, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_9, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_10, GPIO_PIN_SET ); // Pin E
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_11, GPIO_PIN_SET ); // Pin F
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_13, GPIO_PIN_SET ); // Pin G
		break;
	case 9:
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_6, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_7, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_9, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_11, GPIO_PIN_SET ); // Pin F
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_13, GPIO_PIN_SET ); // Pin G
		break;
	case SEG_L:
		// This is to show the colon, ie L1 and L2 (which are the same  as A and
		// B internally)
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_6, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, GPIO_PIN_7, GPIO_PIN_SET ); // Pin B
		break;
	}
}

void hide_digits(){
	HAL_GPIO_WritePin( GPIOD, GPIO_PIN_1, GPIO_PIN_RESET );// Digit 1
	HAL_GPIO_WritePin( GPIOD, GPIO_PIN_2, GPIO_PIN_RESET );// Digit 2
	HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_RESET );// Digit 3
	HAL_GPIO_WritePin( GPIOD, GPIO_PIN_6, GPIO_PIN_RESET );// Digit 4
	HAL_GPIO_WritePin( GPIOD, GPIO_PIN_8, GPIO_PIN_RESET );// Digit L
}

void show_digit(int n){
	switch (n){
	case 1:
		HAL_GPIO_WritePin( GPIOD, GPIO_PIN_1, GPIO_PIN_SET );
		break;
	case 2:
		HAL_GPIO_WritePin( GPIOD, GPIO_PIN_2, GPIO_PIN_SET );
		break;
	case 3:
		HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_SET );
		break;
	case 4:
		HAL_GPIO_WritePin( GPIOD, GPIO_PIN_6, GPIO_PIN_SET );
		break;
	case SEG_L:
		HAL_GPIO_WritePin( GPIOD, GPIO_PIN_8, GPIO_PIN_SET );
		break;
	}
}

void show_time(ClockFace *time_to_show){
	// Note: NOT WORKING

	hide_digits();
	display_number(time_to_show->h1);
	show_digit(1);
	HAL_Delay(DELAY);

	hide_digits();
	display_number(time_to_show->h2);
	show_digit(2);
	HAL_Delay(DELAY);

	hide_digits();
	display_number(time_to_show->m1);
	show_digit(3);
	HAL_Delay(DELAY);

	hide_digits();
	display_number(time_to_show->m2);
	show_digit(4);
	HAL_Delay(DELAY);

	// Show the colon (L1 and L2)
	hide_digits();
	display_number(SEG_L);
	show_digit(SEG_L);
	HAL_Delay(DELAY);
}

#pragma GCC diagnostic pop
