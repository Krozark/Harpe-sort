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
        return (
            gauss((gauss(
                (((vals[6])/(vals[2])) //NB_AA / INTENSITEE_TOTAL_PARENT
                 /(
                     ((sin(sin(vals[3]))) //INTENSITEE_TOTAL
                      -((sin((
                          (((vals[6])/(vals[2]))/((vals[4])*(vals[5])))+(gauss(vals[7])) //(NB_AA / INTENSITEE_TOTAL_PARENT)/(MASS_TOTAL*PERCENT_COUVERTURE)+gauss(MASS_PARENT)

                      )-(vals[3]) //INTENSITEE_TOTAL_PARENT
                      ))-((vals[6])/(gauss((vals[4])*(vals[5])))) //PERCENT_COUVERTURE / gauss(MASS_TOTAL*PERCENT_COUVERTURE)
                      ))*(vals[5])//PERCENT_COUVERTURE
                 ))
                +((((vals[6])+(vals[3]))+(0.246167))-(sin(0.547814)))//NB_AA+INTENSITEE_TOTAL + C
            ))
                *((((0.502697)-((0.823913)/(vals[7])))+(0.0812427))/(sin(0.246167)) //MASS_PARENT
                 )
            ));
    }
}
