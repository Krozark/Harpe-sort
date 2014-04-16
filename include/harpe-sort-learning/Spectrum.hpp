#ifndef HARPE_LEARNING_SPECTRUM_HPP
#define HARPE_LEARNING_SPECTRUM_HPP

namespace harpe
{
    namespace learning
    {
        class Spectrum
        {
            public:
                Spectrum();
                Spectrum(const Spectrum&) = delete;
                Spectrum& operator=(const Spectrum&) = delete;

                static Spectum convert(harpe::Specturm& src);

            protected:

            private:
        };
    }
}
#endif
