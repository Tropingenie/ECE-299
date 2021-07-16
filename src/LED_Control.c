// Functions to abstract the setting of LED digits and segments

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/trace.h"
#include "stm32f4xx.h"
#include "constants.h"

// Not needed when using framework code provided
//#define DELAY 1000

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

extern inline void display_number(const int n){

	// First, reset the display
	HAL_GPIO_WritePin( GPIOE, PIN_ALL_SEGMENTS, GPIO_PIN_RESET );

	switch (n){
	case 0:
		HAL_GPIO_WritePin( GPIOE, PIN_A, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, PIN_B, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, PIN_C, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, PIN_D, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, PIN_E, GPIO_PIN_SET ); // Pin E
		HAL_GPIO_WritePin( GPIOE, PIN_F, GPIO_PIN_SET ); // Pin F
		break;
	case 1:
		HAL_GPIO_WritePin( GPIOE, PIN_B, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, PIN_C, GPIO_PIN_SET ); // Pin C
		break;
	case 2:
		HAL_GPIO_WritePin( GPIOE, PIN_A, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, PIN_B, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, PIN_D, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, PIN_E, GPIO_PIN_SET ); // Pin E
		HAL_GPIO_WritePin( GPIOE, PIN_G, GPIO_PIN_SET ); // Pin G
		break;
	case 3:
		HAL_GPIO_WritePin( GPIOE, PIN_A, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, PIN_B, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, PIN_C, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, PIN_D, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, PIN_G, GPIO_PIN_SET ); // Pin G
		break;
	case 4:
		HAL_GPIO_WritePin( GPIOE, PIN_B, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, PIN_C, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, PIN_F, GPIO_PIN_SET ); // Pin F
		HAL_GPIO_WritePin( GPIOE, PIN_G, GPIO_PIN_SET ); // Pin G
		break;
	case 5:
		HAL_GPIO_WritePin( GPIOE, PIN_A, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, PIN_C, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, PIN_D, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, PIN_F, GPIO_PIN_SET ); // Pin F
		HAL_GPIO_WritePin( GPIOE, PIN_G, GPIO_PIN_SET ); // Pin G
		break;
	case 6:
		HAL_GPIO_WritePin( GPIOE, PIN_C, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, PIN_D, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, PIN_E, GPIO_PIN_SET ); // Pin E
		HAL_GPIO_WritePin( GPIOE, PIN_F, GPIO_PIN_SET ); // Pin F
		HAL_GPIO_WritePin( GPIOE, PIN_G, GPIO_PIN_SET ); // Pin G
		break;
	case 7:
		HAL_GPIO_WritePin( GPIOE, PIN_A, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, PIN_B, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, PIN_C, GPIO_PIN_SET ); // Pin C
		break;
	case 8:
		HAL_GPIO_WritePin( GPIOE, PIN_A, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, PIN_B, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, PIN_C, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, PIN_D, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, PIN_E, GPIO_PIN_SET ); // Pin E
		HAL_GPIO_WritePin( GPIOE, PIN_F, GPIO_PIN_SET ); // Pin F
		HAL_GPIO_WritePin( GPIOE, PIN_G, GPIO_PIN_SET ); // Pin G
		break;
	case 9:
		HAL_GPIO_WritePin( GPIOE, PIN_A, GPIO_PIN_SET ); // Pin A
		HAL_GPIO_WritePin( GPIOE, PIN_B, GPIO_PIN_SET ); // Pin B
		HAL_GPIO_WritePin( GPIOE, PIN_C, GPIO_PIN_SET ); // Pin C
		HAL_GPIO_WritePin( GPIOE, PIN_D, GPIO_PIN_SET ); // Pin D
		HAL_GPIO_WritePin( GPIOE, PIN_F, GPIO_PIN_SET ); // Pin F
		HAL_GPIO_WritePin( GPIOE, PIN_G, GPIO_PIN_SET ); // Pin G
		break;
	case COLON:
		HAL_GPIO_WritePin( GPIOE, PIN_L1, GPIO_PIN_SET);
		HAL_GPIO_WritePin( GPIOE, PIN_L2, GPIO_PIN_SET);

	}
}

extern inline void hide_digits(){
	HAL_GPIO_WritePin( GPIOE, PIN_ALL_DIGITS, GPIO_PIN_RESET );
}

extern inline void show_digit(const int n){
	switch (n){
	case 0:
		HAL_GPIO_WritePin( GPIOE, PIN_DIG_1, GPIO_PIN_SET );
		break;
	case 1:
		HAL_GPIO_WritePin( GPIOE, PIN_DIG_2, GPIO_PIN_SET );
		break;
	case 2:
		HAL_GPIO_WritePin( GPIOE, PIN_DIG_3, GPIO_PIN_SET );
		break;
	case 3:
		HAL_GPIO_WritePin( GPIOE, PIN_DIG_4, GPIO_PIN_SET );
		break;
	case 4:
		HAL_GPIO_WritePin( GPIOE, PIN_DIG_L, GPIO_PIN_SET );
		break;
	}
}

// Probably not needed due to showing each digit in the framework code
//void show_time(int h1, int h2, int m1, int m2){
//	hide_digits();
//	display_number(h1);
//	show_digit(1);
//	HAL_Delay(DELAY);
//
//	hide_digits();
//	display_number(h2);
//	show_digit(2);
//	HAL_Delay(DELAY);
//
//	hide_digits();
//	display_number(m1);
//	show_digit(3);
//	HAL_Delay(DELAY);
//
//	hide_digits();
//	display_number(m2);
//	show_digit(4);
//	HAL_Delay(DELAY);
//
//	// Show the colon (L1 and L2)
//	hide_digits();
//	display_number(SEG_L);
//	show_digit(SEG_L);
//	HAL_Delay(DELAY);
//}

#pragma GCC diagnostic pop
