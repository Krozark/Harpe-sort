/*
*	date: 07-08-2014 21:38
*	score: 0.366912
*	size: 54
*	generation: 22
*	island: 5
*/
#include <utils/maths.hpp>
#define gauss utils::maths::ker::gaussian

extern "C" {
double calc_score(const double* const vals)
{
	return ((vals[0])-((((vals[5])-(((vals[3])-((vals[5])-((vals[6])-(0.210403))))-((0.333492)-(vals[3]))))/(((((vals[5])*((((vals[5])*((vals[1])+((vals[1])+(0.393935))))+(0.393935))+(0.393935)))+(((0.460143)/(vals[8]))/((0.766061)+(vals[4]))))*((vals[1])+(0.393935)))+((sin((vals[7])-((vals[1])/(0.626999))))/((0.766061)+(vals[4])))))*(vals[5])));
}
}
