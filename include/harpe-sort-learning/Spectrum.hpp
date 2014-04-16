#ifndef HARPE_LEARNING_SPECTRUM_HPP
#define HARPE_LEARNING_SPECTRUM_HPP

#include <vector>
#include <string>

#include <mgf/Spectrum.hpp>

#include <harpe-algo/Sequence.hpp>

#include <harpe-sort-learning/Sequence.hpp>

namespace harpe
{
    namespace learning
    {
        class Spectrum
        {
            public:
                Spectrum(const Spectrum&) = delete;
                Spectrum& operator=(const Spectrum&) = delete;

                Spectrum(Spectrum&&) = default;
                Spectrum& operator=(Spectrum&&) = default;

                static Spectrum convert(const mgf::Spectrum& src,std::vector<harpe::Sequence>& src_seq);

                int rate(const std::string& seq)const;

            private:
                Spectrum(const mgf::Spectrum& src,std::vector<harpe::Sequence>& src_seq);

                std::vector<std::string> real_sequences;

                std::vector<Sequence> propositions;

                void sort();
        };
    }
}
#endif
