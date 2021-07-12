#include "clockface.h"

void increment_time(ClockFace *time_to_show){
	// Note: NOT WORKING
	char condition = (time_to_show->m2 != 9) + (time_to_show->m1 != 5) + (time_to_show->h2 != 2) + (time_to_show->h1 != 1);

	switch(condition){
	case 1:
		time_to_show->m2 += 1;
		break;
	case 2:
		time_to_show->m1 += 1;
		time_to_show->m2 = 0;
		break;
	case 3:
		time_to_show->h2 += 1;
		time_to_show->m1 = 0;
		time_to_show->m2 = 0;
		break;
	case 4:
		time_to_show->h1 = 0;
		time_to_show->h2 = 1;
		time_to_show->m1 = 0;
		time_to_show->m2 = 0;
		break;
	}
}
