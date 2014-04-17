#ifndef HARPE_LEARNING_SEQUENCE_HPP
#define HARPE_LEARNING_SEQUENCE_HPP

#include <string>

#include <harpe-algo/Sequence.hpp>

namespace harpe
{
    namespace learning
    {
        class Spectrum;
        class Entity;

        class Sequence
        {
            public:
                Sequence() = default;
                //Sequence(const Sequence&) = default;
                //Sequence& operator=(const Sequence&) = default;
                Sequence(Sequence&&) = default;
                Sequence& operator=(Sequence&&) = default;

                double eval(const Entity& entity)const;

                friend std::ostream& operator<<(std::ostream& output,const Sequence& self);

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
