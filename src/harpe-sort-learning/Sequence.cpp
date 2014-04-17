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

        std::ostream& operator<<(std::ostream& output,const Sequence& self)
        {
            output<<"[real_score :"<<self.real_score<<"][";
            for(unsigned int i=0;i<harpe::Sequence::Stats::SIZE;++i)
                output<<"<"<<i<<":"<<self.stats[i]<<">";
            output<<"]"<<self.sequence<<std::endl;
            return output;
        }
        
        Sequence::Sequence(Spectrum& parent,const harpe::Sequence& src)
        {
            sequence = src.to_string();
            real_score = parent.rate(sequence);
            for(unsigned int i=0;i<harpe::Sequence::Stats::SIZE;++i)
                stats[i] = src.getHeader().stats[i];
        }
        
    }
}
