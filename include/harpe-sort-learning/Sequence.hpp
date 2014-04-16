#ifndef HARPE_LEARNING_SEQUENCE_HPP
#define HARPE_LEARNING_SEQUENCE_HPP

#include <harpe-algo/Sequence.hpp>

#include <harpe-sort-learning/Spectum.hpp>

#include <vector>

namespace harpe
{
    namespace learning
    {
        class Sequence
        {
            public:
                Sequence();
                Sequence(const Sequence&) = delete;
                Sequence& operator=(const Sequence&) = delete;

                static std::vector<Sequence> convert(Spectum& parent,std::vector<harpe::Sequence>& src);

            protected:

            private:
        };
    }
}
#endif
