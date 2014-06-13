/*
*	date: 13-06-2014 16:56
*	score: 1
*	size: 16
*	generation: 11
*/
#include <utils/maths.hpp>
#define gauss utils::maths::ker::gaussian

extern "C" {
double calc_score(const double* const vals)
{
	return (((vals[2])/(gauss(vals[6])))/((0.825851)+(((vals[0])+(sin((0.544378)/(vals[0]))))*(sin(vals[5])))));
}
}
