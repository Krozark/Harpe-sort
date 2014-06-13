#ifndef HARPE_LEARNING_SPECTRUM_HPP
#define HARPE_LEARNING_SPECTRUM_HPP

#include <vector>
#include <string>
#include <iostream>

#include <mgf/Spectrum.hpp>

#include <harpe-algo/Sequence.hpp>

#include <harpe-sort-learning/Sequence.hpp>

namespace harpe
{
    namespace learning
    {
        class Entity;

        class Spectrum
        {
            public:
                Spectrum(const Spectrum&) = delete;
                Spectrum& operator=(const Spectrum&) = delete;

                Spectrum(Spectrum&&) = default;
                Spectrum& operator=(Spectrum&&) = default;

                static Spectrum convert(const mgf::Spectrum& src,const std::vector<harpe::Sequence>& src_seq);

                int rate(const std::string& seq)const;

                template<typename T>
                double eval(T&& e)const;

                friend std::ostream& operator<<(std::ostream& output,const Spectrum& self);

            private:
                Spectrum(const mgf::Spectrum& src,const std::vector<harpe::Sequence>& src_seq);

                std::vector<std::string> real_sequences;

                std::vector<Sequence> propositions;

                void sort();
        };
    }
}
#include <harpe-sort-learning/Spectrum.tpl>
#endif
