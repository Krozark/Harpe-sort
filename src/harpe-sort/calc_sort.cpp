/*
*	date: 25-07-2014 13:06
*	score: 0.433022
*	size: 52
*	generation: 15
*	island: 2
*/
#include <utils/maths.hpp>
#define gauss utils::maths::ker::gaussian

extern "C" {
double calc_score(const double* const vals)
{
	return (gauss((gauss((((vals[6])/(vals[2]))/(((sin(sin(vals[3])))-((sin(((((vals[6])/(vals[2]))/((vals[4])*(vals[5])))+(gauss(vals[7])))-(vals[3])))-((vals[6])/(gauss((vals[4])*(vals[5]))))))*(vals[5])))+((((vals[6])+(vals[3]))+(0.246167))-(sin(0.547814)))))*((((0.502697)-((0.823913)/(vals[7])))+(0.0812427))/(sin(0.246167)))));
}
}
