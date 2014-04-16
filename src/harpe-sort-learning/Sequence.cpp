#include <harpe-sort-learning/Sequence.hpp>
#include <harpe-sort-learning/Spectrum.hpp>


namespace harpe
{
    namespace learning
    {
        double Sequence::eval(const Entity& entity)const
        {
            return 0;
        }
        
        Sequence::Sequence(Spectrum& parent,const harpe::Sequence& src)
        {
            sequence = "";
            real_score = parent.rate(sequence);
            for(unsigned int i=0;i<harpe::Sequence::Stats::SIZE;++i)
                stats[i] = src.getHeader().stats[i];
        }
        
    }
}