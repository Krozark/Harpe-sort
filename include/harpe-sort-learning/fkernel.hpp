#ifndef FKERNEL_HPP
#define FKERNEL_HPP

#include <utils/kernel.hpp>

namespace ker
{
    using namespace utils::ker;
    //unaire
    inline double sin_f(double _1){
        return sin(_1);
    };

    inline double sub_u(double _1) {
        return -_1;
    }
    inline double gaussian_default(double _1){
        return gaussian(_1);
    };
}
#endif
