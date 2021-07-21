// Constants File

#define CLOCK_HOUR_FORMAT_12    (0)       // Display the time with AM/PM format
#define CLOCK_HOUR_FORMAT_24    (1)       // Display the time with 24 Hour format

#define TRUE	( 1 == 1 )
#define FALSE	( 1 == 0 )

#define COLON 10

// LED segments and digit control pins should all be on GPIOE
// Define pins for segment selection
#define PIN_A GPIO_PIN_2
#define PIN_B GPIO_PIN_4
#define PIN_C GPIO_PIN_5
#define PIN_D GPIO_PIN_6
#define PIN_E GPIO_PIN_7
#define PIN_F GPIO_PIN_8
#define PIN_G GPIO_PIN_9
// Define some additional names (L pins
#define PIN_L1 PIN_A
#define PIN_L2 PIN_B
#define PIN_L3 PIN_C
#define PIN_ALL_SEGMENTS (PIN_A | PIN_B | PIN_C | PIN_D | PIN_E | PIN_F | PIN_G)
// Define pins for digit selection
#define PIN_DIG_1 GPIO_PIN_10
#define PIN_DIG_2 GPIO_PIN_11
#define PIN_DIG_3 GPIO_PIN_12
#define PIN_DIG_4 GPIO_PIN_13
#define PIN_DIG_L GPIO_PIN_14
#define PIN_ALL_DIGITS (PIN_DIG_1 | PIN_DIG_2 | PIN_DIG_3 | PIN_DIG_4 | PIN_DIG_L)

// Button inputs should all be on GPIOD
#define PIN_BUT_1 GPIO_PIN_0
#define PIN_BUT_2 GPIO_PIN_1
#define PIN_BUT_3 GPIO_PIN_2
#define PIN_BUT_4 GPIO_PIN_3
#define PIN_BUT_5 GPIO_PIN_6
#define PIN_BUT_6 GPIO_PIN_7
#define PIN_ALL_BUTTONS (PIN_BUT_1 | PIN_BUT_2 | PIN_BUT_3 | PIN_BUT_4 | PIN_BUT_5 | PIN_BUT_6)

// Number of "cycles" to wait before updating a button input (used to determine
// a button value is stable while debouncing)
#define BUTTON_STAB_COUNT 10
