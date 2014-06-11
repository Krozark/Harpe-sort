#ifndef FKERNEL_HPP
#define FKERNEL_HPP

#include <math.h>
#include <utils/maths.hpp>

namespace ker
{
    //unaire
    inline double sin_f(double _1){
        return sin(_1);
    };

    inline double moins_u(double _1) {
        return -_1;
    }
    inline double uniform(double _1){
        if(abs(_1) <=1)
            return _1/2.0;
        return 0;
    };
    inline double triangular(double _1){
        double ab = abs(_1);
        if(ab <=1)
            return 1 - ab;
        return 0;
    };
    inline double epanechnikov(double _1){
        if(abs(_1) <=1)
            return 3.0/4.0*(1-utils::maths::power<2>::of(_1));
        return 0;
    };
    inline double quartic(double _1){

        if(abs(_1) <=1)
        {
            double c = 1-_1*_1;
            return 15.0/16.0*utils::maths::power<2>::of(c);
        }
        return 0;
    };
    inline double triweight(double _1){
        if(abs(_1) <=1)
        {
            double c = 1-_1*_1;
            return 35.0/32.0*utils::maths::power<3>::of(c);
        }
        return 0;
    };
    inline double tricube(double _1){
        double ab = abs(_1);
        if(ab <=1)
        {
            double c = 1-ab*ab*ab;
            return 70.0/81.0*utils::maths::power<3>::of(c);
        }
    };
    inline double gaussian(double _1){
        return (1.0/sqrt(2*M_PI))*exp(-0.5*utils::maths::power<2>::of(_1));
        //return (1.0/_1*sqrt(2*M_PI))*exp(-0.5*utils::maths::power<2>::of((_1-_3)/_2));
    };
    inline double cosine(double _1){
        if(abs(_1)<=  1)
            return M_PI/4.0*cos(M_PI/2.0*_1);
        return 0;
    };
    //binaire
    inline double mul(double _1,double _2){
        return _1*_2;
    };
    inline double div(double _1,double _2){
        return _1 / _2;
    };
    inline double add(double _1,double _2){
        return _1 + _2;
    };
    inline double moins_b(double _1,double _2){
        return _1 - _2;
    };
}
#endif
