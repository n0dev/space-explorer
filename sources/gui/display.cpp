#include <stdlib.h>
#include <stdio.h>

#include "../include/utils.h"

void display_actual_speed(char* text, double speed)
{
	static double abs_speed = abs(speed);
	if (abs_speed < 149597870000) {
		snprintf(text, 79, "Spaceship speed: %.0f KM/S", speed/1000.0);
	}
	else if(abs_speed < 9460700000000000) {
		snprintf(text, 79, "Spaceship speed: %.0f AU/S", convert_m_to_au(speed));
	}
	else if(abs_speed < 9460700000000000000.0) {
		snprintf(text, 79, "Spaceship speed: %.0F LY/S", convert_m_to_ly(speed));
	}
	else if(abs_speed < 9460700000000000000000.0) {
		snprintf(text, 79, "Spaceship speed: %.0F KLY/S", convert_m_to_kly(speed));
	}
	else {
		snprintf(text, 79, "Spaceship speed: %.0F MLY/S", convert_m_to_mly(speed));
	}
}

void display_actual_distance(char* text, double distance)
{
	if (distance < 149597870000) {
		snprintf(text, 79, "Distance of Earth: %.0f km", distance/1000.0);
	}
	else if(distance < 9460700000000000) {
		snprintf(text, 79, "Distance of Earth: %.0f AU", convert_m_to_au(distance));
	}
	else {
		snprintf(text, 79, "Distance of Earth: %.0F ly", convert_m_to_ly(distance));
	}
}


