#include <algorithm>
#include <utils/maths.hpp>

#define TO_BE_SORT 10

namespace harpe
{
    namespace learning
    {
        template<typename T>
        double Spectrum::eval(T&& e)const
        {
            const unsigned int _size = propositions.size();
            std::vector<std::pair<const Sequence *,double>> data(_size);

            std::cout<<"Spectrum"<<std::endl;
            for(auto& u : real_sequences)
                std::cout<<u<<std::endl;
            std::cout<<std::endl;

            for(unsigned int i=0;i<_size;++i)
            {
                data[i].first = &propositions[i];
                data[i].second = propositions[i].eval(std::forward<T>(e));
                std::cout<<propositions[i].sequence<<" : "<<propositions[i].real_score<<" : "<<data[i].second<<std::endl;
            }

            const unsigned int _max = (_size<TO_BE_SORT?_size:TO_BE_SORT);
            std::partial_sort(data.begin(),
                              data.begin()+_max,
                              data.end(),
                              [](const std::pair<const Sequence*,double> _1,const std::pair<const Sequence*,double> _2){
                                  return _1.second > _2.second;
                              });

            unsigned int i = 0;
            std::cout<<std::endl;
            for(;i<_max;++i)
            {
                std::cout<<propositions[i].sequence<<" : "<<propositions[i].real_score<<" , "<<data[i].first->sequence<<" : "<<data[i].second<<std::endl;
                if((propositions[i].real_score != data[i].first->real_score) //because of double seq, do not compare ptr
                   or utils::maths::sign((double)propositions[i].real_score) != utils::maths::sign(data[i].second))

                    break;
            }

            return i/double(_max);
        }
    }
}
