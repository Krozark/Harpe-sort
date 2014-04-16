#ifndef HARPE_LEARNING_SEQUENCE_HPP
#define HARPE_LEARNING_SEQUENCE_HPP

#include <string>

#include <harpe-algo/Sequence.hpp>

namespace harpe
{
    namespace learning
    {
        class Spectrum;

        class Sequence
        {
            public:
                //Sequence(const Sequence&) = default;
                //Sequence& operator=(const Sequence&) = default;
                Sequence(Sequence&&) = default;
                Sequence& operator=(Sequence&&) = default;

            private:
                friend class Spectrum;
                
                Sequence(Spectrum& parent,const harpe::Sequence& src);

                std::string sequence;
                int real_score;

                double stats[harpe::Sequence::Stats::SIZE]; ///< store all the stats for score calc
                
        };
    }
}
#endif
