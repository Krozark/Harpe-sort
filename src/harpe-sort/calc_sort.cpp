/*
*	date: 13-06-2014 17:23
*	score: 0.774194
*	size: 32
*	generation: 2
*/
#include <utils/maths.hpp>
#define gauss utils::maths::ker::gaussian

extern "C" {
double calc_score(const double* const vals)
{
	return (gauss((((((0.365158)/(0.0959336))/(0.915749))/(0.102319))-(gauss(((vals[1])-(0.864675))+((0.721844)*(0.276516)))))*((gauss(vals[2]))*(((sin(vals[6]))-((vals[7])/(vals[6])))+(gauss((vals[3])/(vals[0])))))));
}
}
