/*
 * utils.c
 *
 *  Created on: 19 mai 2013
 *      Author: n0dev
 */

#include "../include/utils.h"

double convert_m_to_au (double km)
{
	return km/KM2UA;
}

double convert_m_to_ly (double km)
{
	return km/KM2LY;
}

double convert_m_to_kly (double km)
{
	return km/KM2KLY;
}

double convert_m_to_mly (double km)
{
	return km/KM2MLY;
}
